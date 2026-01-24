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

int GHMPrivate::initialize()
{
    if (isRunning())            return RC_SUCCESS;

    int rc = doBeforeThreadRun();
    if (rc != RC_SUCCESS)
        return rc;

    workerThread_ = std::thread([this]() {
        workerThreadId_ = std::this_thread::get_id();
        work();
        runningState_ = RS_FREE;
        cvRunningState_.notify_one();
    });
    workerThread_.detach();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState_.wait(lock, [this]() { return runningState_ != RS_FREE; });

    if (runningState_ == RS_TERMINATE)
    {
        workerThreadId_ = std::this_thread::get_id();
        runningState_ = RS_FREE;
    }

    return runningRc_;
}

int GHMPrivate::uninitialize()
{
    if (!isRunning())           return RC_SUCCESS;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = removeAll();
    rc = doBeforeThreadEnd();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState_.wait(lock, [this]() { return runningState_ != RS_RUNNING; });

    return rc;
}

int GHMPrivate::add(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = registerHotkey(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx_);
    fns_[kc] = {autoRepeat, fn};

    return rc;
}

int GHMPrivate::remove(const KeyCombination& kc)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = unregisterHotkey(kc);

    std::lock_guard<std::mutex> lock(mtx_);
    fns_.erase(kc);

    return rc;
}

int GHMPrivate::removeAll()
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = RC_SUCCESS;
    for (const auto& var : fns_)
        rc = unregisterHotkey(var.first);

    std::lock_guard<std::mutex> lock(mtx_);
    fns_.clear();

    return rc;
}

int GHMPrivate::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;
    if (newKc == oldKc)         return RC_SUCCESS;

    auto value = getPairValue(oldKc);
    int rc = unregisterHotkey(oldKc);
    mtx_.lock();
    fns_.erase(oldKc);
    mtx_.unlock();
    rc = registerHotkey(newKc, value.first);
    // No Error Rollback! That is if registering newKc fails, oldKc will still be removed.
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx_);
    fns_[newKc] = value;

    return rc;
}

int GHMPrivate::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    auto value = getPairValue(kc);
    if (value.first == autoRepeat)
        return RC_SUCCESS;
    value.first = autoRepeat;

    int rc = unregisterHotkey(kc);
    mtx_.lock();
    fns_.erase(kc);
    mtx_.unlock();
    rc = registerHotkey(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx_);
    fns_[kc] = value;

    return rc;
}

bool GHMPrivate::has(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return fns_.find(kc) != fns_.end();
}

bool GHMPrivate::isAutoRepeat(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (fns_.find(kc) != fns_.end())
        return fns_.at(kc).first;
    return false;
}

bool GHMPrivate::isRunning() const
{
    return runningState_ == RS_RUNNING;
}

std::vector<KeyCombination> GHMPrivate::getAll() const
{
    std::vector<KeyCombination> rslt;
    for (const auto& var : fns_)
        rslt.emplace_back(var.first);
    return rslt;
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

int GHMPrivate::doBeforeThreadRun()
{ return RC_SUCCESS; }

int GHMPrivate::doBeforeThreadEnd()
{ return RC_SUCCESS; }

bool GHMPrivate::isInWorkerThread() const
{ return std::this_thread::get_id() == workerThreadId_; }

} // namespace gbhk
