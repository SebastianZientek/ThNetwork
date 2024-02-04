#pragma once

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "FS.h"

class FileMock
{
public:
    void close()
    {
        mock().actualCall("FileMock::close");
    }

    std::string readString()
    {
        auto returnVal = mock().actualCall("FileMock::readString").returnPointerValue();
        return *static_cast<std::string*>(returnVal);
    }

    void print(const char *data)
    {
        mock().actualCall("FileMock::print").withStringParameter("data", data);
    }
};
