# Findhiredis - Find hiredis C client library for Redis
# Once loaded this will define:
#   hiredis_FOUND        - system has hiredis
#   hiredis_INCLUDE_DIR  - the hiredis include directory
#   hiredis_LIBRARY      - the hiredis library
#   hiredis::hiredis     - imported target

find_path(hiredis_INCLUDE_DIR hiredis/hiredis.h)
find_library(hiredis_LIBRARY hiredis)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(hiredis
    DEFAULT_MSG
    hiredis_LIBRARY
    hiredis_INCLUDE_DIR
)

if(hiredis_FOUND AND NOT TARGET hiredis::hiredis)
    add_library(hiredis::hiredis UNKNOWN IMPORTED)
    set_target_properties(hiredis::hiredis PROPERTIES
        IMPORTED_LOCATION "${hiredis_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${hiredis_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(hiredis_INCLUDE_DIR hiredis_LIBRARY)
