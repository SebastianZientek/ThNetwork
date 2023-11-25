#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <optional>
#include <functional>

namespace serializer
{
template <typename Arr, typename Val>
void writeData(Arr &arr, const Val &val)
{
    auto indexForValue = arr.size() - sizeof(val);
    auto destPtr = std::addressof(arr[indexForValue]);

    std::memcpy(destPtr, &val, sizeof(val));
}

template <typename Arr, typename Val, typename... Ts>
void writeData(Arr &arr, const Val &val, const Ts &...args)
{
    auto size = (sizeof(Ts) + ... + sizeof(val));
    auto indexForValue = arr.size() - size;
    auto destPtr = std::addressof(arr[indexForValue]);
    std::memcpy(destPtr, &val, sizeof(val));

    writeData(arr, args...);
}

template <typename... Ts>
auto serialize(Ts... args)
{
    constexpr auto arrSize = (sizeof(Ts) + ... + 0);
    std::array<uint8_t, arrSize> data;
    writeData(data, args...);

    return data;
}

// -------------------------------------

template <typename Tuple, size_t tplIndex, typename... Ts>
typename std::enable_if<sizeof...(Ts) == 0>::type readValue(Tuple &values,
                                                            const uint8_t *buffer,
                                                            size_t bsize)
{
}

template <typename Tuple, size_t tplIndex, typename T, typename... Ts>
auto readValue(Tuple &values, const uint8_t *buffer, size_t bsize)
{
    constexpr auto size = (sizeof(Ts) + ... + sizeof(T));
    auto indexOfValue = bsize - size;
    auto srcPtr = std::addressof(buffer[indexOfValue]);
    memcpy(&std::get<tplIndex>(values), srcPtr, sizeof(T));
    readValue<Tuple, tplIndex + 1, Ts...>(values, buffer, bsize);
}

template <typename... Ts>
std::optional<std::tuple<Ts...>> deserialize(const uint8_t *buffer, size_t bsize)
{
    constexpr auto argsSize = (sizeof(Ts) + ... + 0);
    if (argsSize != bsize)
    {
        return std::nullopt;
    }

    std::tuple<Ts...> values;
    readValue<decltype(values), 0, Ts...>(values, buffer, bsize);

    return std::optional{values};
}
}  // namespace serializer
