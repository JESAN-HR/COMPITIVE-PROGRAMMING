#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cp {

struct WeightedEdge {
    std::size_t to;
    std::int64_t weight;
};

using WeightedGraph = std::vector<std::vector<WeightedEdge>>;

// An empty distance means there is no path with an int64_t-representable weight.
[[nodiscard]] inline std::vector<std::optional<std::int64_t>> dijkstra(
    const WeightedGraph& graph, std::size_t source)
{
    if (source >= graph.size()) {
        throw std::out_of_range("source vertex is outside the graph");
    }
    for (const auto& edges : graph) {
        for (const auto& [to, weight] : edges) {
            if (to >= graph.size()) {
                throw std::out_of_range("edge endpoint is outside the graph");
            }
            if (weight < 0) {
                throw std::invalid_argument("Dijkstra requires nonnegative weights");
            }
        }
    }

    using Entry = std::pair<std::int64_t, std::size_t>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<>> queue;
    std::vector<std::optional<std::int64_t>> distances(graph.size());
    distances[source] = 0;
    queue.emplace(0, source);

    constexpr auto maximum = std::numeric_limits<std::int64_t>::max();
    while (!queue.empty()) {
        const auto [distance, vertex] = queue.top();
        queue.pop();
        if (distances[vertex] != distance) {
            continue;
        }
        for (const auto& [to, weight] : graph[vertex]) {
            if (weight > maximum - distance) {
                continue;
            }
            const auto candidate = distance + weight;
            if (!distances[to] || candidate < *distances[to]) {
                distances[to] = candidate;
                queue.emplace(candidate, to);
            }
        }
    }
    return distances;
}

}  // namespace cp
