# CMake — Build System

## What is CMake?

CMake is a **cross-platform build system generator**. It does not compile
your code directly — it generates the build files (Makefiles, Ninja files,
Visual Studio projects) that then do the actual compilation.

---

## CMake workflow

```mermaid
flowchart LR
    src["Source code\n.cpp / .h"]
    cmake["CMakeLists.txt"]
    config["cmake ..\n(configure)"]
    build["cmake --build .\nor make / ninja"]
    exe["Executable\nor library"]

    src --> config
    cmake --> config
    config --> gen["Generated build files\nMakefile / build.ninja\nVS project"]
    gen --> build
    build --> exe

    style exe fill:#267326,color:#fff
    style gen fill:#2a6099,color:#fff
```

---

## Out-of-source build

```mermaid
flowchart TD
    subgraph Source ["Source directory"]
        CMakeLists[CMakeLists.txt]
        maincpp[main.cpp]
        header[mylibrary.h]
    end

    subgraph Build ["build/ directory (generated)"]
        MF[Makefile]
        obj[main.cpp.o]
        exe2[MyProject]
        cache[CMakeCache.txt]
    end

    CMakeLists --> Build
    maincpp --> obj
    obj --> exe2

    note["Source stays clean\nDelete build/ to start fresh"]
    style note fill:#2a2a2a,color:#aaa
```

```bash
mkdir build && cd build
cmake ..        # configure — reads CMakeLists.txt, generates Makefiles
cmake --build . # compile
```

---

## CMakeLists.txt structure

```mermaid
flowchart TD
    A["cmake_minimum_required(VERSION 3.14)"]
    B["project(MyProject VERSION 1.0 LANGUAGES CXX)"]
    C["set(CMAKE_CXX_STANDARD 23)"]
    D["find_package(Qt6 REQUIRED COMPONENTS Core)"]
    E["add_executable(MyProject main.cpp)"]
    F["target_link_libraries(MyProject Qt6::Core stdc++exp)"]
    G["install(TARGETS MyProject ...)"]

    A --> B --> C --> D --> E --> F --> G
```

---

## Key CMake commands

### project()

```cmake
project(MyProject
    VERSION     1.0.0
    DESCRIPTION "My C++ project"
    LANGUAGES   CXX
)
# Sets: PROJECT_NAME, PROJECT_VERSION, PROJECT_VERSION_MAJOR/MINOR/PATCH
```

### add_executable / add_library

```cmake
add_executable(MyApp main.cpp utils.cpp)      # builds an executable
add_library(MyLib STATIC mylibrary.cpp)        # builds libMyLib.a
add_library(MyLib SHARED mylibrary.cpp)        # builds libMyLib.so
add_library(MyLib INTERFACE)                   # header-only library
```

### target_link_libraries

```cmake
target_link_libraries(MyApp
    PRIVATE MyLib        # MyLib used by MyApp only
    PUBLIC  Qt6::Core    # Qt6::Core also exposed to users of MyApp
)
```

### find_package

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(benchmark REQUIRED)
find_package(OpenSSL REQUIRED)

# After find_package — use imported targets
target_link_libraries(MyApp Qt6::Core Qt6::Widgets)
```

### Variables and options

```cmake
set(MY_VAR "hello")                     # set a variable
option(ENABLE_TESTS "Build tests" ON)   # user-configurable ON/OFF

# Use with cmake -DENABLE_TESTS=OFF ..
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()
```

### message() — print during configuration

```cmake
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(WARNING "OpenSSL not found — disabling TLS")
message(FATAL_ERROR "Qt6 is required")
```

---

## Build types

```mermaid
flowchart LR
    cmake["cmake -DCMAKE_BUILD_TYPE=..."]
    cmake --> D["Debug\n-g -O0\nFull debug symbols\nNo optimization"]
    cmake --> R["Release\n-O3\nFull optimization\nNo debug symbols"]
    cmake --> RD["RelWithDebInfo\n-O2 -g\nOptimized + debug info"]
    cmake --> MS["MinSizeRel\n-Os\nMinimum binary size"]

    style D fill:#8B0000,color:#fff
    style R fill:#267326,color:#fff
    style RD fill:#2a6099,color:#fff
```

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

In Qt Creator: **Projects → Build → Build Configuration → Release/Debug**

---

## Modern CMake — target-based approach

```mermaid
flowchart LR
    subgraph Old ["Old CMake — directory-based"]
        include_directories --> everyone["Affects ALL targets\nglobal pollution"]
        link_libraries --> everyone
    end

    subgraph Modern ["Modern CMake — target-based"]
        target_include_directories --> specific["Affects ONLY\nthe named target"]
        target_link_libraries --> specific
        target_compile_definitions --> specific
    end
```

```cmake
# OLD — avoid this
include_directories(include/)
link_libraries(mylib)

# MODERN — always do this
target_include_directories(MyApp PRIVATE include/)
target_link_libraries(MyApp PRIVATE mylib)
target_compile_definitions(MyApp PRIVATE DEBUG_MODE)
```

---

## PRIVATE / PUBLIC / INTERFACE

```mermaid
flowchart LR
    subgraph Lib ["Library target"]
        L[MyLib]
    end
    subgraph App ["App target"]
        A[MyApp]
    end
    subgraph User ["Users of MyApp"]
        U[ThirdParty]
    end

    L --"PRIVATE\nonly MyLib uses it"--> L
    L --"PUBLIC\nMyLib + anyone linking MyLib"--> A
    L --"INTERFACE\nonly propagated, not used by MyLib itself"--> U
```

---

## Useful CMake variables

| Variable | Value |
|----------|-------|
| `CMAKE_BUILD_TYPE` | Debug / Release / RelWithDebInfo |
| `CMAKE_CXX_STANDARD` | 11 / 14 / 17 / 20 / 23 |
| `CMAKE_CXX_COMPILER` | Path to compiler |
| `CMAKE_INSTALL_PREFIX` | Installation root (default `/usr/local`) |
| `PROJECT_NAME` | Set by `project()` |
| `PROJECT_VERSION` | Set by `project(VERSION ...)` |
| `CMAKE_CURRENT_SOURCE_DIR` | Current CMakeLists.txt directory |
| `CMAKE_BINARY_DIR` | Build directory |

---

## Common terminal workflow

```bash
# Configure (create build directory)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Build with parallel jobs (faster)
cmake --build build -j$(nproc)

# Install
cmake --install build

# Clean
cmake --build build --target clean

# List all targets
cmake --build build --target help
```

---

## When does CMake regenerate?

```mermaid
flowchart TD
    change{What changed?}
    change -->|CMakeLists.txt edited| regen[CMake regenerates\nbuild files automatically]
    change -->|.cpp / .h edited| rebuild[Just recompile\nchanged files]
    change -->|New file added| update[Must add to CMakeLists.txt\nthen regenerate]
```

In Qt Creator — after editing `CMakeLists.txt`:
**Build → Clear CMake Configuration → Run CMake**

---

## When to use CMake

✅ Any C++ project with more than one source file
✅ Cross-platform builds (Linux / Windows / macOS)
✅ Managing dependencies (`find_package`)
✅ Qt projects — Qt Creator generates CMakeLists.txt automatically
✅ CI/CD pipelines — deterministic build configuration

❌ Single-file programs → `g++ main.cpp -o app` is enough
