#ifndef GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP
#define GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include <global_hotkey/key.hpp>

#ifdef GLOBAL_HOTKEY_LINUX

namespace gbhk
{

int modifiersToX11Modifiers(const Modifiers& modifiers) noexcept;

int keyToX11Keysym(const Key& key) noexcept;

Modifiers modifiersFromX11Modifiers(int x11Modifiers) noexcept;

Key keyFromX11Keysym(int x11Keysym) noexcept;

}

#endif // GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP
