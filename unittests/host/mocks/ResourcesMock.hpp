#pragma once

#include <CppUTestExt/MockSupport.h>

#include "interfaces/IResources.hpp"

class ResourcesMock : public IResources
{
public:
    [[nodiscard]] const char *getIndexHtml() const override
    {
        return mock("ResourcesMock").actualCall("getIndexHtml").returnStringValueOrDefault("");
    }

    [[nodiscard]] const char *getAdminHtml() const override
    {
        return mock("ResourcesMock").actualCall("getAdminHtml").returnStringValueOrDefault("");
    }

    [[nodiscard]] const char *getMicroChart() const override
    {
        return mock("ResourcesMock").actualCall("getMicroChart").returnStringValueOrDefault("");
    }

    [[nodiscard]] const char *getAdminJs() const override
    {
        return mock("ResourcesMock").actualCall("getAdminJs").returnStringValueOrDefault("");
    }

    [[nodiscard]] const char *getChartsJs() const override
    {
        return mock("ResourcesMock").actualCall("getChartsJs").returnStringValueOrDefault("");
    }

    [[nodiscard]] const char *getPicoCss() const override
    {
        return mock("ResourcesMock").actualCall("getPicoCss").returnStringValueOrDefault("");
    }

    [[nodiscard]] const unsigned char *getFavicon() const override
    {
        auto *value
            = mock("ResourcesMock").actualCall("getFavicon").returnPointerValueOrDefault(nullptr);

        return static_cast<unsigned char *>(value);
    }

    [[nodiscard]] unsigned int getFaviconSize() const override
    {
        return mock("ResourcesMock")
            .actualCall("getFaviconSize")
            .returnUnsignedIntValueOrDefault(0);
    }

    [[nodiscard]] const char *getWifiSettingsHtml() const override
    {
        return mock("ResourcesMock")
            .actualCall("getWifiSettingsHtml")
            .returnStringValueOrDefault("");
    }
};
