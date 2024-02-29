#include <CppUTest/TestHarness.h>

#include <nlohmann/json.hpp>
#include <vector>

#include "WebPageMain.hpp"
#include "mocks/Arduino32AdpMock.hpp"
#include "mocks/ConfStorageMock.hpp"
#include "mocks/ResourcesMock.hpp"
#include "mocks/WebRequestMock.hpp"
#include "mocks/WebServerMock.hpp"
#include "webserver/IWebServer.hpp"

TEST_GROUP(WebPageMainTest)  // NOLINT
{
    void setup() override
    {
        webServerMock = std::make_shared<WebServerMock>();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }

    void mockOnGetCalls()
    {
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/admin");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/favicon.ico");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/microChart.js");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/admin.js");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/charts.js");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/pico.min.css");

        mock("WebServerMock").expectOneCall("onPost").withParameter("url", "/setCredentials");
        mock("WebServerMock").expectOneCall("onPost").withParameter("url", "/updateSensorsMapping");
        mock("WebServerMock").expectOneCall("onPost").withParameter("url", "/setProperties");
        mock("WebServerMock").expectOneCall("onPost").withParameter("url", "/removeSensor");

        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/logout");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/sensorIDsToNames");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/configuration");
        mock("WebServerMock").expectOneCall("onGet").withParameter("url", "/sensorData");
    }

    void mockAuthentication(bool authenticate)
    {
        static std::optional<std::pair<std::string, std::string>> credentials
            = std::make_pair("admin", "password");
        mock("ConfStorageMock").expectOneCall("getAdminCredentials").andReturnValue(&credentials);
        mock("WebRequestMock")
            .expectOneCall("authenticate")
            .ignoreOtherParameters()
            .andReturnValue(authenticate);
    }

    void startServerMock(WebPageMain & webPageMain)
    {
        mock("WebServerMock").expectOneCall("setupEventsSource").ignoreOtherParameters();
        mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();

        webPageMain.startServer(
            []([[maybe_unused]] const std::size_t &identifier)
            {
                return R"({"some": "data"})";
            });
    }

    std::shared_ptr<ConfStorageMock> confStorageMock{std::make_shared<ConfStorageMock>()};
    std::shared_ptr<Arduino32AdpMock> arduino32AdpMock{std::make_shared<Arduino32AdpMock>()};
    std::shared_ptr<WebServerMock> webServerMock;
};

constexpr static auto HTML_OK = 200;
constexpr static auto HTML_BAD_REQ = 400;
constexpr static auto HTML_UNAUTH = 401;
constexpr static auto HTML_NOT_FOUND = 404;
constexpr static auto HTML_INTERNAL_ERR = 500;

TEST(WebPageMainTest, ProvideIndexHtml)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/html")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getIndexHtml");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/", webRequestMock);
}

TEST(WebPageMainTest, ProvideAdminPageWhenUserIsAuthenticated)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mockAuthentication(true);
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/html")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getAdminHtml");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/admin", webRequestMock);
}

TEST(WebPageMainTest, RequestAuthInsteadOfProvidingAdminPageWhenUnauthorized)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mockAuthentication(false);
    mock("WebRequestMock").expectOneCall("requestAuthentication");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/admin", webRequestMock);
}

TEST(WebPageMainTest, ProvideFavIcon)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "image/png")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getFavicon");
    mock("ResourcesMock").expectOneCall("getFaviconSize");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/favicon.ico", webRequestMock);
}

TEST(WebPageMainTest, ProvideMicroChartJs)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "application/javascript")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getMicroChart");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/microChart.js", webRequestMock);
}

TEST(WebPageMainTest, ProvideAdminJs)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "application/javascript")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getAdminJs");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/admin.js", webRequestMock);
}

TEST(WebPageMainTest, ProvideChartJs)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "application/javascript")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getChartsJs");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/charts.js", webRequestMock);
}

TEST(WebPageMainTest, ProvidePicoCss)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/css")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getPicoCss");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/pico.min.css", webRequestMock);
}

TEST(WebPageMainTest, ActionSetCredentialsWhenAuthenticated)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingCredentials = nlohmann::json({
        {"password", "pass"},
        {"rePassword", "pass"},
        {"username", "user"},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("ConfStorageMock")
        .expectOneCall("setAdminCredentials")
        .withParameter("pass", "pass")
        .withParameter("user", "user");

    mock("ConfStorageMock").expectOneCall("save");
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/html")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getAdminHtml");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setCredentials", webRequestMock, incomingCredentials.dump());
}

TEST(WebPageMainTest, ActionSetCredentialsNotAllwWhenNotAuthenticated)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingCredentials = nlohmann::json({
        {"password", "pass"},
        {"rePassword", "pass"},
        {"username", "user"},
    });

    mockOnGetCalls();
    mockAuthentication(false);

    mock("WebRequestMock").expectOneCall("requestAuthentication");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setCredentials", webRequestMock, incomingCredentials.dump());
}

