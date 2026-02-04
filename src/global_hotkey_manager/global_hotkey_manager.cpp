#include <global_hotkey/global_hotkey_manager.hpp>

#include "ghm_private.hpp"

namespace gbhk
{

GlobalHotkeyManager::GlobalHotkeyManager(std::unique_ptr<GHMPrivate> ptr) : ptr_(std::move(ptr)) {}

GlobalHotkeyManager::~GlobalHotkeyManager() = default;

int GlobalHotkeyManager::run()
{ return ptr_->run(); }

int GlobalHotkeyManager::stop()
{ return ptr_->stop(); }

int GlobalHotkeyManager::registerHotkey(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat)
{ return ptr_->registerHotkey(kc, fn, autoRepeat); }

int GlobalHotkeyManager::unregisterHotkey(const KeyCombination& kc)
{ return ptr_->unregisterHotkey(kc); }

int GlobalHotkeyManager::unregisterAllHotkeys()
{ return ptr_->unregisterAllHotkeys(); }

int GlobalHotkeyManager::replaceHotkey(const KeyCombination& oldKc, const KeyCombination& newKc)
{ return ptr_->replaceHotkey(oldKc, newKc); }

int GlobalHotkeyManager::setHotkeyCallback(const KeyCombination& kc, const std::function<void ()>& fn)
{ return ptr_->setHotkeyCallback(kc, fn); }

int GlobalHotkeyManager::setHotkeyAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{ return ptr_->setHotkeyAutoRepeat(kc, autoRepeat); }

bool GlobalHotkeyManager::isHotkeyRegistered(const KeyCombination& kc) const
{ return ptr_->isHotkeyRegistered(kc); }

bool GlobalHotkeyManager::isHotkeyAutoRepeat(const KeyCombination& kc) const
{ return ptr_->isHotkeyAutoRepeat(kc); }

bool GlobalHotkeyManager::isRunning() const
{ return ptr_->isRunning(); }

std::unordered_set<KeyCombination> GlobalHotkeyManager::getRegisteredHotkeys() const
{ return ptr_->getRegisteredHotkeys(); }

} // namespace gbhk
