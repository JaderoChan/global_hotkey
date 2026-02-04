#ifndef GLOBAL_HOTKEY_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_GHM_PRIVATE_HPP

#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <functional>           // function
#include <mutex>                // mutex, lock_guard, unique_lock
#include <thread>               // thread, thread::id
#include <unordered_map>        // unordered_map
#include <unordered_set>

#include <global_hotkey/key_combination.hpp>

namespace gbhk
{

class GHMPrivate
{
public:
    GHMPrivate();
    virtual ~GHMPrivate();

    int run();
    int stop();
    int registerHotkey(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat);
    int unregisterHotkey(const KeyCombination& kc);
    int unregisterAllHotkeys();
    int replaceHotkey(const KeyCombination& oldKc, const KeyCombination& newKc);
    int setHotkeyCallback(const KeyCombination& kc, const std::function<void ()>& fn);
    int setHotkeyAutoRepeat(const KeyCombination& kc, bool autoRepeat);
    bool isHotkeyRegistered(const KeyCombination& kc) const;
    bool isHotkeyAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;
    std::unordered_set<KeyCombination> getRegisteredHotkeys() const;

protected:
    /// @brief Get the auto-repeat flag and callback function for a registered hotkey.
    /// @param kc Key combination of the hotkey to look up.
    /// @return A pair containing the auto-repeat flag and callback function.
    ///         Returns an empty pair (default-constructed) if the key combination
    ///         is not registered or invalid.
    /// @note This function is thread-safe.
    std::pair<bool, std::function<void ()>> getPairValue(const KeyCombination& kc) const;

    /// @brief Signal that the worker thread has run successfully.
    /// @note The running return code will be set to `RC_SUCCESS`.
    void setRunSuccess();

    /// @brief Signal that the worker thread failed to run.
    /// @param errorCode The error code indicating the failure reason.
    /// @note The running return code will be set to the specified error code.
    void setRunFail(int errorCode);

    // Interface for subclasses to implement platform-specific behavior

    /// @brief Perform platform-specific initialize before the worker thread starts.
    /// @return RC_SUCCESS on success, or a platform-specific error code on failure.
    /// @note This function is called before the worker thread begins execution.
    virtual int initialize();

    /// @brief Perform platform-specific stop the worker thread.
    /// @return RC_SUCCESS on success, or a platform-specific error code on failure.
    /// @note This function is called before the worker thread exits.
    /// @note The thread exit semaphore is only signaled after this function returns.
    virtual int stopWork();

    /// @brief Main work function for the platform-specific worker thread.
    /// @attention Implementations must call either `setRunSuccess()` or `setRunFail()`
    ///            to indicate the completion status of the worker thread.
    virtual void work() = 0;

    /// @brief Register a hotkey with the platform-specific hotkey system.
    /// @param kc Key combination of the hotkey to register.
    /// @param autoRepeat Whether the hotkey should auto-repeat when held down.
    /// @return RC_SUCCESS on success, or a platform-specific error code on failure.
    virtual int registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat) = 0;

    /// @brief Unregister a hotkey from the platform-specific hotkey system.
    /// @param kc Key combination of the hotkey to unregister.
    /// @return RC_SUCCESS on success, or a platform-specific error code on failure.
    virtual int unregisterHotkeyImpl(const KeyCombination& kc) = 0;

private:
    enum RunningState
    {
        RS_FREE,
        RS_RUNNING,
        RS_TERMINATE
    };

    bool isInWorkerThread() const;

    std::condition_variable cvRunningState_;
    std::atomic<RunningState> runningState_;
    std::atomic<int> runningRc_;

    std::thread workerThread_;
    std::thread::id workerThreadId_;

    mutable std::mutex mtx_;    // Used for protect fns variable.
    std::unordered_map<KeyCombination, std::pair<bool, std::function<void ()>>> fns_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GHM_PRIVATE_HPP
