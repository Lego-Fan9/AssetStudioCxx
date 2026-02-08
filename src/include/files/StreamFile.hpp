#ifndef STREAM_FILE_H
#define STREAM_FILE_H

#include <string>

#include "helpers/OffsetStream.hpp"

class StreamFile {
public:
    std::string path;
    std::string fileName;
    OffsetStream stream;
};

#endif