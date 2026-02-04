#ifndef KEYBOARD_TOOLS_DEFINES_HPP
#define KEYBOARD_TOOLS_DEFINES_HPP

#ifdef _WIN32
    #define KEYBOARD_TOOLS_WIN
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define KEYBOARD_TOOLS_MAC
    #else
        #error "Unsupported Apple platform"
    #endif // TARGET_OS_MAC
#elif defined(__linux__)
    #define KEYBOARD_TOOLS_LINUX
#else
    #error "Unsupported platform"
#endif // _WIN32

#ifdef KEYBOARD_TOOLS_WIN
    #ifdef KEYBOARD_TOOLS_BUILD_SHARED
        #define KEYBOARD_TOOLS_API __declspec(dllexport)
    #elif defined(KEYBOARD_TOOLS_SHARED)
        #define KEYBOARD_TOOLS_API __declspec(dllimport)
    #else
        #define KEYBOARD_TOOLS_API
    #endif // KEYBOARD_TOOLS_BUILD_SHARED
#else
    #define KEYBOARD_TOOLS_API __attribute__((visibility("default")))
#endif // KEYBOARD_TOOLS_WIN

/// @brief Return code for successful operation.
#define KBDT_RC_SUCCESS    0

#endif // !KEYBOARD_TOOLS_DEFINES_HPP
