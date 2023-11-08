#include <Arduino.h>

#include "App.hpp"
#include "EspNow.hpp"

App app;  // NOLINT
void setup() { app.init(); }

void loop() { app.update(); }
