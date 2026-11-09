#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

#include <cp/detail/checked_int64.hpp>

namespace cp {

class PrefixSums {
public:
    explicit PrefixSums(std::span<const std::int64_t> values)
    {
        sums_.reserve(values.size());
        std::int64_t total = 0;
        for (const auto value : values) {
            total = detail::checked_add(total, value);
            sums_.push_back(total);
        }
    }

    [[nodiscard]] std::size_t size() const noexcept { return sums_.size(); }

    // Sum a zero-based half-open interval [first, last).
    [[nodiscard]] std::int64_t sum(std::size_t first, std::size_t last) const
    {
        if (first > last || last > size()) {
            throw std::out_of_range("invalid prefix sums interval");
        }
        if (first == last) {
            return 0;
        }
        const auto before = first == 0 ? 0 : sums_[first - 1];
        return detail::checked_subtract(sums_[last - 1], before);
    }

private:
    std::vector<std::int64_t> sums_;
};

}  // namespace cp
