#pragma once

#include <string>

class IRaiiFile
{
public:
    virtual ~IRaiiFile() = default;
    IRaiiFile() = default;
    IRaiiFile(const IRaiiFile &) = delete;
    IRaiiFile(IRaiiFile &&) noexcept = default;
    IRaiiFile &operator=(const IRaiiFile &) = delete;
    IRaiiFile &operator=(IRaiiFile &&) noexcept = default;

    [[nodiscard]] virtual std::string readString() = 0;
    virtual void print(const std::string &) = 0;
};
