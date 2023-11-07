#include "RaiiFile.hpp"

#include <SD.h>
#include <SPI.h>

#include "logger.hpp"

RaiiFile::RaiiFile(const String &path, const char *mode, bool create)
{
    m_file = SD.open(path, mode, create);
    if (!m_file)
    {
        logger::logErr("Failed to open file for reading");
    }
}

RaiiFile::~RaiiFile() { m_file.close(); }
