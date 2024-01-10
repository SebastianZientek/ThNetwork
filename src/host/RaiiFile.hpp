#pragma once

#include <Arduino.h>
#include <FS.h>

class RaiiFile
{
public:
    explicit RaiiFile(fs::FS filesystem, const std::string &path, const char *mode = FILE_READ, bool create = false);
    RaiiFile(const RaiiFile &) = delete;
    RaiiFile(RaiiFile &&) = default;
    RaiiFile &operator=(const RaiiFile &) = delete;
    RaiiFile &operator=(RaiiFile &&) = default;

    ~RaiiFile();
    File *operator->();
private:

    File m_file{};
};
