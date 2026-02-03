cmake_minimum_required(VERSION 3.25)

project(simple LANGUAGES CXX)

if(NOT TARGET global_hotkey)
    find_package(global_hotkey REQUIRED)
endif()

add_executable(${PROJECT_NAME} main.cpp)

if(GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK)
    if(APPLE)
        # For AXIsProcessTrustedWithOptions.
        find_library(AX_LIBRARY ApplicationServices)
        if(AX_LIBRARY)
            message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Found ApplicationServices: ${AX_LIBRARY}.")
            target_link_libraries(${PROJECT_NAME} PRIVATE ${AX_LIBRARY})
        else()
            message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Not found ApplicationServices.")
        endif()
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE global_hotkey::global_hotkey)