TEST(WebPageMainTest, ActionSetCredentialsWhenRePasswordNotMatch)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingCredentials = nlohmann::json({
        {"password", "pass"},
        {"rePassword", "wrong pass"},
        {"username", "user"},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setCredentials", webRequestMock, incomingCredentials.dump());
}

TEST(WebPageMainTest, ActionSetCredentialsNotAllowWhenEmptyPassword)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingCredentials = nlohmann::json({
        {"password", ""},
        {"rePassword", ""},
        {"username", "user"},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setCredentials", webRequestMock, incomingCredentials.dump());
}

TEST(WebPageMainTest, ActionSetCredentialsNotAllowWhenEmptyUsername)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingCredentials = nlohmann::json({
        {"password", "pass"},
        {"rePassword", "pass"},
        {"username", ""},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setCredentials", webRequestMock, incomingCredentials.dump());
}

TEST(WebPageMainTest, ActionSetCredentialsNotAllowWhenCorruptedData)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    const auto *corruptedIncomingJsonData = "skjhksdfihs";

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setCredentials", webRequestMock, corruptedIncomingJsonData);
}

TEST(WebPageMainTest, ActionSetCredentialsWhenWrongData)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingCredentials = nlohmann::json({
        {"password", "pass"},
        {"ThisIsWrongField", "pass"},
        {"username", ""},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setCredentials", webRequestMock, incomingCredentials.dump());
}

TEST(WebPageMainTest, UpdateSensorsMappingWhenAuthorized)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingMapping = nlohmann::json({
        {"123456", "sensor 1"},
        {"5678", "some office"},
        {"1354678", "what is this"},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("ConfStorageMock").expectNCalls(3, "addSensor").ignoreOtherParameters();
    mock("ConfStorageMock").expectNCalls(3, "save");

    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/html")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getAdminHtml");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/updateSensorsMapping", webRequestMock,
                                    incomingMapping.dump());
}

TEST(WebPageMainTest, NotUpdateSensorsMappingWhenUnAuthorized)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingMapping = nlohmann::json({
        {"123456", "sensor 1"},
        {"5678", "some office"},
        {"1354678", "what is this"},
    });

    mockOnGetCalls();
    mockAuthentication(false);

    mock("WebRequestMock").expectOneCall("requestAuthentication");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/updateSensorsMapping", webRequestMock,
                                    incomingMapping.dump());
}

TEST(WebPageMainTest, NotUpdateSensorsMappingWhenIncomingDataCorrupted)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    const auto *corruptedIncomingJsonData = "skjhksdfihs";

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/updateSensorsMapping", webRequestMock,
                                    corruptedIncomingJsonData);
}

TEST(WebPageMainTest, EmptyData)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    const auto *emptyData = "";

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/updateSensorsMapping", webRequestMock, emptyData);
}

TEST(WebPageMainTest, UpdateSensorsMappingOnlyForCorrectOne)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingMapping = nlohmann::json({
        {"123456", "correct sensor"},
        {"Wrong sensor id, should be number", "some office"},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("ConfStorageMock")
        .expectNCalls(1, "addSensor")
        .withParameter("identifier", 123456)
        .withParameter("name", "correct sensor");
    mock("ConfStorageMock").expectNCalls(1, "save");
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "text/html")
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getAdminHtml");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/updateSensorsMapping", webRequestMock,
                                    incomingMapping.dump());
}

TEST(WebPageMainTest, SetPropertiesWhenAuthorizedAndCorrectDataIncoming)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingProperties = nlohmann::json({
        {"sensorUpdatePeriodMins", 11},
        {"serverPort", 22},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("ConfStorageMock").expectOneCall("setSensorUpdatePeriodMins").withParameter("minutes", 11);
    mock("ConfStorageMock").expectOneCall("setServerPort").withParameter("port", 22);
    mock("ConfStorageMock").expectOneCall("save");
    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_OK);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setProperties", webRequestMock, incomingProperties.dump());
}

TEST(WebPageMainTest, NotSetPropertiesWhenWrongTypeOfData)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingProperties = nlohmann::json({
        {"sensorUpdatePeriodMins", "stringData"},
        {"serverPort", "123"},
    });

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setProperties", webRequestMock, incomingProperties.dump());
}

TEST(WebPageMainTest, NotSetPropertiesWhenCorruptedData)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    const auto *corruptedData = "asdjhaskjd";

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setProperties", webRequestMock, corruptedData);
}

TEST(WebPageMainTest, NotSetPropertiesWhenUnAuthorized)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingProperties = nlohmann::json({
        {"sensorUpdatePeriodMins", 11},
        {"serverPort", 22},
    });

    mockOnGetCalls();
    mockAuthentication(false);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_UNAUTH);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/setProperties", webRequestMock, incomingProperties.dump());
}

TEST(WebPageMainTest, RemoveSensorWhenAuthorizedAndCorrectDataIncoming)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingProperties = nlohmann::json({{"identifier", 1}});

    mockOnGetCalls();
    mockAuthentication(true);

    mock("ConfStorageMock").expectOneCall("removeSensor").withParameter("identifier", 1);
    mock("ConfStorageMock").expectOneCall("save");
    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_OK);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/removeSensor", webRequestMock, incomingProperties.dump());
}

