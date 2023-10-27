#include <CppUTest/TestHarness.h>

#include <vector>

#include "ReadingsBuffer.hpp"

TEST_GROUP(ReadingsBufferTest)  // NOLINT
{};

TEST(ReadingsBufferTest, ReadingLastValueWhenOverload)  // NOLINT
{
    ReadingsBuffer<int, 3> rb;
    rb.addReading(1);
    CHECK_EQUAL(1, rb.getLast());
    rb.addReading(2);
    CHECK_EQUAL(2, rb.getLast());
    rb.addReading(3);
    CHECK_EQUAL(3, rb.getLast());

    // Should overwrite index 0
    rb.addReading(4);
    CHECK_EQUAL(rb.getLast(), 4);
}

TEST(ReadingsBufferTest, ReadingWithIteratorsNotFullBuffer)  // NOLINT
{
    ReadingsBuffer<int, 3> rb;
    rb.addReading(1);
    rb.addReading(2);

    auto it = rb.begin();
    CHECK_EQUAL(1, *it);
    it++;
    CHECK_EQUAL(2, *it);
}

TEST(ReadingsBufferTest, ReadingWithIteratorsFullBuffer)  // NOLINT
{
    ReadingsBuffer<int, 3> rb;
    rb.addReading(1);
    rb.addReading(2);
    rb.addReading(3);

    auto it = rb.begin();
    CHECK_EQUAL(1, *it);
    it++;
    CHECK_EQUAL(2, *it);
    it++;
    CHECK_EQUAL(3, *it);
}

TEST(ReadingsBufferTest, ReadingWithIteratorsBufferOverload)  // NOLINT
{
    ReadingsBuffer<int, 3> rb;
    rb.addReading(1);
    rb.addReading(2);
    rb.addReading(3);

    // Rotation - should override 1
    rb.addReading(4);

    auto it = rb.begin();
    CHECK_EQUAL(2, *it);
    it++;
    CHECK_EQUAL(3, *it);
    it++;
    CHECK_EQUAL(4, *it);
}

TEST(ReadingsBufferTest, RangeForLoop)  // NOLINT
{
    ReadingsBuffer<int, 3> rb;
    rb.addReading(1);
    rb.addReading(2);
    rb.addReading(3);

    // Rotation - should override 1 and 2
    rb.addReading(4);
    rb.addReading(5);

    std::vector<int> values;
    for (const auto &r : rb)
    {
        values.push_back(r);
    }

    CHECK_EQUAL(3, values.size());
    CHECK_EQUAL(3, values[0]);
    CHECK_EQUAL(4, values[1]);
    CHECK_EQUAL(5, values[2]);
}


TEST(ReadingsBufferTest, RangeForLoopLongerOverride)  // NOLINT
{
    ReadingsBuffer<int, 3> rb;
    rb.addReading(1);
    rb.addReading(2);
    rb.addReading(3);

    // Rotation - should override 1 and 2, and 3 twice
    rb.addReading(4);
    rb.addReading(5);
    rb.addReading(6);
    rb.addReading(7);
    rb.addReading(8);
    rb.addReading(9);

    std::vector<int> values;
    for (const auto &r : rb)
    {
        values.push_back(r);
    }

    CHECK_EQUAL(3, values.size());
    CHECK_EQUAL(7, values[0]);
    CHECK_EQUAL(8, values[1]);
    CHECK_EQUAL(9, values[2]);
}
