#ifndef GLOBAL_HOTKEY_KEY_PRIVATE_HPP
#define GLOBAL_HOTKEY_KEY_PRIVATE_HPP

#include <global_hotkey/key.hpp>

namespace gbhk
{

int modifiersToNativeModifiers(const Modifiers& modifiers) noexcept;

int keyToNativeKey(const Key& key) noexcept;

Modifiers modifiersFromNativeModifiers(int nativeModifiers) noexcept;

Key keyFromNativeKey(int nativeKey) noexcept;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_KEY_PRIVATE_HPP
