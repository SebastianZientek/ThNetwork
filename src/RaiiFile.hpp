#pragma once

#include <FS.h>
#include <Arduino.h>

class RaiiFile
{
public:
    explicit RaiiFile(String path);
    ~RaiiFile();

    String asString();

private:
    File m_file;
};