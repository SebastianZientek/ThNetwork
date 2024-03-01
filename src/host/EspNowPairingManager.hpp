#pragma once

#include <cstddef>
#include <memory>

#include "IConfStorage.hpp"
#include "LedIndicator.hpp"
#include "adapters/IArduino32Adp.hpp"
#include "common/types.hpp"

class EspNowPairingManager
{
public:
    explicit EspNowPairingManager(const std::shared_ptr<IConfStorage> &confStorage,
                                  const std::shared_ptr<IArduino32Adp> &arduinoAdp,
                                  const std::shared_ptr<LedIndicator> &pairingLed = nullptr);

    void enablePairingForPeriod(std::size_t timeout = m_pairingDefaultTimeout);
    void disablePairing();
    [[nodiscard]] bool isPairingEnabled() const;
    [[nodiscard]] bool isPaired(IDType identifier) const;
    bool addNewSensorToStorage(IDType identifier);
    void unpairSensor(IDType identifier);
    void update();

private:
    constexpr static auto m_pairingDefaultTimeout = 1000 * 60 * 2;  // 2 minutes

    std::shared_ptr<IConfStorage> m_confStorage;
    std::shared_ptr<LedIndicator> m_pairingLed;

    bool m_pairingEnabled = false;
    Timer m_pairingTimer;
};
