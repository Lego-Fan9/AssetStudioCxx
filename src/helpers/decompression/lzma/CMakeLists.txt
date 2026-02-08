add_library(7zLzmaWrapper STATIC)

target_sources(7zLzmaWrapper PRIVATE
    DecompressLzma.cpp
)

target_sources(7zLzmaWrapper PUBLIC
    DecompressLzma.hpp
)

target_include_directories(7zLzmaWrapper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(7zLzmaWrapper
    PUBLIC 7zLzmaCore
)