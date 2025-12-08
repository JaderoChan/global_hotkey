#ifndef GLOBAL_HOTKEY_RETURN_CODE_HPP
#define GLOBAL_HOTKEY_RETURN_CODE_HPP

#include <string>   // string, to_string

#include "def.hpp"

namespace gbhk
{

#ifdef GLOBAL_HOTKEY_WIN
    constexpr int RC_MASK = 1 << 29;
#elif defined(GLOBAL_HOTKEY_MAC)
    constexpr int RC_MASK = 0;
#elif defined(GLOBAL_HOTKEY_LINUX)
    constexpr int RC_MASK = 0;
#else
    constexpr int RC_MASK = 0;
#endif // GLOBAL_HOTKEY_WIN

enum ReturnCode : int
{
    RC_SUCCESS              = 0x00 | RC_MASK,
    RC_BAD_TIMING           = 0x10 | RC_MASK,
    RC_BAD_THREAD           = 0x11 | RC_MASK
};

inline std::string getReturnCodeMsg(int rc) noexcept
{
    switch (rc)
    {
        case RC_SUCCESS:
            return "Success";
        case RC_BAD_TIMING:
            return "Can't perform stop, add, remove, replace and the like functions before the GHM is start";
        case RC_BAD_THREAD:
            return "Can't perform stop, add, remove, replace and the like functions in the worker thread \
(usually is the hotkey callback functions)";
        default:
            return "A return value depends on the platform or framwork: " + std::to_string(rc);
    }
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_RETURN_CODE_HPP
