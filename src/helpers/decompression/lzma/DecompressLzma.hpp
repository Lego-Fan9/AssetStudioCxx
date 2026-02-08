#ifndef DECOMPRESS_LZMA_HPP
#define DECOMPRESS_LZMA_HPP

#include <memory>
#include <stdint.h>

#include <lzmaCore/LzmaDec.h>

static void *_lzmaAlloc(ISzAllocPtr, size_t size);
static void _lzmaFree(ISzAllocPtr, void *addr);

std::vector<uint8_t> decompressUnityLzma(std::vector<uint8_t> input, uint32_t expectedOutputSize, uint32_t *outputSize);

std::unique_ptr<uint8_t[]> lzmaDecompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize,
                                          uint32_t expectedOutputSize, const uint8_t props[5]);

#endif