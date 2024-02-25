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

    virtual const char *getIndexHtml() = 0;
    virtual const char *getAdminHtml() = 0;
    virtual const char *getMicroChart() = 0;
    virtual const char *getPicoCss() = 0;
    virtual const unsigned char *getFavicon() = 0;
    virtual unsigned int getFaviconSize() = 0;
    virtual const char *getWifiSettingsHtml() = 0;
};
