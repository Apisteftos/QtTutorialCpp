#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <unordered_map>
#include <benchmark/benchmark.h>

using namespace std;

// ============================================
// BENCHMARKING AND PROFILING
// ============================================

/*
BENCHMARKING & PROFILING:
    "You cannot optimize what you do not measure."
    — Anonymous

    "Measurement is the first step that leads to control
     and eventually to improvement."
    — H. James Harrington

WHAT IS BENCHMARKING?
    Systematic measurement of code performance to:
    ✅ Compare different implementations
    ✅ Track performance over time
    ✅ Validate optimization improvements
    ✅ Set performance budgets

WHAT IS PROFILING?
    Analysis of program execution to identify:
    ✅ Performance bottlenecks
    ✅ Memory usage patterns
    ✅ CPU-intensive functions
    ✅ Cache behavior

GOOGLE BENCHMARK:
    Industry-standard C++ micro-benchmarking library
    ✅ Accurate timing measurements
    ✅ Statistical analysis
    ✅ Automatic iteration counting
    ✅ Comparison reports
*/

// ============================================
// EXAMPLE 1: BASIC BENCHMARKING
// ============================================

/*
BASIC BENCHMARK STRUCTURE:
    - Use BENCHMARK macro
    - Access benchmark::State
    - Loop with state.KeepRunning()
    - Let library handle timing
*/

// Function to benchmark: Vector push_back without reserve
static void BM_VectorPushBack_NoReserve(benchmark::State& state) {
    // This loop runs many times to get accurate measurements
    for (auto _ : state) {
        vector<int> vec;
        for (int i = 0; i < state.range(0); i++) {
            vec.push_back(i);
        }
        // Prevent compiler from optimizing away the vector
        benchmark::DoNotOptimize(vec.data());
    }

    // Set complexity for automatic Big-O calculation
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_VectorPushBack_NoReserve)->Range(1<<10, 1<<18)->Complexity();

// Function to benchmark: Vector push_back with reserve
static void BM_VectorPushBack_WithReserve(benchmark::State& state) {
    for (auto _ : state) {
        vector<int> vec;
        vec.reserve(state.range(0));  // Reserve capacity upfront
        for (int i = 0; i < state.range(0); i++) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec.data());
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_VectorPushBack_WithReserve)->Range(1<<10, 1<<18)->Complexity();

// ============================================
// EXAMPLE 2: COMPARING ALGORITHMS
// ============================================

/*
COMPARING DIFFERENT ALGORITHMS:
    - Bubble sort vs Quick sort
    - Linear search vs Binary search
    - Different data structures
*/

// Bubble Sort - O(n²)
static void BM_BubbleSort(benchmark::State& state) {
    vector<int> data(state.range(0));

    for (auto _ : state) {
        // Setup (not timed)
        state.PauseTiming();
        iota(data.begin(), data.end(), 0);
        shuffle(data.begin(), data.end(), mt19937{random_device{}()});
        state.ResumeTiming();

        // Actual benchmark (timed)
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = 0; j < data.size() - i - 1; j++) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                }
            }
        }

        benchmark::DoNotOptimize(data.data());
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_BubbleSort)->RangeMultiplier(2)->Range(64, 2048)->Complexity(benchmark::oNSquared);

