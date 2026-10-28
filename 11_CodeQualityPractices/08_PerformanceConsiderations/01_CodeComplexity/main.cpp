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
#include <print>

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

    void print_elapsed(const std::string& label) {
        std::println("   ⏱️  {}: {:.3f} ms", label, elapsed_ms());
    }
};

// ============================================
// EXAMPLE 1: O(1) - CONSTANT TIME
// ============================================

namespace constant_time {
// ✅ GOOD: O(1) - Always same time regardless of input size

int getFirstElement(const std::vector<int>& arr) {
    // Array access is O(1)
    return arr[0];
}

void insertAtEnd(std::vector<int>& arr, int value) {
    // Vector push_back is amortized O(1)
    arr.push_back(value);
}

int getHashMapValue(const std::unordered_map<int, int>& map, int key) {
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: O(1) - CONSTANT TIME                ║");
    std::println("╚══════════════════════════════════════════════════╝");

    Timer timer;

    // Test with different sizes - time should be similar
    std::vector<int> sizes = {1000, 10000, 100000, 1000000};

    for (int size : sizes) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = i;

        timer.start();
        // O(1) operation - access first element
        int first = constant_time::getFirstElement(arr);
        double time = timer.elapsed_ms();

        std::println("   n={:>7} → {:.6f} ms (constant!)", size, time);
    }

}

// ============================================
// EXAMPLE 2: O(log n) - LOGARITHMIC TIME
// ============================================

namespace logarithmic_time {
// ✅ GOOD: O(log n) - Binary search

int binarySearch(const std::vector<int>& arr, int target) {
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: O(log n) - LOGARITHMIC TIME         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    Timer timer;

    std::vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};

    for (int size : sizes) {
        // Create sorted array
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = i;

        timer.start();
        // O(log n) operation - binary search
        int result = logarithmic_time::binarySearch(arr, size - 1);
        double time = timer.elapsed_ms();

        std::println("   n={:>8} → {:.6f} ms (log n ≈ {} steps)", size, time, (int)log2(size));
    }

}

// ============================================
// EXAMPLE 3: O(n) - LINEAR TIME
// ============================================

namespace linear_time {
// ✅ GOOD: O(n) - Single pass through array

int findMax(const std::vector<int>& arr) {
    if (arr.empty()) return 0;

    int max_val = arr[0];
    for (int val : arr) {
        if (val > max_val) {
            max_val = val;
        }
    }
    return max_val;
}

int sumArray(const std::vector<int>& arr) {
    int sum = 0;
    for (int val : arr) {
        sum += val;
    }
    return sum;
}

bool linearSearch(const std::vector<int>& arr, int target) {
    for (int val : arr) {
        if (val == target) return true;
    }
    return false;
}
}

void example3_linear_time() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: O(n) - LINEAR TIME                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    Timer timer;

    std::vector<int> sizes = {10000, 50000, 100000, 500000, 1000000};

    for (int size : sizes) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = i;

        timer.start();
        // O(n) operation - find max
        int max_val = linear_time::findMax(arr);
        double time = timer.elapsed_ms();

        std::println("   n={:>7} → {:.3f} ms (linear growth)", size, time);
    }

}

// ============================================
// EXAMPLE 4: O(n log n) - LINEARITHMIC TIME
// ============================================

namespace linearithmic_time {
// ✅ GOOD: O(n log n) - Efficient sorting

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merge
    std::vector<int> temp(right - left + 1);
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

void efficientSort(std::vector<int>& arr) {
    // Using STL sort (typically O(n log n))
    sort(arr.begin(), arr.end());
}
}

void example4_linearithmic_time() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: O(n log n) - LINEARITHMIC TIME      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    Timer timer;

    std::vector<int> sizes = {1000, 5000, 10000, 50000, 100000};

    for (int size : sizes) {
        std::vector<int> arr(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000000);

        for (int i = 0; i < size; i++) arr[i] = dis(gen);

        timer.start();
        // O(n log n) operation - sorting
        linearithmic_time::efficientSort(arr);
        double time = timer.elapsed_ms();

        std::println("   n={:>6} → {:.3f} ms (n*log(n) ≈ {})", size, time, (int)(size * log2(size)));
    }

}

// ============================================
// EXAMPLE 5: O(n²) - QUADRATIC TIME
// ============================================

namespace quadratic_time {
// ❌ BAD: O(n²) - Nested loops

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

bool hasDuplicates(const std::vector<int>& arr) {
    // Naive approach - check every pair
    for (int i = 0; i < arr.size(); i++) {
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[i] == arr[j]) return true;
        }
    }
    return false;
}

void printPairs(const std::vector<int>& arr) {
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

void improvedSort(std::vector<int>& arr) {
    sort(arr.begin(), arr.end());
}

// ✅ GOOD: O(n) - Using hash set
bool hasDuplicates(const std::vector<int>& arr) {
    std::unordered_set<int> seen;
    for (int val : arr) {
        if (seen.count(val)) return true;
        seen.insert(val);
    }
    return false;
}
}

