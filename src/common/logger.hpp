#pragma once

#include <Arduino.h>

#include <array>
#include <string>
#include <type_traits>

namespace logger
{
void init();

enum LogLevel
{
    ERR = 0,
    WRN = 1,
    INF = 2,
    DBG = 3,
};

constexpr std::array<const char *, 4> logCString = {"ERR: ", "WRN: ", "INF: ", "DBG: "};

#ifdef LOGGER_ERR
constexpr auto logLevel = LogLevel::ERR;
#elif LOGGER_WRN
constexpr auto logLevel = LogLevel::WRN;
#elif LOGGER_INF
constexpr auto logLevel = LogLevel::INF;
#elif LOGGER_DBG
constexpr auto logLevel = LogLevel::DBG;
#else
constexpr auto logLevel = LogLevel::INF;
#endif

template <typename T>
decltype(auto) stdStrToCStr(const T &arg)
{
    if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
    {
        return arg.c_str();
    }
    else
    {
        return arg;
    }
}

template <typename T>
void log(const LogLevel logLevel, const T &value)
{
#ifdef ENABLE_LOGGER
    Serial.print(logCString[logLevel]);  // NOLINT
    Serial.println(stdStrToCStr(value));
#endif
}

template <typename F, typename... T>
void log(const LogLevel logLevel, const F &fmt, const T &...values)
{
#ifdef ENABLE_LOGGER
    Serial.print(logCString[logLevel]);           // NOLINT
    Serial.printf(fmt, stdStrToCStr(values)...);  // NOLINT
    Serial.println();
#endif
}

template <typename... Ts>
void logErr(const Ts &...args)
{
    if constexpr (logLevel >= logger::ERR)
    {
        log(logger::ERR, args...);
    }
}

template <typename... Ts>
void logWrn(const Ts &...args)
{
    if constexpr (logLevel >= logger::WRN)
    {
        log(logger::WRN, args...);
    }
}

template <typename... Ts>
void logInf(const Ts &...args)
{
    if constexpr (logLevel >= logger::INF)
    {
        log(logger::INF, args...);
    }
}

template <typename... Ts>
void logDbg(const Ts &...args)
{
    if constexpr (logLevel >= logger::DBG)
    {
        log(logger::DBG, args...);
    }
}

}  // namespace logger
