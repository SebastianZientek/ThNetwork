#include "WifiConfigurator.hpp"

WiFiConfigurator::WiFiConfigurator(
    std::shared_ptr<IArduino32Adp> arduinoAdp,
    std::shared_ptr<IWifi32Adp> wifiAdp,
    std::shared_ptr<IConfStorage> confStorage,
    std::shared_ptr<WifiConfiguratorWebServer> wifiConfiguratorWebServer)
    : m_arduinoAdp(arduinoAdp)
    , m_wifiAdp(wifiAdp)
    , m_confStorage(confStorage)
    , m_wifiConfiguratorWebServer(wifiConfiguratorWebServer)
{
}

void WiFiConfigurator::connect()
{
    m_mode = Mode::INITIALIZE;
}

WiFiConfigurator::Status WiFiConfigurator::processWifiConfiguration()
{
    switch (m_mode)
    {
    case Mode::NOT_INITIALIZED:
        return Status::NOT_INITIALIZED;
        break;

    case Mode::INITIALIZE:
        logger::logDbg("WifiConfigurator Mode::INITIALIZE");
        initialize();
        break;

    case Mode::PROCESSING_CONNECTION:
        m_connectionRetriesTimer.update();
        return Status::CONNECTION_ONGOING;
        // TODO: Reconnect if connection lost
        break;

    case Mode::HOST_WIFI_CONFIGURATOR:
        logger::logDbg("WifiConfigurator Mode::HOST_WIFI_CONFIGURATOR");
        // m_wifiConfiguratorWebServer->startServer();
        return Status::CONFIGURATION_PAGE_HOSTED;
        break;

    case Mode::CONNECTED:
        logger::logDbg("WifiConfigurator Mode::CONNECTED");
        return Status::CONNECTED;
        break;

    case Mode::ERROR:
        return Status::CONNECTION_FAILURE;
        break;
    }

    return Status::NOT_INITIALIZED;
}

void WiFiConfigurator::initialize()
{
    logger::logInf("Connecting to WiFi");
    m_wifiAdp->setMode(IWifi32Adp::Mode::AP_STA);

    auto wifiConfig = m_confStorage->getWifiConfig();
    if (!wifiConfig)
    {
        logger::logWrn("No wifi configuration!");
        m_mode = Mode::HOST_WIFI_CONFIGURATOR;
    };

    auto [ssid, pass] = wifiConfig.value();
    m_wifiAdp->init(ssid, pass);

    auto connectToWifi = [this, retries = 0]() mutable
    {
        if (retries++, retries > m_maxConnectionRetries)
        {
            m_connectionRetriesTimer.stop();
            m_mode = Mode::ERROR;
            // CONNECTION ERROR, STOP TRYING
        }

        logger::logInf(".");
        if (m_wifiAdp->getStatus() == IWifi32Adp::Status::CONNECTED)
        {
            m_mode = Mode::CONNECTED;
            logger::logInf("Connected to %s IP: %s MAC: %s, channel %d", m_wifiAdp->getSsid(),
                           m_wifiAdp->getLocalIp(), m_wifiAdp->getMacAddr(),
                           m_wifiAdp->getChannel());
        }
    };

    connectToWifi();
    m_connectionRetriesTimer.setCallback(connectToWifi);
    m_connectionRetriesTimer.start(m_delayBetweenConnectionRetiresMs, true);

    m_mode = Mode::PROCESSING_CONNECTION;
}
