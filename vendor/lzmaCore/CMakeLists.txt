add_library(7zLzmaCore STATIC)

target_sources(7zLzmaCore PRIVATE
    LzmaDec.c
    LzFind.c
    CpuArch.c
)

target_sources (7zLzmaCore PUBLIC
    LzmaDec.h
)

target_include_directories(7zLzmaCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(7zLzmaCore PROPERTIES
    LINKER_LANGUAGE C
)