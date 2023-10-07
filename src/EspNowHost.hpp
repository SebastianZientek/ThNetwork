#pragma once

#include <Arduino.h>
#include <esp_now.h>

#include <cstdint>

#include "THMessage.hpp"

using RecvCB = void (*)(THMessage, String);
void initEspNow(RecvCB cb);
