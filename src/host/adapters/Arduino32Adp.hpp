#pragma once

#include "IArduino32Adp.hpp"

class Arduino32Adp : public IArduino32Adp
{
public:
    void pinMode(uint8_t pin, Mode mode) const override;
    [[nodiscard]] Lvl digitalRead(uint8_t pin) const override;
    void digitalWrite(uint8_t pin, Lvl val) const override;
    [[nodiscard]] uint8_t getLedBuiltin() const override;
    [[nodiscard]] unsigned long millis() const override;
    void delay(unsigned long milliseconds) const override;
};
