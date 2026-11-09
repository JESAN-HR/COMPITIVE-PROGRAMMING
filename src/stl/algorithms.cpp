#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

int main() {
    std::vector scores{7, 3, 5, 3, 9};
    std::ranges::sort(scores);
    std::cout << "Sorted scores:";
    for (const int score : scores) {
        std::cout << ' ' << score;
    }
    std::cout << '\n';

    constexpr int target = 3;
    const auto bounds = std::pair{
        std::lower_bound(scores.cbegin(), scores.cend(), target),
        std::upper_bound(scores.cbegin(), scores.cend(), target),
    };
    const auto [first, last] = bounds;
    std::cout << "Occurrences of " << target << ": " << std::distance(first, last) << '\n';

    // Sort by time, then penalty, then ID.
    std::array submissions{
        std::tuple{1, 5, 7},
        std::tuple{1, 9, 3},
        std::tuple{2, 1, 6},
    };
    std::ranges::sort(submissions);
    const auto& [time, penalty, id] = submissions.front();
    std::cout << "First submission: " << id << " (time " << time
              << ", penalty " << penalty << ")\n";

    constexpr auto midpoint = std::midpoint(1'500'000'000, 2'000'000'000);
    std::cout << "Safe midpoint: " << midpoint << '\n';
}
