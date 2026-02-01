#include <global_hotkey/register_global_hotkey_manager.hpp>

#ifdef GLOBAL_HOTKEY_WIN
    #include "register_ghm_private_win.hpp"
#elif defined(GLOBAL_HOTKEY_MAC)
    #include "register_ghm_private_mac.hpp"
#elif defined(GLOBAL_HOTKEY_LINUX)
    #include "register_ghm_private_x11.hpp"
#endif // GLOBAL_HOTKEY_WIN

namespace gbhk
{

RegisterGlobalHotkeyManager& RegisterGlobalHotkeyManager::getInstance()
{
    static RegisterGlobalHotkeyManager instance;
    return instance;
}

RegisterGlobalHotkeyManager::RegisterGlobalHotkeyManager() :
#ifdef GLOBAL_HOTKEY_WIN
    GlobalHotkeyManager(std::unique_ptr<GHMPrivate>(new RegisterGHMPrivateWin()))
#elif defined(GLOBAL_HOTKEY_MAC)
    GlobalHotkeyManager(std::unique_ptr<GHMPrivate>(new RegisterGHMPrivateMac()))
#elif defined(GLOBAL_HOTKEY_LINUX)
    GlobalHotkeyManager(std::unique_ptr<GHMPrivate>(new RegisterGHMPrivateX11()))
#endif // GLOBAL_HOTKEY_WIN
{}

RegisterGlobalHotkeyManager::~RegisterGlobalHotkeyManager() = default;

} // namespace gbhk
