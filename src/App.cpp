#include "App.hpp"

#include <Arduino_JSON.h>

#include "SystemInit.hpp"
#include "logger.hpp"

void App::run()
{
    static SystemInit sysInit;
    sysInit.init();

    auto &sensorsMap = sysInit.getConfig().getSensorsMap();

    m_espNow.init(
        [this, sensorsMap](float temp, float hum, String mac)
        {
            JSONVar board;
            board["id"] = 1;
            board["temperature"] = temp;
            board["humidity"] = hum;
            board["readingId"] = "Blabla";
            auto containsMac = sensorsMap.find(mac) != sensorsMap.end();
            board["name"] = containsMac ? sensorsMap.at(mac) : mac;
            String jsonString = JSON.stringify(board);
            m_web.sendEvent(jsonString.c_str(), "new_readings", millis());
        });

    m_web.load("/index.html");
    m_web.startServer();
}