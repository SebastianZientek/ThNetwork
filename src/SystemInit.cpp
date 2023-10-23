#include "SystemInit.hpp"

#include <SD.h>
#include <WiFi.h>
#include <esp_now.h>

#include "RaiiFile.hpp"
#include "boardsettings.hpp"
#include "logger.hpp"

void SystemInit::init()
{
    delay(initDelay);

    if (initLogger() == Status::FAIL) return;
    if (initSD() == Status::FAIL) return;
    if (readConfig() == Status::FAIL) return;
    if (connectWiFi() == Status::FAIL) return;
}

Config &SystemInit::getConfig()
{
    return m_config;
}

SystemInit::Status SystemInit::initLogger()
{
    logger::init();
    return Status::OK;
}

SystemInit::Status SystemInit::initSD()
{
    if (!SD.begin())
    {
        logger::logErr("Card Mount Failed");
        return Status::FAIL;
    }

    if (SD.cardType() == CARD_NONE)
    {
        logger::logErr("No SD card attached");
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
        logger::logErr("Reading config error");
        return Status::FAIL;
    }
    return Status::OK;
}

SystemInit::Status SystemInit::connectWiFi()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(m_config.getWifiSsid(), m_config.getWifiPass());
    logger::logInf("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(wifiConnectDelay);
        logger::logInf(".");
    }

    logger::logInfF("Connected to %s IP: %s MAC: %s, channel %d\n", WiFi.SSID(),
                  WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str(), WiFi.channel());

    return Status::OK;
}