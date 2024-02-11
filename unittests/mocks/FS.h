#pragma once

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

namespace fs
{
class FS
{
public:
    FS() = default;
    FS(const FS &) = default;
    FS(FS &&) = default;
    FS &operator=(const FS &) = default;
    FS &operator=(FS &&) = default;
    virtual ~FS() = default;

    virtual bool exists(const char *path) = 0;
};


class File
{
public:
    void close()
    {
        mock().actualCall("File::close");
    }

    std::string readString()
    {
        auto returnVal = mock().actualCall("File::readString").returnPointerValue();
        return *static_cast<std::string*>(returnVal);
    }

    void print(const char *data)
    {
        mock().actualCall("File::print").withStringParameter("data", data);
    }
};

}  // namespace fs
