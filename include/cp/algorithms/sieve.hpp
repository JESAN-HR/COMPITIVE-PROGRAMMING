#pragma once

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

namespace cp {

[[nodiscard]] inline std::vector<std::size_t> primes_up_to(std::size_t limit)
{
    if (limit < 2) {
        return {};
    }
    if (limit == std::numeric_limits<std::size_t>::max()) {
        throw std::length_error("sieve size exceeds the size_t range");
    }

    std::vector<bool> composite(limit + 1, false);
    for (std::size_t prime = 2; prime <= limit / prime; ++prime) {
        if (composite[prime]) {
            continue;
        }
        for (auto multiple = prime * prime;; multiple += prime) {
            composite[multiple] = true;
            if (limit - multiple < prime) {
                break;
            }
        }
    }

    std::vector<std::size_t> primes;
    for (std::size_t candidate = 2; candidate <= limit; ++candidate) {
        if (!composite[candidate]) {
            primes.push_back(candidate);
        }
    }
    return primes;
}

}  // namespace cp
