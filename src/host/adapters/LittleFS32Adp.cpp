#include "LittleFS32Adp.hpp"

#include <LittleFS.h>

#include "RaiiFile.hpp"

LittleFSAdp::LittleFSAdp()
{
    LittleFS.begin(true);
}

std::unique_ptr<IRaiiFile> LittleFSAdp::open(const std::string &path, Mode mode) const
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

    return std::make_unique<RaiiFile>(LittleFS.open(path.c_str(), nativeMode));
}
