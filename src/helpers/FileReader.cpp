#include <algorithm>
#include <array>
#include <filesystem>
#include <unordered_set>

#include "enums/Endian.h"
#include "enums/FileType.h"
#include "helpers/EndianBinaryReader.hpp"

#include "helpers/FileReader.hpp"

FileReader::FileReader() : EndianBinaryReader({}, EndianType_BigEndian) {}

FileReader::FileReader(std::string path, std::vector<std::uint8_t> stream)
    : EndianBinaryReader(stream, EndianType_BigEndian) {
    FullPath = std::filesystem::absolute(path).string();
    FileName = std::filesystem::path(path).filename().string();
    FileType = CheckFileType();
}

FileType_e FileReader::CheckFileType() {
    std::vector<std::uint8_t> tempBuff(headerBuffLen);
    std::copy(buffer.begin(), buffer.begin() + headerBuffLen, tempBuff.begin());
    EndianBinaryReader headerBuff(tempBuff, EndianType_BigEndian);

    std::string signature = headerBuff.readStringToNull();
    if (signature == "UnityWeb" || signature == "UnityRaw" || signature == "UnityArchive" ||
        signature == "UnityFS") {
        CheckBundleDataOffset();
        return FileType_BundleFile;
    } else if (signature == "UnityWebData1.0" || signature == "TuanjieWebData1.0") {
        return FileType_WebFile;
    } else {
        std::vector<uint8_t> magic;
        
        magic = buffer.size() > 2 ? readBytes(2, 0) : magic;
        if (std::equal(magic.begin(), magic.end(), gzipMagic.begin())) {
            return FileType_GZipFile;
        }

        magic = buffer.size() > 38 ? readBytes(6, 32) : magic;
        if (std::equal(magic.begin(), magic.end(), brotliMagic.begin())) {
            return FileType_BrotliFile;
        }

        if (IsSerializedFile()) {
            return FileType_AssetsFile;
        }

        magic = buffer.size() > 4 ? readBytes(4, 0) : magic;
        if (std::equal(magic.begin(), magic.end(), zipMagic.begin()) ||
            std::equal(magic.begin(), magic.end(), zipSpannedMagic.begin())) {
            return FileType_ZipFile;
        }

        if (CheckBundleDataOffset()) {
            return FileType_BundleFile;
        }

        return FileType_ResourceFile;
    }
}

bool FileReader::IsSerializedFile() {
    size_t fileSize = buffer.size();
    if (fileSize < 20) {
        return false;
    }

    uint32_t m_FileSize = headerBuff.readInt<uint32_t>(4);
    uint32_t m_Version = headerBuff.readInt<uint32_t>(8);
    uint32_t m_DataOffset = headerBuff.readInt<uint32_t>(12);

    if (m_Version >= 22) {
        if (fileSize < 48) {
            return false;
        }
        uint64_t m_FileSize = headerBuff.readInt<uint64_t>(24);
        uint64_t m_DataOffset = headerBuff.readInt<uint64_t>(32);
    }
    if (m_FileSize != fileSize || m_DataOffset > fileSize) {
        return false;
    }

    return true;
}

bool FileReader::CheckBundleDataOffset() {
    int32_t lastOffset = LastIndexOf(unityFsMagic);
    if (lastOffset <= 0) {
        return false;
    }

    int32_t firstOffset = IndexOf(unityFsMagic);
    if (firstOffset == lastOffset || lastOffset - firstOffset < 200) {
        offset = lastOffset;
        return true;
    }

    offset = firstOffset + 12;
    readStringToNull();
    readStringToNull();
    int64_t bundleSize = readInt<int64_t>();
    if (bundleSize > 200 && firstOffset + bundleSize < lastOffset) {
        offset = firstOffset;
        return true;
    }

    offset = lastOffset;
    return true;
}