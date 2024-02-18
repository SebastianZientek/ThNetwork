#pragma once

#include <memory>

#include "adapters/IArduino8266Adp.hpp"

namespace utils
{
void switchOnLed(std::shared_ptr<IArduino8266Adp> arduinoAdp);
void switchOffLed(std::shared_ptr<IArduino8266Adp> arduinoAdp);
}  // namespace utils
