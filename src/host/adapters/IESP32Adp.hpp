#pragma once

class IESP32Adp
{
public:
    virtual ~IESP32Adp() = default;
    IESP32Adp() = default;
    IESP32Adp(const IESP32Adp &) = delete;
    IESP32Adp(IESP32Adp &&) noexcept = default;
    IESP32Adp &operator=(const IESP32Adp &) = delete;
    IESP32Adp &operator=(IESP32Adp &&) noexcept = default;

    virtual void restart() const = 0;
};
