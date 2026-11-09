#pragma once

#include <cstdint>
#include <stdexcept>

namespace cp {

// A 32-bit modulus keeps products of normalized residues within uint64_t.
constexpr std::uint32_t mod_normalize(std::int64_t value, std::uint32_t modulus) {
    if (modulus == 0) {
        throw std::invalid_argument("modulus must be positive");
    }

    const auto signed_modulus = static_cast<std::int64_t>(modulus);
    const auto remainder = value % signed_modulus;
    return static_cast<std::uint32_t>(remainder < 0 ? remainder + signed_modulus : remainder);
}

constexpr std::uint32_t mod_add(std::int64_t left, std::int64_t right, std::uint32_t modulus) {
    const auto left_residue = static_cast<std::uint64_t>(mod_normalize(left, modulus));
    return static_cast<std::uint32_t>((left_residue + mod_normalize(right, modulus)) % modulus);
}

constexpr std::uint32_t mod_multiply(std::int64_t left, std::int64_t right, std::uint32_t modulus) {
    const auto left_residue = static_cast<std::uint64_t>(mod_normalize(left, modulus));
    return static_cast<std::uint32_t>((left_residue * mod_normalize(right, modulus)) % modulus);
}

constexpr std::uint32_t mod_power(std::int64_t value, std::uint64_t exponent, std::uint32_t modulus) {
    auto base = static_cast<std::uint64_t>(mod_normalize(value, modulus));
    std::uint64_t result = 1U % modulus;
    while (exponent != 0) {
        if ((exponent & 1U) != 0) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent >>= 1;
    }
    return static_cast<std::uint32_t>(result);
}

} // namespace cp
