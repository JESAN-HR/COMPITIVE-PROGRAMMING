# Core algorithms

These examples use zero-based indices. The library accepts adjacency lists for graphs and `std::span` for contiguous sequences such as arrays and vectors. A span borrows its storage from the original sequence. [C++ Standard Library span reference](https://learn.microsoft.com/en-us/cpp/standard-library/span-class)

Run [the combined example](../../src/examples/algorithms.cpp) after [building the project](../getting-started.md) with `./build/algorithms_demo` on Linux or `./build-msvc/Release/algorithms_demo.exe` in PowerShell. [The algorithm tests](../../tests/algorithm_tests.cpp) exercise the examples' underlying APIs and their edge cases.

| Algorithm | Time | Extra space |
| --- | --- | --- |
| [Binary and boundary search](#binary-and-boundary-search) | O(log n) | O(1) |
| [Merge sort](#merge-sort) | O(n log n) | O(n) |
| [Prefix sums](#prefix-sums) | O(n) build, O(1) query | O(n) |
| [Sieve of Eratosthenes](#sieve-of-eratosthenes) | O(n log log n) | O(n) |
| [BFS and DFS](#bfs-and-dfs) | O(V + E) | O(V) |
| [Dijkstra](#dijkstra) | O(V + E log(E + 1)) | O(V + E) |

For graph algorithms, `V` counts vertices and `E` counts adjacency-list entries. Space includes output but excludes the input.

## Binary and boundary search

Use [binary search](../../include/cp/algorithms/binary_search.hpp) to find an element, count duplicates, or find the first feasible answer on an ordered domain.

- `first_true(size, predicate)` searches `[0, size)`. The predicate must be false before the boundary and true from that boundary onward. The function returns `size` for an empty domain or when all indices are false.
- `lower_bound_index(values, key)` finds the first element at or after the key in the ordering. `upper_bound_index(values, key)` finds the first strictly later element. Each returns `values.size()` if its boundary falls after the last element.
- `binary_search_index(values, key)` returns the first equivalent index as an optional. Sort the sequence with the same comparison used for the search.

At each step the midpoint eliminates one side of the current interval. A true midpoint remains a possible boundary, so move the right endpoint to it. A false midpoint is strictly before the boundary, so start after it. Computing the midpoint from the interval's length avoids overflow from adding the two endpoints.

For `[2, 4, 4, 9]`, the bounds for `4` are `1` and `3`. Subtract them to count the two copies. A lookup of `6` has lower bound `3` and no exact match. Check that a boundary is below the sequence size before indexing. The standard library provides iterator-returning bounds for the same use case. [C++ Standard Library algorithm reference](https://learn.microsoft.com/en-us/cpp/standard-library/algorithm-functions)

## Merge sort

[Merge sort](../../include/cp/algorithms/merge_sort.hpp) sorts a mutable span of copyable values in place. It divides the sequence in half, sorts both halves, and merges their smallest remaining elements into a shared buffer. Each selected element is the smallest remaining overall because both halves are sorted. Choosing from the left on ties preserves relative order among equivalent elements.

For `[9, 4a, 2, 4b]`, the sorted halves are `[4a, 9]` and `[2, 4b]`. The merge produces `[2, 4a, 4b, 9]`. Use this approach when tie order carries meaning, such as records with equal scores. Empty and singleton spans need no sorting. Use a strict weak order such as `<`. A comparison using `<=` can break stability and violates the ordering contract. The implementation uses one O(n) buffer and O(log n) recursion depth.

## Prefix sums

[Prefix sums](../../include/cp/algorithms/prefix_sums.hpp) answer repeated range-sum queries over an immutable sequence. If `P(k)` is the sum of the first `k` values, then the sum over `[first, last)` is `P(last) - P(first)`. Subtracting cancels everything before `first`.

For `[3, -2, 5, 4]`, `P(4) = 10` and `P(1) = 3`, so `sum(1, 4)` gives `7`. The constructor takes a span of `std::int64_t` values and stores its own totals. Equal endpoints give zero, including `sum(0, 0)` for an empty sequence. Reversed or out-of-range endpoints throw `std::out_of_range`. A prefix or queried difference outside the signed 64-bit range throws `std::overflow_error`. Remember that `last` is exclusive. Use a [Fenwick tree](../data-structures/README.md#fenwick-tree) when values need to change between queries.

## Sieve of Eratosthenes

[The sieve](../../include/cp/algorithms/sieve.hpp) lists all primes through an inclusive limit. For each unmarked number `p` through the square root of the limit, mark its multiples starting at `p * p`. Smaller multiples already have a smaller prime factor. Every composite has a prime factor at most its square root, so any unmarked number greater than one is prime.

For limit `12`, mark `4, 6, 8, 10, 12` with `2`, then `9, 12` with `3`. The result is `[2, 3, 5, 7, 11]`. Limits `0` and `1` produce an empty result. The loop checks `p <= limit / p` before squaring and stops before a multiple would exceed the limit. A limit of `SIZE_MAX` throws `std::length_error` because the flag count would overflow. Choose a limit whose flags and result fit in memory.

## BFS and DFS

[Graph traversal](../../include/cp/algorithms/graph_traversal.hpp) returns the discovery order of vertices reachable from a source. `Graph` is an adjacency list where `graph[u]` lists outgoing neighbors. For an undirected edge, insert both directions.

- **BFS** processes discovered vertices in FIFO order. All vertices at distance `k` are processed before those at distance `k + 1`. This layer property is useful for finding shortest paths by edge count.
- **DFS** uses a stack of frames that record which neighbor to visit next. Each frame resumes when its current branch finishes, giving recursive DFS order without deep function calls. DFS is useful for exploring reachability and building more specialized graph routines.

For edges `0→1`, `0→2`, `1→3`, and `2→1`, BFS from `0` visits `0, 1, 2, 3`. DFS visits `0, 1, 3, 2` when neighbors are listed in that order. Both mark each vertex on discovery so cycles, self-loops, and repeated edges cannot cause a revisit. Disconnected vertices are omitted. Every vertex ID must be in `[0, graph.size())`. An invalid source, including any source on an empty graph, or an invalid endpoint anywhere in the graph throws `std::out_of_range`.

## Dijkstra

[Dijkstra's algorithm](../../include/cp/algorithms/dijkstra.hpp) finds minimum total weights from one source. A `WeightedGraph` stores outgoing edges as `{to, weight}`. Put tentative distances in a min-heap. When a vertex is popped at its current distance, no shorter path can go through a farther vertex because weights are nonnegative. Relax each outgoing edge and push any improved distance. Older heap entries can be skipped because a better entry already replaced them.

For `0→1 (7)`, `0→2 (1)`, `2→1 (3)`, and `1→3 (2)`, the distances from `0` to `0, 1, 2, 3` are `0, 4, 1, 6`. A disconnected vertex has an empty optional. The algorithm accepts zero weights and the exact maximum `std::int64_t` distance. Before addition it checks whether the candidate exceeds that maximum. Because all weights are nonnegative, skipping such a candidate cannot hide a representable path. An empty optional therefore also covers a vertex whose shortest distance exceeds that maximum.

All weights and endpoints are validated, even in disconnected components. A negative weight throws `std::invalid_argument`. An invalid source or endpoint throws `std::out_of_range`. Use a different shortest-path algorithm when negative weights are allowed.
