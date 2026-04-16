#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <memory>
#include <cstring>
#include <iomanip>
#include <unordered_set>
#include <deque>
using namespace std;
using namespace chrono;

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
    high_resolution_clock::time_point start_time;

public:
    void start() {
        start_time = high_resolution_clock::now();
    }

    double elapsed_ms() {
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        return duration.count() / 1000.0;
    }
};

// ============================================
// EXAMPLE 1: ALGORITHM OPTIMIZATION
// ============================================

namespace example1_algorithm {
// ❌ BAD: O(n²) - Nested loops
bool hasDuplicates_Bad(const vector<int>& arr) {
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
bool hasDuplicates_Good(const vector<int>& arr) {
    unordered_set<int> seen;
    for (int val : arr) {
        if (seen.count(val)) {
            return true;
        }
        seen.insert(val);
    }
    return false;
}

// ❌ BAD: O(n²) - Bubble sort
void sort_Bad(vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ✅ GOOD: O(n log n) - STL sort
void sort_Good(vector<int>& arr) {
    sort(arr.begin(), arr.end());
}
}

void run_example1_algorithm() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: ALGORITHM OPTIMIZATION              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Algorithm choice has BIGGEST impact!\n";

    // Test duplicate detection
    vector<int> data(10000);
    for (int i = 0; i < 10000; i++) data[i] = i;
    data[5000] = 42;  // Duplicate

    Timer timer;

    cout << "\n📊 DUPLICATE DETECTION:\n";

    timer.start();
    bool result1 = example1_algorithm::hasDuplicates_Bad(data);
    cout << "   ❌ BAD (O(n²)):  " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    timer.start();
    bool result2 = example1_algorithm::hasDuplicates_Good(data);
    cout << "   ✅ GOOD (O(n)):  " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    cout << "\n💡 IMPROVEMENT: ~1000x faster with better algorithm!\n";
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: CACHE OPTIMIZATION                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Access memory sequentially!\n";
    cout << "   Arrays in C++ are stored row-major (rows are contiguous)\n";

    // Allocate matrix
    int (*matrix)[example2_cache::SIZE] = new int[example2_cache::SIZE][example2_cache::SIZE];
    for (int i = 0; i < example2_cache::SIZE; i++) {
        for (int j = 0; j < example2_cache::SIZE; j++) {
            matrix[i][j] = i + j;
        }
    }

    Timer timer;

    cout << "\n📊 MATRIX SUMMATION:\n";

    timer.start();
    long long sum1 = example2_cache::sumMatrix_Bad(matrix);
    cout << "   ❌ BAD (column-major):  " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms (cache misses)" << endl;

    timer.start();
    long long sum2 = example2_cache::sumMatrix_Good(matrix);
    cout << "   ✅ GOOD (row-major):    " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms (cache hits)" << endl;

    delete[] matrix;

    cout << "\n💡 IMPROVEMENT: 2-3x faster with cache-friendly access!\n";
}

// ============================================
// EXAMPLE 3: MEMORY ALLOCATION
// ============================================

namespace example3_memory {
// ❌ BAD: Multiple allocations
vector<int> createVector_Bad(int size) {
    vector<int> vec;  // No capacity reserved
    for (int i = 0; i < size; i++) {
        vec.push_back(i);  // May reallocate multiple times!
    }
    return vec;
}

// ✅ GOOD: Single allocation with reserve
vector<int> createVector_Good(int size) {
    vector<int> vec;
    vec.reserve(size);  // Allocate once!
    for (int i = 0; i < size; i++) {
        vec.push_back(i);  // No reallocation
    }
    return vec;
}

// ❌ BAD: Allocation inside loop
void processData_Bad(int iterations) {
    for (int i = 0; i < iterations; i++) {
        vector<int> temp(1000);  // Allocate every iteration!
        // Process temp...
    }
}

// ✅ GOOD: Single allocation, reuse
void processData_Good(int iterations) {
    vector<int> temp(1000);  // Allocate once!
    for (int i = 0; i < iterations; i++) {
        fill(temp.begin(), temp.end(), 0);  // Reuse
        // Process temp...
    }
}
}

void run_example3_memory() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: MEMORY ALLOCATION                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Minimize allocations!\n";

    Timer timer;
    int size = 100000;

