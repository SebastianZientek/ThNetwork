#pragma once

#include <string>

class String
{
public:
    std::string str;

    bool isEmpty() { return str.empty(); }

    friend bool operator<(const String &lhs, const String &rhs) { return lhs.str < rhs.str; }
    String &operator=(char *s)
    {
        str = s;
        return *this;
    }
};
