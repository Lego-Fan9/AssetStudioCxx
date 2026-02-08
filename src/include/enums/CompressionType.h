#ifndef COMPRESSION_TYPE_H
#define COMPRESSION_TYPE_H

enum CompressionType_e {
    CompressionType_Auto = -1,
    CompressionType_None,
    CompressionType_Lzma,
    CompressionType_Lz4,
    CompressionType_Lz4HC,
    CompressionType_Lzham,
    CompressionType_Zstd,
    CompressionType_Oodle,
};

#endif