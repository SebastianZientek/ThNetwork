#pragma once

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

class SerialStub
{
public:
    template <typename... Ts>
    void print(Ts... args)
    {
    }

    template <typename... Ts>
    void println(Ts... args)
    {
    }

    template <typename... Ts>
    void printf(Ts... args)
    {
    }
};

SerialStub Serial;  // NOLINT

void delay(int time)
{
    mock().actualCall("delay").withParameter("time", time);
}

int millis()
{
    return mock().actualCall("millis").returnIntValue();
}
