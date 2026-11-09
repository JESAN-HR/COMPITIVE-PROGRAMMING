#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <ranges>
#include <span>
#include <version>

namespace {

void print_batch(std::ranges::input_range auto&& batch) {
    std::cout << "Batch:";
    for (const int reading : batch) {
        std::cout << ' ' << reading;
    }
    std::cout << '\n';
}

}  // namespace

int main() {
    constexpr std::array readings{8, 1, 6, 3, 5};
    auto doubled = readings
        | std::views::filter([](int reading) { return reading >= 5; })
        | std::views::transform([](int reading) { return 2 * reading; });

    std::cout << "Double readings of at least 5:";
    for (const int reading : doubled) {
        std::cout << ' ' << reading;
    }
    std::cout << '\n';

    constexpr std::size_t batch_size = 2;
#if defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    for (auto batch : readings | std::views::chunk(std::ptrdiff_t{batch_size})) {
        print_batch(batch);
    }
#else
    const std::span data(readings);
    for (std::size_t offset = 0; offset < data.size();) {
        const auto count = std::min(batch_size, data.size() - offset);
        print_batch(data.subspan(offset, count));
        offset += count;
    }
#endif
}
