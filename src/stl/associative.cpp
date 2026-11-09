#include <array>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

int main() {
    constexpr std::array votes{4, 1, 4, 2, 1, 4};
    std::map<int, int> ordered_counts;
    std::unordered_map<int, int> hashed_counts;
    hashed_counts.reserve(votes.size());

    for (const int id : votes) {
        ++ordered_counts[id];
        ++hashed_counts[id];
    }

    std::cout << "Votes by ID:";
    for (const auto& [id, count] : ordered_counts) {
        std::cout << ' ' << id << '=' << count;
    }
    std::cout << '\n';

    if (const auto found = hashed_counts.find(4); found != hashed_counts.end()) {
        std::cout << "Votes for 4: " << found->second << '\n';
    }

    const std::set<int> ordered_ids(votes.begin(), votes.end());
    const std::unordered_set<int> hashed_ids(votes.begin(), votes.end());
    if (const auto next = ordered_ids.lower_bound(3); next != ordered_ids.end()) {
        std::cout << "First ID >= 3: " << *next << '\n';
    }
    std::cout << "Has ID 3: " << std::boolalpha << hashed_ids.contains(3) << '\n';
}
