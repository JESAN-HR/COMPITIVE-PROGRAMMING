#include <cp/utils/modular.hpp>

#include <cstdint>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Replace this single-case example with the contest problem's input and logic.
    std::int64_t left{};
    std::int64_t right{};
    if (!(std::cin >> left >> right)) {
        return 1;
    }

    constexpr std::uint32_t modulus = 1'000'000'007;
    std::cout << cp::mod_add(left, right, modulus) << '\n';
}
