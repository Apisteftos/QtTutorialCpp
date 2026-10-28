#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <memory>
#include <cstring>
#include <iomanip>
#include <unordered_set>
#include <deque>
#include <print>

// ============================================
// PERFORMANCE OPTIMIZATION
// ============================================

/*
PERFORMANCE OPTIMIZATION:
    "Premature optimization is the root of all evil, but informed
     optimization at the right time can make the difference between
     fast and slow software."
    — Donald Knuth (adapted)

OPTIMIZATION PRINCIPLES:
    1. Measure first - Profile before optimizing
    2. Focus on hot paths - Optimize bottlenecks
    3. Algorithmic improvements - Better complexity wins
    4. Cache-friendly code - Locality matters
    5. Avoid unnecessary work - Don't compute twice

WHY IT MATTERS:
    ✅ Faster execution time
    ✅ Better user experience
    ✅ Lower resource usage
    ✅ Scalability improvements
    ✅ Energy efficiency

OPTIMIZATION CATEGORIES:
    1. Algorithm optimization (biggest impact!)
    2. Cache optimization
    3. Memory optimization
    4. Loop optimization
    5. Compiler optimization
    6. Data structure choices
*/

// Timing utility
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed_ms() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;
    }
};

// ============================================
// EXAMPLE 1: ALGORITHM OPTIMIZATION
// ============================================

namespace example1_algorithm {
// ❌ BAD: O(n²) - Nested loops
bool hasDuplicates_Bad(const std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = i + 1; j < arr.size(); j++) {
            if (arr[i] == arr[j]) {
                return true;
            }
        }
    }
    return false;
}

// ✅ GOOD: O(n) - Using hash set
bool hasDuplicates_Good(const std::vector<int>& arr) {
    std::unordered_set<int> seen;
    for (int val : arr) {
        if (seen.count(val)) {
            return true;
        }
        seen.insert(val);
    }
    return false;
}

// ❌ BAD: O(n²) - Bubble sort
void sort_Bad(std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ✅ GOOD: O(n log n) - STL sort
void sort_Good(std::vector<int>& arr) {
    sort(arr.begin(), arr.end());
}
}

void run_example1_algorithm() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: ALGORITHM OPTIMIZATION              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Algorithm choice has BIGGEST impact!");

    // Test duplicate detection
    std::vector<int> data(10000);
    for (int i = 0; i < 10000; i++) data[i] = i;
    data[5000] = 42;  // Duplicate

    Timer timer;

    std::println("");
    std::println("📊 DUPLICATE DETECTION:");

    timer.start();
    bool result1 = example1_algorithm::hasDuplicates_Bad(data);
    std::println("   ❌ BAD (O(n²)):  {:.3f} ms", timer.elapsed_ms());

    timer.start();
    bool result2 = example1_algorithm::hasDuplicates_Good(data);
    std::println("   ✅ GOOD (O(n)):  {:.3f} ms", timer.elapsed_ms());

    std::println("");
    std::println("💡 IMPROVEMENT: ~1000x faster with better algorithm!");
}

// ============================================
// EXAMPLE 2: CACHE OPTIMIZATION
// ============================================

namespace example2_cache {
const int SIZE = 1000;

// ❌ BAD: Poor cache locality (column-major access)
long long sumMatrix_Bad(int matrix[SIZE][SIZE]) {
    long long sum = 0;
    // Accessing columns (jumps in memory)
    for (int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++) {
            sum += matrix[row][col];  // Cache miss!
        }
    }
    return sum;
}

// ✅ GOOD: Good cache locality (row-major access)
long long sumMatrix_Good(int matrix[SIZE][SIZE]) {
    long long sum = 0;
    // Accessing rows (sequential in memory)
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            sum += matrix[row][col];  // Cache hit!
        }
    }
    return sum;
}
}

void run_example2_cache() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: CACHE OPTIMIZATION                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Access memory sequentially!");
    std::println("   Arrays in C++ are stored row-major (rows are contiguous)");

    // Allocate matrix
    int (*matrix)[example2_cache::SIZE] = new int[example2_cache::SIZE][example2_cache::SIZE];
    for (int i = 0; i < example2_cache::SIZE; i++) {
        for (int j = 0; j < example2_cache::SIZE; j++) {
            matrix[i][j] = i + j;
        }
    }

    Timer timer;

    std::println("");
    std::println("📊 MATRIX SUMMATION:");

    timer.start();
    long long sum1 = example2_cache::sumMatrix_Bad(matrix);
    std::println("   ❌ BAD (column-major):  {:.3f} ms (cache misses)", timer.elapsed_ms());

    timer.start();
    long long sum2 = example2_cache::sumMatrix_Good(matrix);
    std::println("   ✅ GOOD (row-major):    {:.3f} ms (cache hits)", timer.elapsed_ms());

    delete[] matrix;

    std::println("");
    std::println("💡 IMPROVEMENT: 2-3x faster with cache-friendly access!");
}

