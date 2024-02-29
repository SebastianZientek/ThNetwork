#pragma once

class IResources
{
public:
    IResources() = default;
    IResources(const IResources &) = default;
    IResources(IResources &&) = default;
    virtual ~IResources() = default;

    IResources &operator=(const IResources &) = default;
    IResources &operator=(IResources &&) = default;

    [[nodiscard]] virtual const char *getIndexHtml() const = 0;
    [[nodiscard]] virtual const char *getAdminHtml() const = 0;
    [[nodiscard]] virtual const char *getMicroChart() const = 0;
    [[nodiscard]] virtual const char *getAdminJs() const = 0;
    [[nodiscard]] virtual const char *getChartsJs() const = 0;
    [[nodiscard]] virtual const char *getPicoCss() const = 0;
    [[nodiscard]] virtual const unsigned char *getFavicon() const = 0;
    [[nodiscard]] virtual unsigned int getFaviconSize() const = 0;
    [[nodiscard]] virtual const char *getWifiSettingsHtml() const = 0;
};
