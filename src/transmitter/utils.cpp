#include "utils.hpp"

#include "adapters/ArduinoAdp.hpp"

namespace utils
{
void toggleLed()
{
    auto ledPin = ArduinoAdp::getLedBuiltin();
    auto ledState = ArduinoAdp::digitalRead(ledPin);
    ArduinoAdp::digitalWrite(ledPin, ledState == ArduinoAdp::HI ? ArduinoAdp::LO : ArduinoAdp::HI);
}
}  // namespace utils
