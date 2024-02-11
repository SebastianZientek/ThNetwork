#pragma once

class IEventSrcClient
{
public:
    IEventSrcClient() = default;
    IEventSrcClient(const IEventSrcClient &) = default;
    IEventSrcClient(IEventSrcClient &&) = default;
    virtual ~IEventSrcClient() = default;

    IEventSrcClient &operator=(const IEventSrcClient &) = default;
    IEventSrcClient &operator=(IEventSrcClient &&) = default;

    virtual uint32_t lastId() = 0;
    virtual void send(const char *message, const char *event, uint32_t identifier, uint32_t reconnect) = 0;
};
