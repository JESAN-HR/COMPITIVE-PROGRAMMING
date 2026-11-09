# Data structures

The toolkit's data structures own their storage and use zero-based indices. [The combined example](../../src/examples/algorithms.cpp) demonstrates each structure. After [building the project](../getting-started.md), run `./build/algorithms_demo` on Linux or `./build-msvc/Release/algorithms_demo.exe` in PowerShell. Edge cases are covered in [the algorithm tests](../../tests/algorithm_tests.cpp).

| Structure | Build | Operations | Space |
| --- | --- | --- | --- |
| [Disjoint set union](#disjoint-set-union) | O(n) | O(α(n)) amortized for find, union, connectivity and component size | O(n) |
| [Fenwick tree](#fenwick-tree) | O(n) for a zero-filled tree | O(log n) for point updates, prefix sums and range sums | O(n) |

## Disjoint set union

[Disjoint set union](../../include/cp/data_structures/disjoint_set_union.hpp), or DSU, maintains a partition of `n` elements into components as connections are added. Each element has a parent. Roots point to themselves and store their component size.

`find(index)` follows parent links to a root and shortens the path by linking each visited node to its grandparent. `unite(a, b)` links the smaller component's root to the larger one and updates its size. This preserves exactly one root for the merged component. The combination keeps operations close to constant time in practice. The inverse Ackermann function, `α(n)`, grows extremely slowly.

With four elements, call `unite(0, 1)` and `unite(1, 2)`. Now `same(0, 2)` is true, `component_size(0)` is `3`, and `components()` is `2`. A repeated or self union returns false and leaves the partition intact. A size-zero structure has zero components. Any index outside `[0, size())` throws `std::out_of_range`. Ask `same` for connectivity because root IDs depend on union order. DSU is useful for incremental undirected connectivity and Kruskal's minimum spanning tree algorithm.

## Fenwick tree

[A Fenwick tree](../../include/cp/data_structures/fenwick_tree.hpp) maintains additive point updates and range sums. Each cell `i` stores the sum of a block ending at `i`. Adding a delta visits the blocks containing that index by advancing with `i = i | (i + 1)`. To compute `prefix_sum(end)` for `[0, end)`, read the block at `end - 1` and clear the lowest set bit of `end`. Each step removes a disjoint block from the remaining prefix. A range sum subtracts two prefixes.

Start with four zeros and load `[3, -2, 5, 4]` with calls to `add(index, value)`. Then `sum(1, 4)` gives `7`. Calling `add(1, 3)` changes `-2` to `1`, so the same query gives `10`. This is useful when many score changes and total queries are interleaved. The second argument to `add` is a delta. Point indices must be in `[0, size())` and query endpoints must satisfy `0 <= first <= last <= size()`. Invalid indices throw `std::out_of_range`. Empty ranges give zero, including the only range in a size-zero tree.

Cells, intermediate prefix totals, and range differences must fit in `std::int64_t`. An overflow throws `std::overflow_error`. Updates validate every affected cell before changing the tree, so a rejected update preserves the original sums. Even if a final sum fits, a query can overflow while combining blocks or computing an endpoint prefix. Bound the total absolute magnitude of the values if queries must always fit.
