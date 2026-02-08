#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <array>

#include "enums/FileType.h"
#include "helpers/EndianBinaryReader.hpp"

class FileReader : public EndianBinaryReader {
public:
    std::string FullPath;
    std::string FileName;
    FileType_e FileType;

    FileReader();
    FileReader(std::string path, std::vector<std::uint8_t> stream);
    
private:
    static inline const std::array<uint8_t, 2> gzipMagic{0x1f, 0x8b};
    static inline const std::array<uint8_t, 6> brotliMagic{0x62, 0x72, 0x6F, 0x74, 0x6C, 0x69};
    static inline const std::array<uint8_t, 4> zipMagic{0x50, 0x4B, 0x03, 0x04};
    static inline const std::array<uint8_t, 4> zipSpannedMagic{0x50, 0x4B, 0x07, 0x08};
    static inline const std::array<uint8_t, 8> unityFsMagic{0x55, 0x6E, 0x69, 0x74,
                                                            0x79, 0x46, 0x53, 0x00};

    static inline constexpr size_t headerBuffLen = 1152;
    EndianBinaryReader headerBuff;

    FileType_e CheckFileType();
    bool IsSerializedFile();
    bool CheckBundleDataOffset();
};

#endif