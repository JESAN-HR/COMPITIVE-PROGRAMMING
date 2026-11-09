# Development roadmap

The foundation includes CMake and CTest, five STL lessons, core search and graph algorithms, DSU and Fenwick Tree, a contest starter, and the Checkpoint Scores practice problem.

1. Review the legacy `STL.cpp` source and redistribution terms before deciding how to handle it on the public branch. Review rights for the full tree before choosing a repository-wide license.
2. Verify the Linux GCC and Windows MSVC jobs after workflow changes reach GitHub. Record platform-specific fixes alongside the tests that motivated them.
3. Work through the [learning guide](learning-guide.md). Add a practice exercise that requires Fenwick updates and compare it with a simple array-based solution on small inputs.
4. Extend the toolkit when a verified exercise motivates a module, such as coordinate compression for sparse Fenwick indices or a segment tree for range minima. Add a worked example and tests with the module.
