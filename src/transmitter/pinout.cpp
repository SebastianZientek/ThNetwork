#include "pinout.hpp"

#include <Arduino.h>

namespace pinout
{
int getSDA()
{
    return D6;
}

int getSCL()
{
    return D5;
}

}  // namespace pinout
