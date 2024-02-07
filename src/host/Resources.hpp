#pragma once

#include "IResources.hpp"


class Resources: public IResources
{
public:
    ~Resources() override = default;

    const char *getIndexHtml() override;
    const char *getAdminHtml() override;
    const char *getMicroChart() override;
    const unsigned char *getFavicon() override;
    unsigned int getFaviconSize() override;
    const char *getWifiSettingsHtml() override;
};

// extern "C"
// {
//     // extern const char gIndexHtmlData[];
//     extern const char gAdminHtmlData[];
//     extern const char gMicroChartData[];
//     extern const unsigned char gFaviconData[];
//     extern const unsigned int gFaviconSize;
//     extern const char gWifiSettingsHtmlData[];
// }
