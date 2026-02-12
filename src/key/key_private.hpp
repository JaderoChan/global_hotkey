#ifndef GLOBAL_HOTKEY_KEY_PRIVATE_HPP
#define GLOBAL_HOTKEY_KEY_PRIVATE_HPP

#include <global_hotkey/key.hpp>

namespace gbhk
{

uint32_t modifiersToNativeModifiers(const Modifiers& modifiers) noexcept;

uint32_t keyToNativeKey(const Key& key) noexcept;

Modifiers modifiersFromNativeModifiers(uint32_t nativeModifiers) noexcept;

Key keyFromNativeKey(uint32_t nativeKey) noexcept;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_KEY_PRIVATE_HPP