void example5_quadratic_time() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: O(n²) - QUADRATIC TIME              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    Timer timer;

    std::vector<int> sizes = {100, 500, 1000, 2000, 4000};

    std::println("");
    std::println("❌ BAD - Bubble Sort O(n²):");
    for (int size : sizes) {
        std::vector<int> arr(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10000);

        for (int i = 0; i < size; i++) arr[i] = dis(gen);

        timer.start();
        quadratic_time::bubbleSort(arr);
        double time = timer.elapsed_ms();

        std::println("   n={:>4} → {:.3f} ms (n² = {})", size, time, size*size);
    }

    std::println("");
    std::println("✅ GOOD - Efficient Sort O(n log n):");
    for (int size : sizes) {
        std::vector<int> arr(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10000);

        for (int i = 0; i < size; i++) arr[i] = dis(gen);

        timer.start();
        quadratic_improved::improvedSort(arr);
        double time = timer.elapsed_ms();

        std::println("   n={:>4} → {:.3f} ms (MUCH faster!)", size, time);
    }

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
void generateSubsets(const std::vector<int>& arr, int index, std::vector<int>& current) {
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: O(2^n) - EXPONENTIAL TIME           ║");
    std::println("╚══════════════════════════════════════════════════╝");

    Timer timer;

    // Small sizes only - exponential grows VERY fast!
    std::vector<int> sizes = {10, 15, 20, 25, 30};

    std::println("");
    std::println("❌ BAD - Recursive Fibonacci O(2^n):");
    for (int size : sizes) {
        timer.start();
        long long result = exponential_time::fibonacciRecursive(size);
        double time = timer.elapsed_ms();

        std::println("   n={:>2} → {:.3f} ms (result={})", size, time, result);

        if (time > 1000) {  // Stop if taking too long
            std::println("   ... (stopping - too slow!)");
            break;
        }
    }

    std::println("");
    std::println("✅ GOOD - DP Fibonacci O(n):");
    std::vector<int> large_sizes = {10, 100, 1000, 10000, 50000};
    for (int size : large_sizes) {
        timer.start();
        long long result = exponential_improved::fibonacciDP(size);
        double time = timer.elapsed_ms();

        std::println("   n={:>5} → {:.6f} ms (INSTANT!)", size, time);
    }

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
std::vector<int> doubleArray(const std::vector<int>& arr) {
    std::vector<int> result(arr.size());  // O(n) space
    for (int i = 0; i < arr.size(); i++) {
        result[i] = arr[i] * 2;
    }
    return result;
}

// O(1) space - In-place modification
void doubleArrayInPlace(std::vector<int>& arr) {
    for (int& val : arr) {
        val *= 2;  // No extra space needed
    }
}
}

void example7_space_complexity() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: SPACE COMPLEXITY                    ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    std::println("   ");
}

// ============================================
// EXAMPLE 8: COMPLEXITY COMPARISON
// ============================================

void example8_complexity_comparison() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: COMPLEXITY COMPARISON               ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::vector<int> sizes = {1, 10, 100, 1000, 10000};

    for (int n : sizes) {
        std::print("   {:>5} | ", n);
        std::print("{:>4} | ", 1);
        std::print("{:>8} | ", (int)log2(std::max(1, n)));
        std::print("{:>5} | ", n);
        std::print("{:>10} | ", (int)(n * log2(std::max(1, n))));
        std::print("{:>9} | ", n * n);

        if (n <= 20) {
            std::print("{:>10}", (long long)pow(2, n));
        } else {
            std::print("  ENORMOUS!");
        }
        std::println("");
    }

}

// ============================================
// EXAMPLE 9: BEST/AVERAGE/WORST CASE
// ============================================

namespace case_analysis {
int linearSearch(const std::vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        // Partition and recursively sort
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
}

void example9_case_analysis() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: BEST/AVERAGE/WORST CASE             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    Timer timer;
    int size = 100000;
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) arr[i] = i;

    // Best case - first element
    timer.start();
    int result1 = case_analysis::linearSearch(arr, 0);
    std::println("   Best case (first):  {:.6f} ms", timer.elapsed_ms());

    // Average case - middle element
    timer.start();
    int result2 = case_analysis::linearSearch(arr, size / 2);
    std::println("   Average (middle):   {:.6f} ms", timer.elapsed_ms());

    // Worst case - last element
    timer.start();
    int result3 = case_analysis::linearSearch(arr, size - 1);
    std::println("   Worst case (last):  {:.6f} ms", timer.elapsed_ms());

}

// ============================================
// EXAMPLE 10: PRACTICAL GUIDELINES
// ============================================

void example10_practical_guidelines() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: PRACTICAL GUIDELINES               ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║         CODE COMPLEXITY AND BIG O NOTATION            ║");
    std::println("║                                                       ║");
    std::println("║  \"Understanding algorithm complexity is essential    ║");
    std::println("║   for writing efficient code.\"                       ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

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

    return 0;
}
