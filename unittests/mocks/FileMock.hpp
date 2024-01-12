#pragma once

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

class FileMock
{
public:
    // FileMock(const char *path, const char *mode, bool create)
    // {
    //     mock().actualCall("FileMock::FileMock");
    // }

    void close()
    {
        mock().actualCall("FileMock::close");
    }

    bool operator!() const
    {
        return true;
    }
};
