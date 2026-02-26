#include <global_hotkey/hook_global_hotkey_manager.hpp>

#include "hook_ghm_private.hpp"

namespace gbhk
{

HookGlobalHotkeyManager& HookGlobalHotkeyManager::getInstance()
{
    static HookGlobalHotkeyManager instance;
    return instance;
}

HookGlobalHotkeyManager::HookGlobalHotkeyManager() :
    GlobalHotkeyManager(HookGHMPrivate::getInstance())
{}

HookGlobalHotkeyManager::~HookGlobalHotkeyManager() = default;

} // namespace gbhk
