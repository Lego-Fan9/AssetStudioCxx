#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "enums/Endian.h"

#include "helpers/EndianBinaryReader.hpp"

EndianBinaryReader::EndianBinaryReader() : endian(EndianType_BigEndian) {}

EndianBinaryReader::EndianBinaryReader(std::vector<std::uint8_t> data, EndianType_e endian) : buffer(data), endian(endian) {}

void EndianBinaryReader::align(std::size_t amount) {
    const std::size_t mod = offset % amount;
    if (mod != 0) {
        offset += amount - mod;
    }

    if (offset >= buffer.size())
        throw std::out_of_range(fmt::format("{} : {} Not enough bytes in buffer", __FILE__, std::to_string(__LINE__)));
}

void EndianBinaryReader::align4() {
    return align(4);
}

std::uint8_t EndianBinaryReader::readByte() {
    return buffer[offset++];
}

std::vector<std::uint8_t> EndianBinaryReader::readBytes(std::size_t amount) {
    if (offset + amount > buffer.size()) {
        throw std::out_of_range(fmt::format("{} : {} Not enough bytes in buffer", __FILE__, std::to_string(__LINE__)));
    }

    std::vector<std::uint8_t> result(buffer.begin() + offset, buffer.begin() + offset + amount);

    offset += amount;
    return result;
}

std::vector<uint8_t> EndianBinaryReader::readBytes(size_t amount, size_t fromOffset) {
    const size_t tempOffset = offset;

    offset = fromOffset;
    const std::vector<uint8_t> response = readBytes(amount);
    offset = tempOffset;

    return response;
}

std::string EndianBinaryReader::readStringToNull(std::size_t maxLength, bool align) {
    std::string result;
    for (std::size_t i = 0; i < maxLength; i++) {
        if (offset >= buffer.size())
            throw std::out_of_range(fmt::format("{} : {} Not enough bytes in buffer", __FILE__, std::to_string(__LINE__)));
        if (buffer[offset] == 0)
            break;

        result += static_cast<char>(buffer[offset]);
        offset++;
    }

    if (offset < buffer.size() && buffer[offset] == 0) {
        offset++;
    }

    if (align) {
        align4();
    }

    return result;
}

template <typename T> T EndianBinaryReader::readInt() {
    static_assert(std::is_integral<T>::value, "T must be integral");

    std::size_t numBytes = sizeof(T);
    if (offset + numBytes > buffer.size()) {
        throw std::out_of_range(fmt::format("{} : {} Not enough bytes in buffer", __FILE__, std::to_string(__LINE__)));
    }

    using U = std::make_unsigned_t<T>;
    U result = 0;

    if (endian == EndianType_LittleEndian) {
        for (std::size_t i = 0; i < numBytes; ++i)
            result |= static_cast<U>(buffer[offset + i]) << (8 * i);
    } else {
        for (std::size_t i = 0; i < numBytes; ++i)
            result = (result << 8) | static_cast<U>(buffer[offset + i]);
    }

    offset += numBytes;
    return static_cast<T>(result);
}

template <typename T> T EndianBinaryReader::readInt(size_t fromOffset) {
    const size_t tempOffset = offset;

    offset = fromOffset;
    T response = readInt<T>();
    offset = tempOffset;

    return response;
}

template <size_t N> int32_t EndianBinaryReader::LastIndexOf(const std::array<uint8_t, N> &pattern) const {
    if (buffer.size() < N)
        return -1;

    for (size_t i = buffer.size() - N + 1; i-- > 0;) {
        if (std::memcmp(buffer.data() + i, pattern.data(), N) == 0)
            return static_cast<int>(i);
    }

    return -1;
}

template <size_t N> int32_t EndianBinaryReader::IndexOf(const std::array<uint8_t, N> &pattern) const {
    if (buffer.size() < N)
        return -1;

    for (size_t i = 0; i <= buffer.size() - N; ++i) {
        if (std::memcmp(buffer.data() + i, pattern.data(), N) == 0)
            return static_cast<int>(i);
    }

    return -1;
}

template int8_t EndianBinaryReader::readInt<int8_t>();
template uint8_t EndianBinaryReader::readInt<uint8_t>();
template int16_t EndianBinaryReader::readInt<int16_t>();
template uint16_t EndianBinaryReader::readInt<uint16_t>();
template int32_t EndianBinaryReader::readInt<int32_t>();
template uint32_t EndianBinaryReader::readInt<uint32_t>();
template int64_t EndianBinaryReader::readInt<int64_t>();
template uint64_t EndianBinaryReader::readInt<uint64_t>();

template int8_t EndianBinaryReader::readInt<int8_t>(size_t);
template uint8_t EndianBinaryReader::readInt<uint8_t>(size_t);
template int16_t EndianBinaryReader::readInt<int16_t>(size_t);
template uint16_t EndianBinaryReader::readInt<uint16_t>(size_t);
template int32_t EndianBinaryReader::readInt<int32_t>(size_t);
template uint32_t EndianBinaryReader::readInt<uint32_t>(size_t);
template int64_t EndianBinaryReader::readInt<int64_t>(size_t);
template uint64_t EndianBinaryReader::readInt<uint64_t>(size_t);

template int32_t EndianBinaryReader::IndexOf<2>(const std::array<uint8_t, 2> &) const;
template int32_t EndianBinaryReader::LastIndexOf<2>(const std::array<uint8_t, 2> &) const;
template int32_t EndianBinaryReader::IndexOf<4>(const std::array<uint8_t, 4> &) const;
template int32_t EndianBinaryReader::LastIndexOf<4>(const std::array<uint8_t, 4> &) const;
template int32_t EndianBinaryReader::IndexOf<6>(const std::array<uint8_t, 6> &) const;
template int32_t EndianBinaryReader::LastIndexOf<6>(const std::array<uint8_t, 6> &) const;
template int32_t EndianBinaryReader::IndexOf<8>(const std::array<uint8_t, 8> &) const;
template int32_t EndianBinaryReader::LastIndexOf<8>(const std::array<uint8_t, 8> &) const;