#include "WifiConfiguratorWebServer.hpp"

#include "adapters/IWifi32Adp.hpp"
#include "common/logger.hpp"

WifiConfiguratorWebServer::WifiConfiguratorWebServer(
    const std::shared_ptr<IWifi32Adp> &wifiAdp,
    std::shared_ptr<IWebServer> server,
    std::unique_ptr<IResources> resources,
    const std::shared_ptr<IArduino32Adp> &arduinoAdp)
    : m_wifiAdp(wifiAdp)
    , m_server(std::move(server))
    , m_resources(std::move(resources))
    , m_arduinoAdp(arduinoAdp)
{
}

void WifiConfiguratorWebServer::startServer(const OnConfiguredClbk &onConfiguredClbk)
{
    constexpr auto HTML_OK = 200;
    constexpr auto HTML_BAD_REQ = 400;

    m_wifiAdp->softAp("TH-NETWORK");

    m_arduinoAdp->delay(m_initializationTimeMs);
    logger::logInf("IP addr: %s", m_wifiAdp->getSoftApIp());

    m_server->onGet("/",
                    [this](IWebRequest &request)
                    {
                        request.send(HTML_OK, "text/html", m_resources->getWifiSettingsHtml());
                    });

    m_server->onGet("/pico.min.css",
                    [this](IWebRequest &request)
                    {
                        request.send(HTML_OK, "text/css", m_resources->getPicoCss());
                    });

    m_server->onPost("/setWifi",
                     [this, onConfiguredClbk](IWebRequest &request)
                     {
                         logger::logDbg("Request to set wifi config");
                         auto params = request.getParams();

                         std::string ssid = params["ssid"];
                         std::string pass = params["password"];

                         logger::logDbg("Ssid: %s", ssid);

                         if (!ssid.empty() && !pass.empty())
                         {
                             onConfiguredClbk(ssid, pass);
                             request.send(HTML_OK);
                         }
                         else
                         {
                             request.send(HTML_BAD_REQ);
                         }
                     });

    m_server->start();
}
