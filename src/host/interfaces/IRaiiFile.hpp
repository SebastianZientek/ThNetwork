#pragma once

namespace fs{
    class File;
}

class IRaiiFile
{
public:
    IRaiiFile() = default;
    IRaiiFile(const IRaiiFile &) = delete;
    IRaiiFile(IRaiiFile &&) noexcept = default;
    IRaiiFile &operator=(const IRaiiFile &) = delete;
    IRaiiFile &operator=(IRaiiFile &&) noexcept = default;

    virtual ~IRaiiFile() = default;
    virtual fs::File *operator->() = 0;
};