    cout << "\n📊 VECTOR CREATION:\n";

    timer.start();
    auto vec1 = example3_memory::createVector_Bad(size);
    cout << "   ❌ BAD (no reserve):  " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms (multiple allocations)" << endl;

    timer.start();
    auto vec2 = example3_memory::createVector_Good(size);
    cout << "   ✅ GOOD (with reserve): " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms (single allocation)" << endl;

    cout << "\n💡 IMPROVEMENT: reserve() eliminates reallocations!\n";
}

// ============================================
// EXAMPLE 4: LOOP OPTIMIZATION
// ============================================

namespace example4_loop {
// ❌ BAD: Repeated calculation in loop
double sumWithCalc_Bad(const vector<int>& arr, int divisor) {
    double sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        sum += arr[i] / static_cast<double>(divisor * divisor);  // Recalculated!
    }
    return sum;
}

// ✅ GOOD: Hoist invariant calculation out of loop
double sumWithCalc_Good(const vector<int>& arr, int divisor) {
    double factor = 1.0 / (divisor * divisor);  // Calculate once!
    double sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        sum += arr[i] * factor;  // Reuse
    }
    return sum;
}

// ❌ BAD: Function call in loop condition
void processString_Bad(const string& str) {
    for (size_t i = 0; i < str.length(); i++) {  // length() called every iteration!
        // Process str[i]...
    }
}

// ✅ GOOD: Cache loop limit
void processString_Good(const string& str) {
    size_t len = str.length();  // Call once!
    for (size_t i = 0; i < len; i++) {
        // Process str[i]...
    }
}
}

void run_example4_loop() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: LOOP OPTIMIZATION                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Don't repeat work in loops!\n";

    Timer timer;
    vector<int> data(1000000);
    for (int i = 0; i < 1000000; i++) data[i] = i;

    cout << "\n📊 LOOP WITH CALCULATION:\n";

    timer.start();
    double sum1 = example4_loop::sumWithCalc_Bad(data, 10);
    cout << "   ❌ BAD (recalculate):  " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    timer.start();
    double sum2 = example4_loop::sumWithCalc_Good(data, 10);
    cout << "   ✅ GOOD (hoist out):   " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    cout << "\n💡 IMPROVEMENT: Hoist loop-invariant code!\n";
}

// ============================================
// EXAMPLE 5: MOVE SEMANTICS
// ============================================

namespace example5_move {
// ❌ BAD: Unnecessary copies
vector<int> createLargeVector_Bad() {
    vector<int> temp(1000000, 42);
    vector<int> result = temp;  // Copy!
    return result;  // Another potential copy
}

// ✅ GOOD: Use move semantics
vector<int> createLargeVector_Good() {
    vector<int> temp(1000000, 42);
    return temp;  // Move (RVO/NRVO)
}

// ❌ BAD: Pass by value when not needed
void processVector_Bad(vector<int> vec) {  // Copy!
    // Just reading vec...
    int sum = 0;
    for (int val : vec) sum += val;
}

// ✅ GOOD: Pass by const reference
void processVector_Good(const vector<int>& vec) {  // No copy!
    // Just reading vec...
    int sum = 0;
    for (int val : vec) sum += val;
}

// ✅ BETTER: Use move when transferring ownership
void takeOwnership_Good(vector<int>&& vec) {  // Move!
    vector<int> myVec = move(vec);
    // Now owns the data
}
}

void run_example5_move() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: MOVE SEMANTICS                      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Avoid unnecessary copies!\n";

    Timer timer;

    cout << "\n📊 VECTOR CREATION:\n";

    timer.start();
    auto vec1 = example5_move::createLargeVector_Bad();
    cout << "   ❌ BAD (with copy):    " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    timer.start();
    auto vec2 = example5_move::createLargeVector_Good();
    cout << "   ✅ GOOD (with move):   " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    cout << "\n💡 IMPROVEMENT: Move semantics eliminate deep copies!\n";
}

// ============================================
// EXAMPLE 6: STRING OPTIMIZATION
// ============================================

