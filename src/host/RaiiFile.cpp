#include "RaiiFile.hpp"

#include "common/logger.hpp"

RaiiFile::RaiiFile(fs::FS filesystem, const std::string &path, const char *mode, bool create)
{
    m_file = filesystem.open(path.c_str(), mode, create);
    if (!m_file)
    {
        logger::logErr("Failed to open file for reading");
    }
}

RaiiFile::~RaiiFile()  // NOLINT
{
    m_file.close();
}

File *RaiiFile::operator->()
{
    return &m_file;
}
