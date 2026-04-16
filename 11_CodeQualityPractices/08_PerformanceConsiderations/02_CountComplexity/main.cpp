#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <map>
#include <set>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <unordered_set>
using namespace std;
using namespace chrono;

// ============================================
// CODE COMPLEXITY AND BIG O NOTATION
// ============================================

/*
CODE COMPLEXITY:
    "Understanding algorithm complexity is essential for writing
     efficient code. Big O notation describes how runtime or space
     requirements grow as input size increases."
    — Algorithm Analysis Principle

WHAT IT MEANS:
    • How performance scales with input size
    • Worst-case time/space requirements
    • Comparing algorithm efficiency
    • Predicting performance bottlenecks

WHY IT MATTERS:
    ✅ Choose efficient algorithms
    ✅ Predict performance at scale
    ✅ Identify bottlenecks early
    ✅ Make informed trade-offs
    ✅ Optimize critical paths

COMMON COMPLEXITIES (from best to worst):
    O(1)        - Constant
    O(log n)    - Logarithmic
    O(n)        - Linear
    O(n log n)  - Linearithmic
    O(n²)       - Quadratic
    O(n³)       - Cubic
    O(2^n)      - Exponential
    O(n!)       - Factorial

GROWTH RATES (n=1000):
    O(1):        1 operation
    O(log n):    ~10 operations
    O(n):        1,000 operations
    O(n log n):  ~10,000 operations
    O(n²):       1,000,000 operations
    O(2^n):      10^301 operations (universe-ending!)
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

    void print_elapsed(const string& label) {
        cout << "   ⏱️  " << label << ": " << fixed << setprecision(3)
             << elapsed_ms() << " ms" << endl;
    }
};

// ============================================
// EXAMPLE 1: O(1) - CONSTANT TIME
// ============================================

namespace constant_time {
// ✅ GOOD: O(1) - Always same time regardless of input size

int getFirstElement(const vector<int>& arr) {
    // Array access is O(1)
    return arr[0];
}

void insertAtEnd(vector<int>& arr, int value) {
    // Vector push_back is amortized O(1)
    arr.push_back(value);
}

int getHashMapValue(const unordered_map<int, int>& map, int key) {
    // Hash map lookup is average O(1)
    auto it = map.find(key);
    return (it != map.end()) ? it->second : -1;
}

int calculateSum(int a, int b) {
    // Arithmetic operations are O(1)
    return a + b;
}
}

void example1_constant_time() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: O(1) - CONSTANT TIME               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COMPLEXITY: O(1)\n";
    cout << "   Time stays constant regardless of input size\n";

    cout << "\n✅ EXAMPLES:\n";
    cout << "   • Array access: arr[i]\n";
    cout << "   • Hash table lookup\n";
    cout << "   • Push/pop from stack\n";
    cout << "   • Arithmetic operations\n";

    cout << "\n⏱️  TIMING DEMONSTRATION:\n";
    Timer timer;

    // Test with different sizes - time should be similar
    vector<int> sizes = {1000, 10000, 100000, 1000000};

    for (int size : sizes) {
        vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = i;

        timer.start();
        // O(1) operation - access first element
        int first = constant_time::getFirstElement(arr);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(7) << size << " → " << fixed << setprecision(6)
             << time << " ms (constant!)" << endl;
    }

    cout << "\n💡 OBSERVATION:\n";
    cout << "   Time remains ~constant regardless of size!\n";
    cout << "   This is O(1) - the best complexity possible.\n";
}

// ============================================
// EXAMPLE 2: O(log n) - LOGARITHMIC TIME
// ============================================

namespace logarithmic_time {
// ✅ GOOD: O(log n) - Binary search

int binarySearch(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;  // Not found
}

// Binary search tree operations (balanced)
int treeHeight(int nodes) {
    return log2(nodes);
}
}

void example2_logarithmic_time() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: O(log n) - LOGARITHMIC TIME        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COMPLEXITY: O(log n)\n";
    cout << "   Time grows logarithmically with input size\n";
    cout << "   Each step cuts problem size in half\n";

    cout << "\n✅ EXAMPLES:\n";
    cout << "   • Binary search\n";
    cout << "   • Balanced tree operations\n";
    cout << "   • Finding power (x^n with binary exponentiation)\n";

    cout << "\n⏱️  TIMING DEMONSTRATION:\n";
    Timer timer;

    vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};

    for (int size : sizes) {
        // Create sorted array
        vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = i;

        timer.start();
        // O(log n) operation - binary search
        int result = logarithmic_time::binarySearch(arr, size - 1);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(8) << size << " → " << fixed << setprecision(6)
             << time << " ms (log n ≈ " << (int)log2(size) << " steps)" << endl;
    }

    cout << "\n💡 OBSERVATION:\n";
    cout << "   10x increase in size → only ~3.3x increase in time\n";
    cout << "   Even 1,000,000 elements → only ~20 comparisons!\n";
}

// ============================================
// EXAMPLE 3: O(n) - LINEAR TIME
// ============================================

namespace linear_time {
// ✅ GOOD: O(n) - Single pass through array

int findMax(const vector<int>& arr) {
    if (arr.empty()) return 0;

    int max_val = arr[0];
    for (int val : arr) {
        if (val > max_val) {
            max_val = val;
        }
    }
    return max_val;
}

int sumArray(const vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        sum += val;
    }
    return sum;
}

bool linearSearch(const vector<int>& arr, int target) {
    for (int val : arr) {
        if (val == target) return true;
    }
    return false;
}
}

void example3_linear_time() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: O(n) - LINEAR TIME                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COMPLEXITY: O(n)\n";
    cout << "   Time grows linearly with input size\n";
    cout << "   Must visit each element once\n";

    cout << "\n✅ EXAMPLES:\n";
    cout << "   • Finding max/min in unsorted array\n";
    cout << "   • Linear search\n";
    cout << "   • Summing array elements\n";
    cout << "   • Single loop through data\n";

    cout << "\n⏱️  TIMING DEMONSTRATION:\n";
    Timer timer;

    vector<int> sizes = {10000, 50000, 100000, 500000, 1000000};

    for (int size : sizes) {
        vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = i;

        timer.start();
        // O(n) operation - find max
        int max_val = linear_time::findMax(arr);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(7) << size << " → " << fixed << setprecision(3)
             << time << " ms (linear growth)" << endl;
    }

    cout << "\n💡 OBSERVATION:\n";
    cout << "   Time doubles when size doubles (linear relationship)\n";
    cout << "   This is acceptable for most operations.\n";
}

// ============================================
// EXAMPLE 4: O(n log n) - LINEARITHMIC TIME
// ============================================

namespace linearithmic_time {
// ✅ GOOD: O(n log n) - Efficient sorting

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merge
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int i = 0; i < temp.size(); i++) {
        arr[left + i] = temp[i];
    }
}

void efficientSort(vector<int>& arr) {
    // Using STL sort (typically O(n log n))
    sort(arr.begin(), arr.end());
}
}

void example4_linearithmic_time() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: O(n log n) - LINEARITHMIC TIME     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COMPLEXITY: O(n log n)\n";
    cout << "   Combination of linear and logarithmic\n";
    cout << "   Best achievable for comparison-based sorting\n";

    cout << "\n✅ EXAMPLES:\n";
    cout << "   • Merge sort\n";
    cout << "   • Quick sort (average case)\n";
    cout << "   • Heap sort\n";
    cout << "   • Efficient sorting algorithms\n";

    cout << "\n⏱️  TIMING DEMONSTRATION:\n";
    Timer timer;

    vector<int> sizes = {1000, 5000, 10000, 50000, 100000};

    for (int size : sizes) {
        vector<int> arr(size);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 1000000);

        for (int i = 0; i < size; i++) arr[i] = dis(gen);

        timer.start();
        // O(n log n) operation - sorting
        linearithmic_time::efficientSort(arr);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(6) << size << " → " << fixed << setprecision(3)
             << time << " ms (n*log(n) ≈ " << (int)(size * log2(size)) << ")" << endl;
    }

    cout << "\n💡 OBSERVATION:\n";
    cout << "   Slower than O(n) but much faster than O(n²)\n";
    cout << "   This is optimal for comparison-based sorting!\n";
}

// ============================================
// EXAMPLE 5: O(n²) - QUADRATIC TIME
// ============================================

namespace quadratic_time {
// ❌ BAD: O(n²) - Nested loops

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

bool hasDuplicates(const vector<int>& arr) {
    // Naive approach - check every pair
    for (int i = 0; i < arr.size(); i++) {
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[i] == arr[j]) return true;
        }
    }
    return false;
}

void printPairs(const vector<int>& arr) {
    // Print all pairs
    for (int i = 0; i < arr.size(); i++) {
        for (int j = 0; j < arr.size(); j++) {
            // Do something with pair (i, j)
        }
    }
}
}

namespace quadratic_improved {
// ✅ GOOD: O(n log n) - Better sorting

void improvedSort(vector<int>& arr) {
    sort(arr.begin(), arr.end());
}

// ✅ GOOD: O(n) - Using hash set
bool hasDuplicates(const vector<int>& arr) {
    unordered_set<int> seen;
    for (int val : arr) {
        if (seen.count(val)) return true;
        seen.insert(val);
    }
    return false;
}
}

void example5_quadratic_time() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: O(n²) - QUADRATIC TIME             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COMPLEXITY: O(n²)\n";
    cout << "   Time grows quadratically with input size\n";
    cout << "   Usually involves nested loops\n";

    cout << "\n❌ COMMON CAUSES:\n";
    cout << "   • Nested loops over same data\n";
    cout << "   • Bubble sort, insertion sort\n";
    cout << "   • Naive duplicate detection\n";
    cout << "   • Comparing all pairs\n";

    cout << "\n⏱️  TIMING DEMONSTRATION:\n";
    Timer timer;

    vector<int> sizes = {100, 500, 1000, 2000, 4000};

    cout << "\n❌ BAD - Bubble Sort O(n²):\n";
    for (int size : sizes) {
        vector<int> arr(size);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 10000);

        for (int i = 0; i < size; i++) arr[i] = dis(gen);

        timer.start();
        quadratic_time::bubbleSort(arr);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(4) << size << " → " << fixed << setprecision(3)
             << time << " ms (n² = " << size*size << ")" << endl;
    }

    cout << "\n✅ GOOD - Efficient Sort O(n log n):\n";
    for (int size : sizes) {
        vector<int> arr(size);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 10000);

        for (int i = 0; i < size; i++) arr[i] = dis(gen);

        timer.start();
        quadratic_improved::improvedSort(arr);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(4) << size << " → " << fixed << setprecision(3)
             << time << " ms (MUCH faster!)" << endl;
    }

    cout << "\n💡 OBSERVATION:\n";
    cout << "   O(n²) becomes VERY slow as n grows!\n";
    cout << "   4x increase in size → 16x increase in time\n";
    cout << "   Avoid nested loops when possible!\n";
}

// ============================================
// EXAMPLE 6: O(2^n) - EXPONENTIAL TIME
// ============================================

namespace exponential_time {
// ❌ BAD: O(2^n) - Naive recursive Fibonacci

long long fibonacciRecursive(int n) {
    if (n <= 1) return n;
    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
}

// Subset generation (2^n subsets)
void generateSubsets(const vector<int>& arr, int index, vector<int>& current) {
    if (index == arr.size()) {
        return;  // Process subset
    }

    // Include current element
    current.push_back(arr[index]);
    generateSubsets(arr, index + 1, current);
    current.pop_back();

    // Exclude current element
    generateSubsets(arr, index + 1, current);
}
}

namespace exponential_improved {
// ✅ GOOD: O(n) - Dynamic programming Fibonacci

long long fibonacciDP(int n) {
    if (n <= 1) return n;

    long long prev2 = 0;
    long long prev1 = 1;
    long long current;

    for (int i = 2; i <= n; i++) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
}

void example6_exponential_time() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: O(2^n) - EXPONENTIAL TIME          ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COMPLEXITY: O(2^n)\n";
    cout << "   Time DOUBLES with each additional input!\n";
    cout << "   Grows extremely fast - often impractical\n";

    cout << "\n❌ COMMON CAUSES:\n";
    cout << "   • Naive recursive solutions\n";
    cout << "   • Generating all subsets\n";
    cout << "   • Brute force combinatorial problems\n";

    cout << "\n⏱️  TIMING DEMONSTRATION:\n";
    Timer timer;

    // Small sizes only - exponential grows VERY fast!
    vector<int> sizes = {10, 15, 20, 25, 30};

    cout << "\n❌ BAD - Recursive Fibonacci O(2^n):\n";
    for (int size : sizes) {
        timer.start();
        long long result = exponential_time::fibonacciRecursive(size);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(2) << size << " → " << fixed << setprecision(3)
             << time << " ms (result=" << result << ")" << endl;

        if (time > 1000) {  // Stop if taking too long
            cout << "   ... (stopping - too slow!)" << endl;
            break;
        }
    }

    cout << "\n✅ GOOD - DP Fibonacci O(n):\n";
    vector<int> large_sizes = {10, 100, 1000, 10000, 50000};
    for (int size : large_sizes) {
        timer.start();
        long long result = exponential_improved::fibonacciDP(size);
        double time = timer.elapsed_ms();

        cout << "   n=" << setw(5) << size << " → " << fixed << setprecision(6)
             << time << " ms (INSTANT!)" << endl;
    }

    cout << "\n💡 OBSERVATION:\n";
    cout << "   O(2^n) is CATASTROPHICALLY slow!\n";
    cout << "   n=30 might take seconds, n=40 takes hours!\n";
    cout << "   Always look for DP or memoization solutions!\n";
}

// ============================================
// EXAMPLE 7: SPACE COMPLEXITY
// ============================================

namespace space_complexity {
// O(1) space - Constant
int sumIterative(int n) {
    int sum = 0;  // Only uses constant space
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}

// O(n) space - Linear
int sumRecursive(int n) {
    // Each recursive call uses stack space
    if (n <= 0) return 0;
    return n + sumRecursive(n - 1);  // n stack frames
}

// O(n) space - Creating new array
vector<int> doubleArray(const vector<int>& arr) {
    vector<int> result(arr.size());  // O(n) space
    for (int i = 0; i < arr.size(); i++) {
        result[i] = arr[i] * 2;
    }
    return result;
}

// O(1) space - In-place modification
void doubleArrayInPlace(vector<int>& arr) {
    for (int& val : arr) {
        val *= 2;  // No extra space needed
    }
}
}

void example7_space_complexity() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: SPACE COMPLEXITY                    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 SPACE COMPLEXITY:\n";
    cout << "   How much memory an algorithm uses\n";

    cout << "\n📋 COMMON SPACE COMPLEXITIES:\n";

    cout << "\n✅ O(1) - CONSTANT SPACE:\n";
    cout << "   • Only uses fixed amount of memory\n";
    cout << "   • No growth with input size\n";
    cout << "   • Examples: simple variables, in-place operations\n";
    cout << "   \n";
    cout << "   int sum = 0;\n";
    cout << "   for (int i = 0; i < n; i++) sum += i;  // O(1) space\n";

    cout << "\n⚠️  O(log n) - LOGARITHMIC SPACE:\n";
    cout << "   • Recursive algorithms with halving\n";
    cout << "   • Binary search recursion\n";
    cout << "   • Balanced tree recursion\n";

    cout << "\n⚠️  O(n) - LINEAR SPACE:\n";
    cout << "   • Creating array/vector of size n\n";
    cout << "   • Recursive calls n deep\n";
    cout << "   • Hash map with n elements\n";
    cout << "   \n";
    cout << "   vector<int> copy = original;  // O(n) space\n";

    cout << "\n🚨 O(n²) - QUADRATIC SPACE:\n";
    cout << "   • 2D arrays/matrices\n";
    cout << "   • Dynamic programming tables\n";
    cout << "   \n";
    cout << "   vector<vector<int>> matrix(n, vector<int>(n));  // O(n²)\n";

    cout << "\n💡 KEY POINTS:\n";
    cout << "   • Time-Space Trade-offs are common\n";
    cout << "   • Faster algorithms often use more space\n";
    cout << "   • In-place algorithms save memory (O(1) space)\n";
    cout << "   • Recursion uses stack space\n";
}

// ============================================
// EXAMPLE 8: COMPLEXITY COMPARISON
// ============================================

void example8_complexity_comparison() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: COMPLEXITY COMPARISON               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 GROWTH COMPARISON:\n";
    cout << "   How different complexities scale\n\n";

    cout << "   n     | O(1) | O(log n) | O(n)  | O(n log n) |   O(n²)   |    O(2^n)\n";
    cout << "   ------+------+----------+-------+------------+-----------+------------\n";

    vector<int> sizes = {1, 10, 100, 1000, 10000};

    for (int n : sizes) {
        cout << "   " << setw(5) << n << " | ";
        cout << setw(4) << 1 << " | ";
        cout << setw(8) << (int)log2(max(1, n)) << " | ";
        cout << setw(5) << n << " | ";
        cout << setw(10) << (int)(n * log2(max(1, n))) << " | ";
        cout << setw(9) << n * n << " | ";

        if (n <= 20) {
            cout << setw(10) << (long long)pow(2, n);
        } else {
            cout << "  ENORMOUS!";
        }
        cout << endl;
    }

    cout << "\n📈 VISUAL GROWTH (relative):\n";
    cout << "   n=1000:\n";
    cout << "   O(1):        ▌                              (1)\n";
    cout << "   O(log n):    ▌                              (~10)\n";
    cout << "   O(n):        ██████████                     (1,000)\n";
    cout << "   O(n log n):  ███████████████████████        (~10,000)\n";
    cout << "   O(n²):       ████████████████████... (way off screen!) (1,000,000)\n";
    cout << "   O(2^n):      [Would fill the universe]\n";

    cout << "\n🎯 PRACTICAL LIMITS:\n";
    cout << "   n=1,000,000 (1 million):\n";
    cout << "   • O(1):        Instant ✅\n";
    cout << "   • O(log n):    ~20 operations ✅\n";
    cout << "   • O(n):        ~1ms ✅\n";
    cout << "   • O(n log n):  ~20ms ✅\n";
    cout << "   • O(n²):       ~1000s (16 minutes!) 🚨\n";
    cout << "   • O(2^n):      Heat death of universe 💀\n";
}

// ============================================
// EXAMPLE 9: BEST/AVERAGE/WORST CASE
// ============================================

namespace case_analysis {
int linearSearch(const vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // Partition and recursively sort
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
}

void example9_case_analysis() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: BEST/AVERAGE/WORST CASE            ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 LINEAR SEARCH:\n";
    cout << "   Best case:    O(1)   - Element is first\n";
    cout << "   Average case: O(n/2) - Element in middle\n";
    cout << "   Worst case:   O(n)   - Element is last or not present\n";

    cout << "\n📊 QUICK SORT:\n";
    cout << "   Best case:    O(n log n) - Perfect pivot selection\n";
    cout << "   Average case: O(n log n) - Random pivot\n";
    cout << "   Worst case:   O(n²)      - Already sorted, poor pivot\n";

    cout << "\n📊 HASH TABLE LOOKUP:\n";
    cout << "   Best case:    O(1)   - No collisions\n";
    cout << "   Average case: O(1)   - Few collisions\n";
    cout << "   Worst case:   O(n)   - All keys collide\n";

    cout << "\n⏱️  DEMONSTRATION - Linear Search:\n";
    Timer timer;
    int size = 100000;
    vector<int> arr(size);
    for (int i = 0; i < size; i++) arr[i] = i;

    // Best case - first element
    timer.start();
    int result1 = case_analysis::linearSearch(arr, 0);
    cout << "   Best case (first):  " << fixed << setprecision(6)
         << timer.elapsed_ms() << " ms\n";

    // Average case - middle element
    timer.start();
    int result2 = case_analysis::linearSearch(arr, size / 2);
    cout << "   Average (middle):   " << fixed << setprecision(6)
         << timer.elapsed_ms() << " ms\n";

    // Worst case - last element
    timer.start();
    int result3 = case_analysis::linearSearch(arr, size - 1);
    cout << "   Worst case (last):  " << fixed << setprecision(6)
         << timer.elapsed_ms() << " ms\n";

    cout << "\n💡 IMPORTANT:\n";
    cout << "   • Big O usually refers to WORST CASE\n";
    cout << "   • Average case is often most practical\n";
    cout << "   • Best case is rarely useful for analysis\n";
}

// ============================================
// EXAMPLE 10: PRACTICAL GUIDELINES
// ============================================

void example10_practical_guidelines() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: PRACTICAL GUIDELINES               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 HOW TO ANALYZE COMPLEXITY:\n";

    cout << "\n1. COUNT THE LOOPS:\n";
    cout << "   • One loop: O(n)\n";
    cout << "   • Two nested loops: O(n²)\n";
    cout << "   • Three nested loops: O(n³)\n";

    cout << "\n2. LOOK FOR DIVISIONS:\n";
    cout << "   • Halving problem size: O(log n)\n";
    cout << "   • Binary search pattern: O(log n)\n";

    cout << "\n3. RECURSIVE PATTERNS:\n";
    cout << "   • Single recursive call: Often O(n)\n";
    cout << "   • Two recursive calls: Often O(2^n)\n";
    cout << "   • Divide and conquer: Often O(n log n)\n";

    cout << "\n4. MULTIPLE OPERATIONS:\n";
    cout << "   • Sequential: Add them → O(n + m)\n";
    cout << "   • Nested: Multiply them → O(n * m)\n";
    cout << "   • Take the dominant term → O(n² + n) = O(n²)\n";

    cout << "\n💡 OPTIMIZATION RULES:\n";

    cout << "\n✅ DO:\n";
    cout << "   • Use appropriate data structures\n";
    cout << "   • Avoid unnecessary nested loops\n";
    cout << "   • Use hash tables for O(1) lookup\n";
    cout << "   • Sort data if you need to search often\n";
    cout << "   • Consider space-time tradeoffs\n";

    cout << "\n❌ DON'T:\n";
    cout << "   • Prematurely optimize\n";
    cout << "   • Ignore readability for micro-optimizations\n";
    cout << "   • Use O(n²) algorithms for large data\n";
    cout << "   • Forget about space complexity\n";

    cout << "\n📊 DATA STRUCTURE COMPLEXITIES:\n";
    cout << "   Array:         Access O(1), Search O(n), Insert O(n)\n";
    cout << "   Hash Table:    Access O(1), Search O(1), Insert O(1)\n";
    cout << "   Binary Tree:   Access O(log n), Search O(log n), Insert O(log n)\n";
    cout << "   Linked List:   Access O(n), Search O(n), Insert O(1)\n";

    cout << "\n🎓 WHEN TO USE WHAT:\n";
    cout << "   n < 100:       Any algorithm works\n";
    cout << "   n < 10,000:    O(n²) acceptable\n";
    cout << "   n < 1,000,000: Need O(n log n) or better\n";
    cout << "   n > 1,000,000: Need O(n) or O(log n)\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         CODE COMPLEXITY AND BIG O NOTATION            ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Understanding algorithm complexity is essential      ║\n";
    cout << "║   for writing efficient code.\"                       ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_constant_time();
    example2_logarithmic_time();
    example3_linear_time();
    example4_linearithmic_time();
    example5_quadratic_time();
    example6_exponential_time();
    example7_space_complexity();
    example8_complexity_comparison();
    example9_case_analysis();
    example10_practical_guidelines();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 CODE COMPLEXITY COMPLETE! 🎉               ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Choose algorithms wisely! The right    ║\n";
    cout << "║  complexity can mean the difference between instant   ║\n";
    cout << "║  and impossibly slow.                                 ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
