#pragma once

#include <Arduino.h>

#include <string>
#include <type_traits>
#include <array>

namespace logger
{
void init();

enum LogLevel
{
    INF = 0,
    WRN = 1,
    ERR = 2,
    DBG = 3,
};

constexpr std::array<const char *, 4> logCString = {"INF: ", "WRN: ", "ERR: ", "DBG: "};

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
    log(logger::ERR, args...);
}

template <typename... Ts>
void logWrn(const Ts &...args)
{
    log(logger::WRN, args...);
}

template <typename... Ts>
void logInf(const Ts &...args)
{
    log(logger::INF, args...);
}

template <typename... Ts>
void logDbg(const Ts &...args)
{
    log(logger::DBG, args...);
}

}  // namespace logger
