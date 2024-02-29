#pragma once

#include "IEsp8266Adp.hpp"

class Esp8266Adp : public IEsp8266Adp
{
public:
    void feedWatchdog() const override;
    void yield() const override;
    [[nodiscard]] bool isResetReasonDeepSleepAwake() const override;
};
