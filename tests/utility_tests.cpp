#include <cp/utils/modular.hpp>

#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>

static_assert(cp::mod_normalize(-2, 7) == 5);
static_assert(cp::mod_add(-2, 5, 7) == 3);
static_assert(cp::mod_multiply(-2, 5, 7) == 4);
static_assert(cp::mod_power(3, 4, 7) == 4);
static_assert(cp::mod_power(0, 0, 7) == 1);
static_assert(cp::mod_power(5, 0, 1) == 0);

int main() {
    constexpr auto modulus = std::numeric_limits<std::uint32_t>::max();
    constexpr auto minimum = std::numeric_limits<std::int64_t>::min();
    constexpr auto maximum = std::numeric_limits<std::int64_t>::max();

    if (cp::mod_multiply(modulus - 1, modulus - 1, modulus) != 1 ||
        cp::mod_power(modulus - 1, 2, modulus) != 1 ||
        cp::mod_add(minimum, maximum, modulus) != modulus - 1 ||
        cp::mod_add(maximum, maximum, 7) != 0 ||
        cp::mod_add(minimum, minimum, 7) != 5) {
        std::cerr << "Wide modular arithmetic failed\n";
        return 1;
    }

    try {
        static_cast<void>(cp::mod_power(2, 0, 0));
    } catch (const std::invalid_argument&) {
        return 0;
    }

    std::cerr << "A zero modulus must be rejected\n";
    return 1;
}
