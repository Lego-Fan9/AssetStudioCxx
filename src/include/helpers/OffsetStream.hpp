#ifndef OFFSET_STREAM_HPP
#define OFFSET_STREAM_HPP

#include <cstdint>
#include <stdexcept>
#include <vector>


class OffsetStream {
public:
    OffsetStream();

    OffsetStream(const std::vector<uint8_t> &data, size_t offset, size_t size);

    bool empty() const;
    const uint8_t *data() const;
    size_t size() const;
    uint8_t operator[](size_t i) const;

    size_t offset;

private:
    const std::vector<uint8_t> *data_;
    size_t size_;
};

#endif