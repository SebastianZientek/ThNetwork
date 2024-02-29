#pragma once

#include <CppUTestExt/MockSupport.h>

#include <map>
#include <string>

#include "webserver/IWebServer.hpp"

class WebServerMock : public IWebServer
{
public:
    void start() override
    {
        mock("WebServerMock").actualCall("start");
    }

    void stop() override
    {
        mock("WebServerMock").actualCall("stop");
    }

    void onGet(std::string url, WebRequestClbk clbk) override
    {
        mock("WebServerMock").actualCall("onGet").withParameter("url", url.c_str());
        m_onGetCallbacks[url] = clbk;
    }

    void onPost(std::string url, WebRequestClbk clbk) override
    {
        mock("WebServerMock").actualCall("onPost").withParameter("url", url.c_str());
        m_onPostCallbacks[url] = clbk;
    }

    void onPost(std::string url, WebRequestWithBodyClbk clbk) override
    {
        mock("WebServerMock").actualCall("onPost").withParameter("url", url.c_str());
        m_onPostWithBodyCallbacks[url] = clbk;
    }

    void setupEventsSource(const std::string &src, EventClbk onConnectClbk) override
    {
        mock("WebServerMock").actualCall("setupEventsSource").withParameter("src", src.c_str());
    }

    void sendEvent(const char *message,
                   const char *event,
                   uint32_t identifier,
                   uint32_t reconnect) override
    {
        mock("WebServerMock")
            .actualCall("sendEvent")
            .withParameter("message", message)
            .withParameter("event", event)
            .withParameter("identifier", identifier)
            .withParameter("reconnect", reconnect);
    }

    // Testability functions

    void callGet(const std::string &url, IWebRequest &req)
    {
        m_onGetCallbacks[url](req);
    }

    void callPost(const std::string &url, IWebRequest &req)
    {
        m_onPostCallbacks[url](req);
    }

    void callPostWithBody(const std::string &url, IWebRequest &req, const std::string &body)
    {
        m_onPostWithBodyCallbacks[url](req, body);
    }

private:
    std::map<std::string, IWebServer::WebRequestClbk> m_onGetCallbacks;
    std::map<std::string, IWebServer::WebRequestClbk> m_onPostCallbacks;
    std::map<std::string, IWebServer::WebRequestWithBodyClbk> m_onPostWithBodyCallbacks;
};
