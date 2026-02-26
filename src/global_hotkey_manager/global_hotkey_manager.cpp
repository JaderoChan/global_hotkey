#include <global_hotkey/global_hotkey_manager.hpp>

#include "ghm_private.hpp"

namespace gbhk
{

GlobalHotkeyManager::GlobalHotkeyManager(GHMPrivate& pri) : pri_(pri) {}

GlobalHotkeyManager::~GlobalHotkeyManager() = default;

int GlobalHotkeyManager::run()
{ return pri_.run(); }

int GlobalHotkeyManager::stop()
{ return pri_.stop(); }

int GlobalHotkeyManager::registerHotkey(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat)
{ return pri_.registerHotkey(kc, fn, autoRepeat); }

int GlobalHotkeyManager::unregisterHotkey(const KeyCombination& kc)
{ return pri_.unregisterHotkey(kc); }

int GlobalHotkeyManager::unregisterAllHotkeys()
{ return pri_.unregisterAllHotkeys(); }

int GlobalHotkeyManager::replaceHotkey(const KeyCombination& oldKc, const KeyCombination& newKc)
{ return pri_.replaceHotkey(oldKc, newKc); }

int GlobalHotkeyManager::setHotkeyCallback(const KeyCombination& kc, const std::function<void ()>& fn)
{ return pri_.setHotkeyCallback(kc, fn); }

int GlobalHotkeyManager::setHotkeyAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{ return pri_.setHotkeyAutoRepeat(kc, autoRepeat); }

bool GlobalHotkeyManager::isHotkeyRegistered(const KeyCombination& kc) const
{ return pri_.isHotkeyRegistered(kc); }

bool GlobalHotkeyManager::isHotkeyAutoRepeat(const KeyCombination& kc) const
{ return pri_.isHotkeyAutoRepeat(kc); }

bool GlobalHotkeyManager::isRunning() const
{ return pri_.isRunning(); }

std::unordered_set<KeyCombination> GlobalHotkeyManager::getRegisteredHotkeys() const
{ return pri_.getRegisteredHotkeys(); }

} // namespace gbhk
