#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>

#include <cp/algorithms/binary_search.hpp>
#include <cp/algorithms/dijkstra.hpp>
#include <cp/algorithms/graph_traversal.hpp>
#include <cp/algorithms/merge_sort.hpp>
#include <cp/algorithms/prefix_sums.hpp>
#include <cp/algorithms/sieve.hpp>
#include <cp/data_structures/disjoint_set_union.hpp>
#include <cp/data_structures/fenwick_tree.hpp>

int main()
{
    std::array scores{9, 4, 2, 4};
    cp::merge_sort(std::span{scores});
    std::cout << "Sorted:";
    for (const auto score : scores) {
        std::cout << ' ' << score;
    }
    const auto lower = cp::lower_bound_index(std::span{scores}, 4);
    const auto upper = cp::upper_bound_index(std::span{scores}, 4);
    std::cout << "\nPositions of 4: [" << lower << ", " << upper << ")\n";

    const std::array<std::int64_t, 4> values{3, -2, 5, 4};
    const cp::PrefixSums sums(values);
    std::cout << "Static sum [1, 4): " << sums.sum(1, 4) << '\n';

    cp::FenwickTree tree(values.size());
    for (std::size_t index = 0; index < values.size(); ++index) {
        tree.add(index, values[index]);
    }
    tree.add(1, 3);
    std::cout << "Updated sum [1, 4): " << tree.sum(1, 4) << '\n';

    std::cout << "Primes up to 12:";
    for (const auto prime : cp::primes_up_to(12)) {
        std::cout << ' ' << prime;
    }
    std::cout << '\n';

    const cp::Graph graph{{1, 2}, {3}, {1}, {}, {}};
    std::cout << "BFS from 0:";
    for (const auto vertex : cp::bfs(graph, 0)) {
        std::cout << ' ' << vertex;
    }
    std::cout << "\nDFS from 0:";
    for (const auto vertex : cp::dfs(graph, 0)) {
        std::cout << ' ' << vertex;
    }
    std::cout << '\n';

    const cp::WeightedGraph weighted{{{1, 7}, {2, 1}}, {{3, 2}}, {{1, 3}}, {}, {}};
    std::cout << "Distances from 0:";
    for (const auto& distance : cp::dijkstra(weighted, 0)) {
        if (distance) {
            std::cout << ' ' << *distance;
        } else {
            std::cout << " unreachable";
        }
    }
    std::cout << '\n';

    cp::DisjointSetUnion components(4);
    components.unite(0, 1);
    components.unite(1, 2);
    std::cout << "Size of the component containing 0: " << components.component_size(0)
              << '\n';
}
