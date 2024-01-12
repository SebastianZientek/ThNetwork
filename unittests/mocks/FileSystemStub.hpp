#pragma once
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "FileMock.hpp"

class FileSystemStub
{
public:
    FileMock open([[maybe_unused]] const char *path,
                  [[maybe_unused]] const char *mode,
                  [[maybe_unused]] bool create)
    {
        return FileMock{};
    }
};
