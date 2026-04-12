#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <string>
#include <print>

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

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    // Size MUST be compile-time constant
    std::array<int, 5> a1{};                     // zero-initialized
    std::array<int, 5> a2 = {1, 2, 3, 4, 5};    // initializer list
    std::array<int, 5> a3 = {10, 20};            // rest are zero
    std::array<int, 5> a4 = a2;                  // copy
    std::array<int, 3> strings = {};             // zero-init (int, not string)

    std::cout << "a1 (zero-init):  ";
    for (int x : a1) std::cout << x << ' '; std::cout << '\n';

    std::cout << "a2 (1-5):        ";
    for (int x : a2) std::cout << x << ' '; std::cout << '\n';

    std::cout << "a3 (partial):    ";
    for (int x : a3) std::cout << x << ' '; std::cout << '\n';

    std::cout << "a4 (copy of a2): ";
    for (int x : a4) std::cout << x << ' '; std::cout << '\n';

    // std::to_array — create from C array (C++20)
    int cArr[] = {5, 4, 3, 2, 1};
    auto fromCArray = std::to_array(cArr);
    std::cout << "to_array:        ";
    for (int x : fromCArray) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Element Access\n";
    std::cout << "=====================================================\n";

    std::array<int, 5> arr = {10, 20, 30, 40, 50};

    std::cout << "arr[2]:        " << arr[2]        << '\n';  // no bounds check
    std::cout << "arr.at(2):     " << arr.at(2)     << '\n';  // bounds checked
    std::cout << "arr.front():   " << arr.front()   << '\n';  // first
    std::cout << "arr.back():    " << arr.back()    << '\n';  // last
    std::cout << "arr.data()[1]: " << arr.data()[1] << '\n';  // raw pointer

    // Modify
    arr[0] = 999;
    arr.back() = 888;
    std::cout << "after modify:  ";
    for (int x : arr) std::cout << x << ' '; std::cout << '\n';

    // out_of_range exception
    try {
        int bad = arr.at(100);
        (void)bad;
    } catch (const std::out_of_range& e) {
        std::cout << "at(100) threw: " << e.what() << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Size — always compile-time known\n";
    std::cout << "=====================================================\n";

    std::array<double, 4> d = {1.1, 2.2, 3.3, 4.4};

    std::cout << "size():     " << d.size()     << '\n';  // always 4
    std::cout << "max_size(): " << d.max_size() << '\n';  // same as size()
    std::cout << "empty():    " << d.empty()    << '\n';  // always false (size > 0)

    // Size is a compile-time constant
    constexpr std::size_t N = d.size();
    std::cout << "constexpr size N = " << N << '\n';

    std::array<int, 0> emptyArr{};
    std::cout << "empty array empty(): " << emptyArr.empty() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Iterators\n";
    std::cout << "=====================================================\n";

    std::array<int, 6> it_arr = {6, 3, 8, 1, 9, 4};

    std::cout << "range-based: ";
    for (const auto& x : it_arr) std::cout << x << ' '; std::cout << '\n';

    std::cout << "forward:     ";
    for (auto it = it_arr.begin(); it != it_arr.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    std::cout << "reverse:     ";
    for (auto it = it_arr.rbegin(); it != it_arr.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    std::cout << "const:       ";
    for (auto it = it_arr.cbegin(); it != it_arr.cend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Sorting and Searching\n";
    std::cout << "=====================================================\n";

    std::array<int, 8> s = {5, 3, 8, 1, 9, 2, 7, 4};

    std::sort(s.begin(), s.end());
    std::cout << "sorted:      ";
    for (int x : s) std::cout << x << ' '; std::cout << '\n';

    std::sort(s.begin(), s.end(), std::greater<int>());
    std::cout << "desc:        ";
    for (int x : s) std::cout << x << ' '; std::cout << '\n';

    std::sort(s.begin(), s.end());

    auto it = std::find(s.begin(), s.end(), 7);
    if (it != s.end())
        std::println("find(7) at index: {}", std::distance(s.begin(), it));

    std::cout << "binary_search(7): " << std::binary_search(s.begin(), s.end(), 7) << '\n';
    std::cout << "min: " << *std::min_element(s.begin(), s.end()) << '\n';
    std::cout << "max: " << *std::max_element(s.begin(), s.end()) << '\n';
    std::cout << "sum: " << std::accumulate(s.begin(), s.end(), 0) << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Algorithms with array\n";
    std::cout << "=====================================================\n";

    std::array<int, 5> src = {1, 2, 3, 4, 5};
    std::array<int, 5> dst{};

    // copy
    std::copy(src.begin(), src.end(), dst.begin());
    std::cout << "copy:        ";
    for (int x : dst) std::cout << x << ' '; std::cout << '\n';

    // transform
    std::array<int, 5> doubled{};
    std::transform(src.begin(), src.end(), doubled.begin(),
                   [](int x) { return x * 2; });
    std::cout << "doubled:     ";
    for (int x : doubled) std::cout << x << ' '; std::cout << '\n';

    // fill
    std::array<int, 5> filled{};
    std::fill(filled.begin(), filled.end(), 99);
    std::cout << "filled(99):  ";
    for (int x : filled) std::cout << x << ' '; std::cout << '\n';

    // iota
    std::array<int, 6> seq{};
    std::iota(seq.begin(), seq.end(), 10);   // 10,11,12,13,14,15
    std::cout << "iota(10):    ";
    for (int x : seq) std::cout << x << ' '; std::cout << '\n';

    // reverse
    std::reverse(seq.begin(), seq.end());
    std::cout << "reversed:    ";
    for (int x : seq) std::cout << x << ' '; std::cout << '\n';

    // count_if
    int evens = std::count_if(src.begin(), src.end(),
                              [](int x) { return x % 2 == 0; });
    std::cout << "even count:  " << evens << '\n';

    // swap two arrays of same type and size
    std::array<int, 5> x1 = {1,2,3,4,5};
    std::array<int, 5> x2 = {9,8,7,6,5};
    x1.swap(x2);
    std::cout << "after swap x1: ";
    for (int x : x1) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Array of Objects\n";
    std::cout << "=====================================================\n";

    struct Point { int x, y; };

    std::array<Point, 4> points = {{{1,2}, {5,3}, {2,8}, {4,1}}};

    std::cout << "points:\n";
    for (const auto& p : points)
        std::println("  ({}, {})", p.x, p.y);

    // Sort by x coordinate
    std::sort(points.begin(), points.end(),
              [](const Point& a, const Point& b) { return a.x < b.x; });

    std::cout << "sorted by x:\n";
    for (const auto& p : points)
        std::println("  ({}, {})", p.x, p.y);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Multi-dimensional arrays\n";
    std::cout << "=====================================================\n";

    // 3x3 matrix
    std::array<std::array<int, 3>, 3> matrix = {{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    }};

    std::cout << "3x3 matrix:\n";
    for (const auto& row : matrix) {
        std::cout << "  ";
        for (int val : row) std::cout << val << "\t";
        std::cout << '\n';
    }

    // Transpose
    std::array<std::array<int, 3>, 3> transposed{};
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            transposed[c][r] = matrix[r][c];

    std::cout << "transposed:\n";
    for (const auto& row : transposed) {
        std::cout << "  ";
        for (int val : row) std::cout << val << "\t";
        std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: constexpr array\n";
    std::cout << "=====================================================\n";

    // Computed entirely at compile time
    constexpr std::array<int, 5> primes = {2, 3, 5, 7, 11};
    constexpr int firstPrime  = primes[0];
    constexpr int lastPrime   = primes.back();
    constexpr int primeCount  = primes.size();

    std::println("primes:      {} {} {} {} {}", primes[0], primes[1], primes[2], primes[3], primes[4]);
    std::println("firstPrime:  {}", firstPrime);
    std::println("lastPrime:   {}", lastPrime);
    std::println("primeCount:  {}", primeCount);

    // constexpr lookup table
    constexpr std::array<double, 4> sineTable = {0.0, 0.5, 0.866, 1.0};
    std::cout << "sineTable[2] = " << sineTable[2] << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: std::array vs C array vs std::vector\n";
    std::cout << "=====================================================\n";

    // C array — no size info, decays to pointer, unsafe
    int cArray[5] = {1, 2, 3, 4, 5};
    // sizeof gives bytes, not count — fragile:
    std::cout << "C array sizeof/sizeof[0]: " << sizeof(cArray)/sizeof(cArray[0]) << '\n';
    // void func(int arr[]) — size lost! arr becomes pointer

    // std::array — size always known, never decays
    std::array<int, 5> stdArray = {1, 2, 3, 4, 5};
    std::cout << "std::array size(): " << stdArray.size() << " (always known)\n";

    // Comparison with ==
    std::array<int, 3> p = {1, 2, 3};
    std::array<int, 3> q = {1, 2, 3};
    std::array<int, 3> r = {1, 2, 4};
    std::cout << "p == q: " << (p == q) << '\n';   // true
    std::cout << "p == r: " << (p == r) << '\n';   // false
    std::cout << "p <  r: " << (p <  r) << '\n';   // lexicographic

    std::cout << "\n=== ArraySTL complete ===\n";
    return 0;
}
