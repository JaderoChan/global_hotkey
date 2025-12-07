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

std::string KeyCombination::toString(char connector, bool showKeyValue) const noexcept
{
    std::string rslt;
    std::string connectorStr(1, connector);
    rslt += mod_.toString(connector);
    rslt += (!rslt.empty() ? connectorStr : "") + key_.toString();
    if (showKeyValue)
        rslt += "(" + std::to_string(key_) + ")";
    return rslt;
}

} // namespace gbhk
