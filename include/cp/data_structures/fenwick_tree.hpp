#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include <cp/detail/checked_int64.hpp>

namespace cp {

class FenwickTree {
public:
    explicit FenwickTree(std::size_t size) : cells_(size) {}

    [[nodiscard]] std::size_t size() const noexcept { return cells_.size(); }

    void add(std::size_t index, std::int64_t delta)
    {
        if (index >= size()) {
            throw std::out_of_range("update index is outside the Fenwick tree");
        }
        // Check every touched cell before changing any of them.
        for (auto cell = index; cell < size(); cell |= cell + 1) {
            static_cast<void>(detail::checked_add(cells_[cell], delta));
        }
        for (auto cell = index; cell < size(); cell |= cell + 1) {
            cells_[cell] += delta;
        }
    }

    [[nodiscard]] std::int64_t prefix_sum(std::size_t end) const
    {
        if (end > size()) {
            throw std::out_of_range("prefix end is outside the Fenwick tree");
        }
        std::int64_t total = 0;
        while (end > 0) {
            total = detail::checked_add(total, cells_[end - 1]);
            end &= end - 1;
        }
        return total;
    }

    [[nodiscard]] std::int64_t sum(std::size_t first, std::size_t last) const
    {
        if (first > last || last > size()) {
            throw std::out_of_range("invalid Fenwick tree interval");
        }
        if (first == last) {
            return 0;
        }
        return detail::checked_subtract(prefix_sum(last), prefix_sum(first));
    }

private:
    std::vector<std::int64_t> cells_;
};

}  // namespace cp
