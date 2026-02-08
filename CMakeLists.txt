cmake_minimum_required(VERSION 3.16)
project(AssetStudioCxx VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281
) # 10.2.1
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        79524ddd08a4ec981b7fea76afd08ee05f83755d
) # 1.17.0
set(SPDLOG_FMT_EXTERNAL TRUE CACHE BOOL "Use external fmt" FORCE)
FetchContent_MakeAvailable(spdlog)

add_subdirectory(src)
add_subdirectory(vendor/lzmaCore)
add_subdirectory(src/helpers/decompression/lzma)
add_subdirectory(vendor/lz4Core)
add_subdirectory(src/helpers/decompression/lz4)
add_subdirectory(tools/testing-cli)