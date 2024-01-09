#include <Arduino.h>

#include "App.hpp"

#include <SPIFFS.h>

App app;  // NOLINT
void setup()
{
    app.init();
}

void loop()
{
    app.update();
}
