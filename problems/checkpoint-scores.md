# CP-001 Checkpoint Scores

This is an original practice problem for this repository. The [reference solution](checkpoint_scores.cpp) uses the toolkit's [`cp::PrefixSums`](../include/cp/algorithms/prefix_sums.hpp).

## Task

A runner records a signed score at each of `n` checkpoints. For every query, print the total score from checkpoint `l` through checkpoint `r`, inclusive. Checkpoints are numbered from 1.

The input starts with `n q`, followed by `n` scores and then `q` pairs `l r`. Separate values with whitespace. Write each value using decimal digits and a leading minus sign if needed.

- `0 ≤ n, q ≤ 200000`. If `n` is zero, `q` must also be zero.
- Each score is between −1,000,000,000 and 1,000,000,000, inclusive.
- Each query satisfies `1 ≤ l ≤ r ≤ n`.

Print one integer on its own line for each query. With zero queries, print nothing. The program exits with status 1 and produces empty output for malformed input or a value outside the constraints.

### Sample input

```text
5 4
4 -2 7 0 -3
1 3
2 5
4 4
1 5
```

### Sample output

```text
9
2
0
6
```

## Solution

Define `P[k]` as the sum of the first `k` scores. Begin with `P[0] = 0`. Appending a checkpoint adds its score to the previous prefix, so after each step `P[k]` still equals the sum of exactly the first `k` scores. Then `P[r] - P[l - 1]` cancels the first `l - 1` scores and leaves precisely checkpoints `l` through `r`.

The sample's conceptual prefix table is `[0, 4, 2, 9, 9, 6]`. Query `2 5` evaluates to `P[5] - P[1] = 6 - 4 = 2`. The C++ call is `sums.sum(1, 5)` because the toolkit uses zero-based half-open ranges. Passing `r - 1` as the end would drop the final checkpoint.

Construction takes `O(n)` time and each query takes `O(1)` after parsing. The solution uses `O(n + q)` space because it stores the scores and prefix table, and buffers answers until input validation finishes. Any prefix or query has magnitude at most `200000 × 1000000000 = 200000000000000`, which fits in a signed 64-bit integer.

## Verification

Build the `problem_checkpoint_scores` target and feed it the sample input. With the Linux build directory from the build guide, run `./build/problem_checkpoint_scores < tests/cases/checkpoint_sample.in`. With the Visual Studio build, run `Get-Content tests/cases/checkpoint_sample.in | .\build-msvc\Release\problem_checkpoint_scores.exe` from PowerShell. [Build guide](../docs/getting-started.md)

CTest checks these cases automatically.

| Case | Expected result |
| --- | --- |
| [Sample](../tests/cases/checkpoint_sample.in) | Four inclusive ranges, including a single checkpoint and the full range |
| [Empty](../tests/cases/checkpoint_empty.in) | Success with empty output |
| [Single](../tests/cases/checkpoint_single.in) | `-1000000000` for the only checkpoint |
| [Wide](../tests/cases/checkpoint_wide.in) | Positive and negative totals beyond the signed 32-bit range |
| [Malformed](../tests/cases/checkpoint_invalid.in) | Status 1 and empty output even though the first query was valid |
