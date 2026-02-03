#ifndef KBDT_DEFINES_HPP
#define KBDT_DEFINES_HPP

#ifdef _WIN32
    #define KBDT_WIN
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define KBDT_MAC
    #else
        #error "Unsupported Apple platform"
    #endif // TARGET_OS_MAC
#elif defined(__linux__)
    #define KBDT_LINUX
#else
    #error "Unsupported platform"
#endif // _WIN32

#ifdef KBDT_WIN
    #ifdef KBDT_BUILD_SHARED
        #define KBDT_API __declspec(dllexport)
    #elif defined(KBDT_SHARED)
        #define KBDT_API __declspec(dllimport)
    #else
        #define KBDT_API
    #endif // KBDT_BUILD_SHARED
#else
    #define KBDT_API __attribute__((visibility("default")))
#endif // KBDT_WIN

/// @brief Return code for successful operation.
#define KBDT_RC_SUCCESS     0

#endif // !KBDT_DEFINES_HPP
