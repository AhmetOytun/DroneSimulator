# Findraylib.cmake
find_path(RAYLIB_INCLUDE_DIR raylib.h
    PATHS
    /usr/local/include
    /usr/include
    /opt/homebrew/include
)

find_library(RAYLIB_LIBRARY NAMES raylib
    PATHS
    /usr/local/lib
    /usr/lib
    /opt/homebrew/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(raylib DEFAULT_MSG RAYLIB_INCLUDE_DIR RAYLIB_LIBRARY)

if(raylib_FOUND)
    set(raylib_INCLUDE_DIRS ${RAYLIB_INCLUDE_DIR})
    set(raylib_LIBRARIES ${RAYLIB_LIBRARY})
endif()