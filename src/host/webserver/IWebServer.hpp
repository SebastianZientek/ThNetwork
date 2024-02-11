#pragma once

#include "IWebRequest.hpp"
#include "IEventSrcClient.hpp"

class IWebServer
{
public:
    using WebRequestClbk = std::function<void(IWebRequest &)>;
    using EventClbk = std::function<void(IEventSrcClient &)>;

    virtual ~IWebServer() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void onGet(std::string url, WebRequestClbk clbk) = 0;
    virtual void onPost(std::string url, WebRequestClbk clbk) = 0;
    virtual void setupEventsSource(const std::string &src, EventClbk onConnectClbk) = 0;
    virtual void sendEvent(const char *message,
                           const char *event,
                           uint32_t identifier,
                           uint32_t reconnect)
        = 0;
};