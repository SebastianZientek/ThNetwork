#include "Wifi32Adp.hpp"

void Wifi32Adp::init(std::string ssid, std::string pass) 
{
    WiFi.begin(ssid.c_str(), pass.c_str());
}

void Wifi32Adp::disconnect() 
{
    WiFi.disconnect();
}

void Wifi32Adp::setMode(const Mode &mode) 
{
    switch (mode)
    {
    case Mode::OFF:
        WiFi.mode(WIFI_OFF);
        break;
    case Mode::STA:
        WiFi.mode(WIFI_STA);
        break;
    case Mode::AP:
        WiFi.mode(WIFI_AP);
        break;
    case Mode::AP_STA:
        WiFi.mode(WIFI_AP_STA);
        break;
    }
}

Wifi32Adp::Status Wifi32Adp::getStatus() 
{
    switch (WiFi.status())
    {
    case WL_NO_SHIELD:
        return Status::NO_SHIELD;
    case WL_IDLE_STATUS:
        return Status::IDLE_STATUS;
    case WL_NO_SSID_AVAIL:
        return Status::NO_SSID_AVAIL;
    case WL_SCAN_COMPLETED:
        return Status::SCAN_COMPLETED;
    case WL_CONNECTED:
        return Status::CONNECTED;
    case WL_CONNECT_FAILED:
        return Status::CONNECT_FAILED;
    case WL_CONNECTION_LOST:
        return Status::CONNECTION_LOST;
    case WL_DISCONNECTED:
        return Status::DISCONNECTED;
    }
    return Status::ERROR;
}

std::string Wifi32Adp::getSsid() 
{
    return WiFi.SSID().c_str();
}

std::string Wifi32Adp::getLocalIp() 
{
    return WiFi.localIP().toString().c_str();
}

std::string Wifi32Adp::getMacAddr() 
{
    return WiFi.macAddress().c_str();
}

std::size_t Wifi32Adp::getChannel() 
{
    return WiFi.channel();
}

std::array<uint8_t, Wifi32Adp::macAddrDigits> Wifi32Adp::getSoftApMacAddr() 
{
    std::array<uint8_t, macAddrDigits> softApMac{};
    WiFi.softAPmacAddress(softApMac.data());
    return softApMac;
}

void Wifi32Adp::softAp(std::string ssid, std::string pass) 
{
    WiFi.softAP(ssid.c_str(), pass.c_str());
}

std::string Wifi32Adp::getSoftApIp() 
{
    IPAddress IP = WiFi.softAPIP();
    return IP.toString().c_str();
}
