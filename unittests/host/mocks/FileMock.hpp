#pragma once

#include <CppUTestExt/MockSupport.h>

namespace fs
{
class File
{
public:
    void close()
    {
        mock("File").actualCall("close");
    }

    std::string readString()
    {
        auto returnVal = mock("File").actualCall("readString").returnPointerValue();
        return *static_cast<std::string *>(returnVal);
    }

    void print(const char *data)
    {
        mock("File").actualCall("print").withStringParameter("data", data);
    }
};

}  // namespace fs
