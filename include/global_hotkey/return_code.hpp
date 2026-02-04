#ifndef GLOBAL_HOTKEY_RETURN_CODE_HPP
#define GLOBAL_HOTKEY_RETURN_CODE_HPP

#include <string>   // string, to_string

#include "defines.hpp"

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
    RC_ALREADY_EXIST        = 0x01 | RC_MASK,
    RC_NOT_FOUND            = 0x02 | RC_MASK,
    RC_BAD_TIMING           = 0x10 | RC_MASK,
    RC_BAD_THREAD           = 0x11 | RC_MASK
};

inline std::string getReturnCodeMessage(int rc) noexcept
{
    switch (rc)
    {
        case RC_SUCCESS:
            return "Success";
        case RC_ALREADY_EXIST:
            return "The hotkey to be registered already exists";
        case RC_NOT_FOUND:
            return "The hotkey to be unregistered or set does not exist";
        case RC_BAD_TIMING:
            return "Attempt stop GHM service, register hotkey, unregister hotkey, replace hotkey, or similar functions "
                   "before the Global Hotkey Manager is running";
        case RC_BAD_THREAD:
            return "Attempt stop GHM service, register hotkey, unregister hotkey, replace hotkey, or similar functions "
                   "in the worker thread (typically within hotkey callbacks)";
        default:
            return "An error related to the platform or framework dependent occurred, error code: " +
                   std::to_string(rc);
    }
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_RETURN_CODE_HPP
