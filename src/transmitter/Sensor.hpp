#pragma once

#include <memory>
#include <utility>

class Sensor
{
public:
    Sensor();
    ~Sensor();

    Sensor(const Sensor &) = delete;
    Sensor &operator=(const Sensor &) = delete;
    Sensor(const Sensor &&) = delete;
    Sensor &operator=(const Sensor &&) = delete;

    void init();
    std::pair<float, float> getTempHum();

private:
    struct Impl;

    std::unique_ptr<Impl> m_impl;
};