// STL Sort - O(n log n)
static void BM_STLSort(benchmark::State& state) {
    vector<int> data(state.range(0));

    for (auto _ : state) {
        state.PauseTiming();
        iota(data.begin(), data.end(), 0);
        shuffle(data.begin(), data.end(), mt19937{random_device{}()});
        state.ResumeTiming();

        sort(data.begin(), data.end());

        benchmark::DoNotOptimize(data.data());
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_STLSort)->RangeMultiplier(2)->Range(64, 2048)->Complexity(benchmark::oNLogN);

// ============================================
// EXAMPLE 3: STRING OPERATIONS
// ============================================

/*
STRING BENCHMARKING:
    - Concatenation methods
    - Search operations
    - Parsing techniques
*/

// String concatenation without reserve
static void BM_StringConcat_NoReserve(benchmark::State& state) {
    const string fragment = "text";

    for (auto _ : state) {
        string result;
        for (int i = 0; i < state.range(0); i++) {
            result += fragment;
        }
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(BM_StringConcat_NoReserve)->Range(8, 8<<10);

// String concatenation with reserve
static void BM_StringConcat_WithReserve(benchmark::State& state) {
    const string fragment = "text";

    for (auto _ : state) {
        string result;
        result.reserve(state.range(0) * fragment.size());
        for (int i = 0; i < state.range(0); i++) {
            result += fragment;
        }
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(BM_StringConcat_WithReserve)->Range(8, 8<<10);

// ============================================
// EXAMPLE 4: DATA STRUCTURE OPERATIONS
// ============================================

/*
DATA STRUCTURE BENCHMARKING:
    - Insert performance
    - Lookup performance
    - Iteration performance
*/

// Vector insert at back
static void BM_VectorInsertBack(benchmark::State& state) {
    for (auto _ : state) {
        vector<int> vec;
        vec.reserve(state.range(0));
        for (int i = 0; i < state.range(0); i++) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec.data());
    }
}
BENCHMARK(BM_VectorInsertBack)->Range(1<<10, 1<<15);

// Unordered_map insert
static void BM_UnorderedMapInsert(benchmark::State& state) {
    for (auto _ : state) {
        unordered_map<int, int> map;
        map.reserve(state.range(0));
        for (int i = 0; i < state.range(0); i++) {
            map[i] = i;
        }
        benchmark::DoNotOptimize(map.begin()->second);
    }
}
BENCHMARK(BM_UnorderedMapInsert)->Range(1<<10, 1<<15);

// Vector search (linear)
static void BM_VectorSearch(benchmark::State& state) {
    vector<int> vec(state.range(0));
    iota(vec.begin(), vec.end(), 0);

    for (auto _ : state) {
        auto it = find(vec.begin(), vec.end(), state.range(0) / 2);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(BM_VectorSearch)->Range(1<<10, 1<<15);

// Unordered_map search (hash lookup)
static void BM_UnorderedMapSearch(benchmark::State& state) {
    unordered_map<int, int> map;
    for (int i = 0; i < state.range(0); i++) {
        map[i] = i;
    }

    for (auto _ : state) {
        auto it = map.find(state.range(0) / 2);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(BM_UnorderedMapSearch)->Range(1<<10, 1<<15);

// ============================================
// EXAMPLE 5: CACHE EFFECTS
// ============================================

/*
CACHE-FRIENDLY VS CACHE-UNFRIENDLY:
    Demonstrate impact of memory access patterns
*/

// Sequential memory access (cache-friendly)
static void BM_SequentialAccess(benchmark::State& state) {
    vector<int> data(state.range(0));
    iota(data.begin(), data.end(), 0);

    for (auto _ : state) {
        long long sum = 0;
        for (size_t i = 0; i < data.size(); i++) {
            sum += data[i];
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(int));
}
BENCHMARK(BM_SequentialAccess)->Range(1<<10, 1<<20);

// Random memory access (cache-unfriendly)
static void BM_RandomAccess(benchmark::State& state) {
    vector<int> data(state.range(0));
    iota(data.begin(), data.end(), 0);

    // Create random indices
    vector<size_t> indices(state.range(0));
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), mt19937{42});

    for (auto _ : state) {
        long long sum = 0;
        for (size_t idx : indices) {
            sum += data[idx];
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(int));
}
BENCHMARK(BM_RandomAccess)->Range(1<<10, 1<<20);

// ============================================
// EXAMPLE 6: FUNCTION CALL OVERHEAD
// ============================================

/*
INLINE VS FUNCTION CALL:
    Measure function call overhead
*/

// Regular function call
int square(int x) {
    return x * x;
}

static void BM_FunctionCall(benchmark::State& state) {
    int sum = 0;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            sum += square(i);
        }
    }
    benchmark::DoNotOptimize(sum);
}
BENCHMARK(BM_FunctionCall)->Range(1<<10, 1<<16);

// Inline function
inline int square_inline(int x) {
    return x * x;
}

static void BM_InlineFunction(benchmark::State& state) {
    int sum = 0;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            sum += square_inline(i);
        }
    }
    benchmark::DoNotOptimize(sum);
}
BENCHMARK(BM_InlineFunction)->Range(1<<10, 1<<16);

// Lambda (definitely inlined)
static void BM_Lambda(benchmark::State& state) {
    auto square_lambda = [](int x) { return x * x; };
    int sum = 0;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            sum += square_lambda(i);
        }
    }
    benchmark::DoNotOptimize(sum);
}
BENCHMARK(BM_Lambda)->Range(1<<10, 1<<16);

// ============================================
// EXAMPLE 7: MEMORY ALLOCATION
// ============================================

/*
HEAP VS STACK ALLOCATION:
    Compare allocation strategies
*/

// Heap allocation
static void BM_HeapAllocation(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            int* ptr = new int(42);
            benchmark::DoNotOptimize(ptr);
            delete ptr;
        }
    }
}
BENCHMARK(BM_HeapAllocation)->Range(1<<8, 1<<12);

// Stack allocation
static void BM_StackAllocation(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            int value = 42;
            benchmark::DoNotOptimize(&value);
        }
    }
}
BENCHMARK(BM_StackAllocation)->Range(1<<8, 1<<12);

// ============================================
// EXAMPLE 8: CUSTOM ARGUMENTS
// ============================================

/*
BENCHMARKING WITH CUSTOM ARGUMENTS:
    Test multiple configurations
*/

