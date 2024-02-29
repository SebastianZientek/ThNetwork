#pragma once

#include <CppUTestExt/MockSupport.h>

#include <cinttypes>
#include <map>
#include <string>

#include "webserver/IWebRequest.hpp"
#include "webserver/IWebServer.hpp"

class WebRequestMock : public IWebRequest
{
public:
    void send(int code, std::string contentType, const uint8_t *content, size_t len) override
    {
        mock("WebRequestMock")
            .actualCall("send")
            .withParameter("code", code)
            .withParameter("contentType", contentType.c_str())
            .withParameter("content", content)
            .withParameter("len", len);
    }

    void send(int code, std::string contentType, const char *content) override
    {
        mock("WebRequestMock")
            .actualCall("send")
            .withParameter("code", code)
            .withParameter("contentType", contentType.c_str())
            .withParameter("content", content);
    }

    void send(int code) override
    {
        mock("WebRequestMock").actualCall("send").withParameter("code", code);
    }

    void redirect(std::string url) override
    {
        mock("WebRequestMock").actualCall("redirect").withParameter("url", url.c_str());
    }

    bool authenticate(const std::string &user, const std::string &passwd) override
    {
        return mock("WebRequestMock")
            .actualCall("authenticate")
            .withParameter("user", user.c_str())
            .withParameter("passwd", passwd.c_str())
            .returnBoolValueOrDefault(false);
    }

    void requestAuthentication() override
    {
        mock("WebRequestMock").actualCall("requestAuthentication");
    }

    std::map<std::string, std::string> getParams() override
    {
        auto *value
            = mock("WebRequestMock").actualCall("getParams").returnPointerValueOrDefault({});
        if (value != nullptr)
        {
            return *static_cast<std::map<std::string, std::string> *>(value);
        }
        else
        {
            return {};
        }
    }
};
