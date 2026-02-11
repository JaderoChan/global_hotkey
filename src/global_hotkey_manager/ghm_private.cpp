#include "ghm_private.hpp"

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

GHMPrivate::GHMPrivate() :
    runningState_(RS_FREE),
    runningRc_(0),
    workerThreadId_(std::thread::id())
{}

GHMPrivate::~GHMPrivate() = default;

int GHMPrivate::run()
{
    if (isRunning())            return RC_SUCCESS;

    int rc = initialize();
    if (rc != RC_SUCCESS)
        return rc;

    // Start the worker thread.
    workerThread_ = std::thread([this]() {
        workerThreadId_ = std::this_thread::get_id();
        work();
        runningState_ = RS_FREE;
        cvRunningState_.notify_one();
    });
    workerThread_.detach();

    // Wait for the worker thread to set the running state and the running return code.
    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState_.wait(lock, [this]() { return runningState_ != RS_FREE; });

    // Reset variables if start the worker thread fail.
    if (runningState_ == RS_TERMINATE)
    {
        workerThreadId_ = std::this_thread::get_id();
        runningState_ = RS_FREE;
    }

    return runningRc_;
}

int GHMPrivate::stop()
{
    if (!isRunning())           return RC_SUCCESS;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = unregisterAllHotkeys();
    rc = stopWork();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState_.wait(lock, [this]() { return runningState_ != RS_RUNNING; });

    return rc;
}

int GHMPrivate::registerHotkey(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;
    if (isHotkeyRegistered(kc))       return RC_ALREADY_EXIST;

    int rc = registerHotkeyImpl(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx_);
    fns_[kc] = {autoRepeat, fn};

    return rc;
}

int GHMPrivate::unregisterHotkey(const KeyCombination& kc)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;
    if (!isHotkeyRegistered(kc))      return RC_NOT_FOUND;

    int rc = unregisterHotkeyImpl(kc);

    std::lock_guard<std::mutex> lock(mtx_);
    fns_.erase(kc);

    return rc;
}

int GHMPrivate::unregisterAllHotkeys()
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = RC_SUCCESS;
    for (const auto& var : fns_)
        rc = unregisterHotkeyImpl(var.first);

    std::lock_guard<std::mutex> lock(mtx_);
    fns_.clear();

    return rc;
}

int GHMPrivate::replaceHotkey(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;
    if (newKc == oldKc)         return RC_SUCCESS;
    if (!isHotkeyRegistered(oldKc))   return RC_NOT_FOUND;
    if (isHotkeyRegistered(newKc))    return RC_ALREADY_EXIST;

    auto value = getPairValue(oldKc);
    int rc = unregisterHotkeyImpl(oldKc);
    {
        std::lock_guard<std::mutex> lock(mtx_);
        fns_.erase(oldKc);
    }
    rc = registerHotkeyImpl(newKc, value.first);
    // No Error Rollback! That is if registering new key combination fails,
    // the old key combination will still be removed.
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx_);
    fns_[newKc] = value;

    return rc;
}

int GHMPrivate::setHotkeyCallback(const KeyCombination& kc, const std::function<void()>& fn)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;
    if (!isHotkeyRegistered(kc))      return RC_NOT_FOUND;

    auto value = getPairValue(kc);
    value.second = fn;
    std::lock_guard<std::mutex> lock(mtx_);
    fns_[kc] = value;

    return RC_SUCCESS;
}

int GHMPrivate::setHotkeyAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;
    if (!isHotkeyRegistered(kc))      return RC_NOT_FOUND;

    auto value = getPairValue(kc);
    if (value.first == autoRepeat)
        return RC_SUCCESS;
    value.first = autoRepeat;

    int rc = unregisterHotkeyImpl(kc);
    {
        std::lock_guard<std::mutex> lock(mtx_);
        fns_.erase(kc);
    }
    rc = registerHotkeyImpl(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx_);
    fns_[kc] = value;

    return rc;
}

bool GHMPrivate::isHotkeyRegistered(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return fns_.find(kc) != fns_.end();
}

bool GHMPrivate::isHotkeyAutoRepeat(const KeyCombination& kc) const
{
    if (!isHotkeyRegistered(kc))
        return false;

    std::lock_guard<std::mutex> lock(mtx_);
    return fns_.at(kc).first;
}

bool GHMPrivate::isRunning() const
{
    return runningState_ == RS_RUNNING;
}

std::unordered_set<KeyCombination> GHMPrivate::getRegisteredHotkeys() const
{
    std::unordered_set<KeyCombination> set;
    for (const auto& var : fns_)
        set.insert(var.first);
    return set;
}

std::pair<bool, std::function<void ()>> GHMPrivate::getPairValue(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    const auto& it = fns_.find(kc);
    if (it == fns_.end())
        return std::pair<bool, std::function<void ()>>();
    return it->second;
}

void GHMPrivate::setRunSuccess()
{
    runningRc_ = RC_SUCCESS;
    runningState_ = RS_RUNNING;
    cvRunningState_.notify_one();
}

void GHMPrivate::setRunFail(int errorCode)
{
    runningRc_ = errorCode;
    runningState_ = RS_TERMINATE;
    cvRunningState_.notify_one();
}

int GHMPrivate::initialize()
{ return RC_SUCCESS; }

int GHMPrivate::stopWork()
{ return RC_SUCCESS; }

bool GHMPrivate::isInWorkerThread() const
{ return std::this_thread::get_id() == workerThreadId_; }

} // namespace gbhk
