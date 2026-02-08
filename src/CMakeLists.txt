add_library(AssetStudioCxx STATIC
    "AssetsManager.cpp"
)

set_target_properties(AssetStudioCxx PROPERTIES
    LINKER_LANGUAGE CXX
)

target_compile_features(AssetStudioCxx PUBLIC cxx_std_17)

target_sources(AssetStudioCxx PRIVATE
    "helpers/EndianBinaryReader.cpp"
    "helpers/FileReader.cpp"
    "helpers/UnityVersion.cpp"
    "helpers/OffsetStream.cpp"

    "helpers/decompression/Decompressor.cpp"

    "files/BundleFile.cpp"
)

target_sources(AssetStudioCxx PUBLIC
    "include/AssetsManager.hpp"

    "include/files/BundleFile.hpp"
    "include/files/StreamFile.hpp"

    "include/helpers/BigArrayPool.hpp"
    "include/helpers/EndianBinaryReader.hpp"
    "include/helpers/FileReader.hpp"
    "include/helpers/OffsetStream.hpp"
    "include/helpers/UnityVersion.hpp"
    "include/helpers/UnityVersionExtensions.hpp"

    "include/enums/ArchiveFlags.h"
    "include/enums/BuildTarget.h"
    "include/enums/ClassIdType.h"
    "include/enums/CnEncryptionFlags.h"
    "include/enums/CompressionType.h"
    "include/enums/Endian.h"
    "include/enums/FileType.h"
    "include/enums/SerializedFileFormatVersion.h"
    "include/enums/StorageBlockFlags.h"
)

target_include_directories(AssetStudioCxx 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(AssetStudioCxx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(AssetStudioCxx
    PUBLIC
        fmt
        spdlog
        libLz4Wrapper
        7zLzmaWrapper
)