# Contributing

Open a focused change from a feature branch. Describe the contest use case, the invariant that makes the algorithm work, and the inputs the implementation supports.

- Put reusable C++ in `include/cp/`, runnable lessons in `src/`, and problem solutions in `problems/`. Use standard headers and keep the public interface small.
- Document index bases, interval conventions, invalid inputs, overflow policy, and time and space costs. Include a worked example for each new algorithm.
- Add deterministic tests for the behavior you introduce. Exercise edge cases and run the full suite with `ctest --test-dir build -C Release --output-on-failure` after building.
- Record the source and redistribution terms for any third-party material you propose. Keep the existing `STL.cpp` separate from new work while its provenance is reviewed.

See the [build instructions](docs/getting-started.md) for compiler requirements and commands.
