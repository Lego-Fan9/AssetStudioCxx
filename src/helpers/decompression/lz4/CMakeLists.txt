add_library(libLz4Wrapper STATIC)

target_sources(libLz4Wrapper PRIVATE
    DecompressLz4.cpp
)

target_sources(libLz4Wrapper PUBLIC
    DecompressLz4.hpp
)

target_include_directories(libLz4Wrapper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(libLz4Wrapper
    PRIVATE libLz4Core
)