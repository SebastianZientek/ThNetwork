#pragma once

#include <Arduino.h>

class ReadingsCleaner
{
public:
    void cleanIfNeeded();

private:
    std::string findOldestReadingsDir();
    void removeDirectoryRecursively(const std::string &path);
};
