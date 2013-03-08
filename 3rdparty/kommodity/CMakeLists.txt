find_package(PkgConfig REQUIRED)

pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

add_subdirectory( library )
# add_subdirectory( tests )
add_subdirectory( samples )