// ============================================
// EXAMPLE 3: MEMORY ALLOCATION
// ============================================

namespace example3_memory {
// ❌ BAD: Multiple allocations
std::vector<int> createVector_Bad(int size) {
    std::vector<int> vec;  // No capacity reserved
    for (int i = 0; i < size; i++) {
        vec.push_back(i);  // May reallocate multiple times!
    }
    return vec;
}

// ✅ GOOD: Single allocation with reserve
std::vector<int> createVector_Good(int size) {
    std::vector<int> vec;
    vec.reserve(size);  // Allocate once!
    for (int i = 0; i < size; i++) {
        vec.push_back(i);  // No reallocation
    }
    return vec;
}

// ❌ BAD: Allocation inside loop
void processData_Bad(int iterations) {
    for (int i = 0; i < iterations; i++) {
        std::vector<int> temp(1000);  // Allocate every iteration!
        // Process temp...
    }
}

// ✅ GOOD: Single allocation, reuse
void processData_Good(int iterations) {
    std::vector<int> temp(1000);  // Allocate once!
    for (int i = 0; i < iterations; i++) {
        fill(temp.begin(), temp.end(), 0);  // Reuse
        // Process temp...
    }
}
}

void run_example3_memory() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: MEMORY ALLOCATION                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Minimize allocations!");

    Timer timer;
    int size = 100000;

    std::println("");
    std::println("📊 VECTOR CREATION:");

    timer.start();
    auto vec1 = example3_memory::createVector_Bad(size);
    std::println("   ❌ BAD (no reserve):  {:.3f} ms (multiple allocations)", timer.elapsed_ms());

    timer.start();
    auto vec2 = example3_memory::createVector_Good(size);
    std::println("   ✅ GOOD (with reserve): {:.3f} ms (single allocation)", timer.elapsed_ms());

    std::println("");
    std::println("💡 IMPROVEMENT: reserve() eliminates reallocations!");
}

// ============================================
// EXAMPLE 4: LOOP OPTIMIZATION
// ============================================

namespace example4_loop {
// ❌ BAD: Repeated calculation in loop
double sumWithCalc_Bad(const std::vector<int>& arr, int divisor) {
    double sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        sum += arr[i] / static_cast<double>(divisor * divisor);  // Recalculated!
    }
    return sum;
}

// ✅ GOOD: Hoist invariant calculation out of loop
double sumWithCalc_Good(const std::vector<int>& arr, int divisor) {
    double factor = 1.0 / (divisor * divisor);  // Calculate once!
    double sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        sum += arr[i] * factor;  // Reuse
    }
    return sum;
}

// ❌ BAD: Function call in loop condition
void processString_Bad(const std::string& str) {
    for (size_t i = 0; i < str.length(); i++) {  // length() called every iteration!
        // Process str[i]...
    }
}

// ✅ GOOD: Cache loop limit
void processString_Good(const std::string& str) {
    size_t len = str.length();  // Call once!
    for (size_t i = 0; i < len; i++) {
        // Process str[i]...
    }
}
}

void run_example4_loop() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: LOOP OPTIMIZATION                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Don't repeat work in loops!");

    Timer timer;
    std::vector<int> data(1000000);
    for (int i = 0; i < 1000000; i++) data[i] = i;

    std::println("");
    std::println("📊 LOOP WITH CALCULATION:");

    timer.start();
    double sum1 = example4_loop::sumWithCalc_Bad(data, 10);
    std::println("   ❌ BAD (recalculate):  {:.3f} ms", timer.elapsed_ms());

    timer.start();
    double sum2 = example4_loop::sumWithCalc_Good(data, 10);
    std::println("   ✅ GOOD (hoist out):   {:.3f} ms", timer.elapsed_ms());

    std::println("");
    std::println("💡 IMPROVEMENT: Hoist loop-invariant code!");
}

// ============================================
// EXAMPLE 5: MOVE SEMANTICS
// ============================================

