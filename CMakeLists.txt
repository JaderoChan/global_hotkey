cmake_minimum_required(VERSION 3.25)

project(global_hotkey VERSION 1.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(GLOBAL_HOTKEY_MSG_PREFIX "[Global Hotkey]")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

if(NOT DEFINED GLOBAL_HOTKEY_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(GLOBAL_HOTKEY_MASTER_PROJECT ON)
    else()
        set(GLOBAL_HOTKEY_MASTER_PROJECT OFF)
    endif()
endif()

option(GLOBAL_HOTKEY_BUILD_SHARED "Build shared library" OFF)
set(GLOBAL_HOTKEY_SHARED ${GLOBAL_HOTKEY_BUILD_SHARED})
if(GLOBAL_HOTKEY_BUILD_SHARED)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Shared library will be built.")
else()
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Static library will be built.")
endif()

option(GLOBAL_HOTKEY_DISABLE_HOOK "Disable hook Global Hotkey Manager" OFF)
if(GLOBAL_HOTKEY_DISABLE_HOOK)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Hook Global Hotkey Manager be disable.")
endif()

set(SRC)
option(GLOBAL_HOTKEY_DISABLE_REGISTER "Disable register Global Hotkey Manager" OFF)
if(GLOBAL_HOTKEY_DISABLE_REGISTER)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Register Global Hotkey Manager be disable.")
else()
    set(
        SRC ${SRC}
        src/register_global_hotkey_manager/register_global_hotkey_manager.cpp
        src/register_global_hotkey_manager/register_ghm_private.cpp
    )
    if(WIN32)
        file(GLOB_RECURSE SRC ${SRC} src/register_global_hotkey_manager/*win*.cpp)
    elseif(APPLE)
        file(GLOB_RECURSE SRC ${SRC} src/register_global_hotkey_manager/*mac*.cpp)
    elseif(LINUX)
        file(GLOB_RECURSE SRC ${SRC} src/register_global_hotkey_manager/*x11*.cpp)
    endif()
endif()

if(GLOBAL_HOTKEY_DISABLE_HOOK AND GLOBAL_HOTKEY_DISABLE_REGISTER)
    message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Cannot set both GLOBAL_HOTKEY_DISABLE_REGISTER and GLOBAL_HOTKEY_DISABLE_HOOK to ON.")
endif()

option(GLOBAL_HOTKEY_BUILD_EXAMPLE "Build example" ${GLOBAL_HOTKEY_MASTER_PROJECT})

file(
    GLOB_RECURSE SRC ${SRC}
    src/global_hotkey_manager/*.cpp
    src/key/key.cpp
    src/key_combination/key_combination.cpp
)
if(NOT GLOBAL_HOTKEY_DISABLE_HOOK)
    file(GLOB_RECURSE SRC ${SRC} src/hook_global_hotkey_manager/*.cpp)
endif()

if(WIN32)
    file(GLOB_RECURSE SRC ${SRC} src/key/*win*.cpp)
elseif(APPLE)
    file(GLOB_RECURSE SRC ${SRC} src/key/*mac*.cpp)
elseif(LINUX)
    if(NOT GLOBAL_HOTKEY_DISABLE_REGISTER)
        file(GLOB_RECURSE SRC ${SRC} src/key/*x11*.cpp)
    endif()
    if(NOT GLOBAL_HOTKEY_DISABLE_HOOK)
        file(GLOB_RECURSE SRC ${SRC} src/key/*linux*.cpp)
    endif()
endif()

if(GLOBAL_HOTKEY_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRC})
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLOBAL_HOTKEY_BUILD_SHARED)
else()
    add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

option(
    GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY
    "This option is only for Hook GHM on the Windows platform. If this option is enabled, when the hotkeys 'Ctrl+Shift+ESC' and 'Ctrl+Alt+Delete' are triggered, some tricks will be used to try to prevent abnormal program behavior."
    ON
)
if(GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/config.hpp
)

include(GNUInstallDirs)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

if(NOT GLOBAL_HOTKEY_DISABLE_HOOK)
    message(STATUS "Start configure the 'keyboard tool' library...")
    include(FetchContent)
    FetchContent_Declare(
        kbdt
        SOURCE_DIR ${THIRDPARTY_DIR}/kbdt
    )
    set(KBDT_BUILD_WITH_UTILS OFF)
    FetchContent_MakeAvailable(kbdt)
    message(STATUS "Success to configure the 'keyboard tool' library.")
endif()

if(WIN32)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Windows platform detected.")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
    set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
endif()

if(APPLE)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Apple platform detected.")

    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    if(COREFOUNDATION_LIBRARY)
        message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Found CoreFoundation: ${COREFOUNDATION_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIBRARY})
    else()
        message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Not found CoreFoundation.")
    endif()

    find_library(CARBON_LIBRARY Carbon)
    if(CARBON_LIBRARY)
        message(STATUS "{GLOBAL_HOTKEY_MSG_PREFIX} Found Carbon: ${CARBON_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${CARBON_LIBRARY})
    else()
        message(FATAL_ERROR "{GLOBAL_HOTKEY_MSG_PREFIX} Not found Carbon.")
    endif()

    if(NOT GLOBAL_HOTKEY_DISABLE_HOOK)
        find_library(COREGRAPHICS_LIBRARY CoreGraphics)
        if(COREGRAPHICS_LIBRARY)
            message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Found CoreGraphics: ${COREGRAPHICS_LIBRARY}.")
            target_link_libraries(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_LIBRARY})
        else()
            message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Not found CoreGraphics.")
        endif()
    endif()
endif()

if(LINUX)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Linux platform detected.")

    if(NOT GLOBAL_HOTKEY_DISABLE_REGISTER)
        find_path(X11_INCLUDE_DIR X11/Xlib.h PATHS usr/include)
        find_library(X11_LIBRARY NAMES X11 PATHS /usr/lib /usr/lib/x86_64-linux-gnu)
        if(X11_INCLUDE_DIR AND X11_LIBRARY)
            message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Found X11: ${X11_LIBRARY}.")
            target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})
            target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARY})
        else()
            message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Not found X11.")
        endif()
    endif()
endif()

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
if(NOT GLOBAL_HOTKEY_DISABLE_HOOK)
    target_link_libraries(${PROJECT_NAME} PRIVATE kbdt::kbdt)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
file(GLOB HEADERS ${CMAKE_CURRENT_BINARY_DIR}/include/*.h ${CMAKE_CURRENT_BINARY_DIR}/include/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/global_hotkey)
install(DIRECTORY doc/ DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME})

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

if(GLOBAL_HOTKEY_BUILD_EXAMPLE)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Example will be built.")
    add_subdirectory(example)
endif()

export(PACKAGE ${PROJECT_NAME})
