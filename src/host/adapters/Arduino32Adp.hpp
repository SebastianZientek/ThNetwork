#pragma once

#include "IArduino32Adp.hpp"

class Arduino32Adp : public IArduino32Adp
{
public:
    void pinMode(uint8_t pin, Mode mode) override;
    bool digitalRead(uint8_t pin) override;
    void digitalWrite(uint8_t pin, bool val) override;
    uint8_t getLedBuiltin() override;
    unsigned long millis() override;
    void delay(unsigned long milliseconds) override;
};
