#pragma once

class IWebRequest
{
public:
    virtual ~IWebRequest() = default;
    virtual void send(int code, std::string contentType, const uint8_t *content, size_t len) = 0;
    virtual void send(int code, std::string contentType, const char *content) = 0;
    virtual void send(int code) = 0;
    virtual bool authenticate(const std::string &user, const std::string &passwd) = 0;
    virtual void requestAuthentication() = 0;
    virtual std::map<std::string, std::string> getParams() = 0;
};