# C++23 feature detection

The build requires C++23 mode. The core examples use widely supported
facilities such as `std::span`, range algorithms, and structured bindings.
Newer facilities depend on the standard library shipped with the compiler
toolchain. A compiler accepting C++23 mode can still have partial library
support. See the [build instructions](getting-started.md) for toolchain setup.

## Check a library feature directly

Include `<version>` and test the feature's macro. The
[ranges lesson](../src/stl/ranges.cpp) enables `std::views::chunk` when
`__cpp_lib_ranges_chunk` is at least `202202L`. The example otherwise
uses `std::span::subspan`. Both paths print identical batches, including
the short final batch. The feature-test macro is specified in the
[`<version>` synopsis of the C++23 working draft](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf).

This technique also applies to C++23 additions such as `std::expected`,
`std::print`, and `std::views::zip`. Check the compiler and library
version used by your judge before relying on a new facility.

- [GCC language status](https://gcc.gnu.org/projects/cxx-status.html)
  and [libstdc++ library status](https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html)
- [Clang language status](https://clang.llvm.org/cxx_status.html)
  and [libc++ C++23 library status](https://libcxx.llvm.org/Status/Cxx23.html)
- [Microsoft C++ language and library conformance](https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance)
