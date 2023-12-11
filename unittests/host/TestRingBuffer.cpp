#include <CppUTest/TestHarness.h>

#include <vector>

#include "RingBuffer.hpp"

TEST_GROUP(RingBufferTest)  // NOLINT
{};

TEST(RingBufferTest, ReadingLastValueWhenOverload)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.put(1);
    CHECK_EQUAL(1, rb.getLast());
    rb.put(2);
    CHECK_EQUAL(2, rb.getLast());
    rb.put(3);
    CHECK_EQUAL(3, rb.getLast());

    // Should overwrite index 0
    rb.put(4);
    CHECK_EQUAL(rb.getLast(), 4);
}

TEST(RingBufferTest, ReadingWithIteratorsNotFullBuffer)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.put(1);
    rb.put(2);

    auto it = rb.begin();
    CHECK_EQUAL(1, *it);
    it++;
    CHECK_EQUAL(2, *it);
}

TEST(RingBufferTest, ReadingWithIteratorsFullBuffer)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.put(1);
    rb.put(2);
    rb.put(3);

    auto it = rb.begin();
    CHECK_EQUAL(1, *it);
    it++;
    CHECK_EQUAL(2, *it);
    it++;
    CHECK_EQUAL(3, *it);
}

TEST(RingBufferTest, ReadingWithIteratorsBufferOverload)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.put(1);
    rb.put(2);
    rb.put(3);

    // Rotation - should override 1
    rb.put(4);

    auto it = rb.begin();
    CHECK_EQUAL(2, *it);
    it++;
    CHECK_EQUAL(3, *it);
    it++;
    CHECK_EQUAL(4, *it);
}

TEST(RingBufferTest, RangeForLoop)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.put(1);
    rb.put(2);
    rb.put(3);

    // Rotation - should override 1 and 2
    rb.put(4);
    rb.put(5);

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


TEST(RingBufferTest, RangeForLoopLongerOverride)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.put(1);
    rb.put(2);
    rb.put(3);

    // Rotation - should override 1 and 2, and 3 twice
    rb.put(4);
    rb.put(5);
    rb.put(6);
    rb.put(7);
    rb.put(8);
    rb.put(9);

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