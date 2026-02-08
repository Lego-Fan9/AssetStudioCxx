#include <cstdint>
#include <stdexcept>
#include <vector>

#include "helpers/OffsetStream.hpp"

OffsetStream::OffsetStream() : data_(nullptr), offset(0), size_(0) {}

OffsetStream::OffsetStream(const std::vector<uint8_t> &data, size_t offset, size_t size)
    : data_(&data), offset(offset), size_(size) {
    if (offset + size > data.size()) {
        throw std::out_of_range("OffsetStream out of bounds");
    }
}

bool OffsetStream::empty() const {
    return size_ == 0;
}

const uint8_t *OffsetStream::data() const {
    return data_ ? data_->data() + offset : nullptr;
}

size_t OffsetStream::size() const {
    return size_;
}

uint8_t OffsetStream::operator[](size_t i) const {
    if (!data_ || i >= size_) {
        throw std::out_of_range("OffsetStream access");
    }
    return (*data_)[offset + i];
}