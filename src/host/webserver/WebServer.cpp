#include "WebServer.hpp"

void WebServer::start() 
{
    m_server.begin();
}

void WebServer::stop() 
{
    if (m_events)
    {
        m_events->close();
    }
    m_server.end();
}

void WebServer::onGet(std::string url, WebRequestClbk clbk) 
{
    m_server.on(url.c_str(), HTTP_GET,
                [clbk](AsyncWebServerRequest *request)
                {
                    auto req = WebRequest(request);
                    clbk(req);
                });
}

void WebServer::onPost(std::string url, WebRequestClbk clbk) 
{
    m_server.on(url.c_str(), HTTP_POST,
                [clbk](AsyncWebServerRequest *request)
                {
                    auto req = WebRequest(request);
                    clbk(req);
                });
}

void WebServer::setupEventsSource(const std::string &src, EventClbk onConnectClbk) 
{
    m_events = std::make_unique<AsyncEventSource>(src.c_str());
    m_events->onConnect(
        [onConnectClbk](AsyncEventSourceClient *client)
        {
            auto eventSrcClient = EventSrcClient(client);
            onConnectClbk(eventSrcClient);
        });
    m_server.addHandler(m_events.get());
}

void WebServer::sendEvent(const char *message,
                          const char *event,
                          uint32_t identifier,
                          uint32_t reconnect) 

{
    m_events->send(message, event, identifier, reconnect);
}
