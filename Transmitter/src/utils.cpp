#include "utils.hpp"

#include "ArduinoAdp.hpp"

namespace utils
{
void toggleLed()
{
    auto ledPin = ArduinoAdp::getLedBuiltin();
    auto ledState = ArduinoAdp::digitalRead(ledPin);
    ArduinoAdp::digitalWrite(
        ledPin, ledState == ArduinoAdp::HIGH ? ArduinoAdp::LOW : ArduinoAdp::HIGH);
}
}  // namespace utils
