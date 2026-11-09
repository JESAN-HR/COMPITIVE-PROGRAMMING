#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace cp {

using Vertex = std::size_t;
using Graph = std::vector<std::vector<Vertex>>;

namespace detail {

inline void validate_graph(const Graph& graph, Vertex source)
{
    if (source >= graph.size()) {
        throw std::out_of_range("source vertex is outside the graph");
    }
    for (const auto& neighbors : graph) {
        for (const auto neighbor : neighbors) {
            if (neighbor >= graph.size()) {
                throw std::out_of_range("edge endpoint is outside the graph");
            }
        }
    }
}

}  // namespace detail

[[nodiscard]] inline std::vector<Vertex> bfs(const Graph& graph, Vertex source)
{
    detail::validate_graph(graph, source);
    std::vector<bool> visited(graph.size());
    // The output also serves as the FIFO queue.
    std::vector<Vertex> order{source};
    visited[source] = true;
    for (std::size_t index = 0; index < order.size(); ++index) {
        const auto vertex = order[index];
        for (const auto neighbor : graph[vertex]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                order.push_back(neighbor);
            }
        }
    }
    return order;
}

[[nodiscard]] inline std::vector<Vertex> dfs(const Graph& graph, Vertex source)
{
    detail::validate_graph(graph, source);
    struct Frame {
        Vertex vertex;
        std::size_t next_neighbor = 0;
    };
    std::vector<bool> visited(graph.size());
    std::vector<Frame> stack{{source}};
    std::vector<Vertex> order{source};
    visited[source] = true;

    while (!stack.empty()) {
        auto& frame = stack.back();
        const auto& neighbors = graph[frame.vertex];
        if (frame.next_neighbor == neighbors.size()) {
            stack.pop_back();
            continue;
        }
        const auto neighbor = neighbors[frame.next_neighbor++];
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            order.push_back(neighbor);
            stack.push_back({neighbor});
        }
    }
    return order;
}

}  // namespace cp
