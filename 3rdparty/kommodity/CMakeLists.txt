find_package(PkgConfig REQUIRED)

pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

SET(KOMMODITY_INCLUDE_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/include/Kommodity"
    CACHE PATH "The subdirectory relative to the install prefix for header files (default is include/Kommodity)."
    FORCE
)

add_subdirectory( include )
add_subdirectory( library )
# add_subdirectory( tests )
add_subdirectory( samples )
