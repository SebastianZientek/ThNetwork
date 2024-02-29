#pragma once

#include <CppUTestExt/MockSupport.h>

#include "adapters/IFileSystem32Adp.hpp"

class FileSystem32AdpMock : public IFileSystem32Adp
{
    [[nodiscard]] std::unique_ptr<IRaiiFile> open(const std::string &path, Mode mode) const override
    {
        mock("FileSystem32AdpMock")
            .actualCall("open")
            .withParameter("path", path.c_str())
            .withParameter("mode", static_cast<int>(mode));
        auto *voidPtr = mock("FileSystem32AdpMock").returnPointerValueOrDefault(nullptr);
        auto filePtr = std::unique_ptr<IRaiiFile>(static_cast<IRaiiFile *>(voidPtr));
        return filePtr;
    }
};
