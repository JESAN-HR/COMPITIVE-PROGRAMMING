#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <span>

namespace cp {

// Return size if the predicate is false at every index in [0, size).
template <class Predicate>
    requires std::predicate<Predicate&, const std::size_t&>
[[nodiscard]] constexpr std::size_t first_true(std::size_t size, Predicate predicate)
{
    std::size_t first = 0;
    std::size_t last = size;
    while (first < last) {
        const auto middle = first + (last - first) / 2;
        if (std::invoke(predicate, middle)) {
            last = middle;
        } else {
            first = middle + 1;
        }
    }
    return first;
}

template <class T, std::size_t Extent, class Key, class Compare = std::less<>>
    requires std::strict_weak_order<Compare&, const T&, const Key&>
[[nodiscard]] constexpr std::size_t lower_bound_index(
    std::span<T, Extent> values, const Key& key, Compare compare = {})
{
    return first_true(values.size(), [&](std::size_t index) {
        const auto& value = values[index];
        return !std::invoke(compare, value, key);
    });
}

template <class T, std::size_t Extent, class Key, class Compare = std::less<>>
    requires std::strict_weak_order<Compare&, const T&, const Key&>
[[nodiscard]] constexpr std::size_t upper_bound_index(
    std::span<T, Extent> values, const Key& key, Compare compare = {})
{
    return first_true(values.size(), [&](std::size_t index) {
        const auto& value = values[index];
        return std::invoke(compare, key, value);
    });
}

template <class T, std::size_t Extent, class Key, class Compare = std::less<>>
    requires std::strict_weak_order<Compare&, const T&, const Key&>
[[nodiscard]] constexpr std::optional<std::size_t> binary_search_index(
    std::span<T, Extent> values, const Key& key, Compare compare = {})
{
    const auto index = lower_bound_index(values, key, std::ref(compare));
    if (index == values.size()) {
        return std::nullopt;
    }
    const auto& value = values[index];
    if (std::invoke(compare, key, value)) {
        return std::nullopt;
    }
    return index;
}

}  // namespace cp
