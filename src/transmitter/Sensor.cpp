#include "Sensor.hpp"

#include <Adafruit_AHTX0.h>

#include "PinsAdp.hpp"
#include "WireAdp.hpp"
#include "common/logger.hpp"

struct Sensor::Impl
{
    Adafruit_AHTX0 aht;
};

Sensor::Sensor()
    : m_impl{std::make_unique<Impl>()}
{
}

Sensor::~Sensor() = default;

void Sensor::init()
{
    WireAdp::begin(Pins::getSDA(), Pins::getSCL());
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
    return {temperature.temperature, humidity.relative_humidity};
}
