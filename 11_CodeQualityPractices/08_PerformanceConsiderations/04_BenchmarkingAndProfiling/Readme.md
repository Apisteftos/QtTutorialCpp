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

## Prerequisites — Installing Google Benchmark

This file requires the **Google Benchmark** library (`#include <benchmark/benchmark.h>`).

### Ubuntu / Debian (easiest)

```bash
sudo apt-get install libbenchmark-dev
```

### From source (if package not available)

```bash
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -E make_directory "build"
cmake -E chdir "build" cmake \
    -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on \
    -DCMAKE_BUILD_TYPE=Release ../
cmake --build "build" --config Release
sudo cmake --build "build" --config Release --target install
```

### Verify installation

```bash
dpkg -l | grep benchmark          # Ubuntu package
ls /usr/include/benchmark/        # should show benchmark.h
ls /usr/lib/x86_64-linux-gnu/libbenchmark*
```

---

## CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.14)
project(BenchmarkingAndProfiling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release build for accurate benchmarks
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Find Google Benchmark
find_package(benchmark REQUIRED)

add_executable(BenchmarkingAndProfiling main.cpp)

target_link_libraries(BenchmarkingAndProfiling
    benchmark::benchmark
    pthread
)
```

> **Important:** Always build in **Release** mode for benchmarks.
> Debug builds have no optimizations — numbers will be meaningless.

---

## Compilation (terminal alternative)

```bash
# Basic
g++ -std=c++17 -O2 main.cpp -lbenchmark -lpthread -o benchmark_demo

# With full optimization
g++ -std=c++17 -O3 -march=native main.cpp -lbenchmark -lpthread -o benchmark_demo
```

---

## Running the benchmarks

```bash
# Run all benchmarks
./benchmark_demo

# Run only benchmarks matching a filter
./benchmark_demo --benchmark_filter=VectorPushBack

# More detailed statistics (10 repetitions)
./benchmark_demo --benchmark_repetitions=10

# Output to JSON
./benchmark_demo --benchmark_out=results.json --benchmark_out_format=json

# Output to CSV
./benchmark_demo --benchmark_out=results.csv --benchmark_out_format=csv

# Show CPU info
./benchmark_demo --benchmark_report_aggregates_only=true
```

---

## Example output

```
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
BM_VectorPushBack_NoReserve/1024     1547 ns         1546 ns       452174
BM_VectorPushBack_NoReserve/4096     6789 ns         6787 ns       103085
BM_VectorPushBack_WithReserve/1024    892 ns          891 ns       785234
BM_VectorPushBack_WithReserve/4096   3456 ns         3455 ns       202341
BM_BubbleSort/64                     8923 ns         8921 ns        78432
BM_STLSort/64                         456 ns          455 ns      1534231
BM_SequentialAccess/1024              234 ns          233 ns      2987654
BM_RandomAccess/1024                 1823 ns         1822 ns       384521
```

---

## Examples covered in this file

| # | Example | What it measures |
|---|---------|-----------------|
| 1 | Basic benchmarking | `vector::push_back` with/without `reserve` |
| 2 | Comparing algorithms | Bubble sort O(n²) vs STL sort O(n log n) |
| 3 | String operations | Concatenation with/without `reserve` |
| 4 | Data structure ops | `vector` vs `unordered_map` insert/search |
| 5 | Cache effects | Sequential vs random memory access |
| 6 | Function call overhead | Regular vs inline vs lambda |
| 7 | Memory allocation | Heap (`new`) vs stack |
| 8 | Custom arguments | Matrix multiplication with different sizes |
| 9 | Fixtures | Reusable setup/teardown with `benchmark::Fixture` |
| 10 | Real-world | JSON-like string parsing |

---

## Google Benchmark API reference

### Basic structure

```cpp
static void BM_MyFunction(benchmark::State& state) {
    for (auto _ : state) {
        // code to benchmark goes here
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MyFunction);
```

### Key functions

```cpp
// Prevent compiler from optimizing away a value
benchmark::DoNotOptimize(value);

// Flush all pending memory writes (prevents reordering)
benchmark::ClobberMemory();

// Pause timing during setup inside the loop
state.PauseTiming();
doSetup();
state.ResumeTiming();

// Set input size for Big-O complexity analysis
state.SetComplexityN(state.range(0));

// Report bytes processed (shows MB/s)
state.SetBytesProcessed(state.iterations() * bytes);
```

### Parameterization

```cpp
// Single range (powers of 2 from 1024 to 262144)
BENCHMARK(BM_Func)->Range(1<<10, 1<<18);

// Range with custom multiplier
BENCHMARK(BM_Func)->RangeMultiplier(2)->Range(64, 2048);

// Multiple custom arguments
BENCHMARK(BM_Func)->Args({64, 64})->Args({128, 128});

// Automatic Big-O complexity analysis
BENCHMARK(BM_Func)->Range(1<<10, 1<<18)->Complexity();
BENCHMARK(BM_Func)->Complexity(benchmark::oN);       // O(n)
BENCHMARK(BM_Func)->Complexity(benchmark::oNLogN);   // O(n log n)
BENCHMARK(BM_Func)->Complexity(benchmark::oN2);      // O(n²)
```

---

## Profiling tools (used separately from the benchmark binary)

| Tool | Purpose | Command |
|------|---------|---------|
| `gprof` | Function-level call profiling | `g++ -pg ... && ./app && gprof app gmon.out` |
| `perf` | CPU performance counters | `perf stat ./app` / `perf record ./app` |
| `valgrind/cachegrind` | Cache miss analysis | `valgrind --tool=cachegrind ./app` |
| `callgrind` | Call graph profiling | `valgrind --tool=callgrind ./app` |
| `heaptrack` | Memory allocation profiling | `heaptrack ./app` |
| `perf annotate` | Source-level hotspot view | `perf annotate` |

### Quick perf usage

```bash
# Install
sudo apt-get install linux-tools-common linux-tools-generic

# Count CPU events
perf stat ./benchmark_demo

# Record call graph
perf record -g ./benchmark_demo
perf report
```

### Quick valgrind/cachegrind

```bash
valgrind --tool=cachegrind ./benchmark_demo
cg_annotate cachegrind.out.*
```

---

## Measurement best practices

```
✅ Always build in Release mode (-O2 or -O3)
✅ Use benchmark::DoNotOptimize() — prevents compiler from removing your code
✅ Use PauseTiming()/ResumeTiming() for setup inside the loop
✅ Run with --benchmark_repetitions=10 for statistical significance
✅ Close background applications — CPU contention affects results
✅ Disable CPU frequency scaling for stable results:
      sudo cpupower frequency-set --governor performance
✅ Run on the actual target hardware
✅ Compare relative results — absolute ns vary per machine
```

### Disable CPU frequency scaling

```bash
# Set performance governor (prevents CPU throttling)
sudo cpupower frequency-set --governor performance

# Restore after benchmarking
sudo cpupower frequency-set --governor powersave
```

---

## Common mistakes

```cpp
// MISTAKE 1: Not using DoNotOptimize — compiler removes "dead" code
for (auto _ : state) {
    int result = computeHeavy();
    // result never used → compiler eliminates computeHeavy()!
}

// FIX:
for (auto _ : state) {
    int result = computeHeavy();
    benchmark::DoNotOptimize(result);   // forces computation
}

// MISTAKE 2: Including setup inside the timed loop
for (auto _ : state) {
    vector<int> data(1000000);          // setup inside — times allocation too!
    sort(data.begin(), data.end());
}

// FIX: setup outside, or use PauseTiming
vector<int> data(1000000);              // setup outside loop
for (auto _ : state) {
    state.PauseTiming();
    iota(data.begin(), data.end(), 0);  // reset data — not timed
    state.ResumeTiming();
    sort(data.begin(), data.end());     // only this is timed
}

// MISTAKE 3: Benchmarking in Debug mode
// g++ -g main.cpp -lbenchmark  ← no -O2! results are useless
// Always add: -O2 or -O3
```
