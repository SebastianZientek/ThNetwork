#include "config.hpp"

#include <Arduino.h>
#include <EEPROM.h>
#include <user_interface.h>

#include "logger.hpp"

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
    logger::logInf("Saved to EEPROM");
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
    logger::logInf("Loaded from EEPROM");

    return configuration;
}

void saveToRTC(TransmitterConfig configuration)
{
    if(system_rtc_mem_write(RTCMemAddr, &configuration, sizeof(configuration)))
    {
        logger::logInfF("Saved to RTC %d bytes", sizeof(configuration));
    }
    else
    {
        logger::logErr("Saving to RTC failed!");
    }
}

TransmitterConfig readFromRTC()
{
    TransmitterConfig configuration;
    if (system_rtc_mem_read(RTCMemAddr, &configuration, sizeof(configuration)))
    {
        logger::logInfF("Loaded from RTC %d bytes", sizeof(configuration));
    }
    else
    {
        logger::logErr("Loading from RTC failed!");
    }
    return configuration;
}
}  // namespace config