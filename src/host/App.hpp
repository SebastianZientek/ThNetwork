#pragma once

#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <memory>

#include "BoardSettings.hpp"
#include "Button.hpp"
#include "ConfStorage.hpp"
#include "EspNowPairingManager.hpp"
#include "EspNowServer.hpp"
#include "LedIndicator.hpp"
#include "ReadingsStorage.hpp"
#include "Resources.hpp"
#include "Timer.hpp"
#include "WebPageMain.hpp"
#include "WifiConfigurator.hpp"
#include "WifiConfiguratorWebServer.hpp"
#include "adapters/Arduino32Adp.hpp"
#include "adapters/ESP32Adp.hpp"
#include "adapters/EspNow32Adp.hpp"
#include "adapters/LittleFS32Adp.hpp"
#include "adapters/Wifi32Adp.hpp"
#include "webserver/WebServer.hpp"

class App
{
    enum class Status
    {
        OK,
        FAIL,
        WIFI_CONFIGURATION_NEEDED
    };

    enum class State
    {
        INITIALIZATION_BASIC_COMPONENTS,
        LOADING_CONFIGURATION,
        CONNECTING_TO_WIFI,
        HOSTING_WIFI_CONFIGURATION,
        STARTING_SERVERS,
        ERROR_REBOOTING,
        RUNNING
    };

    enum class Mode
    {
        INITIALIZATION,
        WAITING_FOR_WIFI,
        HOSTING_WIFI_CONFIGURATOR,
        STARTING_SERVICES,
        NORMAL_OPERATION
    };

public:
    App() = default;
    void init();
    void update();

private:
    void startWebWifiConfiguration();

    constexpr static auto m_wifiConfigServerTimeoutMillis = 1000 * 60 * 3;  // 3 minutes
    constexpr static auto m_resetToFactorySettings = 1000 * 10;             // 10 seconds
    constexpr static auto m_wifiConfigWebPort = 80;
    constexpr static auto m_onErrorWaitBeforeRebootMs = 1000;
    constexpr static auto m_delayBetweenConnectionRetiresMs = 1000;
    constexpr static auto m_connectionRetriesBeforeRebootMs = 10;

    Mode m_mode = Mode::INITIALIZATION;
    State m_state = State::INITIALIZATION_BASIC_COMPONENTS;

    std::shared_ptr<LittleFSAdp> m_internalFS{std::make_shared<LittleFSAdp>()};
    std::shared_ptr<Wifi32Adp> m_wifiAdp{std::make_shared<Wifi32Adp>()};
    std::shared_ptr<Arduino32Adp> m_arduinoAdp{std::make_shared<Arduino32Adp>()};
    std::shared_ptr<ConfStorage> m_confStorage{
        std::make_shared<ConfStorage>(m_internalFS, "/config.json")};
    std::shared_ptr<ESP32Adp> m_espAdp{std::make_shared<ESP32Adp>()};
    std::shared_ptr<LedIndicator> m_ledIndicator{
        std::make_shared<LedIndicator>(m_arduinoAdp, boardSettings::ledIndicatorPin)};
    std::shared_ptr<EspNowPairingManager> m_pairingManager{
        std::make_unique<EspNowPairingManager>(m_confStorage, m_arduinoAdp, m_ledIndicator)};
    std::unique_ptr<EspNowServer> m_espNow{std::make_unique<EspNowServer>(
        std::make_unique<EspNow32Adp>(), m_pairingManager, m_wifiAdp, m_confStorage)};
    std::shared_ptr<NTPClient> m_timeClient{std::make_shared<NTPClient>(m_ntpUDP)};
    std::shared_ptr<WifiConfiguratorWebServer> m_webWifiConfig{
        std::make_shared<WifiConfiguratorWebServer>(
            m_wifiAdp,
            std::make_shared<WebServer>(m_wifiConfigWebPort),
            std::make_unique<Resources>(),
            m_arduinoAdp)};

    std::unique_ptr<WebPageMain> m_webPageMain{};
    WiFiUDP m_ntpUDP{};
    ReadingsStorage m_readingsStorage{};

    Button m_wifiButton{m_arduinoAdp, boardSettings::wifiButtonPin};
    Button m_pairAndResetButton{m_arduinoAdp, boardSettings::pairButtonPin};
    Timer m_wifiConfigurationTimer{m_arduinoAdp};
    WiFiConfigurator m_wifiConfigurator{m_arduinoAdp, m_wifiAdp};

    void initConfiguration();
    void initPeripherals();
    void initWifi();
    void startServices();
};
