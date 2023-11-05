#pragma once

#include <Arduino.h>
#include <FS.h>

class RaiiFile
{
public:
    explicit RaiiFile(const String &path, const char* mode = FILE_READ, bool create = false);
    RaiiFile(const RaiiFile &) = delete;
    RaiiFile(RaiiFile &&) = default;
    RaiiFile &operator=(const RaiiFile &) = delete;
    RaiiFile &operator=(RaiiFile &&) = default;

    ~RaiiFile();

    String asString();
    void print(const String &str);
    void println(const String &str);

private:
    File m_file;
};