// Matrix multiplication benchmark with custom dimensions
static void BM_MatrixMultiply(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(1);

    vector<vector<int>> A(rows, vector<int>(cols, 1));
    vector<vector<int>> B(cols, vector<int>(rows, 1));
    vector<vector<int>> C(rows, vector<int>(rows, 0));

    for (auto _ : state) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                int sum = 0;
                for (int k = 0; k < cols; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
        benchmark::DoNotOptimize(C.data());
    }
}
// Test with different matrix sizes
BENCHMARK(BM_MatrixMultiply)->Args({64, 64})->Args({128, 128})->Args({256, 256});

// ============================================
// EXAMPLE 9: FIXTURES FOR SETUP/TEARDOWN
// ============================================

/*
BENCHMARK FIXTURES:
    Reusable setup and teardown code
*/

class DataStructureFixture : public benchmark::Fixture {
public:
    vector<int> data;

    void SetUp(const ::benchmark::State& state) override {
        // Setup runs before each benchmark iteration
        data.resize(state.range(0));
        iota(data.begin(), data.end(), 0);
        shuffle(data.begin(), data.end(), mt19937{42});
    }

    void TearDown(const ::benchmark::State& state) override {
        // Cleanup runs after each benchmark iteration
        data.clear();
    }
};

BENCHMARK_DEFINE_F(DataStructureFixture, SortTest)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto copy = data;  // Don't time the copy
        state.ResumeTiming();

        sort(copy.begin(), copy.end());
        benchmark::DoNotOptimize(copy.data());
    }
}
BENCHMARK_REGISTER_F(DataStructureFixture, SortTest)->Range(1<<10, 1<<14);

// ============================================
// EXAMPLE 10: REAL-WORLD SCENARIO
// ============================================

/*
REAL-WORLD EXAMPLE:
    JSON parsing simulation
*/

// Simulate parsing a JSON-like structure
struct JsonValue {
    string key;
    string value;
};

static void BM_JsonParsing_String(benchmark::State& state) {
    string json_data = R"({"name":"John","age":"30","city":"New York"})";

    for (auto _ : state) {
        vector<JsonValue> parsed;
        size_t pos = 0;
        while ((pos = json_data.find(':', pos)) != string::npos) {
            // Simplified parsing
            size_t start = json_data.rfind('"', pos) + 1;
            size_t end = json_data.find('"', pos + 1);
            string value = json_data.substr(pos + 2, end - pos - 2);
            parsed.push_back({"key", value});
            pos = end;
        }
        benchmark::DoNotOptimize(parsed.data());
    }
}
BENCHMARK(BM_JsonParsing_String);

// ============================================
// MEASUREMENT BEST PRACTICES DEMO
// ============================================

/*
BEST PRACTICES:
    1. Use DoNotOptimize() to prevent optimization
    2. Use PauseTiming()/ResumeTiming() for setup
    3. Run with sufficient iterations
    4. Control CPU frequency scaling
    5. Close other applications
    6. Use multiple runs for statistical significance
*/

// Example: Proper use of DoNotOptimize
static void BM_DoNotOptimize_Demo(benchmark::State& state) {
    for (auto _ : state) {
        int result = 0;
        for (int i = 0; i < 1000; i++) {
            result += i * i;
        }
        // Prevent compiler from eliminating the loop
        benchmark::DoNotOptimize(result);

        // Also prevent compiler from eliminating memory writes
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DoNotOptimize_Demo);

// ============================================
// MAIN FUNCTION
// ============================================

int main(int argc, char** argv) {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║       BENCHMARKING & PROFILING WITH GOOGLE            ║\n";
    cout << "║                  BENCHMARK                            ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"You cannot optimize what you do not measure.\"       ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    cout << "📊 BENCHMARKING EXAMPLES:\n";
    cout << "   1. Basic vector operations\n";
    cout << "   2. Algorithm comparisons (sort)\n";
    cout << "   3. String operations\n";
    cout << "   4. Data structure operations\n";
    cout << "   5. Cache effects\n";
    cout << "   6. Function call overhead\n";
    cout << "   7. Memory allocation\n";
    cout << "   8. Custom arguments\n";
    cout << "   9. Fixtures for setup/teardown\n";
    cout << "   10. Real-world scenarios\n\n";

    cout << "⚙️  PROFILING TOOLS (use separately):\n";
    cout << "   • gprof  - Function-level profiling\n";
    cout << "   • perf   - CPU performance analysis\n";
    cout << "   • valgrind/cachegrind - Cache analysis\n";
    cout << "   • heaptrack - Memory profiling\n\n";

    cout << "💡 BEST PRACTICES:\n";
    cout << "   ✓ Use benchmark::DoNotOptimize()\n";
    cout << "   ✓ Pause timing for setup code\n";
    cout << "   ✓ Run multiple iterations\n";
    cout << "   ✓ Control CPU frequency\n";
    cout << "   ✓ Close background apps\n";
    cout << "   ✓ Compare statistical results\n\n";

    cout << "🚀 Running benchmarks...\n";
    cout << "══════════════════════════════════════════════════════\n\n";

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    return 0;
}
