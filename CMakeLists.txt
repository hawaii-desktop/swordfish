project(Swordfish)

cmake_minimum_required(VERSION 2.8.9)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set version
set(SWORDFISH_VERSION_MAJOR 0)
set(SWORDFISH_VERSION_MINOR 0)
set(SWORDFISH_VERSION_PATCH 0)
set(SWORDFISH_VERSION_STRING "${SWORDFISH_VERSION_MAJOR}.${SWORDFISH_VERSION_MINOR}.${SWORDFISH_VERSION_PATCH}")

# Installation directories
include(HawaiiInstallDirs)

# Log features
include(MacroLogFeature)

# Add custom uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Set up packaging
set(CPACK_PACKAGE_NAME hawaii-swordfish)
set(CPACK_PACKAGE_VERSION ${SWORDFISH_VERSION_STRING})
set(CPACK_SYSTEM_NAME "generic")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SET_DESTDIR FALSE)
include(CPack)

# Build flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -Wall -std=c++0x")

# Assume release build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif()

# Disable debug output for release builds
if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Find Qt
find_package(Qt5LinguistTools REQUIRED)
if(NOT Qt5LinguistTools_FOUND)
    message(FATAL_ERROR "Qt5LinguistTools module is required!")
endif()
macro_log_feature(Qt5LinguistTools_FOUND "Qt5LinguistTools" "Support for Qt5LinguistTools" "http://qt-project.org" "")

# Find Vibe libraries
find_package(VibeWidgets REQUIRED)
if(NOT VibeWidgets_FOUND)
    message(FATAL_ERROR "VibeWidgets module is required!")
endif()
macro_log_feature(VibeWidgets_FOUND "VibeWidgets" "Support for VibeWidgets" "http://www.maui-project.org" "")

# Find pkg-config executable
find_program(PKGCONFIG_EXECUTABLE pkg-config)
if(NOT PKGCONFIG_EXECUTABLE)
    message(FATAL_ERROR "pkg-config is not installed!")
endif()

# Find DBus session bus services directory
execute_process(COMMAND ${PKGCONFIG_EXECUTABLE} --variable=session_bus_services_dir dbus-1
    OUTPUT_VARIABLE DBUS_SESSION_BUS_SERVICES_DIR
    RESULT_VARIABLE _failed)
if(_failed)
    message(FATAL_ERROR "Cannot find session_bus_services_dir propery from DBus: ${_failed}")
endif(_failed)
string(REGEX REPLACE "[ \t\n]+" \; DBUS_SESSION_BUS_SERVICES_DIR "${DBUS_SESSION_BUS_SERVICES_DIR}")

# Subdirectories
add_subdirectory(data)
add_subdirectory(src)

# Display featute log
macro_display_feature_log()
