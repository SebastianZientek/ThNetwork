#pragma once

#include <Arduino.h>

namespace logger
{
void init();

enum LogLevel{
    INF = 0,
    WRN = 1,
    ERR = 2
};

constexpr std::array<const char*, 3> logCString = {
    "INF: ",
    "WRN: ",
    "ERR: "
};

template <typename T>
void log(const LogLevel logLevel, const T &value)
{
#ifdef ENABLE_LOGGER
    Serial.print(logCString[logLevel]); // NOLINT
    Serial.println(value);
#endif
}

template <typename F, typename... T>
void log(const LogLevel logLevel, const F &fmt, const T &...values)
{
#ifdef ENABLE_LOGGER
    Serial.print(logCString[logLevel]);// NOLINT
    Serial.printf(fmt, values...);  // NOLINT
    Serial.println();
#endif
}

template <typename ...Ts>
void logErr(Ts ...args)
{
    log(logger::ERR, args...);
}

template <typename ...Ts>
void logWrn(Ts ...args)
{
    log(logger::WRN, args...);
}

template <typename ...Ts>
void logInf(Ts ...args)
{
    log(logger::INF, args...);
}

}  // namespace logger
