#pragma once

#include <array>
#include <cinttypes>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

template <typename T, uint16_t size>
class RingBuffer
{
    constexpr static uint16_t maxSize = std::numeric_limits<uint16_t>::max() - 1;
    constexpr static uint16_t bufferLenght = size + 1;

    static_assert(size > 0);
    static_assert(size <= maxSize);

    using BufferType = std::array<T, bufferLenght>;

    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = uint16_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        Iterator(BufferType *buffer, difference_type currentIdx)
            : m_buffer(buffer)
            , m_idx(currentIdx)
        {
        }

        reference operator*() const
        {
            return (*m_buffer)[m_idx];
        }

        pointer operator->()
        {
            return &(*m_buffer)[m_idx];
        }

        Iterator &operator++()
        {
            m_idx = (m_idx + 1) % bufferLenght;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs.m_idx == rhs.m_idx;
        };

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs.m_idx != rhs.m_idx;
        };

    private:
        difference_type m_idx;
        BufferType *m_buffer;
    };

public:
    void put(T reading)
    {
        m_head = modInc(m_head);
        m_buffer[m_head] = reading;

        if (m_head == m_tailId)
        {
            m_tailId = modInc(m_tailId);
        }
    }

    const T &getLast() const
    {
        return m_buffer[m_head];
    }

    Iterator begin()
    {
        return Iterator(&m_buffer, modInc(m_tailId));
    }

    Iterator end()
    {
        return Iterator(&m_buffer, modInc(m_head));
    }

private:
    BufferType m_buffer;
    uint16_t m_head{0};
    uint16_t m_tailId{0};

    uint16_t modInc(uint16_t value)
    {
        return (value + 1) % bufferLenght;
    }
};
