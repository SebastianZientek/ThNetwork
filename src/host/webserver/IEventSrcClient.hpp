#pragma once

class IEventSrcClient
{
public:
    virtual ~IEventSrcClient() = default;
    virtual uint32_t lastId() = 0;
    virtual void send(const char *message, const char *event, uint32_t id, uint32_t reconnect) = 0;
};