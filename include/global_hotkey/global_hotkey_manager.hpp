#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP

#include <functional>   // function
#include <memory>       // unique_ptr
#include <unordered_set>

#include "defines.hpp"
#include "key_combination.hpp"

namespace gbhk
{

class GHMPrivate;

class GLOBAL_HOTKEY_API GlobalHotkeyManager
{
public:
    /// @brief Initialize the Global Hotkey Manager.
    int initialize();

    /// @brief Uninitialize the Global Hotkey Manager and release all resources.
    /// @attention Do not call this function from worker thread (typically within hotkey callbacks),
    ///            as it may cause deadlock.
    int uninitialize();

    /// @brief Register a global hotkey.
    /// @param kc The key combination to register as a hotkey.
    /// @param fn The callback function to execute when the hotkey is activated.
    /// @param autoRepeat Whether to repeatedly trigger the callback while the hotkey is held down.
    int add(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat = false);

    /// @brief Unregister a global hotkey.
    /// @param kc The key combination to remove.
    int remove(const KeyCombination& kc);

    /// @brief Unregister all global hotkeys.
    int removeAll();

    /// @brief Replace an existing hotkey with a new key combination.
    /// @param oldKc The existing key combination to replace.
    /// @param newKc The new key combination to register.
    /// @note If 'oldKc' and 'newKc' are the same, do nothing and return RC_SUCCESS.
    /// @note If registering 'newKc' fails, 'oldKc' will still be removed.
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc);

    /// @brief Set the callback function for specified hotkey.
    /// @param kc The key combination to set.
    /// @param fn The callback function to execute when the hotkey is activated.
    int setFunction(const KeyCombination& kc, const std::function<void ()>& fn);

    /// @brief Enable or disable auto-repeat for a registered hotkey.
    /// @param kc The key combination to modify.
    /// @param autoRepeat Whether to enable auto-repeat.
    /// @see isAutoRepeat()
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);

    /// @brief Check if a hotkey is registered.
    /// @param kc The key combination to check.
    /// @return True if the hotkey is registered, false otherwise.
    bool has(const KeyCombination& kc) const;

    /// @brief Check if auto-repeat is enabled for a hotkey.
    /// @param kc The key combination to check.
    /// @return True if auto-repeat is enabled, false otherwise.
    /// @note False will be returned when the specified hotkey does not exist.
    /// @see setAutoRepeat()
    bool isAutoRepeat(const KeyCombination& kc) const;

    /// @brief Check if the Global Hotkey Manager is active.
    /// @return True if the manager is running, false otherwise.
    /// @note This function is thread-safe.
    bool isRunning() const;

    /// @brief Get all registered hotkeys.
    /// @return Set containing all registered key combinations.
    std::unordered_set<KeyCombination> getAll() const;

protected:
    explicit GlobalHotkeyManager(std::unique_ptr<GHMPrivate> ptr);
    virtual ~GlobalHotkeyManager();
    GlobalHotkeyManager(const GlobalHotkeyManager&) = delete;
    GlobalHotkeyManager& operator=(const GlobalHotkeyManager&) = delete;

    /// @brief Implementation-specific private data (PIMPL idiom).
    std::unique_ptr<GHMPrivate> ptr_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
