cmake_minimum_required(VERSION 3.25)

project(kbdt VERSION 1.2.1 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(KBDT_MSG_PREFIX "[KBDT]")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "${KBDT_MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

if(NOT DEFINED KBDT_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(KBDT_MASTER_PROJECT ON)
    else()
        set(KBDT_MASTER_PROJECT OFF)
    endif()
endif()

option(KBDT_BUILD_SHARED "Build shared library" OFF)
set(KBDT_SHARED ${KBDT_BUILD_SHARED})
if(KBDT_BUILD_SHARED)
    message(STATUS "${KBDT_MSG_PREFIX} Shared library will be built.")
else()
    message(STATUS "${KBDT_MSG_PREFIX} Static library will be built.")
endif()

option(KBDT_BUILD_WITH_UTILS "Build with utilitys" ON)
set(KBDT_WITH_UTILS ${KBDT_BUILD_WITH_UTILS})
option(KBDT_BUILD_EXAMPLE "Build example" ${KBDT_MASTER_PROJECT})

set(SRC)
if(WIN32)
    file(GLOB_RECURSE SRC ${SRC} src/win/*.c)
elseif(APPLE)
    file(GLOB_RECURSE SRC ${SRC} src/mac/*.c)
elseif(LINUX)
    file(GLOB_RECURSE SRC ${SRC} src/linux/*.c)
endif()

file(GLOB SRC ${SRC} src/*.c)
if(KBDT_BUILD_WITH_UTILS)
    file(GLOB SRC ${SRC} src/utils/utils.c)
    if(WIN32)
        file(GLOB SRC ${SRC} src/utils/utils_win.c)
    elseif(APPLE)
        file(GLOB SRC ${SRC} src/utils/utils_mac.c)
    elseif(LINUX)
        file(GLOB SRC ${SRC} src/utils/utils_linux.c)
    endif()
endif()

if(KBDT_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRC})
    target_compile_definitions(${PROJECT_NAME} PRIVATE KBDT_BUILD_SHARED)
else()
    add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/config.h
)

include(GNUInstallDirs)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(WIN32)
    message(STATUS "${KBDT_MSG_PREFIX} Windows platform detected.")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
    set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
endif()

if(APPLE)
    message(STATUS "${KBDT_MSG_PREFIX} Apple platform detected.")

    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    if(COREFOUNDATION_LIBRARY)
        message(STATUS "${KBDT_MSG_PREFIX} Found CoreFoundation: ${COREFOUNDATION_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIBRARY})
    else()
        message(FATAL_ERROR "${KBDT_MSG_PREFIX} Not found CoreFoundation.")
    endif()

    find_library(CARBON_LIBRARY Carbon)
    if(CARBON_LIBRARY)
        message(STATUS "{KBDT_MSG_PREFIX} Found Carbon: ${CARBON_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${CARBON_LIBRARY})
    else()
        message(FATAL_ERROR "{KBDT_MSG_PREFIX} Not found Carbon.")
    endif()

    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    if(COREGRAPHICS_LIBRARY)
        message(STATUS "${KBDT_MSG_PREFIX} Found CoreGraphics: ${COREGRAPHICS_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_LIBRARY})
    else()
        message(FATAL_ERROR "${KBDT_MSG_PREFIX} Not found CoreGraphics.")
    endif()
endif()

if(LINUX)
    message(STATUS "${KBDT_MSG_PREFIX} Linux platform detected.")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
file(GLOB HEADERS ${CMAKE_CURRENT_BINARY_DIR}/include/*.h ${CMAKE_CURRENT_BINARY_DIR}/include/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kbdt)
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

if(KBDT_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
