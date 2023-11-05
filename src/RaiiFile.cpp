#include "RaiiFile.hpp"

#include <SD.h>
#include <SPI.h>

#include "logger.hpp"

RaiiFile::RaiiFile(const String &path, const char* mode, bool create)
{
    m_file = SD.open(path, mode, create);
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

void RaiiFile::print(const String &str)
{
    m_file.print(str);
}

void RaiiFile::println(const String &str)
{
    m_file.println(str);
}