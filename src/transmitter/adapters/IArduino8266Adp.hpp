#pragma once

#include <cinttypes>

class IArduino8266Adp
{
public:
    enum class Mode
    {
        PIN_INPUT,
        PIN_INPUT_PULLUP,
        PIN_INPUT_PULLDOWN_16,
        PIN_OUTPUT,
        PIN_OUTPUT_OPEN_DRAIN,
        PIN_WAKEUP_PULLUP,
        PIN_WAKEUP_PULLDOWN,
        PIN_SPECIAL,
    };

    IArduino8266Adp() = default;
    virtual ~IArduino8266Adp() = default;
    IArduino8266Adp(const IArduino8266Adp &) = default;
    IArduino8266Adp(IArduino8266Adp &&) noexcept = default;
    IArduino8266Adp &operator=(const IArduino8266Adp &) = default;
    IArduino8266Adp &operator=(IArduino8266Adp &&) noexcept = default;

    virtual void pinMode(uint8_t pin, Mode mode) const = 0;
    [[nodiscard]] virtual bool digitalRead(uint8_t pin) const = 0;
    virtual void digitalWrite(uint8_t pin, bool val) const = 0;
    [[nodiscard]] virtual uint8_t getLedBuiltin() const = 0;
    [[nodiscard]] virtual unsigned long millis() const = 0;
    virtual void delay(unsigned long milliseconds) const = 0;
    virtual void setupWire(int sda, int scl) const = 0;
};
