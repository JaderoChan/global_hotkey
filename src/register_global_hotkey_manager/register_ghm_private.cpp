#include "register_ghm_private.hpp"

#ifdef GLOBAL_HOTKEY_WIN
    #include "register_ghm_private_win.hpp"
#elif defined(GLOBAL_HOTKEY_MAC)
    #include "register_ghm_private_mac.hpp"
#elif defined(GLOBAL_HOTKEY_LINUX)
    #include "register_ghm_private_x11.hpp"
#endif // GLOBAL_HOTKEY_WIN

namespace gbhk
{

RegisterGHMPrivate& RegisterGHMPrivate::getInstance()
{
#ifdef GLOBAL_HOTKEY_WIN
    static RegisterGHMPrivateWin instance;
#elif defined(GLOBAL_HOTKEY_MAC)
    static RegisterGHMPrivateMac instance;
#elif defined(GLOBAL_HOTKEY_LINUX)
    static RegisterGHMPrivateX11 instance;
#endif // GLOBAL_HOTKEY_WIN
    return instance;
}

RegisterGHMPrivate::RegisterGHMPrivate() = default;

RegisterGHMPrivate::~RegisterGHMPrivate() = default;

} // namespace gbhk
