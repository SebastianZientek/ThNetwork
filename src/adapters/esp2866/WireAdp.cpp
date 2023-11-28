#include "WireAdp.hpp"

#include <Wire.h>

void WireAdp::begin(int sda, int scl)
{
    Wire.begin(sda, scl);
}
