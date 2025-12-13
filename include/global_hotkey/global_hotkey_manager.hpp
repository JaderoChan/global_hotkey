#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP

#include <functional>   // function
#include <memory>       // unique_ptr
#include <vector>       // vector

#include "key_combination.hpp"

namespace gbhk
{

class GHMPrivate;

class GLOBAL_HOTKEY_API GlobalHotkeyManager
{
public:
    /// @brief Initialize the `Global Hotkey Manager`.
    int initialize();
    /// @brief Uninitialize the `Global Hotkey Manager`.
    /// @attention Can't be performed in the worker thread,
    /// that is you can't perform this function in the hotkey callback.
    int uninitialize();
    /// @brief Add a hotkey to the `Global Hotkey Manager`.
    /// @param kc The hotkey you will add.
    /// @param fn The callback function triggered when hotkey is actived.
    /// @param autoRepeat Whether the hotkey automatically repeat the callback function
    /// when it's held down continuously.
    /// @attention Must be performed after #initialize().
    /// @attention Can't be performed in the worker thread,
    int add(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat = false);
    /// @brief Remove a hotkey from the `Global Hotkey Manager`.
    /// @param kc The hotkey you will remove.
    /// @attention Must be performed after #initialize().
    /// @attention Can't be performed in the worker thread,
    int remove(const KeyCombination& kc);
    /// @brief Remove all hotkey in the `Global Hotkey Manager`.
    /// @attention Must be performed after #initialize().
    /// @attention Can't be performed in the worker thread,
    int removeAll();
    /// @brief Replace a old hotkey with a new hotkey.
    /// @param oldKc The old hotkey.
    /// @param newKc The new hotkey.
    /// @attention Must be performed after #initialize().
    /// @attention Can't be performed in the worker thread,
    /// @attention If the newKc is failed to add, the oldKc still will be remove.
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc);
    /// @brief Set whether the hotkey is auto repeat.
    /// @sa #add() #isAutoRepeat()
    /// @attention Must be performed after #initialize().
    /// @attention Can't be performed in the worker thread,
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);
    /// @brief Check whether the hotkey given is exists in the `Global Hotkey Manager`.
    /// @param kc The hotkey you will check.
    bool has(const KeyCombination& kc) const;
    /// @brief Check whether the hotkey given is auto repeat.
    /// @param kc The hotkey you will check.
    /// @sa #setAutoRepeat()
    bool isAutoRepeat(const KeyCombination& kc) const;
    /// @brief Whether the `Global Hotkey Manager` worker thread is running.
    /// @note thread-safe.
    bool isRunning() const;
    /// @brief Fetch all hotkey in the `Global Hotkey Manager`.
    std::vector<KeyCombination> getAll() const;

protected:
    explicit GlobalHotkeyManager(std::unique_ptr<GHMPrivate> ptr);
    virtual ~GlobalHotkeyManager();
    GlobalHotkeyManager(const GlobalHotkeyManager&) = delete;
    GlobalHotkeyManager& operator=(const GlobalHotkeyManager&) = delete;

    std::unique_ptr<GHMPrivate> ptr_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
