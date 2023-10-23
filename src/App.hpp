#pragma once

#include "EspNow.hpp"
#include "WebView.hpp"
#include "boardsettings.hpp"

class App
{
public:
    void run();

private:
    WebView m_web{boardsettings::serverPort};
    EspNow m_espNow;
};