namespace example6_string {
// ❌ BAD: String concatenation in loop
string buildString_Bad(int count) {
    string result;
    for (int i = 0; i < count; i++) {
        result += "text";  // Reallocates each time!
    }
    return result;
}

// ✅ GOOD: Reserve space first
string buildString_Good(int count) {
    string result;
    result.reserve(count * 4);  // Reserve space
    for (int i = 0; i < count; i++) {
        result += "text";  // No reallocation
    }
    return result;
}

// ❌ BAD: Creating temporary strings
string concatenate_Bad(const string& a, const string& b, const string& c) {
    return a + b + c;  // Creates temporaries!
}

// ✅ GOOD: Single allocation
string concatenate_Good(const string& a, const string& b, const string& c) {
    string result;
    result.reserve(a.size() + b.size() + c.size());
    result += a;
    result += b;
    result += c;
    return result;
}
}

void run_example6_string() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: STRING OPTIMIZATION                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Reserve string capacity!\n";

    Timer timer;
    int count = 10000;

    cout << "\n📊 STRING BUILDING:\n";

    timer.start();
    auto str1 = example6_string::buildString_Bad(count);
    cout << "   ❌ BAD (no reserve):   " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    timer.start();
    auto str2 = example6_string::buildString_Good(count);
    cout << "   ✅ GOOD (with reserve): " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    cout << "\n💡 IMPROVEMENT: String reserve prevents reallocations!\n";
}

// ============================================
// EXAMPLE 7: DATA STRUCTURE CHOICES
// ============================================

namespace example7_datastructure {
// ❌ BAD: Using vector for frequent insertions
void frequentInserts_Bad(int count) {
    vector<int> vec;
    for (int i = 0; i < count; i++) {
        vec.insert(vec.begin(), i);  // O(n) each time!
    }
}

// ✅ GOOD: Using deque for front insertions
void frequentInserts_Good(int count) {
    deque<int> deq;
    for (int i = 0; i < count; i++) {
        deq.push_front(i);  // O(1)!
    }
}

// ❌ BAD: Using vector for frequent searches
bool searchInVector_Bad(const vector<int>& vec, int target) {
    return find(vec.begin(), vec.end(), target) != vec.end();  // O(n)
}

// ✅ GOOD: Using set for frequent searches
bool searchInSet_Good(const unordered_set<int>& set, int target) {
    return set.count(target) > 0;  // O(1)
}
}

void run_example7_datastructure() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: DATA STRUCTURE CHOICES              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Choose the right data structure!\n";

    Timer timer;
    int count = 10000;

    cout << "\n📊 FREQUENT FRONT INSERTIONS:\n";

    timer.start();
    example7_datastructure::frequentInserts_Bad(count);
    cout << "   ❌ BAD (vector):  " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms (O(n²) total)" << endl;

    timer.start();
    example7_datastructure::frequentInserts_Good(count);
    cout << "   ✅ GOOD (deque):  " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms (O(n) total)" << endl;

    cout << "\n💡 IMPROVEMENT: Right data structure matters!\n";
}

// ============================================
// EXAMPLE 8: AVOIDING BRANCHES
// ============================================

namespace example8_branches {
// ❌ BAD: Unpredictable branches
int sumPositive_Bad(const vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        if (val > 0) {  // Branch misprediction!
            sum += val;
        }
    }
    return sum;
}

// ✅ GOOD: Branchless version
int sumPositive_Good(const vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        sum += (val > 0) * val;  // No branch!
    }
    return sum;
}

// ❌ BAD: Branch in tight loop
int absoluteSum_Bad(const vector<int>& arr) {
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
int absoluteSum_Good(const vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        int mask = val >> 31;  // -1 if negative, 0 if positive
        sum += (val + mask) ^ mask;  // Branchless abs
    }
    return sum;
}
}

void run_example8_branches() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: AVOIDING BRANCHES                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Branches can be expensive!\n";
    cout << "   (Note: Only optimize hot paths with unpredictable branches)\n";

    // Create random data for unpredictable branches
    vector<int> data(1000000);
    for (int i = 0; i < 1000000; i++) {
        data[i] = (i % 3) - 1;  // Mix of -1, 0, 1
    }

    Timer timer;

    cout << "\n📊 SUM OF POSITIVE VALUES:\n";

    timer.start();
    int sum1 = example8_branches::sumPositive_Bad(data);
    cout << "   ❌ BAD (with branch):     " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    timer.start();
    int sum2 = example8_branches::sumPositive_Good(data);
    cout << "   ✅ GOOD (branchless):     " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    cout << "\n💡 NOTE: Branchless code helps when branches are unpredictable!\n";
}

