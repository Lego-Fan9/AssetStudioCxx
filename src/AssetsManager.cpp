#include <cstdint>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <vector>

#include <fmt/core.h>

#include "enums/FileType.h"

#include "files/BundleFile.hpp"
#include "files/StreamFile.hpp"

#include "helpers/FileReader.hpp"

#include "AssetsManager.hpp"

AssetsManager::AssetsManager() {}
AssetsManager::AssetsManager(std::string fullName, std::vector<uint8_t> stream) {
    LoadFile(fullName, stream);
}

bool AssetsManager::LoadFile(std::string fullName, std::vector<uint8_t> stream) {
    FileReader reader = FileReader(fullName, stream);
    return LoadFile(reader);
}

bool AssetsManager::LoadFile(FileReader &reader, bool fromZip) {
    if (reader.buffer.size() <= 0) {
        return false;
    }

    switch (reader.FileType) {
    case FileType_AssetsFile:
        return LoadAssetsFile(reader, fromZip);
    case FileType_BundleFile:
        return LoadBundleFile(reader);
    default:
        throw std::runtime_error(fmt::format("FileType: {} not supported", static_cast<int>(reader.FileType)));
    }

    return true;
}

bool AssetsManager::LoadAssetsFile(FileReader &reader, bool fromZip) {
    throw std::logic_error("Not implemented");
}

bool AssetsManager::LoadBundleFile(FileReader &reader) {
    bool isLoaded = false;

    // try {
    BundleFile bundleFile(reader);
#ifdef SET_BUNDLE_TEST_FILE
    bundleFileTest = bundleFile;
#endif
    //}

    return isLoaded;
}

bool AssetsManager::LoadBundleFiles(FileReader &reader, BundleFile &bundle) {
    for (StreamFile file : bundle.fileList) {
        if (file.stream.empty()) {
            continue;
        }

        file.stream.offset = 0;
        std::string dummyPath = (std::filesystem::path(reader.FullPath).parent_path() / file.fileName).string();
        FileReader subReader = FileReader(dummyPath, std::vector<uint8_t>(file.stream.data(), file.stream.data() + file.stream.size()));
        if (subReader.FileType == FileType_AssetsFile) {
            throw std::runtime_error("AssetsManager::LoadBundleFiles FileType.AssetsFile Not implemented");
        } else {
            throw std::runtime_error("AssetsManager::LoadBundleFiles res file loading not implemnted");
        }
    }

    return true;
}