#ifndef ASSETS_MANAGER_HPP
#define ASSETS_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <cstdint>

#include "enums/ClassIdType.h"
#include "helpers/EndianBinaryReader.hpp"
#include "helpers/FileReader.hpp"
#include "files/BundleFile.hpp"

class AssetsManager {
public:
    bool LoadViaTypeTree = true;
    bool MeshLazyLoad = true;
    // std::vector<SerializedFile> AssetsFileList = std::vector<SerializedFile>();

    AssetsManager();
    AssetsManager(std::string fullName, std::vector<uint8_t> stream);

    bool LoadFile(std::string fullName, std::vector<uint8_t> stream);
    bool LoadFile(FileReader &reader, bool fromZip = false);

    BundleFile bundleFileTest;

    std::unordered_map<std::string, int32_t> assetsFileIndexCache = std::unordered_map<std::string, int32_t>();
    std::unordered_map<std::string, EndianBinaryReader> resourceFileReaders = std::unordered_map<std::string, EndianBinaryReader>();

private:
    std::vector<std::string> importFiles = std::vector<std::string>();
    std::unordered_set<std::string> importFilesHash = std::unordered_set<std::string>();
    std::unordered_set<std::string> noexistFiles = std::unordered_set<std::string>();
    std::unordered_set<std::string> assetsFileListHash = std::unordered_set<std::string>();

    bool LoadAssetsFile(FileReader &reader, bool fromZip = false);
    bool LoadBundleFile(FileReader &reader);
    bool LoadBundleFiles(FileReader &reader, BundleFile &bundle);
};

#endif