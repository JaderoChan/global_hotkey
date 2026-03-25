#include "ghm_private.hpp"

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

GHMPrivate::GHMPrivate() {}

GHMPrivate::~GHMPrivate() = default;

int GHMPrivate::run()
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (isRunning())                return RC_SUCCESS;

    int rc = initialize();
    if (rc != RC_SUCCESS)
        return rc;

    // Start the worker thread.
    workerThread_ = std::thread([this]()
    {
        {
            std::lock_guard<std::mutex> workerThreadIdLocker(workThreadIdMtx_);
            workerThreadId_ = std::this_thread::get_id();
        }
        work();
        {
            std::lock_guard<std::mutex> locker(runningStateMtx_);
            runningState_ = RS_FREE;
        }
        runningStateCv_.notify_one();
    });

    // Wait for the worker thread to set the running state and the running return code.
    std::unique_lock<std::mutex> locker(runningStateMtx_);
    runningStateCv_.wait(locker, [this]() { return runningState_ != RS_FREE; });

    bool isRunningFail = false;
    {
        // Reset variables if start the worker thread fail.
        if (runningState_ == RS_TERMINATE)
        {
            {
                std::lock_guard<std::mutex> workerThreadIdLocker(workThreadIdMtx_);
                workerThreadId_ = std::thread::id();
            }
            runningState_ = RS_FREE;
            isRunningFail = true;
        }
        rc = runningRc_;
    }

    if (isRunningFail && workerThread_.joinable())
        workerThread_.join();

    return rc;
}

int GHMPrivate::stop()
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (!isRunning())               return RC_SUCCESS;
    if (isInWorkerThread())         return RC_BAD_THREAD;

    for (const auto& var : fns_)
        unregisterHotkeyImpl(var.first);    // Ignore return value.
    fns_.clear();

    int rc = stopWork();
    if (workerThread_.joinable())
        workerThread_.join();

    return rc;
}

int GHMPrivate::registerHotkey(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat)
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (!isRunning())               return RC_BAD_TIMING;
    if (isInWorkerThread())         return RC_BAD_THREAD;

    std::lock_guard<std::mutex> locker(fnsMtx_);
    if (fns_.find(kc) != fns_.end())
        return RC_ALREADY_EXIST;

    int rc = registerHotkeyImpl(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    fns_[kc] = {autoRepeat, fn};

    return rc;
}

int GHMPrivate::unregisterHotkey(const KeyCombination& kc)
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (!isRunning())               return RC_BAD_TIMING;
    if (isInWorkerThread())         return RC_BAD_THREAD;

    std::lock_guard<std::mutex> lock(fnsMtx_);
    if (fns_.find(kc) == fns_.end())
        return RC_NOT_FOUND;

    int rc = unregisterHotkeyImpl(kc);

    fns_.erase(kc);

    return rc;
}

int GHMPrivate::unregisterAllHotkeys()
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (!isRunning())               return RC_BAD_TIMING;
    if (isInWorkerThread())         return RC_BAD_THREAD;

    std::lock_guard<std::mutex> locker(fnsMtx_);
    int rc = RC_SUCCESS;    // Only keep last return code.
    for (const auto& var : fns_)
        rc = unregisterHotkeyImpl(var.first);
    fns_.clear();

    return rc;
}

int GHMPrivate::replaceHotkey(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (!isRunning())               return RC_BAD_TIMING;
    if (isInWorkerThread())         return RC_BAD_THREAD;
    if (newKc == oldKc)             return RC_SUCCESS;

    std::lock_guard<std::mutex> locker(fnsMtx_);

    auto oldIt = fns_.find(oldKc);
    if (oldIt == fns_.end())
        return RC_NOT_FOUND;
    if (fns_.find(newKc) != fns_.end())
        return RC_ALREADY_EXIST;

    const auto value = oldIt->second;

    int rc = unregisterHotkeyImpl(oldKc);
    if (rc != RC_SUCCESS)
        return rc;

    fns_.erase(oldKc);
    rc = registerHotkeyImpl(newKc, value.first);
    // No Error Rollback! That is if registering new key combination fails,
    // the old key combination will still be removed.
    if (rc != RC_SUCCESS)
        return rc;

    fns_[newKc] = value;

    return rc;
}

int GHMPrivate::setHotkeyCallback(const KeyCombination& kc, const std::function<void()>& fn)
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (!isRunning())               return RC_BAD_TIMING;
    if (isInWorkerThread())         return RC_BAD_THREAD;

    std::lock_guard<std::mutex> locker(fnsMtx_);
    auto it = fns_.find(kc);
    if (it == fns_.end())
        return RC_NOT_FOUND;

    auto value = it->second;
    value.second = fn;

    fns_[kc] = value;

    return RC_SUCCESS;
}

int GHMPrivate::setHotkeyAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    std::lock_guard<std::mutex> operateLocker(operateMtx_);

    if (!isRunning())               return RC_BAD_TIMING;
    if (isInWorkerThread())         return RC_BAD_THREAD;

    std::lock_guard<std::mutex> locker(fnsMtx_);
    auto it = fns_.find(kc);
    if (it == fns_.end())
        return RC_NOT_FOUND;

    auto value = it->second;
    if (value.first == autoRepeat)
        return RC_SUCCESS;
    value.first = autoRepeat;

    int rc = unregisterHotkeyImpl(kc);
    if (rc != RC_SUCCESS)
        return rc;

    fns_.erase(kc);
    rc = registerHotkeyImpl(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    fns_[kc] = value;

    return rc;
}

bool GHMPrivate::isHotkeyRegistered(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> locker(fnsMtx_);
    return fns_.find(kc) != fns_.end();
}

bool GHMPrivate::isHotkeyAutoRepeat(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> locker(fnsMtx_);
    auto it = fns_.find(kc);
    if (it == fns_.end())
        return false;
    return it->second.first;
}

bool GHMPrivate::isRunning() const
{
    std::lock_guard<std::mutex> locker(runningStateMtx_);
    return runningState_ == RS_RUNNING;
}

std::unordered_set<KeyCombination> GHMPrivate::getRegisteredHotkeys() const
{
    std::lock_guard<std::mutex> locker(fnsMtx_);

    std::unordered_set<KeyCombination> set;
    for (const auto& var : fns_)
        set.insert(var.first);
    return set;
}

std::pair<bool, std::function<void ()>> GHMPrivate::getPairValue(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> locker(fnsMtx_);

    const auto& it = fns_.find(kc);
    if (it == fns_.end())
        return std::pair<bool, std::function<void ()>>();
    return it->second;
}

void GHMPrivate::setRunSuccess()
{
    {
        std::lock_guard<std::mutex> locker(runningStateMtx_);
        runningRc_ = RC_SUCCESS;
        runningState_ = RS_RUNNING;
    }
    runningStateCv_.notify_one();
}

void GHMPrivate::setRunFail(int errorCode)
{
    {
        std::lock_guard<std::mutex> locker(runningStateMtx_);
        runningRc_ = errorCode;
        runningState_ = RS_TERMINATE;
    }
    runningStateCv_.notify_one();
}

int GHMPrivate::initialize()
{ return RC_SUCCESS; }

int GHMPrivate::stopWork()
{ return RC_SUCCESS; }

bool GHMPrivate::isInWorkerThread() const
{
    std::lock_guard<std::mutex> workerThreadIdLocker(workThreadIdMtx_);
    return std::this_thread::get_id() == workerThreadId_;
}

} // namespace gbhk
