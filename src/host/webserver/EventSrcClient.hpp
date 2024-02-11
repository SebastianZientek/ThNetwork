#pragma once

#include <ESPAsyncWebServer.h>

#include "IEventSrcClient.hpp"

class EventSrcClient : public IEventSrcClient
{
public:
    explicit EventSrcClient(AsyncEventSourceClient *client);
    uint32_t lastId() override;
    void send(const char *message,
              const char *event = nullptr,
              uint32_t id = 0,
              uint32_t reconnect = 0) override;

private:
    AsyncEventSourceClient *m_client;
};
