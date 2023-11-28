#include "PinsAdp.hpp"

#include <Arduino.h>

int Pins::getSDA()
{
    return D2;
}

int Pins::getSCL()
{
    return D3;
}
