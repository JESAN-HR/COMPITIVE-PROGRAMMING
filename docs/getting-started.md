# Getting started

Use CMake 3.25 or newer and a compiler with C++23 mode and a compatible standard library. CMake checks `if consteval`, `std::span`, and a ranges algorithm during configuration. The GitHub Actions workflow uses GCC 14 on Ubuntu 24.04 and MSVC from Visual Studio 2026 on Windows Server 2025.

## Linux with GCC

On Ubuntu 24.04, install `cmake` and `g++-14` with `sudo apt install cmake g++-14`. Run these commands from the repository root.

```bash
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-14 -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure --no-tests=error
```

## Windows with MSVC

Install Visual Studio 2026 with **Desktop development with C++** and CMake 4.2 or newer. Run these commands from Developer PowerShell for Visual Studio.

```powershell
cmake -S . -B build-msvc -G 'Visual Studio 18 2026' -A x64
cmake --build build-msvc --config Release --parallel
ctest --test-dir build-msvc -C Release --output-on-failure --no-tests=error
```

For Visual Studio 2022 17.14 or later, use the `Visual Studio 17 2022` generator. Visual Studio is a multi-configuration generator. Its Release executables are under `build-msvc/Release/`.

## Windows with GCC

The Windows GCC build was verified with the portable WinLibs GCC 15.2.0 UCRT bundle, which includes CMake and Ninja. After extracting it, substitute your path to `mingw64\bin` below. The PATH change applies to the current PowerShell session. [WinLibs release](https://github.com/brechtsanders/winlibs_mingw/releases/tag/15.2.0posix-14.0.0-ucrt-r7)

```powershell
$env:Path = "C:\path\to\mingw64\bin;$env:Path"
cmake -S . -B build-gcc -G Ninja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
cmake --build build-gcc --parallel
ctest --test-dir build-gcc --output-on-failure --no-tests=error
.\build-gcc\algorithms_demo.exe
```

## Troubleshooting

- Use a fresh build directory when switching compilers or generators. CMake caches those selections.
- If the C++23 check fails, inspect the `CP_HAS_REQUIRED_CXX23_SUPPORT` entry in `build/CMakeFiles/CMakeConfigureLog.yaml` (CMake 3.26+) or `build/CMakeFiles/CMakeError.log` (CMake 3.25). Check the compiler and standard library selected at configure time.
- CTest is enabled by default. Configure with `-DBUILD_TESTING=OFF` for a build of the examples and tools without test executables.

See [CMake’s C++ standard settings](https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html) and [MSVC language and library conformance](https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance?view=msvc-170).
