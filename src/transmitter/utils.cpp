#include "utils.hpp"

namespace utils
{

void switchOnLed(const std::shared_ptr<IArduino8266Adp> &arduinoAdp)
{
    auto ledPin = arduinoAdp->getLedBuiltin();
    arduinoAdp->digitalWrite(ledPin, false);
}

void switchOffLed(const std::shared_ptr<IArduino8266Adp> &arduinoAdp)
{
    auto ledPin = arduinoAdp->getLedBuiltin();
    arduinoAdp->digitalWrite(ledPin, true);
}

}  // namespace utils
