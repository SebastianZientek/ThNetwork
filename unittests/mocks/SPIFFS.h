#pragma once

#include "FS.h"

namespace fs
{
class SPIFFSFS
{
public:
    fs::File open(const char *path, const char *mode = "w", const bool create = false)
    {
        return fs::File{};
    }
};

}  // namespace fs

fs::SPIFFSFS SPIFFS;
