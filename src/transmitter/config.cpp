#include "config.hpp"

#include <Arduino.h>
#include <EEPROM.h>
#include <user_interface.h>

#include "common/logger.hpp"

namespace config
{
constexpr auto EEPROMInitTime = 500;
constexpr auto RTCMemAddr = 64;

void saveToEEPROM(TransmitterConfig configuration)
{
    EEPROM.begin(sizeof(configuration));
    delay(EEPROMInitTime);
    EEPROM.put(0, configuration);
    EEPROM.commit();
    EEPROM.end();
    logger::logDbg("Saved to EEPROM");
}

TransmitterConfig readFromEEPROM()
{
    TransmitterConfig configuration;
    EEPROM.begin(sizeof(configuration));
    delay(EEPROMInitTime);
    EEPROM.get(0, configuration);
    EEPROM.commit();
    EEPROM.end();
    delay(EEPROMInitTime);
    logger::logDbg("Loaded from EEPROM");

    return configuration;
}

void saveToRTC(TransmitterConfig configuration)
{
    if (system_rtc_mem_write(RTCMemAddr, &configuration, sizeof(configuration)))
    {
        logger::logDbg("Saved to RTC %d bytes", sizeof(configuration));
    }
    else
    {
        logger::logDbg("Saving to RTC failed!");
    }
}

TransmitterConfig readFromRTC()
{
    TransmitterConfig configuration;
    if (system_rtc_mem_read(RTCMemAddr, &configuration, sizeof(configuration)))
    {
        logger::logDbg("Loaded from RTC %d bytes", sizeof(configuration));
    }
    else
    {
        logger::logErr("Loading from RTC failed!");
    }
    return configuration;
}
}  // namespace config
