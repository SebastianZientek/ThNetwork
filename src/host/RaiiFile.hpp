#pragma once

#include <Arduino.h>

#include "common/logger.hpp"

class File;

template <typename FileSystem, typename FileType = File>
class RaiiFile
{
public:
    explicit RaiiFile(FileSystem filesystem,
                      const std::string &path,
                      const char *mode = "r",
                      bool create = false)
        : m_file(filesystem.open(path.c_str(), mode, create))
    {
        if (!m_file)
        {
            logger::logErr("Failed to open file for reading");
        }
    }
    RaiiFile(const RaiiFile &) = delete;
    RaiiFile(RaiiFile &&) noexcept = default;
    RaiiFile &operator=(const RaiiFile &) = delete;
    RaiiFile &operator=(RaiiFile &&) noexcept = default;

    ~RaiiFile()  // NOLINT
    {
        m_file.close();
    }

    FileType *operator->()
    {
        return &m_file;
    }

private:
    FileType m_file{};
};
