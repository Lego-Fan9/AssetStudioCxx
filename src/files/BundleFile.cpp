#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <stdexcept>
#include <tuple>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "enums/ArchiveFlags.h"
#include "enums/CnEncryptionFlags.h"
#include "enums/CompressionType.h"
#include "enums/StorageBlockFlags.h"

#include "helpers/BigArrayPool.hpp"
#include "helpers/FileReader.hpp"
#include "helpers/OffsetStream.hpp"
#include "helpers/UnityVersion.hpp"
#include "helpers/UnityVersionExtensions.hpp"
#include "helpers/decompression/Decompressor.hpp"

#include "files/StreamFile.hpp"

#include "files/BundleFile.hpp"

BundleFile::BundleFile() {}
BundleFile::BundleFile(FileReader &reader, bool isMultiBundle) {
    m_Header = Header(reader);

    if (m_Header.signature == "UnityArchive") {
        throw std::logic_error("Not implemented");
    } else if (m_Header.signature == "UnityWeb" || m_Header.signature == "UnityRaw") {
        if (m_Header.version == 6) {
            LoadUnityFS(reader, isMultiBundle);
        }
        throw std::logic_error("Not implemented");
    } else if (m_Header.signature == "UnityFS") {
        LoadUnityFS(reader, isMultiBundle);
    }
}

void BundleFile::LoadUnityFS(FileReader &reader, bool isMultiBundle) {
    ReadHeader(reader);

    int64_t bundleSize = m_Header.size;
    size_t streamSize = reader.buffer.size();
    if (bundleSize > streamSize) {
        spdlog::error("BundleFile::LoadUnityFS : Bundle does not match stream size. bundleSize: {}, streamSize: {}", bundleSize,
                      streamSize);
    }

    isDataAfterBundle = streamSize - bundleSize > 200;

    UnityCnCheck(reader);
    ReadBlocksInfoAndDirectory(reader, m_Header.unityRevision);

    if (isUncompressedBundle() && !isDataAfterBundle && !isMultiBundle) {
        spdlog::debug("[Uncompressed bundle] BlockData count: {}", m_BlocksInfo.size());
        ReadFiles(reader);
        return;
    }

    FileReader blocksResp = FileReader();
    ReadBlocks(reader, blocksResp);
    ReadFiles(blocksResp);
}

void BundleFile::ReadHeader(FileReader &reader) {
    m_Header.size = reader.readInt<int64_t>();
    m_Header.compressedBlocksInfoSize = reader.readInt<uint32_t>();
    m_Header.uncompressedBlocksInfoSize = reader.readInt<uint32_t>();
    m_Header.flags = static_cast<ArchiveFlags_e>(reader.readInt<uint32_t>());
    if (m_Header.signature != "UnityFS") {
        reader.readByte();
    }
}

void BundleFile::UnityCnCheck(FileReader &reader) {
    if ((m_Header.flags & ArchiveFlags_BlocksInfoAtTheEnd) != 0)
        return;

    bool hasUnityCnFlag = false;
    if (!m_Header.unityRevision.IsStripped()) {
        if (m_Header.unityRevision < 2020 ||
            UnityVersionExtensions::IsInRange(m_Header.unityRevision, 2020, std::make_tuple(2020, 3, 34)) ||
            UnityVersionExtensions::IsInRange(m_Header.unityRevision, 2021, std::make_tuple(2021, 3, 2)) ||
            UnityVersionExtensions::IsInRange(m_Header.unityRevision, 2022, std::make_tuple(2022, 1, 1))) {
            hasUnityCnFlag = (m_Header.flags & CnEncryptionFlags_V1) != 0;
        } else {
            hasUnityCnFlag = (m_Header.flags & CnEncryptionFlags_V2_V3) != 0;
        }
    }

    if (!hasUnityCnFlag) {
        return;
    }

    size_t pos = reader.offset;
    reader.offset += 70;
    try {
        ReadBlocksInfoAndDirectory(reader, m_Header.unityRevision, true);
    } catch (...) {
        reader.offset = pos;
        return;
    }

    throw std::logic_error("Unsupported bundle file. UnityCN encryption was detected.");
}

