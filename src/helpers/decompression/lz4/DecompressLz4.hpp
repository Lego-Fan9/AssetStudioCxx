#ifndef DECOMPRESS_LZ4_HPP
#define DECOMPRESS_LZ4_HPP

#include <cstdint>
#include <vector>

std::vector<uint8_t> decompressLz4(std::vector<uint8_t> input, uint32_t expectedLength, uint32_t *outputSize);

#endif