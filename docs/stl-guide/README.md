# STL guide

Use the standard library for common contest operations. Each lesson is a
standalone program with fixed input values so you can predict the result, run
it, and change one condition at a time. See the
[build and run instructions](../getting-started.md) for executable locations.

| Lesson | Executable | Contest use |
| --- | --- | --- |
| [Sequences and text](#sequences-and-text) | `stl_sequences` | Store input and split tokens |
| [Ordered and hashed lookup](#ordered-and-hashed-lookup) | `stl_associative` | Count keys, check membership, find a successor |
| [Stacks, queues, and heaps](#stacks-queues-and-heaps) | `stl_adapters` | Track work in DFS, BFS, or Dijkstra |
| [Sorting, searching, and utilities](#sorting-searching-and-utilities) | `stl_algorithms` | Count repeated values and break ties |
| [Ranges and views](#ranges-and-views) | `stl_ranges` | Filter, transform, and group data |

## Sequences and text

[Source for `stl_sequences`](../../src/stl/sequences.cpp)

- Use `std::array<T, N>` when the number of elements is fixed at compile
  time, such as four moves on a grid. Use `std::vector<T>` when the input
  determines the size. Both provide constant-time indexed access.
- `vector::push_back` takes amortized constant time. A single append can
  reallocate and move all elements, invalidating existing iterators, pointers,
  and references. `reserve` preallocates capacity while keeping the same size.
- `std::string` owns the text. A `std::string_view` can read a token without
  copying characters. Here `find(' ')` locates the separator in linear time,
  then `substr` makes a subview in constant time. Keep the owning string alive
  and avoid operations that invalidate its character storage while using the
  views.

```text
Round scores: 3 1 4
Attempt scores: 3 1 4 2
Record: team-blue 8
Team: team-blue
```

## Ordered and hashed lookup

[Source for `stl_associative`](../../src/stl/associative.cpp)

| Container | Stores | Lookup and single-element insertion | Useful property |
| --- | --- | --- | --- |
| `std::map` | Unique keys with values | O(log n) | Iterate frequency counts in key order |
| `std::set` | Unique keys | O(log n) | `lower_bound` finds the first key at least as large as the query |
| `std::unordered_map` | Unique keys with values | Average O(1), worst O(n) | Look up a frequency by key |
| `std::unordered_set` | Unique keys | Average O(1), worst O(n) | Track membership, such as visited states |

The vote counter uses `++ordered_counts[id]`, which inserts a zero count for a
new key before incrementing it. Use `find` or `contains` for read-only
queries. Ordered iteration is useful when output must be sorted. Iteration
order in a hash container is unspecified.

```text
Votes by ID: 1=2 2=1 4=3
Votes for 4: 3
First ID >= 3: 4
Has ID 3: false
```

## Stacks, queues, and heaps

[Source for `stl_adapters`](../../src/stl/adapters.cpp)

- `std::stack` serves the newest item first and is useful for iterative DFS
  or matching brackets. `std::queue` serves the oldest item first, as in BFS.
  Their default containers support constant-time insertion and removal at the
  ends used by these adapters.
- `std::priority_queue` serves the largest item first by default.
  `std::greater<int>` makes the smallest integer come first, as needed
  for processing the shortest tentative distance in Dijkstra's algorithm.
  With the default vector storage, `top` takes O(1) time, `pop` takes
  O(log n), and `push` takes amortized O(log n).
- Check `empty()` before reading or removing an element. Read `top()` or
  `front()` before calling `pop()`, which returns void.

```text
Stack: 9 2 6
Queue: 6 2 9
Min priority: 2 6 9
```

## Sorting, searching, and utilities

[Source for `stl_algorithms`](../../src/stl/algorithms.cpp)

- `std::ranges::sort` sorts the scores with O(n log n) comparisons. Then
  `std::lower_bound` finds the first score at least equal to the target and
  `std::upper_bound` finds the first score greater than it. Use the same
  ordering when sorting and searching.
- The bounds form a half-open iterator range `[first, last)`. A
  `std::pair` stores the two iterators, and a structured binding names
  them. Each bound takes O(log n) time on a vector. `std::distance`
  takes O(1) time with random-access iterators like these. An empty interval
  means the score is absent. Either boundary can equal `end()`.
- Tuples compare lexicographically. The ranking example sorts by time,
  breaks ties with penalty, and then uses ID as the final tie-breaker.
- `std::midpoint` finds the middle of two integers without overflowing
  when their sum is too large for their type. It rounds an odd-length
  integral midpoint toward its first argument.

```text
Sorted scores: 3 3 5 7 9
Occurrences of 3: 2
First submission: 7 (time 1, penalty 5)
Safe midpoint: 1750000000
```

## Ranges and views

[Source for `stl_ranges`](../../src/stl/ranges.cpp)

`std::views::filter` selects readings at least 5. `std::views::transform`
then doubles each selected reading as the loop requests it. The pipeline
borrows the original array and does one linear pass with constant auxiliary
space. Keep the borrowed array alive throughout iteration.

`std::views::chunk` from C++23 groups the original readings into disjoint
batches of two. The final batch contains a single reading. With a library
that lacks the adaptor, the example walks the same array in slices using
`std::span::subspan`. Both paths visit O(n) elements with constant
auxiliary space. See [C++23 feature detection](../cpp23-features.md).

```text
Double readings of at least 5: 16 12 10
Batch: 8 1
Batch: 6 3
Batch: 5
```

## Try it yourself

1. Change the search target to 4 and predict both bound positions before
   running `stl_algorithms`.
2. Add a vote for ID 3 and compare the membership and successor queries.
3. Set the batch size to 3 and predict how both implementations handle
   the final short batch.

For exact library contracts, see the containers, algorithms, and ranges
clauses of the [C++23 working draft](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf).
