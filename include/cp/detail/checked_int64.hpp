#pragma once

#include <cstdint>
#include <limits>
#include <stdexcept>

namespace cp::detail {

[[nodiscard]] inline std::int64_t checked_add(std::int64_t left, std::int64_t right)
{
    constexpr auto minimum = std::numeric_limits<std::int64_t>::min();
    constexpr auto maximum = std::numeric_limits<std::int64_t>::max();
    if ((right > 0 && left > maximum - right) ||
        (right < 0 && left < minimum - right)) {
        throw std::overflow_error("sum exceeds the int64_t range");
    }
    return left + right;
}

[[nodiscard]] inline std::int64_t checked_subtract(std::int64_t left, std::int64_t right)
{
    constexpr auto minimum = std::numeric_limits<std::int64_t>::min();
    constexpr auto maximum = std::numeric_limits<std::int64_t>::max();
    if ((right > 0 && left < minimum + right) ||
        (right < 0 && left > maximum + right)) {
        throw std::overflow_error("difference exceeds the int64_t range");
    }
    return left - right;
}

}  // namespace cp::detail
