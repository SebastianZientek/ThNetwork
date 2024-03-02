#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <functional>
#include <memory>
#include <nlohmann/json.hpp>

#include "WifiConfiguratorWebServer.hpp"
#include "mocks/Arduino32AdpMock.hpp"
#include "mocks/ResourcesMock.hpp"
#include "mocks/WebRequestMock.hpp"
#include "mocks/WebServerMock.hpp"
#include "mocks/Wifi32AdpMock.hpp"

TEST_GROUP(WifiConfiguratorWebServerTest)  // NOLINT
{
    void setup() override
    {
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }

    void mockOnGetAndOnPostCalls()
    {
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/pico.min.css");
        mock("WebServerMock").expectOneCall("onPost").withParameter("url", "/setWifi");
    }

    std::shared_ptr<Arduino32AdpMock> arduinoAdpMock{std::make_shared<Arduino32AdpMock>()};
    std::shared_ptr<Wifi32AdpMock> wifiAdpMock{std::make_shared<Wifi32AdpMock>()};
    std::shared_ptr<WebServerMock> webServerMock{std::make_shared<WebServerMock>()};
    WifiConfiguratorWebServer::OnConfiguredClbk onConfiguredClbk
        = [](const std::string &ssid, const std::string &pass)
    {
        mock("Lambda")
            .actualCall("onConfiguredClbk")
            .withStringParameter("ssid", ssid.c_str())
            .withStringParameter("pass", pass.c_str());
    };

    WifiConfiguratorWebServer wifiConfiguratorWebServer{
        wifiAdpMock, webServerMock, std::make_unique<ResourcesMock>(), arduinoAdpMock};
};

constexpr static auto HTML_OK = 200;
constexpr static auto HTML_BAD_REQ = 400;
constexpr static auto HTML_UNAUTH = 401;
constexpr static auto HTML_NOT_FOUND = 404;
constexpr static auto HTML_INTERNAL_ERR = 500;

TEST(WifiConfiguratorWebServerTest, OnIndexLoad)  // NOLINT
{
    mock("Wifi32AdpMock").expectOneCall("softAp").ignoreOtherParameters();
    mock("Wifi32AdpMock").expectOneCall("getSoftApIp").ignoreOtherParameters();
    mock("Arduino32Adp").expectOneCall("delay").ignoreOtherParameters();

    mockOnGetAndOnPostCalls();

    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/html")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getWifiSettingsHtml");

    mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();
    wifiConfiguratorWebServer.startServer(
        [](const std::string &ssid, const std::string &pass)
        {
        });

    WebRequestMock webRequestMock;
    webServerMock->callGet("/", webRequestMock);
}

TEST(WifiConfiguratorWebServerTest, OnPicoCssLoad)  // NOLINT
{
    mock("Wifi32AdpMock").expectOneCall("softAp").ignoreOtherParameters();
    mock("Wifi32AdpMock").expectOneCall("getSoftApIp").ignoreOtherParameters();
    mock("Arduino32Adp").expectOneCall("delay").ignoreOtherParameters();

    mockOnGetAndOnPostCalls();

    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/css")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getPicoCss");

    mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();
    wifiConfiguratorWebServer.startServer(
        [](const std::string &ssid, const std::string &pass)
        {
        });

    WebRequestMock webRequestMock;
    webServerMock->callGet("/pico.min.css", webRequestMock);
}

TEST(WifiConfiguratorWebServerTest, OnSetWifiWhenCorrectParametersAreSend)  // NOLINT
{
    mock("Wifi32AdpMock").expectOneCall("softAp").ignoreOtherParameters();
    mock("Wifi32AdpMock").expectOneCall("getSoftApIp").ignoreOtherParameters();
    mock("Arduino32Adp").expectOneCall("delay").ignoreOtherParameters();

    mockOnGetAndOnPostCalls();

    std::map<std::string, std::string> postParams
        = {{"ssid", "some ssid"}, {"password", "some pass"}};

    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .ignoreOtherParameters();
    mock("WebRequestMock").expectOneCall("getParams").andReturnValue(&postParams);
    mock("Lambda")
        .expectOneCall("onConfiguredClbk")
        .withStringParameter("ssid", "some ssid")
        .withStringParameter("pass", "some pass");

    mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();
    wifiConfiguratorWebServer.startServer(onConfiguredClbk);

    WebRequestMock webRequestMock;
    webServerMock->callPost("/setWifi", webRequestMock);
}

TEST(WifiConfiguratorWebServerTest, OnSetWifiWhenParametersAreNotSend)  // NOLINT
{
    mock("Wifi32AdpMock").expectOneCall("softAp").ignoreOtherParameters();
    mock("Wifi32AdpMock").expectOneCall("getSoftApIp").ignoreOtherParameters();
    mock("Arduino32Adp").expectOneCall("delay").ignoreOtherParameters();

    mockOnGetAndOnPostCalls();

    std::map<std::string, std::string> postParams = {};

    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_BAD_REQ)
        .ignoreOtherParameters();
    mock("WebRequestMock").expectOneCall("getParams").andReturnValue(&postParams);
    mock("Lambda").expectNoCall("onConfiguredClbk");

    mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();
    wifiConfiguratorWebServer.startServer(onConfiguredClbk);

    WebRequestMock webRequestMock;
    webServerMock->callPost("/setWifi", webRequestMock);
}
