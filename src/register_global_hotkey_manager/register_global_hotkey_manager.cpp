#include <global_hotkey/register_global_hotkey_manager.hpp>

#include "register_ghm_private.hpp"

namespace gbhk
{

RegisterGlobalHotkeyManager& RegisterGlobalHotkeyManager::getInstance()
{
    static RegisterGlobalHotkeyManager instance;
    return instance;
}

RegisterGlobalHotkeyManager::RegisterGlobalHotkeyManager() :
    GlobalHotkeyManager(RegisterGHMPrivate::getInstance())
{}

RegisterGlobalHotkeyManager::~RegisterGlobalHotkeyManager() = default;

} // namespace gbhk
