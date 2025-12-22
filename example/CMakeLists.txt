cmake_minimum_required(VERSION 3.25)

project(global_hotkey_examples LANGUAGES CXX)

option(GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK "Build example using hook Global Hotkey Manager" OFF)
if(GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Build example using hook Global Hotkey Manager.")
endif()

if(GLOBAL_HOTKEY_DISABLE_REGISTER AND NOT GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Cannot build example using register Global Hotkey Manager with GLOBAL_HOTKEY_DISABLE_REGISTER.")
elseif(GLOBAL_HOTKEY_DISABLE_HOOK AND GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Cannot build example using hook Global Hotkey Manager with GLOBAL_HOTKEY_DISABLE_HOOK.")
endif()

option(GLOBAL_HOTKEY_EXAMPLE_BUILD_SIMPLE "Build simple example" ON)
option(GLOBAL_HOTKEY_EXAMPLE_BUILD_EVENT_QUEUE "Build event queue example" ON)

if(GLOBAL_HOTKEY_EXAMPLE_BUILD_SIMPLE)
    add_subdirectory(simple)
endif()

if(GLOBAL_HOTKEY_EXAMPLE_BUILD_EVENT_QUEUE)
    add_subdirectory(event_queue)
endif()