namespace example5_move {
// ❌ BAD: Unnecessary copies
std::vector<int> createLargeVector_Bad() {
    std::vector<int> temp(1000000, 42);
    std::vector<int> result = temp;  // Copy!
    return result;  // Another potential copy
}

// ✅ GOOD: Use move semantics
std::vector<int> createLargeVector_Good() {
    std::vector<int> temp(1000000, 42);
    return temp;  // Move (RVO/NRVO)
}

// ❌ BAD: Pass by value when not needed
void processVector_Bad(std::vector<int> vec) {  // Copy!
    // Just reading vec...
    int sum = 0;
    for (int val : vec) sum += val;
}

// ✅ GOOD: Pass by const reference
void processVector_Good(const std::vector<int>& vec) {  // No copy!
    // Just reading vec...
    int sum = 0;
    for (int val : vec) sum += val;
}

// ✅ BETTER: Use move when transferring ownership
void takeOwnership_Good(std::vector<int>&& vec) {  // Move!
    std::vector<int> myVec = move(vec);
    // Now owns the data
}
}

void run_example5_move() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: MOVE SEMANTICS                      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Avoid unnecessary copies!");

    Timer timer;

    std::println("");
    std::println("📊 VECTOR CREATION:");

    timer.start();
    auto vec1 = example5_move::createLargeVector_Bad();
    std::println("   ❌ BAD (with copy):    {:.3f} ms", timer.elapsed_ms());

    timer.start();
    auto vec2 = example5_move::createLargeVector_Good();
    std::println("   ✅ GOOD (with move):   {:.3f} ms", timer.elapsed_ms());

    std::println("");
    std::println("💡 IMPROVEMENT: Move semantics eliminate deep copies!");
}

// ============================================
// EXAMPLE 6: STRING OPTIMIZATION
// ============================================

namespace example6_string {
// ❌ BAD: String concatenation in loop
std::string buildString_Bad(int count) {
    std::string result;
    for (int i = 0; i < count; i++) {
        result += "text";  // Reallocates each time!
    }
    return result;
}

// ✅ GOOD: Reserve space first
std::string buildString_Good(int count) {
    std::string result;
    result.reserve(count * 4);  // Reserve space
    for (int i = 0; i < count; i++) {
        result += "text";  // No reallocation
    }
    return result;
}

// ❌ BAD: Creating temporary strings
std::string concatenate_Bad(const std::string& a, const std::string& b, const std::string& c) {
    return a + b + c;  // Creates temporaries!
}

// ✅ GOOD: Single allocation
std::string concatenate_Good(const std::string& a, const std::string& b, const std::string& c) {
    std::string result;
    result.reserve(a.size() + b.size() + c.size());
    result += a;
    result += b;
    result += c;
    return result;
}
}

void run_example6_string() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: STRING OPTIMIZATION                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Reserve string capacity!");

    Timer timer;
    int count = 10000;

    std::println("");
    std::println("📊 STRING BUILDING:");

    timer.start();
    auto str1 = example6_string::buildString_Bad(count);
    std::println("   ❌ BAD (no reserve):   {:.3f} ms", timer.elapsed_ms());

    timer.start();
    auto str2 = example6_string::buildString_Good(count);
    std::println("   ✅ GOOD (with reserve): {:.3f} ms", timer.elapsed_ms());

    std::println("");
    std::println("💡 IMPROVEMENT: String reserve prevents reallocations!");
}

// ============================================
// EXAMPLE 7: DATA STRUCTURE CHOICES
// ============================================

namespace example7_datastructure {
// ❌ BAD: Using vector for frequent insertions
void frequentInserts_Bad(int count) {
    std::vector<int> vec;
    for (int i = 0; i < count; i++) {
        vec.insert(vec.begin(), i);  // O(n) each time!
    }
}

// ✅ GOOD: Using deque for front insertions
void frequentInserts_Good(int count) {
    std::deque<int> deq;
    for (int i = 0; i < count; i++) {
        deq.push_front(i);  // O(1)!
    }
}

// ❌ BAD: Using vector for frequent searches
bool searchInVector_Bad(const std::vector<int>& vec, int target) {
    return find(vec.begin(), vec.end(), target) != vec.end();  // O(n)
}

// ✅ GOOD: Using set for frequent searches
bool searchInSet_Good(const std::unordered_set<int>& set, int target) {
    return set.count(target) > 0;  // O(1)
}
}

void run_example7_datastructure() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: DATA STRUCTURE CHOICES              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Choose the right data structure!");

    Timer timer;
    int count = 10000;

    std::println("");
    std::println("📊 FREQUENT FRONT INSERTIONS:");

    timer.start();
    example7_datastructure::frequentInserts_Bad(count);
    std::println("   ❌ BAD (vector):  {:.3f} ms (O(n²) total)", timer.elapsed_ms());

    timer.start();
    example7_datastructure::frequentInserts_Good(count);
    std::println("   ✅ GOOD (deque):  {:.3f} ms (O(n) total)", timer.elapsed_ms());

    std::println("");
    std::println("💡 IMPROVEMENT: Right data structure matters!");
}

