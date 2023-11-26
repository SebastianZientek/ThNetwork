#include <CppUTest/TestHarness.h>

#include "serializer.hpp"

TEST_GROUP(TestSerializer)  // NOLINT
{};

TEST(TestSerializer, shouldSerializeAndDeserializeOneValue)  // NOLINT
{
    const int toSerialize = 123;
    auto serialized = serializer::serialize<int>(toSerialize);
    CHECK_EQUAL(serialized.size(), sizeof(toSerialize));  // NOLINT

    auto deserialized = serializer::deserialize<int>(serialized.data(), serialized.size());
    CHECK_TRUE(deserialized);  // NOLINT

    auto [deserializedInt] = deserialized.value();
    CHECK_EQUAL(toSerialize, deserializedInt);  // NOLINT
}

TEST(TestSerializer, shouldSerializeAndDeserializeMultipleValuesOfDifferentSizes)  // NOLINT
{
    const int inIntVal = 123;
    const char inCharVal = 'c';
    const short inShortVal = 321;

    auto serialized = serializer::serialize(inIntVal, inCharVal, inShortVal);
    CHECK_EQUAL(serialized.size(),                                           // NOLINT
                sizeof(inIntVal) + sizeof(inCharVal) + sizeof(inShortVal));  // NOLINT

    auto deserialized
        = serializer::deserialize<int, char, short>(serialized.data(), serialized.size());
    CHECK_TRUE(deserialized);  // NOLINT

    auto [outIntVal, outCharVal, outShortVal] = deserialized.value();
    CHECK_EQUAL(inIntVal, outIntVal);      // NOLINT
    CHECK_EQUAL(inCharVal, outCharVal);    // NOLINT
    CHECK_EQUAL(inShortVal, outShortVal);  // NOLINT
}

TEST(TestSerializer, shouldSerializeAndDeserializeArray)  // NOLINT
{
    const std::array<int, 3> inArr{4, 5, 6};

    auto serialized = serializer::serialize(inArr);
    CHECK_EQUAL(serialized.size(), inArr.size() * sizeof(int));  // NOLINT

    auto deserialized
        = serializer::deserialize<std::array<int, 3>>(serialized.data(), serialized.size());
    CHECK_TRUE(deserialized);  // NOLINT

    auto [outArr] = deserialized.value();
    CHECK_EQUAL(outArr[0], 4);  // NOLINT
    CHECK_EQUAL(outArr[1], 5);  // NOLINT
    CHECK_EQUAL(outArr[2], 6);  // NOLINT
}

TEST(TestSerializer, shouldSerializeAndDeserializeMultipleArraysAndValues)  // NOLINT
{
    const int inInt = 321;
    const std::array<int, 3> inArr1{4, 5, 6};
    const std::array<int, 2> inArr2{7, 8};
    const short inShort = 123;

    auto serialized = serializer::serialize(inInt, inArr1, inArr2, inShort);
    CHECK_EQUAL(serialized.size(),                                                 // NOLINT
                sizeof(int) + sizeof(inArr1) + sizeof(inArr2) + sizeof(inShort));  // NOLINT

    auto deserialized = serializer::deserialize<int, std::array<int, 3>, std::array<int, 2>, short>(
        serialized.data(), serialized.size());
    CHECK_TRUE(deserialized);  // NOLINT

    auto [outInt, outArr1, outArr2, outShort] = deserialized.value();

    CHECK_EQUAL(inInt, outInt);  // NOLINT

    CHECK_EQUAL(outArr1[0], 4);  // NOLINT
    CHECK_EQUAL(outArr1[1], 5);  // NOLINT
    CHECK_EQUAL(outArr1[2], 6);  // NOLINT

    CHECK_EQUAL(outArr2[0], 7);  // NOLINT
    CHECK_EQUAL(outArr2[1], 8);  // NOLINT

    CHECK_EQUAL(inShort, outShort);  // NOLINT
}

TEST(TestSerializer, shouldReturnNulloptWhenWrongSizeOfDeserializedData)  // NOLINT
{
    const int toSerialize = 123;
    auto serialized = serializer::serialize<int>(toSerialize);
    CHECK_EQUAL(serialized.size(), sizeof(toSerialize));  // NOLINT

    auto deserialized = serializer::deserialize<int, short>(serialized.data(), serialized.size());
    CHECK_FALSE(deserialized);  // NOLINT
}
