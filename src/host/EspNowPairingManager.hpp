#pragma once

#include <SPIFFS.h>

#include <cstddef>
#include <memory>

#include "LedIndicator.hpp"
#include "RaiiFile.hpp"
#include "common/types.hpp"
#include "interfaces/IConfStorage.hpp"

class EspNowPairingManager
{
public:
    explicit EspNowPairingManager(std::shared_ptr<IConfStorage> confStorage,
                                std::shared_ptr<LedIndicator> pairingLed = nullptr)
        : m_confStorage(confStorage)
        , m_pairingLed(pairingLed)
    {
    }

    void enablePairing()
    {
        if (isPairingEnabled())
        {
            return;
        }

        m_pairingEnabled = true;
        m_pairingLed->blinking();
        m_pairingTimer.setCallback(
            [this]
            {
                m_pairingLed->switchOn(false);
                m_pairingEnabled = false;
            });
        m_pairingTimer.start(m_pairingTimeout);
    }

    void disablePairing()
    {
        m_pairingEnabled = false;
    }

    [[nodiscard]] bool isPairingEnabled() const
    {
        return m_pairingEnabled;
    }

    bool isPaired(IDType identifier)
    {
        return m_confStorage->isSensorMapped(identifier);
    }

    bool pairSensor(IDType identifier)
    {
        if (isPaired(identifier))
        {
            return true;
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

    void unpairSensor(IDType identifier)
    {
        m_confStorage->removeSensor(identifier);
    }

    void update()
    {
        m_pairingTimer.update();
    }

private:
    constexpr static auto m_pairingTimeout = 1000 * 60 * 2;  // 2 minutes

    std::shared_ptr<IConfStorage> m_confStorage;
    std::shared_ptr<LedIndicator> m_pairingLed;

    bool m_pairingEnabled = false;
    Timer m_pairingTimer{};
};
