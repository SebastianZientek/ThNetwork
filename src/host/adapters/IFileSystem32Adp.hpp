#pragma once

#include <memory>

#include "IRaiiFile.hpp"

class IFileSystem32Adp
{
public:
    enum class Mode
    {
        F_READ,
        F_WRITE,
        F_APPEND
    };

    IFileSystem32Adp() = default;
    virtual ~IFileSystem32Adp() = default;
    IFileSystem32Adp(const IFileSystem32Adp &) = default;
    IFileSystem32Adp(IFileSystem32Adp &&) noexcept = default;
    IFileSystem32Adp &operator=(const IFileSystem32Adp &) = default;
    IFileSystem32Adp &operator=(IFileSystem32Adp &&) noexcept = default;

    virtual std::unique_ptr<IRaiiFile> open(const std::string &path, Mode mode) = 0;
};
