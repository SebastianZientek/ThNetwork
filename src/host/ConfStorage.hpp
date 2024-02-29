#pragma once

#include <array>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>

#include "adapters/IFileSystem32Adp.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"
#include "interfaces/IConfStorage.hpp"

class ConfStorage : public IConfStorage
{
public:
    ConfStorage(const std::shared_ptr<IFileSystem32Adp> &fileSystem, std::string path);

    State load() override;
    State save() override;

    void setDefault();

    void setSensorUpdatePeriodMins(uint16_t minutes) override;
    [[nodiscard]] uint16_t getSensorUpdatePeriodMins() const override;
    void setServerPort(std::size_t port) override;
    [[nodiscard]] std::size_t getServerPort() const override;
    void setWifiConfig(const std::string &ssid, const std::string &pass) override;
    std::optional<std::pair<std::string, std::string>> getWifiConfig() override;
    void setAdminCredentials(const std::string &user, const std::string &pass) override;
    [[nodiscard]] std::optional<std::pair<std::string, std::string>> getAdminCredentials()
        const override;

    [[nodiscard]] std::string getConfigWithoutCredentials() const override;

    bool isAvailableSpaceForNextSensor() override;
    bool addSensor(IDType identifier, const std::string &name = "") override;
    bool removeSensor(IDType identifier) override;
    [[nodiscard]] std::string getSensorsMapping() const override;
    bool isSensorMapped(IDType identifier) override;

private:
    // Limited because of space for readings
    constexpr static auto maxSensorsNum = 7;
    constexpr static auto defaultSrvPortNumber = 80;
    constexpr static auto defaultSensorUpdateMins = 1;

    nlohmann::json m_jsonData{};
    std::shared_ptr<IFileSystem32Adp> m_fileSystem;
    std::string m_path;
};
