#pragma once

namespace boardsettings
{
constexpr auto enableLogger = true;
constexpr auto loggerBaud = 115200;
constexpr auto serverPort = 80;
constexpr auto wifiConnectionRetriesBeforeReboot = 10;
constexpr auto wifiConnectDelay = 1000;
constexpr auto wifiConnectionIssueRebootDelay = 1000;
constexpr auto initDelay = 500;
constexpr auto failRebootDelay = 5000;
}  // namespace boardsettings
