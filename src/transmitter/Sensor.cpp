#include "Sensor.hpp"

#include <Adafruit_AHTX0.h>

#include "common/logger.hpp"

struct Sensor::Impl
{
    Adafruit_AHTX0 aht;
};

Sensor::Sensor(std::shared_ptr<IArduino8266Adp> arduinoAdp)
    : m_impl{std::make_unique<Impl>()}
    , m_arduinoAdp(arduinoAdp)
{
}

Sensor::~Sensor() = default;

void Sensor::init(int sda, int scl)
{
    m_arduinoAdp->setupWire(sda, scl);
    if (m_impl->aht.begin())
    {
        logger::logInf("Found AHT20");
    }
    else
    {
        logger::logErr("Didn't find AHT20");
    }
}

std::pair<float, float> Sensor::getTempHum()
{
    sensors_event_t humidity{};     // NOLINT
    sensors_event_t temperature{};  // NOLINT
    m_impl->aht.getEvent(&humidity, &temperature);
    return {temperature.temperature, humidity.relative_humidity};  // NOLINT
}
