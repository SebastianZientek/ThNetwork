#include <CppUTest/TestHarness.h>

#include <vector>

#include "RingBuffer.hpp"

TEST_GROUP(RingBufferTest)  // NOLINT
{};

TEST(RingBufferTest, LastValueWhenOverload)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    CHECK_EQUAL(1, rb.peekLast());
    rb.push(2);
    CHECK_EQUAL(2, rb.peekLast());
    rb.push(3);
    CHECK_EQUAL(3, rb.peekLast());

    // Should overwrite index 0
    rb.push(4);
    CHECK_EQUAL(rb.peekLast(), 4);
}

TEST(RingBufferTest, WithIteratorsNotFullBuffer)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);

    auto it = rb.begin();
    CHECK_EQUAL(1, *it);
    it++;
    CHECK_EQUAL(2, *it);
}

TEST(RingBufferTest, WithIteratorsFullBuffer)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);

    auto it = rb.begin();
    CHECK_EQUAL(1, *it);
    it++;
    CHECK_EQUAL(2, *it);
    it++;
    CHECK_EQUAL(3, *it);
}

TEST(RingBufferTest, WithIteratorsBufferOverload)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);

    // Rotation - should override 1
    rb.push(4);

    auto it = rb.begin();
    CHECK_EQUAL(2, *it);
    it++;
    CHECK_EQUAL(3, *it);
    it++;
    CHECK_EQUAL(4, *it);
}

TEST(RingBufferTest, ForLoop)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);

    // Rotation - should override 1 and 2
    rb.push(4);
    rb.push(5);

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


TEST(RingBufferTest, ForLoopLongerOverride)  // NOLINT
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);

    // Rotation - should override 1 and 2, and 3 twice
    rb.push(4);
    rb.push(5);
    rb.push(6);
    rb.push(7);
    rb.push(8);
    rb.push(9);

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
