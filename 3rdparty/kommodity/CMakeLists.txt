project(kommodity)

cmake_minimum_required(VERSION 2.6.0)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(GLIB2 REQUIRED)
find_package(GIO REQUIRED)
find_package(GObject REQUIRED)
find_package(Qt4 REQUIRED)

add_definitions(${QT_DEFINITIONS})

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

