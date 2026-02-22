#ifndef GLOBAL_HOTKEY_DEFINES_HPP
#define GLOBAL_HOTKEY_DEFINES_HPP

#include <global_hotkey/config.hpp>

#ifdef _MSVC_LANG
    #define GLOBAL_HOTKEY_CPPVERS     _MSVC_LANG
#else
    #define GLOBAL_HOTKEY_CPPVERS     __cplusplus
#endif // _MSVC_LANG

#if GLOBAL_HOTKEY_CPPVERS < 201103L
    #error "Only support C++11 and above"
#endif // GLOBAL_HOTKEY_CPPVERS < 201103L

#ifdef _WIN32
    #define GLOBAL_HOTKEY_WIN
#elif defined(__APPLE__)
    #define GLOBAL_HOTKEY_MAC
#elif defined(__linux__)
    #define GLOBAL_HOTKEY_LINUX
#else
    #error "Unsupported platform"
#endif // _WIN32

#ifdef GLOBAL_HOTKEY_WIN
    #ifdef GLOBAL_HOTKEY_BUILD_SHARED
        #define GLOBAL_HOTKEY_API __declspec(dllexport)
    #elif defined(GLOBAL_HOTKEY_SHARED)
        #define GLOBAL_HOTKEY_API __declspec(dllimport)
    #else
        #define GLOBAL_HOTKEY_API
    #endif // GLOBAL_HOTKEY_BUILD_SHARED
#else
    #define GLOBAL_HOTKEY_API __attribute__((visibility("default")))
#endif // GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DEFINES_HPP
