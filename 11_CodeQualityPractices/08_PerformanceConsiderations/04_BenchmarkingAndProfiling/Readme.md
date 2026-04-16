# Benchmarking and Profiling in C++

> *"You cannot optimize what you do not measure."* — Anonymous
>
> *"Measurement is the first step that leads to control and eventually to improvement."* — H. James Harrington

---

## What is Benchmarking?

Systematic measurement of code performance to:
- Compare different implementations
- Track performance over time
- Validate optimization improvements
- Set performance budgets

## What is Profiling?

Analysis of program execution to identify:
- Performance bottlenecks
- Memory usage patterns
- CPU-intensive functions
- Cache behavior

---

## Step 1 — Install Google Benchmark

This file requires `#include <benchmark/benchmark.h>`.

```bash
sudo apt-get install libbenchmark-dev
```

Verify:
```bash
ls /usr/include/benchmark/benchmark.h   # should exist
ls /usr/lib/x86_64-linux-gnu/libbenchmark*
```

---

## Step 2 — Compile and Run

### Terminal (simplest)

```bash
# Navigate to the file
cd /path/to/BenchmarkingAndProfiling

# Compile — MUST include -lbenchmark -lpthread
g++ -std=c++23 -O2 benchmarking_and_profiling.cpp -lbenchmark -lpthread -o benchmarking_and_profiling

# Run
./benchmarking_and_profiling
```

For maximum accuracy use `-O3 -march=native`:
```bash
g++ -std=c++23 -O3 -march=native benchmarking_and_profiling.cpp -lbenchmark -lpthread -o benchmarking_and_profiling && ./benchmarking_and_profiling
```

> **Common mistake:** forgetting `-lbenchmark -lpthread` causes:
> `undefined reference to benchmark::State::StartKeepRunning()`
> Always put the flags **after** the source file.

---

## Step 3 — Compile in VS Code

### Option A — Integrated Terminal (easiest)

1. Open the folder in VS Code
2. Open terminal: **Ctrl + `** (backtick)
3. Run the compile command directly:

```bash
g++ -std=c++23 -O3 -march=native benchmarking_and_profiling.cpp \
    -lbenchmark -lpthread \
    -o benchmarking_and_profiling && \
./benchmarking_and_profiling
```

---

### Option B — tasks.json (Build Task)

Create `.vscode/tasks.json` in your project folder:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Benchmark",
            "type": "shell",
            "command": "g++",
            "args": [
                "-std=c++23",
                "-O3",
                "-march=native",
                "${file}",
                "-lbenchmark",
                "-lpthread",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"],
            "detail": "Compile with Google Benchmark"
        },
        {
            "label": "Build and Run Benchmark",
            "type": "shell",
            "command": "bash",
            "args": [
                "-c",
                "g++ -std=c++23 -O3 -march=native ${file} -lbenchmark -lpthread -o ${fileDirname}/${fileBasenameNoExtension} && ${fileDirname}/${fileBasenameNoExtension}"
            ],
            "group": "build",
            "problemMatcher": ["$gcc"],
            "detail": "Compile and run benchmark"
        }
    ]
}
```

**How to use:**
- Build only: **Ctrl + Shift + B**
- Build and run: **Ctrl + Shift + P** → `Tasks: Run Task` → `Build and Run Benchmark`

---

### Option C — launch.json (Debug/Run with F5)

