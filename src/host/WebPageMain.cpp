#include "WebPageMain.hpp"

WebPageMain::WebPageMain(const std::shared_ptr<IArduino32Adp> &arduinoAdp,
                         const std::shared_ptr<IWebServer> &webServer,
                         std::unique_ptr<IResources> resources,
                         const std::shared_ptr<IConfStorage> &confStorage)
    : m_arduinoAdp(arduinoAdp)
    , m_server(webServer)
    , m_confStorage(confStorage)
    , m_resources(std::move(resources))
{
}

void WebPageMain::sendEvent(const char *message,
                            const char *event,
                            uint32_t identifier,
                            uint32_t reconnect)

{
    logger::logDbg("Send event %s, %lu", event, identifier);
    m_server->sendEvent(message, event, identifier, reconnect);
}

void WebPageMain::setupResources()
{
    m_server->onGet("/",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /");
                        request.send(HTML_OK, "text/html", m_resources->getIndexHtml());
                    });

    m_server->onGet("/admin",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /admin");
                        if (!auth(request))
                        {
                            return request.requestAuthentication();
                        }
                        request.send(HTML_OK, "text/html", m_resources->getAdminHtml());
                    });

    m_server->onGet("/favicon.ico",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /favicon.ico");
                        request.send(HTML_OK, "image/png", m_resources->getFavicon(),
                                     m_resources->getFaviconSize());
                    });

    m_server->onGet("/microChart.js",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /microChart.js");
                        request.send(HTML_OK, "application/javascript",
                                     m_resources->getMicroChart());
                    });

    m_server->onGet("/admin.js",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /admin.js");
                        request.send(HTML_OK, "application/javascript", m_resources->getAdminJs());
                    });

    m_server->onGet("/charts.js",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /charts.js");
                        request.send(HTML_OK, "application/javascript", m_resources->getChartsJs());
                    });

    m_server->onGet("/pico.min.css",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /pico.min.css");
                        request.send(HTML_OK, "text/css", m_resources->getPicoCss());
                    });
}

void WebPageMain::setupActions()
{
    m_server->onPost("/setCredentials",
                     [this](IWebRequest &request, const std::string &body)
                     {
                         logger::logDbg("post /setCredentials");
                         setCredentials(request, body);
                     });

    m_server->onPost("/updateSensorsMapping",
                     [this](IWebRequest &request, const std::string &body)
                     {
                         logger::logDbg("post /updateSensorsMapping");
                         updateSensorsMapping(request, body);
                     });

    m_server->onPost("/setProperties",
                     [this](IWebRequest &request, const std::string &body)
                     {
                         logger::logDbg("post /setProperties");
                         setProperties(request, body);
                     });

    m_server->onPost("/removeSensor",
                     [this](IWebRequest &request, const std::string &body)
                     {
                         logger::logDbg("post /removeSensor");
                         removeSensor(request, body);
                     });

    m_server->onGet("/logout",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /logout");
                        request.send(HTML_UNAUTH, "text/html", m_resources->getAdminHtml());
                    });

    m_server->onGet("/sensorIDsToNames",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /sensorIDsToNames");
                        sensorIDsToNames(request);
                    });

    m_server->onGet("/configuration",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /configuration");
                        configuration(request);
                    });

    m_server->onGet("/sensorData",
                    [this](IWebRequest &request)
                    {
                        logger::logDbg("get /sensorData");
                        sensorData(request);
                    });
}

void WebPageMain::startServer(const GetSensorDataCb &getSensorDataCb)
{
    m_getSensorDataCb = getSensorDataCb;

    setupResources();
    setupActions();

    m_server->setupEventsSource(
        "/events",
        [this](IEventSrcClient &client)
        {
            logger::logDbg("Client connected");
            if (client.lastId() != 0)
            {
                logger::logDbg("Client reconnected, last ID: %u\n", client.lastId());
            }
            client.send("init", nullptr, m_arduinoAdp->millis(), RECONNECT_TIMEOUT);
        });

    m_server->start();
}

void WebPageMain::stopServer()
{
    m_server->stop();
}

bool WebPageMain::auth(IWebRequest &request)
{
    auto credentials = m_confStorage->getAdminCredentials();

    if (!credentials.has_value())
    {
        return false;
    }

    auto [user, passwd] = credentials.value();
    return request.authenticate(user, passwd);
};

