#include "MacAddr.hpp"

#include <cstring>

MacAddr::MacAddr(const uint8_t *mac)
{
    std::memcpy(macAddr.data(), mac, macDigits);
}

MacAddr::operator String()
{
    if (macString.isEmpty())
    {
        char macCStr[18];  // NOLINT
        snprintf(macCStr, sizeof(macCStr), "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1],
                 macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
        macString = &macCStr[0];
    }
    return macString;
}

MacAddr::operator uint8_t*()
{
    return macAddr.data();
}
