#include <array>
#include <functional>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

int main() {
    constexpr std::array checkpoints{6, 2, 9};
    std::stack<int> pending_depth_first;
    std::queue<int> pending_breadth_first;
    std::priority_queue<int, std::vector<int>, std::greater<int>> pending_cheapest_first;

    for (const int checkpoint : checkpoints) {
        pending_depth_first.push(checkpoint);
        pending_breadth_first.push(checkpoint);
        pending_cheapest_first.push(checkpoint);
    }

    std::cout << "Stack:";
    while (!pending_depth_first.empty()) {
        std::cout << ' ' << pending_depth_first.top();
        pending_depth_first.pop();
    }
    std::cout << "\nQueue:";
    while (!pending_breadth_first.empty()) {
        std::cout << ' ' << pending_breadth_first.front();
        pending_breadth_first.pop();
    }
    std::cout << "\nMin priority:";
    while (!pending_cheapest_first.empty()) {
        std::cout << ' ' << pending_cheapest_first.top();
        pending_cheapest_first.pop();
    }
    std::cout << '\n';
}
