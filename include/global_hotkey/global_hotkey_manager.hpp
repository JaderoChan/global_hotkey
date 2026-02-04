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
    /// @brief Run the Global Hotkey Manager service.
    int run();

    /// @brief Stop the Global Hotkey Manager service, and release all resources.
    /// @attention Do not call this function from worker thread (typically within hotkey callbacks),
    ///            as it may cause deadlock.
    int stop();

    /// @brief Register a global hotkey.
    /// @param kc Key combination of the hotkey to register.
    /// @param fn The callback function to execute when the hotkey is activated.
    /// @param autoRepeat Whether to repeatedly trigger the callback while the hotkey is held down.
    int registerHotkey(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat = false);

    /// @brief Unregister a global hotkey.
    /// @param kc Key combination of the hotkey to unregister.
    int unregisterHotkey(const KeyCombination& kc);

    /// @brief Unregister all registered global hotkeys.
    int unregisterAllHotkeys();

    /// @brief Replace an registered hotkey with a new hotkey.
    /// @param oldKc Key combination of the registered hotkey to replace.
    /// @param newKc Key combination of the new hotkey to register.
    /// @note If 'oldKc' and 'newKc' are the same, do nothing and return RC_SUCCESS.
    /// @note If registering 'newKc' fails, 'oldKc' will still be removed.
    int replaceHotkey(const KeyCombination& oldKc, const KeyCombination& newKc);

    /// @brief Set the callback function for specified hotkey.
    /// @param kc Key combination of the hotkey to set.
    /// @param fn The callback function to execute when the hotkey is activated.
    int setHotkeyCallback(const KeyCombination& kc, const std::function<void ()>& fn);

    /// @brief Enable or disable auto-repeat for a registered hotkey.
    /// @param kc Key combination of the hotkey to modify.
    /// @param autoRepeat Whether to enable auto-repeat.
    /// @see isHotkeyAutoRepeat()
    int setHotkeyAutoRepeat(const KeyCombination& kc, bool autoRepeat);

    /// @brief Check if a hotkey is registered.
    /// @param kc Key combination of the hotkey to check.
    /// @return True if the hotkey is registered, false otherwise.
    /// @note This function is thread-safe.
    bool isHotkeyRegistered(const KeyCombination& kc) const;

    /// @brief Check if auto-repeat is enabled for a hotkey.
    /// @param kc Key combination of the hotkey to check.
    /// @return True if auto-repeat is enabled, false otherwise.
    /// @note False will be returned when the specified hotkey does not exist.
    /// @see setHotkeyAutoRepeat()
    bool isHotkeyAutoRepeat(const KeyCombination& kc) const;

    /// @brief Check if the Global Hotkey Manager service is running.
    /// @return True if the GHM service is running, false otherwise.
    /// @note This function is thread-safe.
    bool isRunning() const;

    /// @brief Get all registered hotkeys.
    /// @return A set containing key combinations of all registered hotkey.
    /// @note This function is thread-safe.
    std::unordered_set<KeyCombination> getRegisteredHotkeys() const;

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
