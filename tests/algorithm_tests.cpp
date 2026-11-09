#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <cp/algorithms/binary_search.hpp>
#include <cp/algorithms/dijkstra.hpp>
#include <cp/algorithms/graph_traversal.hpp>
#include <cp/algorithms/merge_sort.hpp>
#include <cp/algorithms/prefix_sums.hpp>
#include <cp/algorithms/sieve.hpp>
#include <cp/data_structures/disjoint_set_union.hpp>
#include <cp/data_structures/fenwick_tree.hpp>

namespace {

constexpr auto maximum = std::numeric_limits<std::int64_t>::max();
constexpr auto minimum = std::numeric_limits<std::int64_t>::min();

void check(bool condition, std::string_view description)
{
    if (!condition) {
        throw std::runtime_error(std::string{description});
    }
}

template <class Exception, class Function>
void check_throws(Function function, std::string_view description)
{
    try {
        function();
    } catch (const Exception&) {
        return;
    }
    throw std::runtime_error(std::string{description});
}

void test_binary_search()
{
    const std::array<int, 0> empty{};
    check(cp::lower_bound_index(std::span{empty}, 1) == 0, "empty lower bound");
    check(cp::upper_bound_index(std::span{empty}, 1) == 0, "empty upper bound");
    check(!cp::binary_search_index(std::span{empty}, 1), "empty exact search");

    const std::array singleton{1};
    check(cp::binary_search_index(std::span{singleton}, 1) == std::size_t{0},
          "singleton exact search");
    const std::array values{std::numeric_limits<int>::min(), 4, 4, 9,
                            std::numeric_limits<int>::max()};
    const std::span sorted{values};
    check(cp::lower_bound_index(sorted, 4) == 1, "first duplicate");
    check(cp::upper_bound_index(sorted, 4) == 3, "past last duplicate");
    check(cp::binary_search_index(sorted, 4) == std::size_t{1}, "first matching index");
    check(!cp::binary_search_index(sorted, 6), "missing value inside the range");
    check(cp::lower_bound_index(sorted, std::numeric_limits<int>::min()) == 0,
          "minimum value");
    check(cp::upper_bound_index(sorted, std::numeric_limits<int>::max()) == sorted.size(),
          "maximum value");

    const std::array descending{9, 4, 4, 1};
    check(cp::lower_bound_index(std::span{descending}, 4, std::greater<>{}) == 1,
          "descending lower bound");
    check(cp::upper_bound_index(std::span{descending}, 4, std::greater<>{}) == 3,
          "descending upper bound");

    constexpr auto size_limit = std::numeric_limits<std::size_t>::max();
    static_assert(cp::first_true(size_limit, [](std::size_t) { return false; }) == size_limit);
    static_assert(cp::first_true(size_limit, [](std::size_t index) {
        return index >= size_limit - 1;
    }) == size_limit - 1);
    static_assert(cp::first_true(10, [](std::size_t) { return true; }) == 0);
}

void test_merge_sort()
{
    std::array<int, 0> empty{};
    cp::merge_sort(std::span{empty});
    std::array singleton{1};
    cp::merge_sort(std::span{singleton});
    check(singleton == std::array{1}, "singleton merge sort");

    std::array values{4, 2, 3, 4, 1, -2, 0};
    const std::array ascending{-2, 0, 1, 2, 3, 4, 4};
    cp::merge_sort(std::span{values});
    check(values == ascending, "sort mixed values");
    cp::merge_sort(std::span{values});
    check(values == ascending, "sort already sorted values");
    cp::merge_sort(std::span{values}, std::greater<>{});
    check(values == std::array{4, 4, 3, 2, 1, 0, -2}, "descending merge sort");

    struct Record {
        int key;
        int id;
        bool operator==(const Record&) const = default;
    };
    std::array records{Record{2, 1}, Record{1, 2}, Record{2, 3}, Record{1, 4}};
    cp::merge_sort(std::span{records}, [](const Record& left, const Record& right) {
        return left.key < right.key;
    });
    check(records == std::array{Record{1, 2}, Record{1, 4}, Record{2, 1}, Record{2, 3}},
          "merge sort stability");

    std::array flags{true, false, true};
    cp::merge_sort(std::span{flags});
    check(flags == std::array{false, true, true}, "sort a contiguous boolean array");
}

void test_prefix_sums()
{
    const cp::PrefixSums empty(std::span<const std::int64_t>{});
    check(empty.size() == 0 && empty.sum(0, 0) == 0, "empty prefix sums");

    const std::array<std::int64_t, 4> values{3, -2, 5, 4};
    const cp::PrefixSums sums(values);
    check(sums.size() == 4 && sums.sum(0, 4) == 10, "whole prefix sums interval");
    check(sums.sum(1, 4) == 7 && sums.sum(1, 2) == -2, "interior prefix sums intervals");
    check(sums.sum(4, 4) == 0, "empty interval at the end");
    check_throws<std::out_of_range>([&] { static_cast<void>(sums.sum(2, 1)); },
                                    "reversed prefix sums interval");
    check_throws<std::out_of_range>([&] { static_cast<void>(sums.sum(0, 5)); },
                                    "prefix sums endpoint outside the array");

    const std::array<std::int64_t, 1> boundary{minimum};
    check(cp::PrefixSums(boundary).sum(0, 1) == minimum, "representable minimum prefix");
    check_throws<std::overflow_error>([] {
        const std::array<std::int64_t, 2> overflow{maximum, 1};
        static_cast<void>(cp::PrefixSums(overflow));
    }, "positive prefix overflow");
    check_throws<std::overflow_error>([] {
        const std::array<std::int64_t, 2> underflow{minimum, -1};
        static_cast<void>(cp::PrefixSums(underflow));
    }, "negative prefix overflow");

    const std::array<std::int64_t, 3> difference{minimum, maximum, 1};
    const cp::PrefixSums wide_range(difference);
    check(wide_range.sum(0, 3) == 0, "representable wide-range total");
    check_throws<std::overflow_error>([&] { static_cast<void>(wide_range.sum(1, 3)); },
                                      "unrepresentable prefix difference");
}

void test_sieve()
{
    check(cp::primes_up_to(0).empty() && cp::primes_up_to(1).empty(), "sieve below two");
    check(cp::primes_up_to(2) == std::vector<std::size_t>{2}, "inclusive sieve boundary");
    check(cp::primes_up_to(25) == std::vector<std::size_t>{2, 3, 5, 7, 11, 13, 17, 19, 23},
          "sieve excludes perfect squares");
    check_throws<std::length_error>([] {
        static_cast<void>(cp::primes_up_to(std::numeric_limits<std::size_t>::max()));
    }, "sieve length addition overflow");
}

void test_traversals()
{
    const cp::Graph graph{{1, 2, 1}, {3}, {1, 2}, {0}, {}};
    check(cp::bfs(graph, 0) == std::vector<cp::Vertex>{0, 1, 2, 3},
          "BFS handles cycles, duplicate edges, and disconnected vertices");
    check(cp::dfs(graph, 0) == std::vector<cp::Vertex>{0, 1, 3, 2},
          "DFS follows recursive neighbor order");
    check(cp::bfs(graph, 4) == std::vector<cp::Vertex>{4}, "BFS from an isolated vertex");
    check(cp::dfs(graph, 4) == std::vector<cp::Vertex>{4}, "DFS from an isolated vertex");
    check_throws<std::out_of_range>([] { static_cast<void>(cp::bfs({}, 0)); },
                                    "BFS rejects an empty graph source");
    check_throws<std::out_of_range>([&] { static_cast<void>(cp::dfs(graph, graph.size())); },
                                    "DFS rejects source beyond the last vertex");
    const cp::Graph invalid{{}, {2}};
    check_throws<std::out_of_range>([&] { static_cast<void>(cp::bfs(invalid, 0)); },
                                    "BFS validates disconnected edges");
    check_throws<std::out_of_range>([&] { static_cast<void>(cp::dfs(invalid, 0)); },
                                    "DFS validates disconnected edges");
}

void test_dijkstra()
{
    using Distances = std::vector<std::optional<std::int64_t>>;
    const cp::WeightedGraph graph{{{1, 7}, {2, 1}}, {{3, 2}}, {{1, 3}}, {}, {}};
    check(cp::dijkstra(graph, 0) == Distances{0, 4, 1, 6, std::nullopt},
          "Dijkstra updates a tentative distance and skips stale heap entries");
    check(cp::dijkstra(cp::WeightedGraph{{}}, 0) == Distances{0}, "single-vertex graph");

    const cp::WeightedGraph boundary{{{1, maximum}, {4, 8}}, {{2, 0}, {3, 1}, {4, 1}},
                                      {}, {}, {}, {}};
    check(cp::dijkstra(boundary, 0) == Distances{0, maximum, maximum, std::nullopt, 8,
                                                std::nullopt},
          "maximum distance is valid, overflowing paths are ignored");
    check_throws<std::out_of_range>([] { static_cast<void>(cp::dijkstra({}, 0)); },
                                    "Dijkstra rejects an empty graph source");
    check_throws<std::out_of_range>([] {
        static_cast<void>(cp::dijkstra(cp::WeightedGraph{{}, {{2, 1}}}, 0));
    }, "Dijkstra validates disconnected edge endpoints");
    check_throws<std::invalid_argument>([] {
        static_cast<void>(cp::dijkstra(cp::WeightedGraph{{}, {{0, -1}}}, 0));
    }, "Dijkstra rejects negative weights even on disconnected edges");
}

void test_disjoint_set_union()
{
    cp::DisjointSetUnion empty(0);
    check(empty.size() == 0 && empty.components() == 0, "empty disjoint set union");
    check_throws<std::out_of_range>([&] { static_cast<void>(empty.find(0)); },
                                    "find in an empty disjoint set union");

    cp::DisjointSetUnion sets(5);
    check(sets.components() == 5 && sets.component_size(4) == 1, "initial singleton sets");
    check(sets.unite(0, 1) && sets.unite(2, 1) && sets.unite(3, 4), "join distinct sets");
    check(sets.same(0, 2) && !sets.same(0, 3), "DSU connectivity");
    check(!sets.unite(2, 0) && !sets.unite(4, 4), "duplicate union and self union");
    check(sets.components() == 2 && sets.component_size(1) == 3, "DSU component count and size");
    check_throws<std::out_of_range>([&] { sets.unite(0, 5); }, "DSU rejects invalid endpoint");
    check(sets.components() == 2 && sets.component_size(1) == 3,
          "invalid union preserves the partition");
}

void test_fenwick_tree()
{
    cp::FenwickTree empty(0);
    check(empty.size() == 0 && empty.sum(0, 0) == 0 && empty.prefix_sum(0) == 0,
          "empty Fenwick tree");
    check_throws<std::out_of_range>([&] { empty.add(0, 1); }, "update in an empty tree");

    cp::FenwickTree tree(5);
    std::array<std::int64_t, 5> values{2, -3, 4, 0, 5};
    for (std::size_t index = 0; index < values.size(); ++index) {
        tree.add(index, values[index]);
    }
    tree.add(0, 3);
    values[0] += 3;
    tree.add(4, -7);
    values[4] -= 7;
    for (std::size_t first = 0; first <= values.size(); ++first) {
        std::int64_t expected = 0;
        for (std::size_t last = first; last <= values.size(); ++last) {
            check(tree.sum(first, last) == expected, "Fenwick range differs from direct sum");
            if (last < values.size()) {
                expected += values[last];
            }
        }
    }
    check_throws<std::out_of_range>([&] { tree.add(5, 1); }, "invalid Fenwick update index");
    check_throws<std::out_of_range>([&] { static_cast<void>(tree.prefix_sum(6)); },
                                    "invalid Fenwick prefix endpoint");
    check_throws<std::out_of_range>([&] { static_cast<void>(tree.sum(3, 2)); },
                                    "reversed Fenwick interval");

    cp::FenwickTree update_boundary(4);
    update_boundary.add(2, maximum);
    check_throws<std::overflow_error>([&] { update_boundary.add(0, 1); },
                                      "Fenwick update overflows a later cell");
    check(update_boundary.prefix_sum(1) == 0 && update_boundary.prefix_sum(4) == maximum,
          "overflowing update leaves all Fenwick cells unchanged");

    cp::FenwickTree query_boundary(3);
    query_boundary.add(0, maximum);
    query_boundary.add(2, 1);
    check_throws<std::overflow_error>([&] { static_cast<void>(query_boundary.prefix_sum(3)); },
                                      "Fenwick prefix overflows across valid cells");
    check(query_boundary.sum(3, 3) == 0, "empty interval needs no overflowing prefixes");

    cp::FenwickTree difference_boundary(2);
    difference_boundary.add(0, minimum);
    difference_boundary.add(1, maximum);
    difference_boundary.add(1, 1);
    check_throws<std::overflow_error>([&] { static_cast<void>(difference_boundary.sum(1, 2)); },
                                      "unrepresentable Fenwick difference");
}

}  // namespace

int main()
{
    try {
        test_binary_search();
        test_merge_sort();
        test_prefix_sums();
        test_sieve();
        test_traversals();
        test_dijkstra();
        test_disjoint_set_union();
        test_fenwick_tree();
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
