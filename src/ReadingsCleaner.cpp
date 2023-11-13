#include "ReadingsCleaner.hpp"

#include <SD.h>

#include "logger.hpp"

void ReadingsCleaner::cleanIfNeeded()
{
    constexpr auto MBFactor = 1024 * 1024;
    constexpr auto minimalAvailableSpace = 10 * MBFactor;  // 10 MB
    // constexpr auto minimalAvailableSpace = 15070LL * MBFactor;
    auto spaceLeft = SD.totalBytes() - SD.usedBytes();

    findOldestReadingsDir();

    // logger::logInfF("Memcard space left: %lluMB", spaceLeft / MBFactor);
    if (spaceLeft < minimalAvailableSpace)
    {
        auto dirToRemove = findOldestReadingsDir();
        logger::logInf("Minimal space exceed, removing the oldest readings!");
        logger::logInfF("Directory to remove: %s", dirToRemove);
        removeDirectoryRecursively("/readings/" + dirToRemove);
    }
}

String ReadingsCleaner::findOldestReadingsDir()
{
    auto dir = SD.open("/readings");
    auto entry = dir.openNextFile();
    if (!entry)
    {
        return "";
    }

    String dirToRemove = entry.name();
    for (; entry; entry = dir.openNextFile())
    {
        if (entry.isDirectory())
        {
            if (String(entry.name()) < dirToRemove)
            {
                dirToRemove = entry.name();
            }
        }
    }

    return dirToRemove;
}

void ReadingsCleaner::removeDirectoryRecursively(const String &path)
{
    auto dir = SD.open(path);
    for (auto entry = dir.openNextFile(); entry; entry = dir.openNextFile())
    {
        if (entry.isDirectory())
        {
            logger::logInfF("Path %s", entry.path());
            removeDirectoryRecursively(entry.path());
            SD.rmdir(entry.path());
        }
        else
        {
            logger::logInfF("Path %s", entry.path());
            SD.remove(entry.path());
        }
    }

    if (dir.isDirectory())
    {
        SD.rmdir(dir.path());
    }
}
