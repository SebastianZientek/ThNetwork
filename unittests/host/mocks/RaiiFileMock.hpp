#pragma once

#include <CppUTestExt/MockSupport.h>

#include <string>

#include "adapters/IRaiiFile.hpp"

class RaiiFileMock : public IRaiiFile
{
    std::string readString() override
    {
        return mock("RaiiFileMock").actualCall("readString").returnStringValueOrDefault("");
    };

    void print(const std::string &str) override
    {
        mock("RaiiFileMock").actualCall("print").withStringParameter("str", str.c_str());
    };
};
