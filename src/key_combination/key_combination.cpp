#include <string>   // to_string

#include <global_hotkey/key_combination.hpp>

namespace gbhk
{

KeyCombination KeyCombination::fromString(const std::string& str, char connector) noexcept
{
    size_t pos = str.rfind(connector);
    if (pos == std::string::npos)
        return {};
    Modifiers mod(str.substr(0, pos));
    Key key(str.substr(pos + 1));
    return {mod, key};
}

std::string KeyCombination::toString(ModifierTextFormat format, char connector, bool showKeyValue) const noexcept
{
    std::string str;
    std::string connectorStr(1, connector);
    str += mod_.toString(format, connector);
    str += (!str.empty() ? connectorStr : "") + key_.toString();
    if (showKeyValue)
        str += "(" + std::to_string(key_) + ")";
    return str;
}

} // namespace gbhk
