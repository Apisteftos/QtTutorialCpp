#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>
#include <stdexcept>
#include <string_view>

// ============================================================
// std::array — Fixed-Size Array in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  Element access: [], at(), front(), back(), data()
//   3.  Size and empty
//   4.  Iterators
//   5.  Sorting and searching
//   6.  Algorithms with array
//   7.  Array of objects
//   8.  Multi-dimensional arrays
//   9.  constexpr array
//  10.  std::array vs C array vs std::vector
// ============================================================

// Print a label followed by all elements of a range
template<typename Range>
void printAll(std::string_view label, const Range& r)
{
    std::print("{}", label);
    for (const auto& x : r) {
        std::print("{} ", x);
    }
    std::println("");
}

// Print the "=====" banner that introduces an example
void banner(std::string_view title)
{
    std::println("");
    std::println("=====================================================");
    std::println("{}", title);
    std::println("=====================================================");
}

int main()
{
    // === EXAMPLE 1: Declaration and Initialization ===
    banner("Example 1: Declaration and Initialization");

    // Size MUST be compile-time constant
    std::array<int, 5> a1{};                     // zero-initialized
    std::array<int, 5> a2 = {1, 2, 3, 4, 5};    // initializer list
    std::array<int, 5> a3 = {10, 20};            // rest are zero
    std::array<int, 5> a4 = a2;                  // copy

    printAll("a1 (zero-init):  ", a1);
    printAll("a2 (1-5):        ", a2);
    printAll("a3 (partial):    ", a3);
    printAll("a4 (copy of a2): ", a4);

    // std::to_array — create from C array (C++20)
    int cArr[] = {5, 4, 3, 2, 1};
    auto fromCArray = std::to_array(cArr);
    printAll("to_array:        ", fromCArray);


    // === EXAMPLE 2: Element Access ===
    banner("Example 2: Element Access");

    std::array<int, 5> arr = {10, 20, 30, 40, 50};

    std::println("arr[2]:        {}", arr[2]);         // no bounds check
    std::println("arr.at(2):     {}", arr.at(2));      // bounds checked
    std::println("arr.front():   {}", arr.front());    // first
    std::println("arr.back():    {}", arr.back());     // last
    std::println("arr.data()[1]: {}", arr.data()[1]);  // raw pointer

    // Modify
    arr[0] = 999;
    arr.back() = 888;
    printAll("after modify:  ", arr);

    // out_of_range exception
    try {
        int bad = arr.at(100);
        (void)bad;
    } catch (const std::out_of_range& e) {
        std::println("at(100) threw: {}", e.what());
    }


    // === EXAMPLE 3: Size — always compile-time known ===
    banner("Example 3: Size — always compile-time known");

    std::array<double, 4> d = {1.1, 2.2, 3.3, 4.4};

    std::println("size():     {}", d.size());      // always 4
    std::println("max_size(): {}", d.max_size());  // same as size()
    std::println("empty():    {}", d.empty());     // always false (size > 0)

    // Size is a compile-time constant
    constexpr std::size_t N = d.size();
    std::println("constexpr size N = {}", N);

    std::array<int, 0> emptyArr{};
    std::println("empty array empty(): {}", emptyArr.empty());


    // === EXAMPLE 4: Iterators ===
    banner("Example 4: Iterators");

    std::array<int, 6> it_arr = {6, 3, 8, 1, 9, 4};

    printAll("range-based: ", it_arr);

    std::print("forward:     ");
    for (auto it = it_arr.begin(); it != it_arr.end(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");

    std::print("reverse:     ");
    for (auto it = it_arr.rbegin(); it != it_arr.rend(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");

    std::print("const:       ");
    for (auto it = it_arr.cbegin(); it != it_arr.cend(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");


    // === EXAMPLE 5: Sorting and Searching ===
    banner("Example 5: Sorting and Searching");

    std::array<int, 8> s = {5, 3, 8, 1, 9, 2, 7, 4};

    std::sort(s.begin(), s.end());
    printAll("sorted:      ", s);

    std::sort(s.begin(), s.end(), std::greater<int>());
    printAll("desc:        ", s);

    std::sort(s.begin(), s.end());

    auto found = std::find(s.begin(), s.end(), 7);
    if (found != s.end()) {
        std::println("find(7) at index: {}", std::distance(s.begin(), found));
    }

    std::println("binary_search(7): {}", std::binary_search(s.begin(), s.end(), 7));
    std::println("min: {}", *std::min_element(s.begin(), s.end()));
    std::println("max: {}", *std::max_element(s.begin(), s.end()));
    std::println("sum: {}", std::accumulate(s.begin(), s.end(), 0));


    // === EXAMPLE 6: Algorithms with array ===
    banner("Example 6: Algorithms with array");

    std::array<int, 5> src = {1, 2, 3, 4, 5};
    std::array<int, 5> dst{};

    // copy
    std::copy(src.begin(), src.end(), dst.begin());
    printAll("copy:        ", dst);

    // transform
    std::array<int, 5> doubled{};
    std::transform(src.begin(), src.end(), doubled.begin(),
                   [](int x) { return x * 2; });
    printAll("doubled:     ", doubled);

    // fill
    std::array<int, 5> filled{};
    std::fill(filled.begin(), filled.end(), 99);
    printAll("filled(99):  ", filled);

    // iota
    std::array<int, 6> seq{};
    std::iota(seq.begin(), seq.end(), 10);   // 10,11,12,13,14,15
    printAll("iota(10):    ", seq);

    // reverse
    std::reverse(seq.begin(), seq.end());
    printAll("reversed:    ", seq);

    // count_if
    auto evens = std::count_if(src.begin(), src.end(),
                               [](int x) { return x % 2 == 0; });
    std::println("even count:  {}", evens);

    // swap two arrays of same type and size
    std::array<int, 5> x1 = {1, 2, 3, 4, 5};
    std::array<int, 5> x2 = {9, 8, 7, 6, 5};
    x1.swap(x2);
    printAll("after swap x1: ", x1);


    // === EXAMPLE 7: Array of Objects ===
    banner("Example 7: Array of Objects");

    struct Point { int x, y; };

    std::array<Point, 4> points = {{{1, 2}, {5, 3}, {2, 8}, {4, 1}}};

    std::println("points:");
    for (const auto& pt : points) {
        std::println("  ({}, {})", pt.x, pt.y);
    }

    // Sort by x coordinate
    std::sort(points.begin(), points.end(),
              [](const Point& lhs, const Point& rhs) { return lhs.x < rhs.x; });

    std::println("sorted by x:");
    for (const auto& pt : points) {
        std::println("  ({}, {})", pt.x, pt.y);
    }


    // === EXAMPLE 8: Multi-dimensional arrays ===
    banner("Example 8: Multi-dimensional arrays");

    // 3x3 matrix
    std::array<std::array<int, 3>, 3> matrix = {{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    }};

    std::println("3x3 matrix:");
    for (const auto& row : matrix) {
        std::print("  ");
        for (int val : row) {
            std::print("{}\t", val);
        }
        std::println("");
    }

    // Transpose
    std::array<std::array<int, 3>, 3> transposed{};
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    std::println("transposed:");
    for (const auto& row : transposed) {
        std::print("  ");
        for (int val : row) {
            std::print("{}\t", val);
        }
        std::println("");
    }


    // === EXAMPLE 9: constexpr array ===
    banner("Example 9: constexpr array");

    // Computed entirely at compile time
    constexpr std::array<int, 5> primes = {2, 3, 5, 7, 11};
    constexpr int firstPrime  = primes[0];
    constexpr int lastPrime   = primes.back();
    constexpr std::size_t primeCount = primes.size();

    std::println("primes:      {} {} {} {} {}", primes[0], primes[1], primes[2], primes[3], primes[4]);
    std::println("firstPrime:  {}", firstPrime);
    std::println("lastPrime:   {}", lastPrime);
    std::println("primeCount:  {}", primeCount);

    // constexpr lookup table
    constexpr std::array<double, 4> sineTable = {0.0, 0.5, 0.866, 1.0};
    std::println("sineTable[2] = {}", sineTable[2]);


    // === EXAMPLE 10: std::array vs C array vs std::vector ===
    banner("Example 10: std::array vs C array vs std::vector");

    // C array — no size info, decays to pointer, unsafe
    int cArray[5] = {1, 2, 3, 4, 5};
    // sizeof gives bytes, not count — fragile:
    std::println("C array sizeof/sizeof[0]: {}", sizeof(cArray) / sizeof(cArray[0]));
    // void func(int arr[]) — size lost! arr becomes pointer

    // std::array — size always known, never decays
    std::array<int, 5> stdArray = {1, 2, 3, 4, 5};
    std::println("std::array size(): {} (always known)", stdArray.size());

    // Comparison with ==
    std::array<int, 3> p = {1, 2, 3};
    std::array<int, 3> q = {1, 2, 3};
    std::array<int, 3> r = {1, 2, 4};
    std::println("p == q: {}", p == q);   // true
    std::println("p == r: {}", p == r);   // false
    std::println("p <  r: {}", p <  r);   // lexicographic

    std::println("");
    std::println("=== ArraySTL complete ===");
    return 0;
}
