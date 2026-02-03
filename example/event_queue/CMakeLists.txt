cmake_minimum_required(VERSION 3.25)

project(event_queue LANGUAGES CXX)

if(NOT TARGET global_hotkey)
    find_package(global_hotkey REQUIRED)
endif()

add_executable(${PROJECT_NAME}
    event_work.cpp
    utilitys.cpp
    main.cpp
)

if(GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE global_hotkey::global_hotkey)
