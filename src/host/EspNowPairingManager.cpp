#include "EspNowPairingManager.hpp"

#include <SPIFFS.h>

EspNowPairingManager::EspNowPairingManager(std::shared_ptr<IConfStorage> confStorage,
                                           std::shared_ptr<IArduino32Adp> arduinoAdp,
                                           std::shared_ptr<LedIndicator> pairingLed)
    : m_confStorage(confStorage)
    , m_pairingLed(pairingLed)
    , m_pairingTimer(arduinoAdp)
{
}

void EspNowPairingManager::enablePairingForPeriod(std::size_t timeout)
{
    if (isPairingEnabled())
    {
        return;
    }

    m_pairingEnabled = true;
    if (m_pairingLed)
    {
        m_pairingLed->blinking();
    }

    m_pairingTimer.setCallback(
        [this]
        {
            if (m_pairingLed)
            {
                m_pairingLed->switchOn(false);
            }
            m_pairingEnabled = false;
        });
    m_pairingTimer.start(timeout);
}

void EspNowPairingManager::disablePairing()
{
    m_pairingEnabled = false;
}

[[nodiscard]] bool EspNowPairingManager::isPairingEnabled() const
{
    return m_pairingEnabled;
}

bool EspNowPairingManager::isPaired(IDType identifier)
{
    return m_confStorage->isSensorMapped(identifier);
}

bool EspNowPairingManager::addNewSensorToStorage(IDType identifier)
{
    if (isPaired(identifier))
    {
        return false;
    }

    if (!m_confStorage->isAvailableSpaceForNextSensor())
    {
        logger::logWrn("No space for more sensors");
        return false;
    }

    logger::logInf("Paired sensor with ID: %u", identifier);
    m_confStorage->addSensor(identifier);

    RaiiFile configFile(SPIFFS.open("/config.json", "w"));
    m_confStorage->save(configFile);

    return true;
}

void EspNowPairingManager::unpairSensor(IDType identifier)
{
    m_confStorage->removeSensor(identifier);
}

void EspNowPairingManager::update()
{
    m_pairingTimer.update();
}
