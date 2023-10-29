#pragma once

#include <array>
#include <iterator>
#include <type_traits>
#include <utility>

template <typename T, uint8_t size>
class RingBuffer
{
    constexpr static auto maxSize = 254;
    constexpr static auto bufferLenght = size + 1;

    static_assert(size > 0);
    static_assert(size <= maxSize);
    static_assert(std::is_trivial<T>::value);

    using BufferType = std::array<T, bufferLenght>;

    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = uint8_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        Iterator(BufferType *buffer, difference_type currentIdx)
            : m_buffer(buffer)
            , m_idx(currentIdx)
        {
        }

        reference operator*() const { return (*m_buffer)[m_idx]; }
        pointer operator->() { return &(*m_buffer)[m_idx]; }
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

        friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_idx == b.m_idx; };
        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_idx != b.m_idx; };

    private:
        difference_type m_idx;
        BufferType *m_buffer;
    };

public:
    void addReading(T reading)
    {
        m_head = modInc(m_head);
        m_buffer[m_head] = reading;

        if (m_head == m_tailId)
        {
            m_tailId = modInc(m_tailId);
        }
    }

    T getLast() { return m_buffer[m_head]; }
    Iterator begin() { return Iterator(&m_buffer, modInc(m_tailId)); }
    Iterator end() { return Iterator(&m_buffer, modInc(m_head)); }

private:
    BufferType m_buffer;
    uint8_t m_head{0};
    uint8_t m_tailId{0};

    void addAndRotate(uint8_t &value) { value = (value + 1) % bufferLenght; }
    uint8_t modInc(uint8_t value) { return (value + 1) % bufferLenght; }
};
