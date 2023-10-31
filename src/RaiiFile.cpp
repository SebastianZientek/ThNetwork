#include "RaiiFile.hpp"

#include <SD.h>
#include <SPI.h>

#include "logger.hpp"

RaiiFile::RaiiFile(const String &path, const char* mode)
{
    m_file = SD.open(path, mode);
    if (!m_file)
    {
        logger::logErr("Failed to open file for reading");
    }
}

RaiiFile::~RaiiFile() { m_file.close(); }

String RaiiFile::asString()
{
    String str;
    while (m_file.available())
    {
        str += static_cast<char>(m_file.read());
    }

    return str;
}

void RaiiFile::saveString(const String &str)
{
    m_file.print(str);
}
