# Contest templates

Copy [contest.cpp](contest.cpp) as a starting point for a single-case problem. Initialize stream settings before any input or output. Keep I/O on C++ streams when synchronization is off. For an interactive problem, explicitly flush each response when the judge expects it.

The example reads two signed 64-bit integers and prints their sum modulo `1,000,000,007`. Run `contest_template`, enter `-2 5`, and expect `3`. For multiple test cases, read the case count and repeat the per-case logic inside a loop.

Compile the template from the repository root with `g++ -std=c++23 -Iinclude templates/contest.cpp -o contest_template`, or use the CMake target of the same name. For a single-file judge, bring the functions and standard headers you use into the submission. The template depends on [modular.hpp](../include/cp/utils/modular.hpp).

## Modular arithmetic

Pass a positive `std::uint32_t` modulus and signed 64-bit operands. A zero modulus throws `std::invalid_argument`. Normalize residues into `[0, modulus)` before adding or multiplying so even negative operands and the largest 32-bit modulus are safe. The largest possible product of residues fits in a `std::uint64_t`.

| Function | Cost | Example |
| --- | --- | --- |
| `mod_normalize` | `O(1)` | `mod_normalize(-2, 7)` gives `5` |
| `mod_add` | `O(1)` | `mod_add(-2, 5, 7)` gives `3` |
| `mod_multiply` | `O(1)` | `mod_multiply(-2, 5, 7)` gives `4` |
| `mod_power` | `O(log(exponent + 1))` for positive exponents, `O(1)` for zero | `mod_power(3, 4, 7)` gives `4` |

All four use `O(1)` extra space. `mod_power` takes an unsigned 64-bit exponent. It scans the exponent’s bits while repeatedly squaring the base. A set bit contributes the corresponding power to the running product. An exponent of zero returns `1 % modulus`, even for a zero base.
