#include <cstdint>
#include <vector>

#include <lz4Core/lz4.h>

#include "DecompressLz4.hpp"

std::vector<uint8_t> decompressLz4(std::vector<uint8_t> input, uint32_t expectedLength, uint32_t *outputSize) {
    std::vector<uint8_t> output(expectedLength);

    int decompressedSize = LZ4_decompress_safe(reinterpret_cast<const char *>(input.data()),
                                                    reinterpret_cast<char *>(output.data()), input.size(), expectedLength);

    *outputSize = static_cast<uint32_t>(decompressedSize);

    return output;
}