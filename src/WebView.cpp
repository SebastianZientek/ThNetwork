#include "WebView.hpp"

#include "logger.hpp"

constexpr auto htmlData =
#include "index.html"

WebView::WebView(int port)
    : m_server(port)
    , m_events("/events")
    , m_newClientCb()
{
}

void WebView::sendEvent(const char *message,
                        const char *event,
                        uint32_t identifier,
                        uint32_t reconnect)
{
    m_events.send(message, event, identifier, reconnect);
}

void WebView::startServer(const NewClientCb &newClientCb)
{
    m_newClientCb = newClientCb;

    m_server.on("/", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                { request->send_P(200, "text/html", htmlData); });

    m_events.onConnect(
        [this](AsyncEventSourceClient *client)
        {
            logger::logInf("Client connected");
            if (client->lastId())
            {
                logger::logInfF("Client reconnected, last ID: %u\n", client->lastId());
            }
            client->send("init", NULL, millis(), 10000);
            m_newClientCb();
        });
    m_server.addHandler(&m_events);
    m_server.begin();
}
