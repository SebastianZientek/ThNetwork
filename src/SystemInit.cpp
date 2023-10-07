#include "SystemInit.hpp"

#include <SD.h>
#include <WiFi.h>
#include <esp_now.h>

#include "RaiiFile.hpp"

void SystemInit::init()
{
    if (initSerial() == Status::FAIL) return;
    if (initSD() == Status::FAIL) return;
    if (readConfig() == Status::FAIL) return;
    if (connectWiFi() == Status::FAIL) return;
}

SystemInit::Status SystemInit::initSerial()
{
    Serial.begin(115200);
    return Status::OK;
}

SystemInit::Status SystemInit::initSD()
{
    if (!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return Status::FAIL;
    }

    if (SD.cardType() == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return Status::FAIL;
    }
    return Status::OK;
}

SystemInit::Status SystemInit::readConfig()
{
    RaiiFile file("/config.json");
    String data = file.asString();
    if (!m_config.load(data))
    {
        Serial.println("Reading config error");
        return Status::FAIL;
    }
    return Status::OK;
}

SystemInit::Status SystemInit::connectWiFi()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(m_config.getWifiSsid(), m_config.getWifiPass());
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();

    Serial.printf("Connected to %s IP: %s MAC: %s\n", WiFi.SSID(),
    WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());
    Serial.println(WiFi.channel());

    return Status::OK;
}