#ifndef GLOBAL_HOTKEY_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_GHM_PRIVATE_HPP

#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <functional>           // function
#include <mutex>                // mutex, lock_guard, unique_lock
#include <thread>               // thread, thread::id
#include <unordered_map>        // unordered_map
#include <vector>               // vector

#include <global_hotkey/key_combination.hpp>

namespace gbhk
{

class GHMPrivate
{
public:
    GHMPrivate();
    virtual ~GHMPrivate();

    int initialize();
    int uninitialize();
    int add(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat);
    int remove(const KeyCombination& kc);
    int removeAll();
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc);
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);
    bool has(const KeyCombination& kc) const;
    bool isAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;
    std::vector<KeyCombination> getAll() const;

protected:
    /// @brief Get the pair of `autoRepeat` and `callback` values corresponding to the specified key combination.
    /// @return Return a empty pair if the key combination given is not exists or it is invalid
    /// else return the pair of `autoRepeat` and `callback` values.
    /// @note Thread-safe.
    std::pair<bool, std::function<void ()>> getPairValue(const KeyCombination& kc) const;

    /// @brief Indicates the worker thread running successfully.
    /// @note The `Running Return Code` will be set to '#RC_SUCCESS`.
    void setRunSuccess();
    /// @brief Indicates the worker thread running failed.
    /// @note The `Running Return Code` will be set to errorCode.
    void setRunFail(int errorCode);

    // Some interfaces for subclasses to specialize.

    /// @note This function will be performed before the worker thread is running.
    virtual int doBeforeThreadRun();
    /// @note This function will be performed before the worker thread is end.
    /// @note Specifically, only when this function returns will the semaphore controlling
    /// the thread's end be changed to enable the thread to exit.
    virtual int doBeforeThreadEnd();
    /// @note The specific working logic of the worker thread.
    /// @attention The `#setRunSuccess()` or `#setRunFail()` must be performed in the implementation
    /// of this function to indicate whether the work running successfully.
    virtual void work() = 0;
    /// @note The specific logic of register hotkey.
    virtual int registerHotkey(const KeyCombination& kc, bool autoRepeat) = 0;
    /// @note The specific logic of unregister hotkey.
    virtual int unregisterHotkey(const KeyCombination& kc) = 0;

private:
    enum RunningState
    {
        RS_FREE,
        RS_RUNNING,
        RS_TERMINATE
    };

    bool isInWorkerThread_() const;

    std::condition_variable cvRunningState_;
    std::atomic<RunningState> runningState_;
    std::atomic<int> runningRc_;

    std::thread workerThread_;
    std::thread::id workerThreadId_;

    mutable std::mutex mtx_;
    std::unordered_map<KeyCombination, std::pair<bool, std::function<void ()>>> fns_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GHM_PRIVATE_HPP
