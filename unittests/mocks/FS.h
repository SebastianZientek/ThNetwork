#pragma once

namespace fs
{
class FS
{
public:
    FS() = default;
    FS(const FS &) = default;
    FS(FS &&) = default;
    FS &operator=(const FS &) = default;
    FS &operator=(FS &&) = default;
    virtual ~FS() = default;

    virtual bool exists(const char *path) = 0;
};

}  // namespace fs
