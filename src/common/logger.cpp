#include "logger.hpp"

#include <Arduino.h>

// #include "boardsettings.hpp"

namespace logger
{
void init()
{
#ifdef ENABLE_LOGGER
    Serial.begin(115200);
#endif
}

template <>
decltype(auto) stdStrToCStr<std::string>(const std::string &arg)
{
    return arg.c_str();
}
}  // namespace logger
