#pragma once
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "FS.h"

class FileSystemStub
{
public:
    fs::File open([[maybe_unused]] const char *path,
                  [[maybe_unused]] const char *mode,
                  [[maybe_unused]] bool create)
    {
        return fs::File{};
    }
};