TEST(WebPageMainTest, DontRemoveSensorWhenUnauthorized)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingProperties = nlohmann::json({{"identifier", 1}});

    mockOnGetCalls();
    mockAuthentication(false);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_UNAUTH);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/removeSensor", webRequestMock, incomingProperties.dump());
}

TEST(WebPageMainTest, DontRemoveSensorWhenDataCorrupted)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    const char *corruptedData = "skwdjhfjksd";

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/removeSensor", webRequestMock, corruptedData);
}

TEST(WebPageMainTest, DontRemoveSensorWhenWrongDataType)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingProperties = nlohmann::json({{"identifier", "string"}});

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/removeSensor", webRequestMock, incomingProperties.dump());
}

TEST(WebPageMainTest, DontRemoveSensorWhenMissingIdentifierInJson)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    auto incomingProperties = nlohmann::json({{"thisIsNotCorrectField", 1}});

    mockOnGetCalls();
    mockAuthentication(true);

    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callPostWithBody("/removeSensor", webRequestMock, incomingProperties.dump());
}

TEST(WebPageMainTest, Logout)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();

    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_UNAUTH)
        .ignoreOtherParameters();
    mock("ResourcesMock").expectOneCall("getAdminHtml");

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/logout", webRequestMock);
}

TEST(WebPageMainTest, sensorIDsToNames)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();

    auto sensorsMapping = nlohmann::json({
        {"123", 321},
        {"456", 654},
    });

    auto sensorsMappingStr = sensorsMapping.dump();

    mock("ConfStorageMock")
        .expectOneCall("getSensorsMapping")
        .andReturnValue(sensorsMappingStr.c_str());
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("content", sensorsMappingStr.c_str())
        .ignoreOtherParameters();

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/sensorIDsToNames", webRequestMock);
}

TEST(WebPageMainTest, getConfigurationWhenAuthorized)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();
    mockAuthentication(true);

    auto someConfiguration = nlohmann::json({
        {"aaa", 1},
        {"bbb", 0},
    });

    auto someConfigurationStr = someConfiguration.dump();

    mock("ConfStorageMock")
        .expectOneCall("getConfigWithoutCredentials")
        .andReturnValue(someConfigurationStr.c_str());
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("content", someConfigurationStr.c_str())
        .ignoreOtherParameters();

    startServerMock(sut);

    WebRequestMock webRequestMock;
    webServerMock->callGet("/configuration", webRequestMock);
}

TEST(WebPageMainTest, getSensorDataForSensor)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();

    std::map<std::string, std::string> htmlGetRequestParams = {{"identifier", "123"}};

    mock("WebServerMock").expectOneCall("setupEventsSource").ignoreOtherParameters();
    mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();
    mock("WebRequestMock").expectOneCall("getParams").andReturnValue(&htmlGetRequestParams);
    mock("WebRequestMock")
        .expectOneCall("send")
        .withParameter("code", HTML_OK)
        .withParameter("contentType", "application/json")
        .withParameter("content", R"({"some": "data"})");

    sut.startServer(
        []([[maybe_unused]] const std::size_t &identifier)
        {
            return R"({"some": "data"})";
        });

    WebRequestMock webRequestMock;
    webServerMock->callGet("/sensorData", webRequestMock);
}

TEST(WebPageMainTest, NotGetSensorDataWhenNoIdentifierParameter)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();

    std::map<std::string, std::string> htmlGetRequestParams = {{"wrongPatameter", "123"}};

    mock("WebServerMock").expectOneCall("setupEventsSource").ignoreOtherParameters();
    mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();
    mock("WebRequestMock").expectOneCall("getParams").andReturnValue(&htmlGetRequestParams);
    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    sut.startServer(
        []([[maybe_unused]] const std::size_t &identifier)
        {
            return R"({"some": "data"})";
        });

    WebRequestMock webRequestMock;
    webServerMock->callGet("/sensorData", webRequestMock);
}

TEST(WebPageMainTest, NotGetSensorDataWhenParameterIsWrong)  // NOLINT
{
    WebPageMain sut(arduino32AdpMock, webServerMock, std::make_unique<ResourcesMock>(),
                    confStorageMock);

    mockOnGetCalls();

    std::map<std::string, std::string> htmlGetRequestParams
        = {{"identifier", "should be number but is string"}};

    mock("WebServerMock").expectOneCall("setupEventsSource").ignoreOtherParameters();
    mock("WebServerMock").expectOneCall("start").ignoreOtherParameters();
    mock("WebRequestMock").expectOneCall("getParams").andReturnValue(&htmlGetRequestParams);
    mock("WebRequestMock").expectOneCall("send").withParameter("code", HTML_BAD_REQ);

    sut.startServer(
        []([[maybe_unused]] const std::size_t &identifier)
        {
            return R"({"some": "data"})";
        });

    WebRequestMock webRequestMock;
    webServerMock->callGet("/sensorData", webRequestMock);
}
