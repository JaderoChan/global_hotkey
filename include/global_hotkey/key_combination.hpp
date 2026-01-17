#ifndef GLOBAL_HOTKEY_KEY_COMBINATION_HPP
#define GLOBAL_HOTKEY_KEY_COMBINATION_HPP

#include <functional>   // hash
#include <string>       // string

#include "defines.hpp"
#include "key.hpp"

namespace gbhk
{

class GLOBAL_HOTKEY_API KeyCombination
{
public:
    constexpr inline KeyCombination() noexcept = default;
    constexpr inline KeyCombination(const Modifiers& modifiers, const Key& key) noexcept :
        mod_(modifiers), key_(key) {}
    constexpr inline explicit KeyCombination(int64_t toCombinedValue) noexcept :
        mod_((int32_t) (toCombinedValue >> 32)), key_((int32_t) toCombinedValue) {}
    KeyCombination(const std::string& str, char connector = '+') noexcept
    { *this = std::move(fromString(str, '+')); }
    KeyCombination(const char* str, char connector = '+') noexcept :
        KeyCombination(std::string(str), connector) {}

    static KeyCombination fromString(const std::string& str, char connector = '+') noexcept;
    std::string toString(char connector = '+', bool showKeyValue = false) const noexcept;

    static constexpr inline KeyCombination fromCombinedValue(int64_t value) noexcept
    { return KeyCombination((int32_t) (value >> 32), (int32_t) value); }
    constexpr inline int64_t toCombinedValue() const noexcept
    { return ((int64_t) mod_ << 32) | ((int64_t) key_ << 0); }

    constexpr inline Modifiers modifiers() const noexcept { return mod_; }
    constexpr inline Key key() const noexcept { return key_; }

    /// @brief Check if this key combination represents a valid hotkey.
    /// @return true if the combination contains at least one modifier and a valid key value, false otherwise.
    constexpr inline bool isValid() const noexcept { return mod_.isValid() && key_.isValid(); }

#if GLOBAL_HOTKEY_CPPVERS >= 201703L
    // In C++17, constexpr member functions are no longer implicitly const.
    constexpr inline void setModifiers(const Modifiers& modifiers) noexcept { mod_ = modifiers; }
    constexpr inline void setKey(const Key& key) noexcept { key_ = key; }
#else
    inline void setModifiers(const Modifiers& modifiers) noexcept { mod_ = modifiers; }
    inline void setKey(const Key& key) noexcept { key_ = key; }
#endif // GLOBAL_HOTKEY_CPPVERS >= 201703L

    friend constexpr inline bool operator==(const KeyCombination& lhs, const KeyCombination& rhs) noexcept
    { return lhs.mod_ == rhs.mod_ && lhs.key_ == rhs.key_; }
    friend constexpr inline bool operator!=(const KeyCombination& lhs, const KeyCombination& rhs) noexcept
    { return lhs.mod_ != rhs.mod_ || lhs.key_ != rhs.key_; }

private:
    friend struct std::hash<KeyCombination>;

    Modifiers mod_ = 0;
    Key key_       = 0;
};

} // namespace gbhk

namespace std
{

template <>
struct hash<gbhk::KeyCombination>
{
    size_t operator()(const gbhk::KeyCombination& obj) const noexcept
    {
        size_t h1 = std::hash<int32_t>()(obj.mod_);
        size_t h2 = std::hash<int32_t>()(obj.key_);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std

#endif // !GLOBAL_HOTKEY_KEY_COMBINATION_HPP
