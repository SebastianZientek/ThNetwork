#pragma once

#include <functional>
#include <string>

#include "IEventSrcClient.hpp"
#include "IWebRequest.hpp"

class IWebServer
{
public:
    using WebRequestClbk = std::function<void(IWebRequest &)>;
    using WebRequestWithBodyClbk = std::function<void(IWebRequest &, const std::string &body)>;
    using EventClbk = std::function<void(IEventSrcClient &)>;

    IWebServer() = default;
    IWebServer(const IWebServer &) = default;
    IWebServer(IWebServer &&) = default;
    virtual ~IWebServer() = default;

    IWebServer &operator=(const IWebServer &) = default;
    IWebServer &operator=(IWebServer &&) = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void onGet(const std::string &url, WebRequestClbk clbk) = 0;
    virtual void onPost(const std::string &url, WebRequestClbk clbk) = 0;
    virtual void onPost(const std::string &url, WebRequestWithBodyClbk clbk) = 0;
    virtual void setupEventsSource(const std::string &src, EventClbk onConnectClbk) = 0;
    virtual void sendEvent(const char *message,
                           const char *event,
                           uint32_t identifier,
                           uint32_t reconnect)
        = 0;
};
