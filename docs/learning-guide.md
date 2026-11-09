# Learning guide

Build the toolkit and run its tests first. For each exercise below, predict the result before running the example, sketch the invariant on paper, then implement a change and add a test that would have failed before your change.

## 1. Learn the containers

Start with [sequences](../src/stl/sequences.cpp) and [associative containers](../src/stl/associative.cpp). Replace one frequency counter with a fixed array and write down the assumptions on the keys that make the change safe. In the text example, identify the object that owns the memory behind the view.

## 2. Find a boundary

Trace [`first_true`](../include/cp/algorithms/binary_search.hpp) on a predicate with four false values followed by two true values. Test all-true and all-false predicates. Explain why the search still terminates when there is only one candidate. Use the lower and upper boundaries to count duplicates in a sorted array.

## 3. Compare range-query approaches

Implement [Checkpoint Scores](../problems/checkpoint-scores.md) with a loop over each query first. Replace the loop with [prefix sums](../include/cp/algorithms/prefix_sums.hpp) and verify both versions agree on small arrays with negative values. Then allow one score to change between queries and trace which nodes the [Fenwick Tree](../include/cp/data_structures/fenwick_tree.hpp) updates.

## 4. Trace a graph search

Run [the core demo](../src/examples/algorithms.cpp) and draw a graph with a cycle and a disconnected vertex. Record the queue or stack after each [BFS or DFS](../include/cp/algorithms/graph_traversal.hpp) step. Add weights and trace [Dijkstra](../include/cp/algorithms/dijkstra.hpp). Find a graph where the fewest-hop path costs more than a longer path and add it as a test.

## 5. Explain the implementation back

Read the [merge sort and sieve notes](algorithms/README.md) and [DSU guide](data-structures/README.md). Write one explanation in your own words and hand-trace its worked example. For merge sort, sort records with equal keys and verify their original order. For DSU, predict the component count after a duplicate union.

Use the [algorithm tests](../tests/algorithm_tests.cpp) to see how each contract is checked. Being able to explain the invariant, implement it from memory, and devise a failing input for a broken version is the goal of these exercises.
