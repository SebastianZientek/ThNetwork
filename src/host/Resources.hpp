#pragma once

#include "interfaces/IResources.hpp"

class Resources : public IResources
{
public:
    ~Resources() override = default;

    const char *getIndexHtml() override;
    const char *getAdminHtml() override;
    const char *getMicroChart() override;
    const char *getPicoCss() override;
    const char *getAdminJs() override;
    const char *getChartsJs() override;
    const unsigned char *getFavicon() override;
    unsigned int getFaviconSize() override;
    const char *getWifiSettingsHtml() override;
};
