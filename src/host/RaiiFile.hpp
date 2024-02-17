#pragma once

#include <FS.h>

#include "adapters/IRaiiFile.hpp"

class RaiiFile : public IRaiiFile
{
public:
    explicit RaiiFile(const fs::File &file)
        : m_file(file)
    {
    }

    RaiiFile(const RaiiFile &) = delete;
    RaiiFile(RaiiFile &&) noexcept = default;
    RaiiFile &operator=(const RaiiFile &) = delete;
    RaiiFile &operator=(RaiiFile &&) noexcept = default;

    virtual ~RaiiFile()  // NOLINT
    {
        m_file.close();
    }

    std::string readString() override
    {
        return m_file.readString().c_str();
    }

    void print(const std::string &str) override
    {
        m_file.print(str.c_str());
    }

    // fs::File *operator->() override
    // {
    //     return &m_file;
    // }

private:
    fs::File m_file;
};
