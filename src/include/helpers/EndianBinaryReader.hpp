#ifndef ENDIAN_BINARY_READER_HPP
#define ENDIAN_BINARY_READER_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "enums/Endian.h"

class EndianBinaryReader {
public:
    std::vector<std::uint8_t> buffer;
    std::size_t offset = 0;
    EndianType_e endian;

    EndianBinaryReader();
    EndianBinaryReader(std::vector<std::uint8_t> data, EndianType_e endian);

    void align(std::size_t amount);
    void align4();

    std::uint8_t readByte();
    std::vector<std::uint8_t> readBytes(std::size_t amount);
    std::vector<uint8_t> readBytes(size_t amount, size_t fromOffset);
    std::string readStringToNull(std::size_t maxLength = 2000, bool align = false);

    template <typename T> T readInt();
    template <typename T> T readInt(size_t fromOffset);

    template <size_t N> int32_t LastIndexOf(const std::array<uint8_t, N> &pattern) const;
    template <size_t N> int32_t IndexOf(const std::array<uint8_t, N> &pattern) const;
};

#endif