#pragma once

#include "IFileSystem32Adp.hpp"
#include "IRaiiFile.hpp"

class LittleFSAdp : public IFileSystem32Adp
{
public:
    LittleFSAdp();
    [[nodiscard]] std::unique_ptr<IRaiiFile> open(const std::string &path,
                                                  Mode mode) const override;
};