Create `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Run Benchmark",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "preLaunchTask": "Build Benchmark",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

**How to use:** Press **F5** — it compiles then runs automatically.

---

### Option D — CMakeLists.txt (Qt Creator or VS Code with CMake Tools)

```cmake
cmake_minimum_required(VERSION 3.14)
project(04_BenchmarkingAndProfiling LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Benchmark — install first: sudo apt-get install libbenchmark-dev
find_package(benchmark REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(04_BenchmarkingAndProfiling
    main.cpp
)

target_link_libraries(04_BenchmarkingAndProfiling
    Qt${QT_VERSION_MAJOR}::Core
    benchmark::benchmark
    pthread
    stdc++exp
)

include(GNUInstallDirs)
install(TARGETS 04_BenchmarkingAndProfiling
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
```

In VS Code with CMake Tools extension:
1. **Ctrl + Shift + P** → `CMake: Configure`
2. Select **Release** kit (not Debug!)
3. **Ctrl + Shift + P** → `CMake: Build`
4. **Ctrl + Shift + P** → `CMake: Run Without Debugging`

> **Important in Qt Creator:** Projects → Build & Run → Build Configuration → **Release**

---

## Running with options

```bash
# Run all benchmarks
./benchmarking_and_profiling

# Filter — run only vector benchmarks
./benchmarking_and_profiling --benchmark_filter=Vector

# Filter — run only sort benchmarks
./benchmarking_and_profiling --benchmark_filter=Sort

# 10 repetitions for statistical confidence
./benchmarking_and_profiling --benchmark_repetitions=10

# Save results to JSON
./benchmarking_and_profiling --benchmark_out=results.json --benchmark_out_format=json

# Save results to CSV
./benchmarking_and_profiling --benchmark_out=results.csv --benchmark_out_format=csv
```

---

## Examples covered

| # | Example | What it measures |
|---|---------|-----------------|
| 1 | Basic benchmarking | `vector::push_back` with/without `reserve` |
| 2 | Comparing algorithms | Bubble sort O(n²) vs STL sort O(n log n) |
| 3 | String operations | Concatenation with/without `reserve` |
| 4 | Data structure ops | `vector` vs `unordered_map` insert/search |
| 5 | Cache effects | Sequential vs random memory access |
| 6 | Function call overhead | Regular vs inline vs lambda |
| 7 | Memory allocation | Heap (`new`) vs stack |
| 8 | Custom arguments | Matrix multiplication at different sizes |
| 9 | Fixtures | Reusable setup/teardown |
| 10 | Real-world | JSON-like string parsing |

---

## Google Benchmark API reference

```cpp
// Prevent compiler from optimizing away a value
benchmark::DoNotOptimize(value);

// Flush pending memory writes
benchmark::ClobberMemory();

// Pause/resume timing inside the loop (for setup code)
state.PauseTiming();
doSetup();
state.ResumeTiming();

// Set input size for automatic Big-O analysis
state.SetComplexityN(state.range(0));

// Report throughput (shows MB/s or GB/s in output)
state.SetBytesProcessed(state.iterations() * bytes);
```

---

## Profiling tools

| Tool | Install | Use |
|------|---------|-----|
| `perf` | `sudo apt install linux-tools-generic` | `perf stat ./app` |
| `valgrind` | `sudo apt install valgrind` | `valgrind --tool=cachegrind ./app` |
| `gprof` | built into GCC | `g++ -pg ... && ./app && gprof app gmon.out` |
| `heaptrack` | `sudo apt install heaptrack` | `heaptrack ./app` |

---

## Best practices

```
✅ Always compile with -O2 or -O3 (Release mode)
✅ Use benchmark::DoNotOptimize() — prevents dead code elimination
✅ Use PauseTiming()/ResumeTiming() for setup inside the loop
✅ Run --benchmark_repetitions=10 for statistical confidence
✅ Close other applications — reduces CPU noise
✅ Disable CPU frequency scaling:
      sudo cpupower frequency-set --governor performance
✅ Compare relative results — absolute ns vary per machine
```

---

## Common mistakes

```bash
# WRONG — missing -lbenchmark -lpthread
g++ -std=c++23 main.cpp -o app
# Error: undefined reference to benchmark::State::StartKeepRunning()

# CORRECT
g++ -std=c++23 -O2 main.cpp -lbenchmark -lpthread -o app

# WRONG — Debug build (no optimizations)
g++ -g main.cpp -lbenchmark -lpthread -o app
# Results are meaningless — 5-10x slower than real code

# CORRECT — Release build
g++ -O3 -march=native main.cpp -lbenchmark -lpthread -o app
```
