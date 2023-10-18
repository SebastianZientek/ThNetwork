#include "RaiiFile.hpp"

#include <SD.h>
#include <SPI.h>

#include "logger.hpp"

RaiiFile::RaiiFile(String path)
{
    m_file = SD.open(path);
    if (!m_file)
    {
        logger::logErr("Failed to open file for reading");
    }
}

RaiiFile::~RaiiFile()
{
    m_file.close();
}

String RaiiFile::asString()
{
    String str;
    while (m_file.available())
    {
        str += static_cast<char>(m_file.read());
    }

    return str;
}