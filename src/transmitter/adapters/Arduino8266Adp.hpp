#pragma once

#include "IArduino8266Adp.hpp"

class Arduino8266Adp : public IArduino8266Adp
{
public:
    void pinMode(uint8_t pin, Mode mode) const override;
    [[nodiscard]] bool digitalRead(uint8_t pin) const override;
    void digitalWrite(uint8_t pin, bool val) const override;
    [[nodiscard]] uint8_t getLedBuiltin() const override;
    [[nodiscard]] unsigned long millis() const override;
    void delay(unsigned long milliseconds) const override;
    void setupWire(int sda, int scl) const override;
};
