#pragma once

#include "interfaces/IResources.hpp"

class Resources : public IResources
{
public:
    [[nodiscard]] const char *getIndexHtml() const override;
    [[nodiscard]] const char *getAdminHtml() const override;
    [[nodiscard]] const char *getMicroChart() const override;
    [[nodiscard]] const char *getPicoCss() const override;
    [[nodiscard]] const char *getAdminJs() const override;
    [[nodiscard]] const char *getChartsJs() const override;
    [[nodiscard]] const unsigned char *getFavicon() const override;
    [[nodiscard]] unsigned int getFaviconSize() const override;
    [[nodiscard]] const char *getWifiSettingsHtml() const override;
};
