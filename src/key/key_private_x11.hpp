#ifndef GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP
#define GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP

#include <global_hotkey/key.hpp>

namespace gbhk
{

uint32_t modifiersToX11Modifiers(const Modifiers& modifiers) noexcept;

uint32_t keyToX11Keysym(const Key& key) noexcept;

Modifiers modifiersFromX11Modifiers(uint32_t x11Modifiers) noexcept;

Key keyFromX11Keysym(uint32_t x11Keysym) noexcept;

}

#endif // !GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP
