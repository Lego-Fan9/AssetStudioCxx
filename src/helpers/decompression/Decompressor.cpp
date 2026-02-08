#include <cstdint>
#include <stdexcept>
#include <vector>


#include <fmt/core.h>

#include <lz4/DecompressLz4.hpp>
#include <lzma/DecompressLzma.hpp>

#include "enums/CompressionType.h"

#include "helpers/decompression/Decompressor.hpp"

std::vector<uint8_t> Decompress(std::vector<uint8_t> input, CompressionType_e compressionType, uint32_t expectedLength,
                                uint32_t *outputSize) {
    std::vector<uint8_t> response;

    switch (compressionType) {
    case CompressionType_Lzma:
        response = decompressUnityLzma(input, expectedLength, outputSize);
        break;
    case CompressionType_Lz4HC:
    case CompressionType_Lz4:
        response = decompressLz4(input, expectedLength, outputSize);
        break;
    default:
        throw std::runtime_error(fmt::format("Could not decode enum type: {}", static_cast<int>(compressionType)));
    }

    if (*outputSize != expectedLength) {
        throw std::runtime_error(fmt::format("Failed to decompress {} type encoding. Expedted: {}, got {}.",
                                             static_cast<int>(compressionType), expectedLength, *outputSize));
    }

    return response;
}