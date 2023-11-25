#include <Arduino.h>

#include "App.hpp"

App app{};  // NOLINT

void setup()
{
    app.setup();
}

void loop()
{
    app.loop();
}
