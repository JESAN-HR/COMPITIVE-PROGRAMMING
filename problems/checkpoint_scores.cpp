#include <cp/algorithms/prefix_sums.hpp>

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

namespace {

constexpr std::int64_t max_count = 200'000;
constexpr std::int64_t max_score = 1'000'000'000;

bool read_integer(std::int64_t& value) {
    std::string token;
    if (!(std::cin >> token)) {
        return false;
    }

    const auto end = token.data() + token.size();
    const auto result = std::from_chars(token.data(), end, value);
    return result.ec == std::errc{} && result.ptr == end;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::int64_t checkpoint_count = 0;
    std::int64_t query_count = 0;
    if (!read_integer(checkpoint_count) || !read_integer(query_count) ||
        checkpoint_count < 0 || checkpoint_count > max_count ||
        query_count < 0 || query_count > max_count ||
        (checkpoint_count == 0 && query_count != 0)) {
        return 1;
    }

    std::vector<std::int64_t> scores(static_cast<std::size_t>(checkpoint_count));
    for (auto& score : scores) {
        if (!read_integer(score) || score < -max_score || score > max_score) {
            return 1;
        }
    }

    const cp::PrefixSums sums(scores);
    std::vector<std::int64_t> answers;
    answers.reserve(static_cast<std::size_t>(query_count));
    for (std::int64_t query = 0; query < query_count; ++query) {
        std::int64_t first = 0;
        std::int64_t last = 0;
        if (!read_integer(first) || !read_integer(last) ||
            first < 1 || first > last || last > checkpoint_count) {
            return 1;
        }

        answers.push_back(sums.sum(static_cast<std::size_t>(first - 1),
                                   static_cast<std::size_t>(last)));
    }

    std::cin >> std::ws;
    if (!std::cin.eof() || std::cin.bad()) {
        return 1;
    }

    for (const auto answer : answers) {
        std::cout << answer << '\n';
    }
}
