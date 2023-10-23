#pragma once

#include <Arduino.h>
#include <esp_now.h>

#include <cstdint>
#include <functional>

#include "THMessage.hpp"

using RecvCB = std::function<void(float temp, float hum, String mac)>;
void initEspNow(RecvCB cb);
