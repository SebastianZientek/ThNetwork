#pragma once

#include <CppUTestExt/MockSupport.h>

#include "interfaces/IResources.hpp"

class ResourcesMock : public IResources
{
public:
    const char *getIndexHtml() override
    {
        return mock("ResourcesMock").actualCall("getIndexHtml").returnStringValueOrDefault("");
    }

    const char *getAdminHtml() override
    {
        return mock("ResourcesMock").actualCall("getAdminHtml").returnStringValueOrDefault("");
    }

    const char *getMicroChart() override
    {
        return mock("ResourcesMock").actualCall("getMicroChart").returnStringValueOrDefault("");
    }

    const char *getAdminJs() override
    {
        return mock("ResourcesMock").actualCall("getAdminJs").returnStringValueOrDefault("");
    }

    const char *getChartsJs() override
    {
        return mock("ResourcesMock").actualCall("getChartsJs").returnStringValueOrDefault("");
    }

    const char *getPicoCss() override
    {
        return mock("ResourcesMock").actualCall("getPicoCss").returnStringValueOrDefault("");
    }

    const unsigned char *getFavicon() override
    {
        auto *value
            = mock("ResourcesMock").actualCall("getFavicon").returnPointerValueOrDefault(nullptr);

        return static_cast<unsigned char *>(value);
    }

    unsigned int getFaviconSize() override
    {
        return mock("ResourcesMock").actualCall("getFaviconSize").returnUnsignedIntValueOrDefault(0);
    }

    const char *getWifiSettingsHtml() override
    {
        return mock("ResourcesMock").actualCall("getWifiSettingsHtml").returnStringValueOrDefault("");
    }
};
