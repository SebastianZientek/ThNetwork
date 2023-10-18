#pragma once

#include <Arduino.h>
#include <esp_now.h>

#include <cstdint>

#include "THMessage.hpp"

using RecvCB = void (*)(float temp, float hum, String mac);
void initEspNow(RecvCB cb);
