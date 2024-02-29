#pragma once

#include <Esp.h>

#include "IESP32Adp.hpp"

class ESP32Adp : public IESP32Adp
{
public:
    void restart() const override;
};