void BundleFile::ReadBlocksInfoAndDirectory(FileReader &reader, UnityVersion unityVer, bool silent) {
    std::vector<uint8_t> blocksInfoBytes{};

    if (m_Header.version >= 7) {
        reader.align(16);
    } else if (m_Header.unityRevision >=
               std::make_tuple(2019, 4 && m_Header.flags != ArchiveFlags_BlocksAndDirectoryInfoCombined)) {
        size_t preAlign = reader.offset;
        std::vector<uint8_t> alignData = reader.readBytes((16 - (size_t)(preAlign % 16)) % 16);
        if (std::any_of(alignData.begin(), alignData.end(), [](uint8_t x) { return x != 0; })) {
            reader.offset = preAlign;
        }
    }

    uint32_t compressedSize = m_Header.compressedBlocksInfoSize;
    uint32_t uncompressedSize = m_Header.uncompressedBlocksInfoSize;
    if (uncompressedSize < 0 || compressedSize < 0 || compressedSize > reader.buffer.size()) {
        throw std::logic_error("Incorrect blockInfo length.\nBlockInfo sizes might be encrypted.");
    }

    if ((m_Header.flags & ArchiveFlags_BlocksInfoAtTheEnd) != 0) {
        size_t position = reader.offset;
        reader.offset = m_Header.size - compressedSize;
        blocksInfoBytes = reader.readBytes(compressedSize);
        reader.offset = position;
    } else {
        blocksInfoBytes = reader.readBytes(compressedSize);
    }

    uint32_t numWrite = 0;
    std::vector<uint8_t> blocksInfoBytesUncompressed{};
    int32_t compressionType = m_Header.flags & ArchiveFlags_CompressionTypeMask;
    CompressionType_e compressionEnum = static_cast<CompressionType_e>(compressionType);
    std::vector<uint8_t> blocksInfoBytesVector = std::vector<uint8_t>(blocksInfoBytes);
    switch (compressionType) {
    case CompressionType_None:
        blocksInfoBytesUncompressed = std::vector<uint8_t>(blocksInfoBytes);
        numWrite = compressedSize;
        break;
    case CompressionType_Lzma:
    case CompressionType_Lz4HC:
    case CompressionType_Lz4:
        blocksInfoBytesUncompressed = Decompress(blocksInfoBytesVector, compressionEnum, uncompressedSize, &numWrite);
        break;
    default:
        throw std::logic_error(fmt::format("Could not decompress {}", compressionType));
    }

    if (numWrite != uncompressedSize) {
        throw new std::logic_error(fmt::format("Decompression failed! Type: {}, Expected: {} bytes, Got: {} bytes.",
                                               compressionType, uncompressedSize, numWrite));
    }

    EndianBinaryReader blocksInfoReader(blocksInfoBytesUncompressed, reader.endian);

    blocksInfoReader.readBytes(16); // uncompressedDataHash

    int32_t blocksInfoCount = blocksInfoReader.readInt<int32_t>();
    m_BlocksInfo = std::vector<StorageBlock>(blocksInfoCount);
    for (int32_t i = 0; i < blocksInfoCount; i++) {
        m_BlocksInfo[i] = StorageBlock(blocksInfoReader);
    }

    int32_t nodesCount = blocksInfoReader.readInt<int32_t>();
    m_DirectoryInfo = std::vector<Node>(nodesCount);
    for (int32_t i = 0; i < nodesCount; i++) {
        m_DirectoryInfo[i] = Node(blocksInfoReader);
    }

    if ((m_Header.flags & ArchiveFlags_BlockInfoNeedPaddingAtStart) != 0) {
        reader.align(16);
    }
}

bool BundleFile::isUncompressedBundle() {
    return std::all_of(m_BlocksInfo.begin(), m_BlocksInfo.end(), [](const StorageBlock &x) {
        CompressionType_e type = static_cast<CompressionType_e>(x.flags & StorageBlockFlags_CompressionTypeMask);
        return type == CompressionType_None;
    });
}

void BundleFile::ReadFiles(FileReader &reader) {
    for (Node node : m_DirectoryInfo) {
        StreamFile file = StreamFile();
        file.path = node.path;
        file.fileName = std::filesystem::path(node.path).filename().string();

        try {
            file.stream = OffsetStream(reader.buffer, node.offset + reader.offset, node.size);
        } catch (const std::exception &e) {
            spdlog::warn("Failed to load file: {} with error: {}", file.fileName, e.what());
        } catch (...) {
            spdlog::warn("Failed to load file: {}", file.fileName);
        }

        fileList.push_back(file);
    }
}

