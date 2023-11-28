#include "utils.hpp"

#include "adapters/ArduinoAdp.hpp"

namespace utils
{
void toggleLed()
{
    auto ledPin = ArduinoAdp::getLedBuiltin();
    auto ledState = ArduinoAdp::digitalRead(ledPin);
    ArduinoAdp::digitalWrite(ledPin, ledState == ArduinoAdp::LVL_HI ? ArduinoAdp::LVL_LO : ArduinoAdp::LVL_HI);
}
}  // namespace utils
