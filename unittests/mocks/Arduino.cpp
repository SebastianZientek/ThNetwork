#include "Arduino.h"

void delay(int time)
{
    mock().actualCall("delay").withParameter("time", time);
}

int millis()
{
    return mock().actualCall("millis").returnIntValue();
}