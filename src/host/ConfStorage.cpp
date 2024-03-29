#include "ConfStorage.hpp"

#include <algorithm>
#include <optional>

#include "adapters/IRaiiFile.hpp"
#include "common/logger.hpp"
#include "host/adapters/IFileSystem32Adp.hpp"

ConfStorage::ConfStorage(const std::shared_ptr<IFileSystem32Adp> &fileSystem, std::string path)
    : m_fileSystem(fileSystem)
    , m_path(std::move(path))
{
    setDefault();
}

ConfStorage::State ConfStorage::load()
{
    auto file = m_fileSystem->open(m_path, IFileSystem32Adp::Mode::F_READ);
    std::string data = file->readString();
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

ConfStorage::State ConfStorage::save()
{
    auto file = m_fileSystem->open(m_path, IFileSystem32Adp::Mode::F_WRITE);
    try
    {
        auto data = m_jsonData.dump();
        file->print(data);
    }
    catch (nlohmann::json::type_error err)
    {
        logger::logErr("Can't dump json data of configuration file, %s", err.what());
        return State::FAIL;
    }

    return State::OK;
}

void ConfStorage::setDefault()
{
    auto defaultData = nlohmann::json();
    defaultData["admin"]["user"] = "admin";
    defaultData["admin"]["pass"] = "admin";
    defaultData["sensors"] = {};
    defaultData["serverPort"] = defaultSrvPortNumber;
    defaultData["sensorUpdatePeriodMins"] = defaultSensorUpdateMins;

    m_jsonData = defaultData;
}

void ConfStorage::setSensorUpdatePeriodMins(uint16_t minutes)
{
    m_jsonData["sensorUpdatePeriodMins"] = minutes;
}

uint16_t ConfStorage::getSensorUpdatePeriodMins() const
{
    return m_jsonData["sensorUpdatePeriodMins"];
}

void ConfStorage::setServerPort(std::size_t port)
{
    m_jsonData["serverPort"] = port;
}

std::size_t ConfStorage::getServerPort() const
{
    return m_jsonData["serverPort"];
}

void ConfStorage::setWifiConfig(const std::string &ssid, const std::string &pass)
{
    m_jsonData["wifi"]["ssid"] = ssid;
    m_jsonData["wifi"]["pass"] = pass;
}

std::optional<std::pair<std::string, std::string>> ConfStorage::getWifiConfig()
{
    try
    {
        std::string ssid = m_jsonData["wifi"]["ssid"];
        std::string pass = m_jsonData["wifi"]["pass"];

        return std::make_pair(ssid, pass);
    }
    catch (nlohmann::json::type_error err)
    {
        return std::nullopt;
    }

    return std::nullopt;
}

void ConfStorage::setAdminCredentials(const std::string &user, const std::string &pass)
{
    m_jsonData["admin"]["user"] = user;
    m_jsonData["admin"]["pass"] = pass;
}

std::optional<std::pair<std::string, std::string>> ConfStorage::getAdminCredentials() const
{
    try
    {
        auto admin = m_jsonData["admin"];
        return std::make_pair(admin["user"], admin["pass"]);
    }
    catch (nlohmann::json::type_error err)
    {
        return std::nullopt;
    }
    return std::nullopt;
}

std::string ConfStorage::getConfigWithoutCredentials() const
{
    nlohmann::json dataWithoutCred
        = {{"sensorUpdatePeriodMins", m_jsonData["sensorUpdatePeriodMins"]},
           {"sensors", m_jsonData["sensors"]},
           {"serverPort", m_jsonData["serverPort"]}};

    return dataWithoutCred.dump();
}

bool ConfStorage::isAvailableSpaceForNextSensor()
{
    return m_jsonData["sensors"].size() < maxSensorsNum;
}

bool nameExists(const nlohmann::json &data, const std::string &name)
{
    return std::any_of(data.begin(), data.end(),
                       [name](auto sensor)
                       {
                           return sensor == name;
                       });
}

bool ConfStorage::addSensor(IDType identifier, const std::string &name)
{
    std::string newSensorName = name;
    if (name.empty())
    {
        for (size_t i = 0; i < maxSensorsNum; ++i)
        {
            std::string nameToSet = "Unnamed " + std::to_string(i + 1);
            if (!nameExists(m_jsonData["sensors"], nameToSet))
            {
                newSensorName = nameToSet;
                break;
            }
        }
    }

    if (isAvailableSpaceForNextSensor())
    {
        m_jsonData["sensors"][std::to_string(identifier)] = newSensorName;
        return true;
    }

    return false;
}

bool ConfStorage::removeSensor(IDType identifier)
{
    auto toRemove = m_jsonData["sensors"].find(std::to_string(identifier));
    auto end = m_jsonData["sensors"].end();

    if (toRemove != end)
    {
        m_jsonData["sensors"].erase(toRemove);
        return true;
    }

    return false;
}

std::string ConfStorage::getSensorsMapping() const
{
    return m_jsonData["sensors"].dump();
}

bool ConfStorage::isSensorMapped(IDType identifier)
{
    auto sensorIt = m_jsonData["sensors"].find(std::to_string(identifier));
    auto end = m_jsonData["sensors"].end();

    return sensorIt != end;
}