uint64_t BundleFile::CheckBlockStreamLength() {
    uint64_t uncompressedSizeSum = 0;
    for (StorageBlock block : m_BlocksInfo) {
        if (block.uncompressedSize > std::numeric_limits<uint64_t>::max() - uncompressedSizeSum) {
            throw std::overflow_error("uncompressedSizeSum overflow");
        }

        uncompressedSizeSum += block.uncompressedSize;
    }

    return uncompressedSizeSum;
}

void BundleFile::ReadBlocks(FileReader &reader, FileReader &respReader) {
    std::vector<uint8_t> blocksStream(CheckBlockStreamLength());

    uint32_t blocksCompression = 0;
    for (StorageBlock block : m_BlocksInfo) {
        uint32_t type = block.flags & StorageBlockFlags_CompressionTypeMask;
        if (type > blocksCompression)
            blocksCompression = type;
    }

    uint32_t blockSize = 0;
    for (StorageBlock block : m_BlocksInfo) {
        if (block.uncompressedSize > blockSize)
            blockSize = block.uncompressedSize;
    }

    std::vector<uint8_t> sharedCompressedBuff = BigArrayPool<uint8_t>::Rent(blockSize);
    std::vector<uint8_t> sharedUncompressedBuff = BigArrayPool<uint8_t>::Rent(blockSize);

    try {
        for (size_t i = 0; i < m_BlocksInfo.size(); i++) {
            StorageBlock blockInfo = m_BlocksInfo[i];
            CompressionType_e compressionType =
                static_cast<CompressionType_e>(blockInfo.flags & StorageBlockFlags_CompressionTypeMask);

            uint32_t numWrite = 0;
            size_t bytesRead;
            uint8_t *srcPtr = reader.buffer.data() + reader.offset;
            switch (compressionType) {
            case CompressionType_None:
                blocksStream.insert(blocksStream.end(), srcPtr, srcPtr + blockInfo.compressedSize);
                reader.offset += blockInfo.compressedSize;
            case CompressionType_Lzma:
            case CompressionType_Lz4:
            case CompressionType_Lz4HC:
                sharedCompressedBuff.clear();
                sharedUncompressedBuff.clear();

                bytesRead =
                    std::min(static_cast<uint64_t>(blockInfo.compressedSize), reader.buffer.size() - reader.offset);
                std::copy(reader.buffer.begin() + reader.offset, reader.buffer.begin() + reader.offset + bytesRead,
                          reader.buffer.begin());
                reader.offset += bytesRead;

                sharedUncompressedBuff = Decompress(sharedCompressedBuff, compressionType, blockInfo.uncompressedSize, &numWrite);
                if (numWrite == blockInfo.uncompressedSize) {
                    blocksStream.insert(blocksStream.end(), sharedUncompressedBuff.begin(),
                                        sharedUncompressedBuff.begin() + blockInfo.uncompressedSize);
                }
                break;
            default:
                throw std::logic_error(fmt::format("Cannot decode type {}", static_cast<uint32_t>(compressionType)));
            }

            if (numWrite != blockInfo.uncompressedSize) {
                throw std::logic_error(fmt::format("Failed to decode type {}. Expected: {}, Got: {}", static_cast<uint32_t>(compressionType), blockInfo.uncompressedSize, numWrite));
            }
        }

    } catch (...) {
        BigArrayPool<uint8_t>::Return(sharedCompressedBuff);
        BigArrayPool<uint8_t>::Return(sharedUncompressedBuff);
    }

    respReader.buffer = blocksStream;
    respReader.offset = 0;
}

Header::Header() {}
Header::Header(FileReader &reader) {
    signature = reader.readStringToNull();
    version = reader.readInt<uint32_t>();
    unityVersion = reader.readStringToNull();
    unityRevision = UnityVersion(reader.readStringToNull());
}

StorageBlock::StorageBlock() {}
StorageBlock::StorageBlock(EndianBinaryReader &reader) {
    uncompressedSize = reader.readInt<uint32_t>();
    compressedSize = reader.readInt<uint32_t>();
    flags = static_cast<StorageBlockFlags_e>(reader.readInt<uint16_t>());
}

Node::Node() {}
Node::Node(EndianBinaryReader reader) {
    offset = reader.readInt<int64_t>();
    size = reader.readInt<int64_t>();
    flags = reader.readInt<uint32_t>();
    path = reader.readStringToNull();
}