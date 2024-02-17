#pragma once

#include "IFileSystem32Adp.hpp"
#include "IRaiiFile.hpp"
#include <SPIFFS.h>  // TODO: move to cpp

class SPIFFS32Adp : public IFileSystem32Adp
{
public:
    SPIFFS32Adp()
    {
        SPIFFS.begin(true);
    }

    std::unique_ptr<IRaiiFile> open(const std::string &path, Mode mode) override
    {
        auto nativeMode = FILE_READ;
        switch (mode)
        {
        case Mode::F_READ:
            nativeMode = FILE_READ;
            break;
        case Mode::F_WRITE:
            nativeMode = FILE_WRITE;
            break;
        case Mode::F_APPEND:
            nativeMode = FILE_APPEND;
            break;
        }

        return std::make_unique<RaiiFile>(SPIFFS.open(path.c_str(), nativeMode));
    }
};