void WebPageMain::setCredentials(IWebRequest &request, const std::string &body)
{
    if (!auth(request))
    {
        request.requestAuthentication();
    }
    else
    {
        nlohmann::json credentials{};
        try
        {
            credentials = nlohmann::json::parse(body);
        }
        catch (nlohmann::json::parse_error err)
        {
            logger::logErr("Can't parse json data, %s", err.what());
            request.send(HTML_BAD_REQ);
            return;
        }

        if (credentials["password"] != credentials["rePassword"]
            || credentials["password"].get<std::string>().empty()
            || credentials["username"].get<std::string>().empty())
        {
            logger::logWrn("Can't update credentials");
            request.send(HTML_BAD_REQ);
            return;
        }

        m_confStorage->setAdminCredentials(credentials["username"], credentials["password"]);
        m_confStorage->save();

        request.send(HTML_OK, "text/html", m_resources->getAdminHtml());
    }
}

void WebPageMain::updateSensorsMapping(IWebRequest &request, const std::string &body)
{
    if (!auth(request))
    {
        request.requestAuthentication();
    }
    else
    {
        nlohmann::json sensorsMapping{};
        try
        {
            sensorsMapping = nlohmann::json::parse(body);
        }
        catch (nlohmann::json::parse_error err)
        {
            logger::logErr("Can't parse json data, %s", err.what());
            request.send(HTML_BAD_REQ);
            return;
        }

        for (const auto &sensor : sensorsMapping.items())
        {
            auto name = std::string(sensor.value());
            try
            {
                auto identifier = std::stoull(sensor.key());

                m_confStorage->addSensor(identifier, name);
                m_confStorage->save();
            }
            catch (std::invalid_argument err)
            {
                logger::logErr("can't get sensor identifier, %s", err.what());
            }
        }

        request.send(HTML_OK, "text/html", m_resources->getAdminHtml());
    }
}

void WebPageMain::setProperties(IWebRequest &request, const std::string &body)
{
    if (!auth(request))
    {
        request.send(HTML_UNAUTH);
    }
    else
    {
        try
        {
            auto configuration = nlohmann::json::parse(body);
            std::uint16_t sensorUpdatePeriodMins = configuration["sensorUpdatePeriodMins"];
            std::size_t serverPort = configuration["serverPort"];

            m_confStorage->setSensorUpdatePeriodMins(sensorUpdatePeriodMins);
            m_confStorage->setServerPort(serverPort);
            m_confStorage->save();
        }
        catch (nlohmann::json::parse_error err)
        {
            logger::logErr("Can't set properties");
            request.send(HTML_BAD_REQ);
            return;
        }
        catch (nlohmann::json::type_error err)
        {
            logger::logErr("Can't set properties");
            request.send(HTML_BAD_REQ);
            return;
        }

        request.send(HTML_OK);
    }
}

void WebPageMain::removeSensor(IWebRequest &request, const std::string &body)
{
    if (!auth(request))
    {
        request.send(HTML_UNAUTH);
    }
    else
    {
        try
        {
            auto sensor = nlohmann::json::parse(body);
            IDType sensorId = sensor["identifier"];
            m_confStorage->removeSensor(sensorId);
            m_confStorage->save();
        }
        catch (nlohmann::json::parse_error err)
        {
            logger::logErr("Can't remove sensor, wrong data, %s", err.what());
            request.send(HTML_BAD_REQ);
            return;
        }
        catch (nlohmann::json::type_error err)
        {
            logger::logErr("Can't remove sensor, wrong data, %s", err.what());
            request.send(HTML_BAD_REQ);
            return;
        }
        request.send(HTML_OK);
    }
}

void WebPageMain::sensorIDsToNames(IWebRequest &request)
{
    auto sensorsMappingJsonStr = m_confStorage->getSensorsMapping();
    request.send(HTML_OK, "application/json", sensorsMappingJsonStr.c_str());
}

void WebPageMain::configuration(IWebRequest &request)
{
    if (!auth(request))
    {
        request.send(HTML_UNAUTH);
    }

    auto config = m_confStorage->getConfigWithoutCredentials();
    request.send(HTML_OK, "application/json", config.c_str());
}

void WebPageMain::sensorData(IWebRequest &request)
{
    auto params = request.getParams();
    if (params.find("identifier") != params.end())
    {
        try
        {
            auto identifier = std::stoull(params["identifier"]);
            request.send(HTML_OK, "application/json", m_getSensorDataCb(identifier).c_str());
        }
        catch (std::invalid_argument err)
        {
            logger::logErr("can't get sensor identifier, %s", err.what());
            request.send(HTML_BAD_REQ);
        }
    }
    else
    {
        request.send(HTML_BAD_REQ);
    }
}
