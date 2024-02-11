#pragma once

class IResources
{
public:
    virtual ~IResources() = default;

    virtual const char *getIndexHtml() = 0;
    virtual const char *getAdminHtml() = 0;
    virtual const char *getMicroChart() = 0;
    virtual const unsigned char *getFavicon() = 0;
    virtual unsigned int getFaviconSize() = 0;
    virtual const char *getWifiSettingsHtml() = 0;
};
