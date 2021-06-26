# FindTidy.cmake
#
# Finds the tidy library
# from  https://www.html-tidy.org/
#
# This will define the following variables
#
#    Tidy_FOUND
#    tidy_INCLUDE_DIRS
#    tidy_LIBRARY
#
# and the following imported targets
#
#     Tidy::Tidy
#
# Author: Simon Meaden - simon.meaden@virginmedia.com
SET(Tidy_SEARCH_PATHS
  /usr/local
  /usr
  /opt
)

find_package(PkgConfig)
pkg_search_module(tidy REQUIRED)

find_path(tidy_INCLUDE_DIR
    NAMES tidy.h tidybuffio.h tidyenum.h tidyplatform.h
    PATH_SUFFIXES include
    PATHS ${tidy_SEARCH_PATHS}
)

find_library(tidy_LIBRARY
    NAMES tidy
    PATH_SUFFIXES lib64 lib
    PATHS ${tidy_SEARCH_PATHS}
)

if (tidy_FOUND)
  message("Tidy found : " ${Tidy_LIBRARY})
  message("Includes : " ${Tidy_INCLUDE_DIR})
endif()

#set(tidy_VERSION ${PC_Tidy_VERSION})

mark_as_advanced(Tidy_FOUND tidy_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Tidy
    REQUIRED_VARS tidy_INCLUDE_DIR
)

if(tidy_FOUND)
    set(tidy_INCLUDE_DIRS ${tidy_INCLUDE_DIR})
endif()

if(tidy_FOUND AND NOT TARGET tidy)
    add_library(tidy SHARED IMPORTED)
    set_target_properties(tidy PROPERTIES
        PUBLIC_INCLUDE_DIRECTORIES "${tidy_INCLUDE_DIR}"
        IMPORTED_SONAME "${tidy_LIBRARY}"
        IMPORTED_LOCATION "${tidy_LIBRARY}"
    )
endif()
