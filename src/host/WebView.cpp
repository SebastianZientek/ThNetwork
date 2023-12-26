#include "WebView.hpp"

#include <incbin.h>

#include "common/logger.hpp"

INCTXT(IndexHtml, "src/host/html/index.html");
INCTXT(MicroChart, "src/host/html/microChart.js");

WebView::WebView(int port)
    : m_htmlData{}
    , m_server(port)
    , m_events("/events")
    , m_newClientCb()
{
}

void WebView::sendEvent(const char *message,
                        const char *event,
                        uint32_t identifier,
                        uint32_t reconnect)
{
    logger::logInf("Send event %s, %lu", event, identifier);
    m_events.send(message, event, identifier, reconnect);
}

void WebView::startServer(const NewClientCb &newClientCb)
{
    m_newClientCb = newClientCb;

    m_server.on("/", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                { request->send_P(200, "text/html", gIndexHtmlData); });

    m_server.on("/microChart.js", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                { request->send_P(200, "application/javascript", gMicroChartData); });

    m_server
        .on("/sensorNames", HTTP_GET,
            [this](AsyncWebServerRequest *request)
            {
                int params = request->params();
                for (int i = 0; i < params; i++)
                {
                    AsyncWebParameter *p = request->getParam(i);
                    logger::logInf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                }
                request->send_P(200, "application/json", "{}");
            });

            m_events.onConnect(
                [this](AsyncEventSourceClient *client)
                {
                    logger::logInf("Client connected");
                    if (client->lastId())
                    {
                        logger::logInf("Client reconnected, last ID: %u\n", client->lastId());
                    }
                    client->send("init", NULL, millis(), 10000);
                    m_newClientCb();
                });
    m_server.addHandler(&m_events);
    m_server.begin();
}
