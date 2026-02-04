#include <keyboard_tools/keyboard_tools.hpp>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "keyboard_tools_details.hpp"

namespace kbdt
{

enum RunningState
{
    RS_FREE,
    RS_RUNNING,
    RS_TERMINATE
};

static std::thread workerThread;
static std::condition_variable runningStateCv;
static std::atomic<RunningState> runningState{RS_FREE};
static std::atomic<int> runningRc{-1};

static void threadWork()
{
    details::work();
    // If the work exits normally (running state is RS_RUNNING), set the state to RS_FREE.
    // If the work exits due to an error (running state is RS_TERMINATE), preserve the error state.
    if (runningState == RS_RUNNING)
        runningState = RS_FREE;
    runningStateCv.notify_one();
}

KeyboardToolsManager& KeyboardToolsManager::getInstance()
{
    static KeyboardToolsManager instance;
    return instance;
}

int KeyboardToolsManager::run()
{
    int rc = details::initialize();
    if (rc != KBDT_RC_SUCCESS)
        return rc;

    workerThread = std::thread(&threadWork);
    workerThread.detach();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> locker(dummyMtx);
    // Wait for the worker thread to set the running state.
    runningStateCv.wait(locker, [&]() { return runningState != RS_FREE; });
    if (runningState == RS_TERMINATE)
        runningState = RS_FREE;
    rc = runningRc;

    return rc;
}

int KeyboardToolsManager::stop()
{
    int rc = details::stopWork();
    if (rc != KBDT_RC_SUCCESS)
        return rc;

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> locker(dummyMtx);
    // Wait for the worker thread to exit and transition to free state.
    runningStateCv.wait(locker, [&]() { return runningState == RS_FREE; });

    return KBDT_RC_SUCCESS;
}

int KeyboardToolsManager::setEventHandler(KeyEventHandler handler)
{
    return details::setEventHandler(handler);
}

bool KeyboardToolsManager::isRunning() noexcept
{
    return runningState == RS_RUNNING;
}

size_t sendEvents(const std::vector<KeyEvent>& events)
{
    return details::sendEvents(events);
}

bool sendEvent(const KeyEvent& event)
{
    return sendEvents({event}) == 1;
}

void setRunSuccess()
{
    runningRc = KBDT_RC_SUCCESS;
    runningState = RS_RUNNING;
    runningStateCv.notify_one();
}

void setRunFail(int errorCode)
{
    runningRc = errorCode;
    runningState = RS_TERMINATE;
}

} // namespace kbdt
