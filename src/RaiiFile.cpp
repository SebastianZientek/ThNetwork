#include "RaiiFile.hpp"

#include <SD.h>
#include <SPI.h>

RaiiFile::RaiiFile(String path)
{
    m_file = SD.open(path);
    if (!m_file)
    {
        Serial.println("Failed to open file for reading");
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
        char c = m_file.read();
        str += c;
    }

    return str;
}