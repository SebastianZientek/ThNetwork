#include "WebView.hpp"

#include "logger.hpp"

WebView::WebView(int port)
    : m_server(port)
    , m_events("/events")
{
}

void WebView::load(const String &path)
{
    RaiiFile webpage(path);
    m_pageData = webpage.asString();
}

void WebView::sendEvent(const char *message,
                        const char *event,
                        uint32_t identifier,
                        uint32_t reconnect)
{
    m_events.send(message, event, identifier, reconnect);
}

void WebView::startServer(NewClientCb newClientCb)
{
    m_server.on("/", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                { request->send_P(200, "text/html", m_pageData.c_str()); });
    m_events.onConnect(
        [newClientCb](AsyncEventSourceClient *client)
        {
            logger::logInf("Client connected");
            if (client->lastId())
            {
                logger::logInfF("Client reconnected, last ID: %u\n", client->lastId());
            }
            client->send("init", NULL, millis(), 10000);
            newClientCb();
        });
    m_server.addHandler(&m_events);
    m_server.begin();
}
