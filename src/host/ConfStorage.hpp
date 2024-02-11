#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <optional>

#include "interfaces/IConfStorage.hpp"
#include "RaiiFile.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"

class ConfStorage : public IConfStorage
{
public:
    enum class State
    {
        OK,
        FAIL
    };

    template <typename RaiiFileT>
    State load(RaiiFileT &file)
    {
        std::string data = file->readString().c_str();
        try
        {
            m_jsonData = nlohmann::json::parse(data);
        }
        catch (nlohmann::json::parse_error err)
        {
            logger::logErr("Can't parse json data, %s", err.what());
            return State::FAIL;
        }

        return State::OK;
    }

    template <typename RaiiFileT>
    State save(RaiiFileT &file)
    {
        try
        {
            auto data = m_jsonData.dump();
            file->print(data.c_str());
        }
        catch (nlohmann::json::type_error err)
        {
            logger::logErr("Can't dump json data of configuration file, %s", err.what());
            return State::FAIL;
        }

        return State::OK;
    }

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

private:
    // Limited because of space for readings
    constexpr static auto maxSensorsNum = 7;
    constexpr static auto defaultSrvPortNumber = 80;
    constexpr static auto defaultSensorUpdateMins = 1;
    nlohmann::json m_jsonData{};
};
