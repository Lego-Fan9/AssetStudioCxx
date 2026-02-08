#ifndef BUNDLE_FILE_HPP
#define BUNDLE_FILE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "enums/ArchiveFlags.h"
#include "enums/StorageBlockFlags.h"

#include "helpers/FileReader.hpp"
#include "helpers/UnityVersion.hpp"

#include "files/StreamFile.hpp"


class Header {
public:
    std::string signature;
    uint32_t version;
    std::string unityVersion;
    UnityVersion unityRevision;
    int64_t size;
    uint32_t compressedBlocksInfoSize;
    uint32_t uncompressedBlocksInfoSize;
    ArchiveFlags_e flags;

    Header();
    Header(FileReader &reader);
};

class Node {
public:
    int64_t offset;
    int64_t size;
    uint32_t flags;
    std::string path;

    Node();
    Node(EndianBinaryReader reader);
};

class StorageBlock {
public:
    uint32_t compressedSize;
    uint32_t uncompressedSize;
    StorageBlockFlags_e flags;

    StorageBlock();
    StorageBlock(EndianBinaryReader &reader);
};

class BundleFile {
public:
    bool isDataAfterBundle;
    Header m_Header;
    std::vector<StorageBlock> m_BlocksInfo;
    std::vector<Node> m_DirectoryInfo;
    std::vector<StreamFile> fileList;

    BundleFile();
    BundleFile(FileReader &reader, bool isMultiBundle = false);
private:
    void LoadUnityFS(FileReader &reader, bool isMultiBundle = false);
    void ReadHeader(FileReader &reader);
    void UnityCnCheck(FileReader &reader);
    void ReadBlocksInfoAndDirectory(FileReader &reader, UnityVersion unityVer, bool silent = false);
    bool isUncompressedBundle();
    void ReadFiles(FileReader &reader);
    void ReadBlocks(FileReader &reader, FileReader &respReader);
    uint64_t CheckBlockStreamLength();
};

#endif