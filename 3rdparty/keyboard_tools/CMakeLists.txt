cmake_minimum_required(VERSION 3.25)

project(keyboard_tools VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(KBDT_MSG_PREFIX "[Keyboard Tools]")

# Set the default CMAKE_BUILD_TYPE to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Projects cannot be built in the same-level directory of the source code.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "${KBDT_MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

# Check whether the project is the root project.
if(NOT DEFINED KBDT_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(KBDT_MASTER_PROJECT ON)
    else()
        set(KBDT_MASTER_PROJECT OFF)
    endif()
endif()

# Collect source files.
set(SRC)
list(
    APPEND SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kbdt.cpp
)
if(WIN32)
    file(GLOB SRC ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/win/*.cpp)
elseif(APPLE)
    file(GLOB SRC ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/mac/*.cpp)
elseif(LINUX)
    file(GLOB SRC ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/*.cpp)
endif()

option(KBDT_WITH_KEYUTILS "Build with key utilitys" ON)
if(KBDT_WITH_KEYUTILS)
    list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/keyutils/keyutils.cpp)
    if(WIN32)
        list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/keyutils/keyutils_win.cpp)
    elseif(APPLE)
        list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/keyutils/keyutils_mac.cpp)
    elseif(LINUX)
        list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/keyutils/keyutils_linux.cpp)
    endif()
endif()

#########
# Build #
#########

option(KBDT_BUILD_SHARED "Build shared library" OFF)
if(KBDT_BUILD_SHARED)
    message(STATUS "${KBDT_MSG_PREFIX} Shared library will be built.")
    add_library(${PROJECT_NAME} SHARED ${SRC})
    target_compile_definitions(${PROJECT_NAME} PRIVATE KBDT_BUILD_SHARED)
else()
    message(STATUS "${KBDT_MSG_PREFIX} Static library will be built.")
    add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set(KBDT_SHARED ${KBDT_BUILD_SHARED})

####################
# Configure target #
####################

if(WIN32)
    message(STATUS "${KBDT_MSG_PREFIX} Windows platform detected.")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
    set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

########################
# Configure dependency #
########################

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/config.hpp
)

include(GNUInstallDirs)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

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

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

###########
# Install #
###########

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/kbdt/*.hpp)
if(NOT KBDT_WITH_KEYUTILS)
    list(FILTER HEADERS EXCLUDE REGEX ".*keyutils\.hpp")
endif()
list(APPEND HEADERS ${CMAKE_CURRENT_BINARY_DIR}/include/config.hpp)
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

#################
# Build Example #
#################

option(KBDT_BUILD_EXAMPLE "Build example" ${KBDT_MASTER_PROJECT})
if(KBDT_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
