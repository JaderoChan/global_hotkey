#ifndef GLOBAL_HOTKEY_KEY_PRIVATE_HPP
#define GLOBAL_HOTKEY_KEY_PRIVATE_HPP

#include <global_hotkey/key.hpp>

namespace gbhk
{

int32_t modifiersToNativeModifiers(const Modifiers& modifiers) noexcept;

int32_t keyToNativeKey(const Key& key) noexcept;

Modifiers modifiersFromNativeModifiers(int32_t nativeModifiers) noexcept;

Key keyFromNativeKey(int32_t nativeKey) noexcept;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_KEY_PRIVATE_HPP
