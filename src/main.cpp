#include <Arduino.h>

#include "EspNowHost.hpp"
#include "SystemInit.hpp"
#include "WebView.hpp"
#include "logger.hpp"

WebView web(80);
JSONVar board;
void setup()
{
    SystemInit sysInit;
    sysInit.init();

    // Init ESP-NOW
    initEspNow(
        [](float temp, float hum, String s)
        {
            board["id"] = 1;
            board["temperature"] = temp;
            board["humidity"] = hum;
            board["readingId"] = "Blabla";
            String jsonString = JSON.stringify(board);
            web.sendEvent(jsonString.c_str(), "new_readings", millis());
        });

    web.load("/index.html");
    web.startServer();
}

void loop()
{
}
