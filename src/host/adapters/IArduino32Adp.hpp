#pragma once

#include <cinttypes>

class IArduino32Adp
{
public:
    enum class Mode
    {
        PIN_OUTPUT,
        PIN_PULLUP,
        PIN_INPUT_PULLUP,
        PIN_PULLDOWN,
        PIN_INPUT_PULLDOWN,
        PIN_OPEN_DRAIN,
        PIN_OUTPUT_OPEN_DRAIN,
        PIN_ANALOG,
    };

    enum class Lvl
    {
        Low,
        High
    };

    IArduino32Adp() = default;
    virtual ~IArduino32Adp() = default;
    IArduino32Adp(const IArduino32Adp &) = default;
    IArduino32Adp(IArduino32Adp &&) noexcept = default;
    IArduino32Adp &operator=(const IArduino32Adp &) = default;
    IArduino32Adp &operator=(IArduino32Adp &&) noexcept = default;

    virtual void pinMode(uint8_t pin, Mode mode) const = 0;
    [[nodiscard]] virtual Lvl digitalRead(uint8_t pin) const = 0;
    virtual void digitalWrite(uint8_t pin, Lvl val) const = 0;
    [[nodiscard]] virtual uint8_t getLedBuiltin() const = 0;
    [[nodiscard]] virtual unsigned long millis() const = 0;
    virtual void delay(unsigned long milliseconds) const = 0;
};
