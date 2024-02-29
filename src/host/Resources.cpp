#include "Resources.hpp"

#include <incbin.h>

INCTXT(IndexHtml, "src/host/html/index.html");
INCTXT(AdminHtml, "src/host/html/admin.html");
INCTXT(MicroChart, "src/host/html/microChart.js");
INCTXT(AdminJs, "src/host/html/admin.js");
INCTXT(ChartsJs, "src/host/html/charts.js");
INCTXT(PicoCss, "src/host/html/pico.min.css");
INCBIN(Favicon, "src/host/html/fav.png");
INCTXT(WifiSettingsHtml, "src/host/html/wifiSettings.html");

const char *Resources::getIndexHtml() const
{
    return gIndexHtmlData;
}

const char *Resources::getAdminHtml() const
{
    return gAdminHtmlData;
}

const char *Resources::getMicroChart() const
{
    return gMicroChartData;
}

const char *Resources::getAdminJs() const
{
    return gAdminJsData;
}

const char *Resources::getChartsJs() const
{
    return gChartsJsData;
}

const char *Resources::getPicoCss() const
{
    return gPicoCssData;
}

const unsigned char *Resources::getFavicon() const
{
    return gFaviconData;
}

unsigned int Resources::getFaviconSize() const
{
    return gFaviconSize;
}

const char *Resources::getWifiSettingsHtml() const
{
    return gWifiSettingsHtmlData;
}
