#pragma once

#include <Arduino.h>

class ReadingsCleaner
{
public:
    void cleanIfNeeded();

private:
    String findOldestReadingsDir();
    void removeDirectoryRecursively(String path);
};
