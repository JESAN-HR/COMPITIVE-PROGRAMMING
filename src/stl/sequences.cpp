#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int main() {
    constexpr std::array round_scores{3, 1, 4};

    std::vector<int> attempt_scores;
    attempt_scores.reserve(round_scores.size() + 1);
    for (const int score : round_scores) {
        attempt_scores.push_back(score);
    }
    attempt_scores.push_back(2);

    std::cout << "Round scores:";
    for (const int score : round_scores) {
        std::cout << ' ' << score;
    }
    std::cout << "\nAttempt scores:";
    for (const int score : attempt_scores) {
        std::cout << ' ' << score;
    }
    std::cout << '\n';

    std::string record = "team-blue";
    record += " 8";
    const std::string_view line = record;
    const std::string_view team = line.substr(0, line.find(' '));
    std::cout << "Record: " << record << "\nTeam: " << team << '\n';
}
