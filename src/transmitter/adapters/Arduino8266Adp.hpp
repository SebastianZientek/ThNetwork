#pragma once

#include "IArduino8266Adp.hpp"

class Arduino8266Adp: public IArduino8266Adp
{
public:
    void pinMode(uint8_t pin, Mode mode) override;
    bool digitalRead(uint8_t pin) override;
    void digitalWrite(uint8_t pin, bool val) override;
    uint8_t getLedBuiltin() override;
    unsigned long millis() override;
    void delay(unsigned long milliseconds) override;
};
