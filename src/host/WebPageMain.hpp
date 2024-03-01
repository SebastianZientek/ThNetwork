#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>

#include "IConfStorage.hpp"
#include "IResources.hpp"
#include "adapters/IArduino32Adp.hpp"
#include "common/logger.hpp"
#include "webserver/IWebServer.hpp"

class WebPageMain
{
    using GetSensorDataCb = std::function<std::string(const std::size_t &)>;

    constexpr static auto HTML_OK = 200;
    constexpr static auto HTML_BAD_REQ = 400;
    constexpr static auto HTML_UNAUTH = 401;
    constexpr static auto HTML_NOT_FOUND = 404;
    constexpr static auto HTML_INTERNAL_ERR = 500;
    constexpr static auto RECONNECT_TIMEOUT = 10000;

public:
    WebPageMain(const std::shared_ptr<IArduino32Adp> &arduinoAdp,
                const std::shared_ptr<IWebServer> &webServer,
                std::unique_ptr<IResources> resources,
                const std::shared_ptr<IConfStorage> &confStorage);

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0);
    void startServer(const GetSensorDataCb &getSensorDataCb);
    void stopServer();

private:
    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
    std::shared_ptr<IWebServer> m_server;
    std::unique_ptr<IResources> m_resources;
    std::shared_ptr<IConfStorage> m_confStorage;

    GetSensorDataCb m_getSensorDataCb;

    void setupResources();
    void setupActions();

    bool auth(IWebRequest &request);
    void setCredentials(IWebRequest &request, const std::string &body);
    void updateSensorsMapping(IWebRequest &request, const std::string &body);
    void setProperties(IWebRequest &request, const std::string &body);
    void removeSensor(IWebRequest &request, const std::string &body);
    void sensorIDsToNames(IWebRequest &request);
    void configuration(IWebRequest &request);
    void sensorData(IWebRequest &request);
};
