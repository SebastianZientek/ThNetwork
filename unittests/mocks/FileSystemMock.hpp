#pragma once
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "FS.h"

class FileSystemMock : public fs::FS
{
    bool exists(const char *path) override
    {
        return mock().actualCall("FileSystemMock::exists").returnBoolValue();
    }
};
