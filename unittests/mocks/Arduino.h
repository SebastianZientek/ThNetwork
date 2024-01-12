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

extern SerialStub Serial;  // NOLINT

void delay(int time);
int millis();