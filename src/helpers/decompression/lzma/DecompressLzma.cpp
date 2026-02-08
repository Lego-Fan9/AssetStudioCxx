

#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <lzmaCore/LzmaDec.h>

#include "DecompressLzma.hpp"

static void *_lzmaAlloc(ISzAllocPtr, size_t size) {
    return new uint8_t[size];
}

static void _lzmaFree(ISzAllocPtr, void *addr) {
    if (!addr)
        return;

    delete[] reinterpret_cast<uint8_t *>(addr);
}

static ISzAlloc _allocFuncs = {_lzmaAlloc, _lzmaFree};

std::vector<uint8_t> decompressUnityLzma(std::vector<uint8_t> input, uint32_t expectedOutputSize, uint32_t *outputSize) {
    if (input.size() < 5) {
        *outputSize = 0;
        return input;
    }

    uint8_t props[5];
    std::copy(input.begin(), input.begin() + 5, props);

    std::unique_ptr<uint8_t[]> result = lzmaDecompress(input.data() + 5, input.size() - 5, outputSize, expectedOutputSize, props);
    if (result == nullptr || *outputSize != expectedOutputSize) {
        *outputSize = 0;
        return input;
    }

    return std::vector<uint8_t>(result.get(), result.get() + *outputSize);
}

std::unique_ptr<uint8_t[]> lzmaDecompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize,
                                          uint32_t expectedOutputSize, const uint8_t props[5]) {
    if (expectedOutputSize <= (256 * 1024 * 1024)) {
        auto blob = std::make_unique<uint8_t[]>(expectedOutputSize);

        ELzmaStatus lzmaStatus;
        SizeT procOutSize = expectedOutputSize;
        SizeT procInSize = inputSize;
        int status =
            LzmaDecode(blob.get(), &procOutSize, input, &procInSize, props, 5, LZMA_FINISH_END, &lzmaStatus, &_allocFuncs);

        if (status == SZ_OK && procOutSize == expectedOutputSize) {
            *outputSize = expectedOutputSize;
            return blob;
        }
    }

    return nullptr;
}