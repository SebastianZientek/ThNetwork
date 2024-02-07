#include "Resources.hpp"

#include <incbin.h>

INCTXT(IndexHtml, "src/host/html/index.html");
INCTXT(AdminHtml, "src/host/html/admin.html");
INCTXT(MicroChart, "src/host/html/microChart.js");
INCBIN(Favicon, "src/host/html/fav.png");

INCTXT(WifiSettingsHtml, "src/host/html/wifiSettings.html");

const char *Resources::getIndexHtml()
{
    return gIndexHtmlData;
}

const char *Resources::getAdminHtml()
{
    return gAdminHtmlData;
}

const char *Resources::getMicroChart()
{
    return gMicroChartData;
}

const unsigned char *Resources::getFavicon()
{
    return gFaviconData;
}

unsigned int Resources::getFaviconSize()
{
    return gFaviconSize;
}

const char *Resources::getWifiSettingsHtml()
{
    return gWifiSettingsHtmlData;
}
