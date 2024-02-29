#pragma once

#include <memory>

#include "adapters/IArduino8266Adp.hpp"

namespace utils
{
void switchOnLed(const std::shared_ptr<IArduino8266Adp> &arduinoAdp);
void switchOffLed(const std::shared_ptr<IArduino8266Adp> &arduinoAdp);
}  // namespace utils
