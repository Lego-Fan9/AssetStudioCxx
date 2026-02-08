#ifndef SRC_HELPERS_DECOMPRESSION_DECOMPRESSOR_HPP
#define SRC_HELPERS_DECOMPRESSION_DECOMPRESSOR_HPP

#include <cstdint>
#include <vector>

#include "enums/CompressionType.h"

std::vector<uint8_t>Decompress(std::vector<uint8_t> input, CompressionType_e compressionType, uint32_t expectedLength, uint32_t *outputSize);

#endif