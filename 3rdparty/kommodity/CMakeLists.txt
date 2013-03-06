project(kommodity)

cmake_minimum_required(VERSION 2.8.9)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(GLIB2 REQUIRED)
find_package(GIO REQUIRED)
find_package(GObject REQUIRED)

find_package(Qt5Core REQUIRED)
if(NOT Qt5Core_FOUND)
    message(FATAL_ERROR "Qt5Core module is required!")
endif()
#macro_log_feature(Qt5Core_FOUND "Qt5Core" "Support for Qt5Core" "http://qt-project.org" "")

# include(MacroLibrary)

SET(KOMMODITY_INCLUDE_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/include/Kommodity"
    CACHE PATH "The subdirectory relative to the install prefix for header files (default is include/Kommodity)."
    FORCE
)

add_subdirectory( include )
add_subdirectory( library )
# add_subdirectory( tests )
add_subdirectory( samples )

