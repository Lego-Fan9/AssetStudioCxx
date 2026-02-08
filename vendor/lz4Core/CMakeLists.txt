add_library(libLz4Core STATIC)

target_sources(libLz4Core PRIVATE
    lz4.c
    lz4hc.c
    xxhash.c
)

target_include_directories(libLz4Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(libLz4Core PROPERTIES
    LINKER_LANGUAGE C
)