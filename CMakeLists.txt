cmake_minimum_required(VERSION 3.25)

project(global_hotkey VERSION 2.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLOBAL_HOTKEY_MSG_PREFIX "[Global Hotkey]")

# Set the default CMAKE_BUILD_TYPE to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Projects cannot be built in the same-level directory of the source code.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

# Check whether the project is the root project.
if(NOT DEFINED GLOBAL_HOTKEY_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(GLOBAL_HOTKEY_MASTER_PROJECT ON)
    else()
        set(GLOBAL_HOTKEY_MASTER_PROJECT OFF)
    endif()
endif()

# Process the "GLOBAL_HOTKEY_DISABLE_REGISTER" option to collect relevant files。
set(SRC)
option(GLOBAL_HOTKEY_DISABLE_REGISTER "Disable register Global Hotkey Manager" OFF)
if(GLOBAL_HOTKEY_DISABLE_REGISTER)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Register Global Hotkey Manager be disable.")
else()
    list(
        APPEND SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/register_global_hotkey_manager/register_global_hotkey_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/register_global_hotkey_manager/register_ghm_private.cpp
    )

    if(WIN32)
        file(GLOB SRC ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/register_global_hotkey_manager/*win*.cpp)
    elseif(APPLE)
        file(GLOB SRC ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/register_global_hotkey_manager/*mac*.cpp)
    elseif(LINUX)
        file(GLOB SRC ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/register_global_hotkey_manager/*x11*.cpp)
    endif()
endif()

# Process the "GLOBAL_HOTKEY_DISABLE_HOOK" option to collect relevant files。
option(GLOBAL_HOTKEY_DISABLE_HOOK "Disable hook Global Hotkey Manager" OFF)
if(GLOBAL_HOTKEY_DISABLE_HOOK)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Hook Global Hotkey Manager be disable.")
else()
    list(
        APPEND SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hook_global_hotkey_manager/hook_global_hotkey_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hook_global_hotkey_manager/hook_ghm_private.cpp
    )
endif()

# Hook GHM and Register GHM cannot be disabled simultaneously.
if(GLOBAL_HOTKEY_DISABLE_HOOK AND GLOBAL_HOTKEY_DISABLE_REGISTER)
    message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Cannot set both GLOBAL_HOTKEY_DISABLE_REGISTER and GLOBAL_HOTKEY_DISABLE_HOOK to ON.")
endif()

# Collect the necessary source files.
list(
    APPEND SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/global_hotkey_manager/global_hotkey_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/global_hotkey_manager/ghm_private.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/key/key.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/key_combination/key_combination.cpp
)

# Collect key-related source files based on the platform and build options.
if(WIN32)
    list(
        APPEND SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/key/key_private_win.cpp
    )
elseif(APPLE)
    list(
        APPEND SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/key/key_private_mac.cpp
    )
elseif(LINUX)
    if(NOT GLOBAL_HOTKEY_DISABLE_REGISTER)
        list(
            APPEND SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/key/key_private_x11.cpp
        )
    endif()
    if(NOT GLOBAL_HOTKEY_DISABLE_HOOK)
        list(
            APPEND SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/key/key_private_linux.cpp
        )
    endif()
endif()

#########
# Build #
#########

option(GLOBAL_HOTKEY_BUILD_SHARED "Build shared library" OFF)
if(GLOBAL_HOTKEY_BUILD_SHARED)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Shared library will be built.")
    add_library(${PROJECT_NAME} SHARED ${SRC})
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLOBAL_HOTKEY_BUILD_SHARED)
else()
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Static library will be built.")
    add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set(GLOBAL_HOTKEY_SHARED ${GLOBAL_HOTKEY_BUILD_SHARED})

####################
# Configure target #
####################

option(
    GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY
    "This option is only for Hook GHM on the Windows platform. If this option is enabled, when the hotkeys 'Ctrl+Shift+ESC' and 'Ctrl+Alt+Delete' are triggered, some tricks will be used to try to prevent abnormal program behavior."
    ON
)
if(GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY)
endif()

# Set the suffix of the library file in Windows.
if(WIN32)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Windows platform detected.")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
    set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

########################
# Configure dependency #
########################

set(CONFIG_HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/include/global_hotkey/config.hpp)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/config.hpp.in
    ${CONFIG_HEADER_PATH}
)

include(GNUInstallDirs)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(NOT GLOBAL_HOTKEY_DISABLE_HOOK)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Start configure the 'keyboard tools' library...")
    set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
    include(FetchContent)
    FetchContent_Declare(
        keyboard_tools
        SOURCE_DIR ${THIRDPARTY_DIR}/keyboard_tools
    )
    set(KEYBOARD_TOOLS_WITH_KEYUTIL OFF)
    FetchContent_MakeAvailable(keyboard_tools)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Success to configure the 'keyboard tools' library.")
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
        message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Found Carbon: ${CARBON_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${CARBON_LIBRARY})
    else()
        message(FATAL_ERROR "${GLOBAL_HOTKEY_MSG_PREFIX} Not found Carbon.")
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
    target_link_libraries(${PROJECT_NAME} PRIVATE keyboard_tools::keyboard_tools)
endif()

###########
# Install #
###########

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/global_hotkey/*.hpp)
if(GLOBAL_HOTKEY_DISABLE_HOOK)
    list(FILTER HEADERS EXCLUDE REGEX ".*hook_global_hotkey_manager\.hpp")
endif()
if(GLOBAL_HOTKEY_DISABLE_REGISTER)
    list(FILTER HEADERS EXCLUDE REGEX ".*register_global_hotkey_manager\.hpp")
endif()
list(APPEND HEADERS ${CONFIG_HEADER_PATH})
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

#################
# Build Example #
#################

option(GLOBAL_HOTKEY_BUILD_EXAMPLE "Build example" ${GLOBAL_HOTKEY_MASTER_PROJECT})
if(GLOBAL_HOTKEY_BUILD_EXAMPLE)
    message(STATUS "${GLOBAL_HOTKEY_MSG_PREFIX} Example will be built.")
    add_subdirectory(example)
endif()
