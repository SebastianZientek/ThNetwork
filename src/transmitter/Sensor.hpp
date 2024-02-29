#pragma once

#include <memory>
#include <utility>

#include "adapters/IArduino8266Adp.hpp"

class Sensor
{
public:
    Sensor(std::shared_ptr<IArduino8266Adp> arduinoAdp);
    ~Sensor();

    Sensor(const Sensor &) = delete;
    Sensor &operator=(const Sensor &) = delete;
    Sensor(const Sensor &&) = delete;
    Sensor &operator=(const Sensor &&) = delete;

    void init(int sda, int scl);
    std::pair<float, float> getTempHum() const;

private:
    struct Impl;

    std::unique_ptr<Impl> m_impl;
    std::shared_ptr<IArduino8266Adp> m_arduinoAdp;
};
