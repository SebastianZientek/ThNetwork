#include "RaiiFile.hpp"

#include <SD.h>
#include <SPI.h>

#include "common/logger.hpp"

RaiiFile::RaiiFile(const std::string &path, const char *mode, bool create)
{
    m_file = SD.open(path.c_str(), mode, create);
    if (!m_file)
    {
        logger::logErr("Failed to open file for reading");
    }
}

RaiiFile::~RaiiFile()  // NOLINT
{
    m_file.close();
}
