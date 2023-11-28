#include "utils.hpp"

#include "adapters/ArduinoAdp.hpp"

namespace utils
{

void switchOnLed()
{
    auto ledPin = ArduinoAdp::getLedBuiltin();
    ArduinoAdp::digitalWrite(ledPin, ArduinoAdp::LVL_LO);
}

void switchOffLed()
{
    auto ledPin = ArduinoAdp::getLedBuiltin();
    ArduinoAdp::digitalWrite(ledPin, ArduinoAdp::LVL_HI);
}

}  // namespace utils
