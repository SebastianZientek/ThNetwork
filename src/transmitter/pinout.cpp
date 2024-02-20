#include "pinout.hpp"

#include <Arduino.h>

namespace pinout
{
int getSDA()
{
    return D2;
}

int getSCL()
{
    return D3;
}

}  // namespace pinout
