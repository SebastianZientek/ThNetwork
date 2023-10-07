#pragma once

#include <Arduino.h>
#include <Arduino_JSON.h>

#include <map>

class Config
{
public:
    bool load(String data);

    String getWifiSsid();
    String getWifiPass();
    std::map<String, String> getSensorsMap();

private:
    String m_wifiSsid;
    String m_wifiPass;
    std::map<String, String> m_sensorsMap;
};