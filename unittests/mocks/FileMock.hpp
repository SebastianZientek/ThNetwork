#pragma once

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

class FileMock
{
public:
    void close()
    {
        mock().actualCall("FileMock::close");
    }

    bool operator!() const
    {
        return true;
    }
};
