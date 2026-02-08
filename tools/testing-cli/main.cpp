#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <spdlog/spdlog.h>

#include <AssetsManager.hpp>
#include <files/BundleFile.hpp>
#include <files/StreamFile.hpp>

std::vector<uint8_t> GetFile(std::filesystem::path filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << filepath << "\n";
        return std::vector<uint8_t>{};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
        std::cerr << "Failed to read file.\n";
        return std::vector<uint8_t>{};
    }

    return buffer;
}

void ReadABunchOfData(AssetsManager &assetsManager) {
    spdlog::info("Signature: {}", assetsManager.bundleFileTest.m_Header.signature);
    spdlog::info("Unity Version: {}", assetsManager.bundleFileTest.m_Header.unityVersion);
    spdlog::info("Unity Revision: {}", assetsManager.bundleFileTest.m_Header.unityRevision.ToString());
    spdlog::info("Bundle Size: {}", assetsManager.bundleFileTest.m_Header.size);
    spdlog::info("CompressedBlocksInfoSize: {}", assetsManager.bundleFileTest.m_Header.compressedBlocksInfoSize);
    spdlog::info("UncompressedBlocksInfoSize: {}", assetsManager.bundleFileTest.m_Header.uncompressedBlocksInfoSize);
    spdlog::info("Header flags: {}", static_cast<int64_t>(assetsManager.bundleFileTest.m_Header.flags));

    spdlog::info("m_BlocksInfo length: {}", assetsManager.bundleFileTest.m_BlocksInfo.size());

    uint32_t counter = 0;
    for (StorageBlock val : assetsManager.bundleFileTest.m_BlocksInfo) {
        spdlog::info("========== m_BlocksInfo section {} starting ==========", counter++);
        spdlog::info("m_BlocksInfo compressedSize: {}", val.compressedSize);
        spdlog::info("m_BlocksInfo uncompressedSize: {}", val.uncompressedSize);
        spdlog::info("m_BlocksInfo flags: {}", static_cast<int64_t>(val.flags));
    }

    spdlog::info("m_DirectoryInfo length: {}", assetsManager.bundleFileTest.m_DirectoryInfo.size());

    counter = 0;
    for (Node val : assetsManager.bundleFileTest.m_DirectoryInfo) {
        spdlog::info("========== m_DirectoryInfo section {} starting ==========", counter++);
        spdlog::info("m_DirectoryInfo offset: {}", val.offset);
        spdlog::info("m_DirectoryInfo size: {}", val.size);
        spdlog::info("m_DirectoryInfo flags: {}", static_cast<int64_t>(val.flags));
        spdlog::info("m_DirectoryInfo path: {}", val.path);
    }

    counter = 0;
    for (StreamFile val : assetsManager.bundleFileTest.fileList) {
        spdlog::info("========== fileList section {} starting ==========", counter++);
        spdlog::info("fileList path: {}", val.path);
        spdlog::info("fileList fileName: {}", val.fileName);
        spdlog::info("fileList stream length: {}", val.stream.size());
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::filesystem::path inputPath(argv[1]);
    std::filesystem::path absolutePath = std::filesystem::absolute(inputPath);

    std::vector<uint8_t> fileData = GetFile(absolutePath);

    try {
        AssetsManager assetsManager(argv[1], fileData);

        ReadABunchOfData(assetsManager);
    } catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR: Unknown exception occurred!" << std::endl;
    }
}