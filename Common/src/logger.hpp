#pragma once

#include <Arduino.h>

namespace logger
{
void init();

template <typename T>
void logInf(T &&value)
{
#ifdef ENABLE_LOGGER
    Serial.print("INF: ");
    Serial.println(std::forward<decltype(value)>(value));
#endif
}

template <typename F, typename... T>
void logInfF(F &&fmt, T &&...values)
{
#ifdef ENABLE_LOGGER
    Serial.print("INF: ");
    Serial.printf(std::forward<decltype(fmt)>(fmt),            // NOLINT
                  std::forward<decltype(values)>(values)...);  // NOLINT
    Serial.println();
#endif
}

template <typename T>
void logErr(T &&value)
{
#ifdef ENABLE_LOGGER
    Serial.print("ERR: ");
    Serial.println(std::forward<decltype(value)>(value));
#endif
}

template <typename F, typename... T>
void logErrF(F &&fmt, T &&...values)
{
#ifdef ENABLE_LOGGER
    Serial.print("ERR: ");
    Serial.printf(std::forward<decltype(fmt)>(fmt),            // NOLINT
                  std::forward<decltype(values)>(values)...);  // NOLINT
    Serial.println();
#endif
}
}  // namespace logger
