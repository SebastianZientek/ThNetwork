#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <optional>

#include "RaiiFile.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"
#include "interfaces/IConfStorage.hpp"

class ConfStorage : public IConfStorage
{
public:
    State load(IRaiiFile &file) override;
    State save(IRaiiFile &file) override;

    void setDefault();

    void setSensorUpdatePeriodMins(std::size_t minutes) override;
    std::size_t getSensorUpdatePeriodMins() override;
    void setServerPort(std::size_t port) override;
    std::size_t getServerPort() override;
    void setWifiConfig(std::string ssid, std::string pass) override;
    std::optional<std::pair<std::string, std::string>> getWifiConfig() override;
    void setAdminCredentials(std::string user, std::string pass) override;
    std::optional<std::pair<std::string, std::string>> getAdminCredentials() override;

    std::string getConfigWithoutCredentials() override;

    bool isAvailableSpaceForNextSensor() override;
    bool addSensor(IDType identifier, const std::string &name = "Unnamed") override;
    bool removeSensor(IDType identifier) override;
    std::string getSensorsMapping() override;
    bool isSensorMapped(IDType identifier) override;

private:
    // Limited because of space for readings
    constexpr static auto maxSensorsNum = 7;
    constexpr static auto defaultSrvPortNumber = 80;
    constexpr static auto defaultSensorUpdateMins = 1;
    nlohmann::json m_jsonData{};
};
