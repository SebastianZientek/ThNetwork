#include <CppUTest/TestHarness.h>

#include <nlohmann/json.hpp>

#include "ReadingsStorage.hpp"

// clang-format off
TEST_GROUP(ReadingStorageTest)  // NOLINT
{   
};
// clang-format on

TEST(ReadingStorageTest, addAndReturnReadingsForSensorAsJson)  // NOLINT
{
    ReadingsStorage storage;

    auto sensorId = 1;

    auto temperature1 = 10.0;
    auto humidity1 = 20.0;
    auto time1 = 30;
    storage.addReading(sensorId, temperature1, humidity1, time1);

    auto temperature2 = 10.0;
    auto humidity2 = 20.0;
    auto time2 = 30;
    storage.addReading(sensorId, temperature2, humidity2, time2);

    auto expected = nlohmann::json(
        {{"identifier", sensorId},
         {"values", {{time1, temperature1, humidity1}, {time2, temperature2, humidity2}}}});

    auto results = storage.getReadingsAsJsonStr(sensorId);
    CHECK_TRUE(results == expected.dump());
}

TEST(ReadingStorageTest, getLastReadingAsJson)  // NOLINT
{
    ReadingsStorage storage;

    auto sensorId = 1;

    auto temperature1 = 10.0;
    auto humidity1 = 20.0;
    auto time1 = 30;
    storage.addReading(sensorId, temperature1, humidity1, time1);

    auto temperature2 = 10.0;
    auto humidity2 = 20.0;
    auto time2 = 30;
    storage.addReading(sensorId, temperature2, humidity2, time2);

    auto expected = nlohmann::json(
        {{"identifier", sensorId}, {"values", {{time2, temperature2, humidity2}}}});

    auto results = storage.getLastReadingAsJsonStr(sensorId);
    CHECK_TRUE(results == expected.dump());
}

TEST(ReadingStorageTest, addAndReturnReadingsForSensorAsJsonWhenMultipleSensorsInStorage)  // NOLINT
{
    ReadingsStorage storage;

    auto sensorId1 = 1;
    auto temperature1 = 10.0;
    auto humidity1 = 20.0;
    auto time1 = 30;
    storage.addReading(sensorId1, temperature1, humidity1, time1);

    auto sensorId2 = 2;
    auto temperature2 = 10.0;
    auto humidity2 = 20.0;
    auto time2 = 30;
    storage.addReading(sensorId2, temperature2, humidity2, time2);

    auto expected = nlohmann::json(
        {{"identifier", sensorId2}, {"values", {{time2, temperature2, humidity2}}}});

    auto results = storage.getReadingsAsJsonStr(sensorId2);
    CHECK_TRUE(results == expected.dump());
}

TEST(ReadingStorageTest, getLastReadingAsJsonWhenMultipleSensorsInStorage)  // NOLINT
{
    ReadingsStorage storage;

    auto sensorId1 = 1;
    auto temperature1 = 10.0;
    auto humidity1 = 20.0;
    auto time1 = 30;
    storage.addReading(sensorId1, temperature1, humidity1, time1);

    auto sensorId2 = 2;
    auto temperature2 = 10.0;
    auto humidity2 = 20.0;
    auto time2 = 30;
    storage.addReading(sensorId2, temperature2, humidity2, time2);

    auto expected = nlohmann::json(
        {{"identifier", sensorId2}, {"values", {{time2, temperature2, humidity2}}}});

    auto results = storage.getLastReadingAsJsonStr(sensorId2);
    CHECK_TRUE(results == expected.dump());
}

TEST(ReadingStorageTest, getReadingsReturnsEmptyValuesWhenSensorIdNotExisting)  // NOLINT
{
    ReadingsStorage storage;

    auto sensorId = 1;

    auto temperature1 = 10.0;
    auto humidity1 = 20.0;
    auto time1 = 30;
    storage.addReading(sensorId, temperature1, humidity1, time1);

    auto temperature2 = 10.0;
    auto humidity2 = 20.0;
    auto time2 = 30;
    storage.addReading(sensorId, temperature2, humidity2, time2);

    auto notExistingSensorId = 3;
    auto expected = nlohmann::json(
        {{"identifier", notExistingSensorId}, {"values", nlohmann::json::array()}});

    auto results = storage.getReadingsAsJsonStr(3);
    CHECK_TRUE(results == expected.dump());
}

TEST(ReadingStorageTest, getLastReadingsReturnsEmptyValuesWhenSensorIdNotExisting)  // NOLINT
{
    ReadingsStorage storage;

    auto sensorId = 1;
    auto temperature1 = 10.0;
    auto humidity1 = 20.0;
    auto time1 = 30;
    storage.addReading(sensorId, temperature1, humidity1, time1);

    auto temperature2 = 10.0;
    auto humidity2 = 20.0;
    auto time2 = 30;
    storage.addReading(sensorId, temperature2, humidity2, time2);

    auto notExistingSensorId = 3;
    auto expected = nlohmann::json(
        {{"identifier", notExistingSensorId}, {"values", nlohmann::json::array()}});

    auto results = storage.getLastReadingAsJsonStr(3);
    CHECK_TRUE(results == expected.dump());
}
