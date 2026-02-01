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
    GlobalHotkeyManager(std::unique_ptr<GHMPrivate>(new HookGHMPrivate()))
{}

HookGlobalHotkeyManager::~HookGlobalHotkeyManager() = default;

} // namespace gbhk
