# QtTutorialCpp — C++23 Tutorial Series

A hands-on C++23 tutorial repository. It goes from the fundamentals (types, control flow, functions) through object-oriented programming, memory management, templates and the STL to modern C++ (lambdas, threads, coroutines, concepts, ranges), design patterns, software architecture, system programming, code quality and the build toolchain.

---

## Purpose

- **Learn by running code.** Every topic is a small, self-contained `main.cpp` that you can build and run. The program prints what happens; the explanation lives in the `Readme.md` next to it.
- **One idea per example.** Examples are kept short and focused, so you can read the code and the output side by side.
- **Modern style.** The code is written for C++23: `std::println` for output, `std::` written out for every standard name, smart pointers and RAII instead of manual memory handling.
- **Honest documentation.** Each example README lists what was built and run, and what was not tested.
- **For self-study and teaching.** The topics are ordered so they can be read from the first folder to the last, but each folder can also be used on its own.

---

## Environment

| Item | Value |
|------|-------|
| Language standard | C++23 |
| Compiler | GCC 15.2.0 |
| Build system | CMake (or a single `g++` command) |
| Platform | Linux |
| IDE | any; Qt Creator project files (CMake) are included |

`std::println` and `std::print` need `<print>`, which is available from **GCC 14**. The examples are written and checked against GCC 15.2.0.

---

## Build and run

### Plain compiler (works for almost every example)

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o example
./example
```

Examples that use threads need `-pthread`. A few examples need extra libraries; their `Readme.md` says which.

### CMake

Most folders contain a `CMakeLists.txt` that sets C++23. It was created from an IDE wizard, so it also asks CMake to find Qt Core. Only a few examples really use Qt; for all others the plain compiler command above is enough.

Minimal `CMakeLists.txt` without Qt:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MyProject main.cpp)
```

Configure and build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/MyProject
```

For C++23 in an IDE, add `-DCMAKE_CXX_STANDARD:STRING=23` and `-DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON` to the kit's CMake configuration, and select GCC 15.2.0 as the kit's C and C++ compiler.

### Special cases

- **Qt examples** (for example the `QThread` one) need Qt Core, and `QtConcurrent` where stated. If a `main.cpp` includes `"main.moc"`, that include must be the very last line of the file.
- **Benchmark example:** needs the Benchmark library (`libbenchmark-dev` on Debian-based systems). Build in **Release** mode, because Debug timings are meaningless:

  ```bash
  g++ -std=c++23 -O3 main.cpp -lbenchmark -lpthread -o bench && ./bench
  ```

- **`<stacktrace>`:** if the linker reports undefined references, add `-lstdc++exp` to the link command.

---

## Common issues

| Problem | Likely cause | Fix |
|-------|-------|-----|
| `<print>` not found | compiler older than GCC 14 | use GCC 14 or newer, or check which compiler the IDE kit uses |
| `std::println` cannot format an argument | the type has no formatter, for example a `volatile` variable | print a copy: `static_cast<int>(value)` |
| `undefined reference` to `std::stacktrace` functions | missing library | add `-lstdc++exp` |
| `undefined reference to benchmark::...` | benchmark library not linked | add `-lbenchmark -lpthread` after the source file |
| false red errors in the editor | the code model uses other headers than the compiler | point the code model at the same GCC, or disable it |
| `cmake_check_build_system` error after moving a project | stale build files | delete the build folder and configure again |
| `CMAKE_CXX_STANDARD 17` in a new project | the IDE template still uses C++17 | change it to 23 in the project's `CMakeLists.txt` |

---

## Learning path

```
Beginner:     01_Fundamentals → 02_OOP → 03_MemoryManagement
Intermediate: 04_Templates → 05_STL → 06_ModernCpp
Advanced:     07_DesignPatterns → 08_ModernCppDesignPatterns
Professional: 09_SoftwareArchitecture → 10_SystemProgramming → 11_CodeQualityPractices → 12_BuildAndToolchain
```

---

## License

Copyright © 2024–2026 **Konstantinos Angeloulis**

This tutorial series is licensed under the
**[Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License (CC BY-NC-ND 4.0)](https://creativecommons.org/licenses/by-nc-nd/4.0/)**.

You may read and share this material with attribution, but you may **not** use it
commercially or publish modified versions of it.

See [LICENSE.md](LICENSE.md) for full terms.
