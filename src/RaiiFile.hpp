#pragma once

#include <FS.h>
#include <Arduino.h>

class RaiiFile
{
public:
    explicit RaiiFile(String path);
    RaiiFile(const RaiiFile&) = delete;
    RaiiFile(RaiiFile&&) = default;
    RaiiFile& operator=(const RaiiFile&) = delete;
    RaiiFile& operator=(RaiiFile&&) = default;

    ~RaiiFile();

    String asString();

private:
    File m_file;
};