#ifndef STORAGE_BLOCK_FLAGS_H
#define STORAGE_BLOCK_FLAGS_H

enum StorageBlockFlags_e {
    StorageBlockFlags_CompressionTypeMask = 0x3f,
    StorageBlockFlags_Streamed = 0x40
};

#endif