// ============================================
// EXAMPLE 9: INLINE FUNCTIONS
// ============================================

namespace example9_inline {
// ❌ BAD: Function call overhead
int square(int x) {
    return x * x;
}

int sumSquares_Bad(const vector<int>& arr) {
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

int sumSquares_Good(const vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        sum += squareInline(val);  // Inlined by compiler
    }
    return sum;
}

// ✅ BETTER: Lambda (definitely inlined)
int sumSquares_Better(const vector<int>& arr) {
    int sum = 0;
    auto square = [](int x) { return x * x; };
    for (int val : arr) {
        sum += square(val);  // Inlined
    }
    return sum;
}
}

void run_example9_inline() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: INLINE FUNCTIONS                    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Eliminate function call overhead!\n";

    vector<int> data(1000000);
    for (int i = 0; i < 1000000; i++) data[i] = i;

    Timer timer;

    cout << "\n📊 SUM OF SQUARES:\n";

    timer.start();
    int sum1 = example9_inline::sumSquares_Bad(data);
    cout << "   ❌ BAD (function call): " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    timer.start();
    int sum2 = example9_inline::sumSquares_Good(data);
    cout << "   ✅ GOOD (inline):       " << fixed << setprecision(3)
         << timer.elapsed_ms() << " ms" << endl;

    cout << "\n💡 IMPROVEMENT: Inline small, frequently-called functions!\n";
}

// ============================================
// EXAMPLE 10: COMPILER OPTIMIZATIONS
// ============================================

void run_example10_compiler() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: COMPILER OPTIMIZATIONS             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 PRINCIPLE: Use compiler optimization flags!\n";

    cout << "\n📊 OPTIMIZATION LEVELS:\n";
    cout << "   -O0: No optimization (debugging)\n";
    cout << "       • Easiest to debug\n";
    cout << "       • Slowest execution\n";
    cout << "       • ~1x speed\n";

    cout << "\n   -O1: Basic optimization\n";
    cout << "       • Some optimizations\n";
    cout << "       • Still debuggable\n";
    cout << "       • ~1.5-2x speed\n";

    cout << "\n   -O2: Moderate optimization (recommended)\n";
    cout << "       • Most optimizations\n";
    cout << "       • Good balance\n";
    cout << "       • ~2-3x speed\n";

    cout << "\n   -O3: Aggressive optimization\n";
    cout << "       • All optimizations\n";
    cout << "       • May increase binary size\n";
    cout << "       • ~3-5x speed\n";

    cout << "\n   -Os: Optimize for size\n";
    cout << "       • Minimize binary size\n";
    cout << "       • Good for embedded\n";

    cout << "\n   -Ofast: Maximum speed (non-standard)\n";
    cout << "       • May break standards compliance\n";
    cout << "       • ~5-10x speed (use carefully!)\n";

    cout << "\n💡 SPECIFIC OPTIMIZATIONS:\n";
    cout << "   -march=native      # Use CPU-specific instructions\n";
    cout << "   -flto              # Link-time optimization\n";
    cout << "   -ffast-math        # Fast floating-point (less precise)\n";
    cout << "   -funroll-loops     # Loop unrolling\n";
    cout << "   -finline-functions # Aggressive inlining\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║           PERFORMANCE OPTIMIZATION GUIDE              ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Premature optimization is evil, but informed        ║\n";
    cout << "║   optimization at the right time makes the            ║\n";
    cout << "║   difference between fast and slow.\"                 ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║      🎉 PERFORMANCE OPTIMIZATION COMPLETE! 🎉         ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Profile first, optimize hot paths,     ║\n";
    cout << "║  choose better algorithms, and use compiler flags!    ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    cout << "⚠️  REMEMBER:\n";
    cout << "   1. Measure before optimizing\n";
    cout << "   2. Algorithm choice > micro-optimizations\n";
    cout << "   3. Profile to find bottlenecks\n";
    cout << "   4. Readability matters - don't sacrifice needlessly\n";
    cout << "   5. Compile with -O2 or -O3 for production\n\n";

    return 0;
}
