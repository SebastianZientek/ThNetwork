#pragma once

#include <Arduino.h>

#include "boardsettings.hpp"

namespace logger
{
void init();

template <typename T>
void logInf(T &&value)
{
    if constexpr (boardsettings::enableLogger)
    {
        Serial.print("INF: ");
        Serial.println(std::forward<decltype(value)>(value));
    }
}

template <typename F, typename... T>
void logInfF(F &&fmt, T &&...values)
{
    if constexpr (boardsettings::enableLogger)
    {
        Serial.print("INF: ");
        Serial.printf(std::forward<decltype(fmt)>(fmt),            // NOLINT
                      std::forward<decltype(values)>(values)...);  // NOLINT
        Serial.println();
    }
}

template <typename T>
void logErr(T &&value)
{
    if constexpr (boardsettings::enableLogger)
    {
        Serial.print("ERR: ");
        Serial.println(std::forward<decltype(value)>(value));
    }
}

template <typename F, typename... T>
void logErrF(F &&fmt, T &&...values)
{
    if constexpr (boardsettings::enableLogger)
    {
        Serial.print("ERR: ");
        Serial.printf(std::forward<decltype(fmt)>(fmt),            // NOLINT
                      std::forward<decltype(values)>(values)...);  // NOLINT
        Serial.println();
    }
}
}  // namespace logger
