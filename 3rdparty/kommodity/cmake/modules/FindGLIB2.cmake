# - Try to find the GLIB2 libraries
# Once done this will define
#
#  GLIB2_FOUND - system has glib2
#  GLIB2_INCLUDE_DIR - the glib2 include directory
#  GLIB2_LIBRARIES - glib2 library

if(GLIB2_INCLUDE_DIR AND GLIB2_LIBRARIES)
    # Already in cache, be silent
    set(GLIB2_FIND_QUIETLY TRUE)
endif(GLIB2_INCLUDE_DIR AND GLIB2_LIBRARIES)

if (NOT WIN32)
   include(UsePkgConfig)
   pkgconfig(glib-2.0 _LibGLIB2IncDir _LibGLIB2LinkDir _LibGLIB2LinkFlags _LibGLIB2Cflags)
endif(NOT WIN32)

MESSAGE(STATUS "glib include dir: ${_LibGLIB2IncDir}")

# first try without default paths to respect PKG_CONFIG_PATH

find_path(GLIB2_MAIN_INCLUDE_DIR glib.h
          PATH_SUFFIXES glib-2.0
          PATHS ${_LibGLIB2IncDir} 
          NO_DEFAULT_PATH)

find_path(GLIB2_MAIN_INCLUDE_DIR glib.h
          PATH_SUFFIXES glib-2.0
          PATHS ${_LibGLIB2IncDir})

          
MESSAGE(STATUS "found glib main include dir: ${GLIB2_MAIN_INCLUDE_DIR}")

# search the glibconfig.h include dir under the same root where the library is found

find_library(GLIB2_LIBRARIES 
             NAMES glib-2.0 
             PATHS ${_LibGLIB2LinkDir} 
             NO_DEFAULT_PATH)

find_library(GLIB2_LIBRARIES 
             NAMES glib-2.0 
             PATHS ${_LibGLIB2LinkDir})

get_filename_component(glib2LibDir "${GLIB2_LIBRARIES}" PATH)

find_path(GLIB2_INTERNAL_INCLUDE_DIR glibconfig.h
          PATH_SUFFIXES glib-2.0/include
          PATHS ${_LibGLIB2IncDir} "${glib2LibDir}" ${CMAKE_SYSTEM_LIBRARY_PATH}
          NO_DEFAULT_PATH)

find_path(GLIB2_INTERNAL_INCLUDE_DIR glibconfig.h
          PATH_SUFFIXES glib-2.0/include
          PATHS ${_LibGLIB2IncDir} "${glib2LibDir}" ${CMAKE_SYSTEM_LIBRARY_PATH})

set(GLIB2_INCLUDE_DIR "${GLIB2_MAIN_INCLUDE_DIR}")

# not sure if this include dir is optional or required
# for now it is optional
if(GLIB2_INTERNAL_INCLUDE_DIR)
  set(GLIB2_INCLUDE_DIR ${GLIB2_INCLUDE_DIR} "${GLIB2_INTERNAL_INCLUDE_DIR}")
endif(GLIB2_INTERNAL_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLIB2  DEFAULT_MSG  GLIB2_LIBRARIES GLIB2_MAIN_INCLUDE_DIR)

mark_as_advanced(GLIB2_INCLUDE_DIR GLIB2_LIBRARIES)
