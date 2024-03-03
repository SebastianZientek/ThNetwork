#include "WifiConfigurator.hpp"

WiFiConfigurator::WiFiConfigurator(const std::shared_ptr<IArduino32Adp> &arduinoAdp,
                                   const std::shared_ptr<IWifi32Adp> &wifiAdp)
    : m_arduinoAdp(arduinoAdp)
    , m_wifiAdp(wifiAdp)
{
}

void WiFiConfigurator::connect(const std::string &ssid, const std::string &pass)
{
    m_ssid = ssid;
    m_pass = pass;
    initialize();
}

WiFiConfigurator::Status WiFiConfigurator::status()
{
    return m_status;
}

void WiFiConfigurator::update()
{
    m_connectionRetriesTimer.update();
}

void WiFiConfigurator::initialize()
{
    logger::logInf("Connecting to WiFi");
    m_wifiAdp->setMode(IWifi32Adp::Mode::AP_STA);
    m_wifiAdp->init(m_ssid, m_pass);

    auto connectToWifi = [this, retries = 0]() mutable
    {
        if (retries++, retries >= m_maxConnectionRetries)
        {
            m_connectionRetriesTimer.stop();
            m_status = Status::CONNECTION_FAILURE;
        }

        logger::logInf(".");
        if (m_wifiAdp->getStatus() == IWifi32Adp::Status::CONNECTED)
        {
            m_status = Status::CONNECTION_SUCCESS;
            m_connectionRetriesTimer.stop();
            logger::logInf("Connected to %s IP: %s MAC: %s, channel %d", m_wifiAdp->getSsid(),
                           m_wifiAdp->getLocalIp(), m_wifiAdp->getMacAddr(),
                           m_wifiAdp->getChannel());
        }
    };

    m_connectionRetriesTimer.setCallback(connectToWifi);
    m_connectionRetriesTimer.start(m_delayBetweenConnectionRetiresMs, true);

    m_status = Status::CONNECTION_ONGOING;
}
