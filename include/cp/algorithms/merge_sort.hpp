#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <span>
#include <utility>
#include <vector>

namespace cp {
namespace detail {

template <class T, class Compare>
void merge_sort_range(std::span<T> values, std::vector<T>& buffer, Compare& compare)
{
    if (values.size() < 2) {
        return;
    }

    const auto middle = values.size() / 2;
    merge_sort_range(values.first(middle), buffer, compare);
    merge_sort_range(values.subspan(middle), buffer, compare);

    std::size_t left = 0;
    std::size_t right = middle;
    for (std::size_t index = 0; index < values.size(); ++index) {
        // Take from the left on ties to preserve the original order.
        if (left < middle &&
            (right == values.size() ||
             !std::invoke(compare, std::as_const(values[right]),
                          std::as_const(values[left])))) {
            buffer[index] = values[left++];
        } else {
            buffer[index] = values[right++];
        }
    }
    for (std::size_t index = 0; index < values.size(); ++index) {
        values[index] = buffer[index];
    }
}

}  // namespace detail

template <std::copyable T, std::size_t Extent, class Compare = std::less<>>
    requires std::strict_weak_order<Compare&, const T&, const T&>
void merge_sort(std::span<T, Extent> values, Compare compare = {})
{
    if (values.size() < 2) {
        return;
    }
    std::vector<T> buffer(values.begin(), values.end());
    detail::merge_sort_range(std::span<T>{values}, buffer, compare);
}

}  // namespace cp
