#pragma once

#include "IESP32Adp.hpp"

#include <Esp.h>

class ESP32Adp: public IESP32Adp
{
public:
    void restart() override
    {
        ESP.restart();
    }
};