// ============================================
// EXAMPLE 8: AVOIDING BRANCHES
// ============================================

namespace example8_branches {
// ❌ BAD: Unpredictable branches
int sumPositive_Bad(const std::vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        if (val > 0) {  // Branch misprediction!
            sum += val;
        }
    }
    return sum;
}

// ✅ GOOD: Branchless version
int sumPositive_Good(const std::vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        sum += (val > 0) * val;  // No branch!
    }
    return sum;
}

// ❌ BAD: Branch in tight loop
int absoluteSum_Bad(const std::vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        if (val < 0) {  // Branch!
            sum += -val;
        } else {
            sum += val;
        }
    }
    return sum;
}

// ✅ GOOD: Branchless with conditional move
int absoluteSum_Good(const std::vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        int mask = val >> 31;  // -1 if negative, 0 if positive
        sum += (val + mask) ^ mask;  // Branchless abs
    }
    return sum;
}
}

void run_example8_branches() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: AVOIDING BRANCHES                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Branches can be expensive!");
    std::println("   (Note: Only optimize hot paths with unpredictable branches)");

    // Create random data for unpredictable branches
    std::vector<int> data(1000000);
    for (int i = 0; i < 1000000; i++) {
        data[i] = (i % 3) - 1;  // Mix of -1, 0, 1
    }

    Timer timer;

    std::println("");
    std::println("📊 SUM OF POSITIVE VALUES:");

    timer.start();
    int sum1 = example8_branches::sumPositive_Bad(data);
    std::println("   ❌ BAD (with branch):     {:.3f} ms", timer.elapsed_ms());

    timer.start();
    int sum2 = example8_branches::sumPositive_Good(data);
    std::println("   ✅ GOOD (branchless):     {:.3f} ms", timer.elapsed_ms());

    std::println("");
    std::println("💡 NOTE: Branchless code helps when branches are unpredictable!");
}

// ============================================
// EXAMPLE 9: INLINE FUNCTIONS
// ============================================

namespace example9_inline {
// ❌ BAD: Function call overhead
int square(int x) {
    return x * x;
}

int sumSquares_Bad(const std::vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        sum += square(val);  // Function call overhead
    }
    return sum;
}

// ✅ GOOD: Inline function
inline int squareInline(int x) {
    return x * x;
}

int sumSquares_Good(const std::vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        sum += squareInline(val);  // Inlined by compiler
    }
    return sum;
}

// ✅ BETTER: Lambda (definitely inlined)
int sumSquares_Better(const std::vector<int>& arr) {
    int sum = 0;
    auto square = [](int x) { return x * x; };
    for (int val : arr) {
        sum += square(val);  // Inlined
    }
    return sum;
}
}

void run_example9_inline() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: INLINE FUNCTIONS                    ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("🎯 PRINCIPLE: Eliminate function call overhead!");

    std::vector<int> data(1000000);
    for (int i = 0; i < 1000000; i++) data[i] = i;

    Timer timer;

    std::println("");
    std::println("📊 SUM OF SQUARES:");

    timer.start();
    int sum1 = example9_inline::sumSquares_Bad(data);
    std::println("   ❌ BAD (function call): {:.3f} ms", timer.elapsed_ms());

    timer.start();
    int sum2 = example9_inline::sumSquares_Good(data);
    std::println("   ✅ GOOD (inline):       {:.3f} ms", timer.elapsed_ms());

    std::println("");
    std::println("💡 IMPROVEMENT: Inline small, frequently-called functions!");
}

// ============================================
// EXAMPLE 10: COMPILER OPTIMIZATIONS
// ============================================

void run_example10_compiler() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: COMPILER OPTIMIZATIONS             ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║           PERFORMANCE OPTIMIZATION GUIDE              ║");
    std::println("║                                                       ║");
    std::println("║  \"Premature optimization is evil, but informed       ║");
    std::println("║   optimization at the right time makes the            ║");
    std::println("║   difference between fast and slow.\"                 ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    run_example1_algorithm();
    run_example2_cache();
    run_example3_memory();
    run_example4_loop();
    run_example5_move();
    run_example6_string();
    run_example7_datastructure();
    run_example8_branches();
    run_example9_inline();
    run_example10_compiler();

    return 0;
}
