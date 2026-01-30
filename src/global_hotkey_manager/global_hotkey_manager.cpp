#include <global_hotkey/global_hotkey_manager.hpp>

#include "ghm_private.hpp"

namespace gbhk
{

GlobalHotkeyManager::GlobalHotkeyManager(std::unique_ptr<GHMPrivate> ptr) : ptr_(std::move(ptr)) {}

GlobalHotkeyManager::~GlobalHotkeyManager() = default;

int GlobalHotkeyManager::initialize()
{ return ptr_->initialize(); }

int GlobalHotkeyManager::uninitialize()
{ return ptr_->uninitialize(); }

int GlobalHotkeyManager::add(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat)
{ return ptr_->add(kc, fn, autoRepeat); }

int GlobalHotkeyManager::remove(const KeyCombination& kc)
{ return ptr_->remove(kc); }

int GlobalHotkeyManager::removeAll()
{ return ptr_->removeAll(); }

int GlobalHotkeyManager::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{ return ptr_->replace(oldKc, newKc); }

int GlobalHotkeyManager::setFunction(const KeyCombination& kc, const std::function<void ()>& fn)
{ return ptr_->setFunction(kc, fn); }

int GlobalHotkeyManager::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{ return ptr_->setAutoRepeat(kc, autoRepeat); }

bool GlobalHotkeyManager::has(const KeyCombination& kc) const
{ return ptr_->has(kc); }

bool GlobalHotkeyManager::isAutoRepeat(const KeyCombination& kc) const
{ return ptr_->isAutoRepeat(kc); }

bool GlobalHotkeyManager::isRunning() const
{ return ptr_->isRunning(); }

std::unordered_set<KeyCombination> GlobalHotkeyManager::getAll() const
{ return ptr_->getAll(); }

} // namespace gbhk
