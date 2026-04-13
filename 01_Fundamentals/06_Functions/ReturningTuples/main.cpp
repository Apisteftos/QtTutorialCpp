#include <iostream>
#include <tuple>
#include <utility>
#include <print>

// ============================================================
// RETURNING MULTIPLE VALUES FROM A FUNCTION in C++23
// ============================================================
// Topics covered:
//   1.  Output parameters via references
//   2.  std::pair   — two values
//   3.  std::tuple  — three or more values
//   4.  std::tuple  + std::get<N>
//   5.  std::tie    — C++11 unpacking into existing variables
//   6.  Structured bindings — C++17 auto [a, b, c]
//   7.  Local struct as return type (named fields)
//   8.  Struct with methods as return type
//   9.  Comparison of all approaches
// ============================================================


// ============================================================
// Helper functions used across examples
// ============================================================

// Example 1 — output parameters via references
void getValues_byRef(int& intValue, char& charValue, double& doubleValue)
{
    intValue   = 10;
    charValue  = 'A';
    doubleValue = 3.14;
}

// Example 2 — std::pair (two values only)
std::pair<int, double> getValues_pair()
{
    return std::make_pair(10, 3.14);
    // or simply: return {10, 3.14};
}

// Example 3 — std::tuple (any number of values)
std::tuple<int, char, double> getValues_tuple()
{
    return std::make_tuple(10, 'A', 3.14);
    // or simply: return {10, 'A', 3.14};
}

// Example 5 — std::tie — C++11 unpacking
std::tuple<int, int> mathOps(int a, int b)
{
    return {a + b, a * b};
}

// Example 7 — local struct as return type
struct Point3D {
    double x, y, z;
};

Point3D getOrigin()
{
    return {1.0, 2.5, -0.5};
}

// Example 8 — struct with constructor and methods
struct Stats {
    int    count;
    double average;
    int    min;
    int    max;

    Stats(int c, double avg, int mn, int mx)
        : count(c), average(avg), min(mn), max(mx) {}

    void print() const {
        std::println("  count={} avg={:.2f} min={} max={}",
                     count, average, min, max);
    }
};

Stats computeStats(const int arr[], int size)
{
    int   mn  = arr[0], mx = arr[0], sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
        if (arr[i] < mn) mn = arr[i];
        if (arr[i] > mx) mx = arr[i];
    }
    return Stats(size, (double)sum / size, mn, mx);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Output parameters via references\n";
    std::cout << "=====================================================\n";

    {
        int    intVal;
        char   charVal;
        double doubleVal;

        getValues_byRef(intVal, charVal, doubleVal);

        std::println("intVal:    {}", intVal);
        std::println("charVal:   {}", charVal);
        std::println("doubleVal: {}", doubleVal);

        std::cout << "\nPros: simple, no extra types needed\n";
        std::cout << "Cons: caller must pre-declare variables, verbose\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: std::pair — two values\n";
    std::cout << "=====================================================\n";

    {
        std::pair<int, double> result = getValues_pair();

        // Access via .first and .second
        std::println("first (int):    {}", result.first);
        std::println("second (double):{}", result.second);

        // C++17 structured binding — cleaner
        auto [intVal, doubleVal] = getValues_pair();
        std::println("structured: int={} double={}", intVal, doubleVal);

        std::cout << "\nPros: simple for exactly two values\n";
        std::cout << "Cons: .first/.second not descriptive — use struct for clarity\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: std::tuple — three or more values\n";
    std::cout << "=====================================================\n";

    {
        std::tuple<int, char, double> result = getValues_tuple();

        // Access via std::get<index>
        int    intVal    = std::get<0>(result);
        char   charVal   = std::get<1>(result);
        double doubleVal = std::get<2>(result);

        std::println("get<0> int:    {}", intVal);
        std::println("get<1> char:   {}", charVal);
        std::println("get<2> double: {}", doubleVal);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: std::tuple + auto return\n";
    std::cout << "=====================================================\n";

    {
        // auto deduces the tuple type
        auto result = getValues_tuple();

        std::println("auto get<0>: {}", std::get<0>(result));
        std::println("auto get<1>: {}", std::get<1>(result));
        std::println("auto get<2>: {}", std::get<2>(result));

        std::cout << "\nPros: works for any number of values\n";
        std::cout << "Cons: get<0>, get<1> not readable — prefer struct or structured binding\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: std::tie — C++11 unpacking\n";
    std::cout << "=====================================================\n";

    {
        // tie() unpacks tuple into existing variables
        int sum, product;
        std::tie(sum, product) = mathOps(23, 44);

        std::println("sum:     {}", sum);
        std::println("product: {}", product);
        std::println("sum + product = {}", sum + product);

        // std::ignore — skip a value
        int sumOnly;
        std::tie(sumOnly, std::ignore) = mathOps(10, 5);
        std::println("sumOnly (ignore product): {}", sumOnly);

        std::cout << "\nNote: tie() requires variables declared beforehand\n";
        std::cout << "      C++17 structured bindings are cleaner\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Structured bindings — C++17 (preferred)\n";
    std::cout << "=====================================================\n";

    {
        // auto [a, b] — declare and unpack in one line
        auto [sum, product] = mathOps(23, 44);
        std::println("sum:     {}", sum);
        std::println("product: {}", product);
        std::println("dividing: {}", sum / product);

        // Works with pair too
        auto [intVal, doubleVal] = getValues_pair();
        std::println("pair: int={} double={}", intVal, doubleVal);

        // Works with tuple
        auto [i, c, d] = getValues_tuple();
        std::println("tuple: int={} char={} double={}", i, c, d);

        std::cout << "\nPros: cleanest syntax, no index numbers, descriptive names\n";
        std::cout << "Cons: requires C++17 or later\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Local struct as return type (named fields)\n";
    std::cout << "=====================================================\n";

    {
        Point3D p = getOrigin();

        std::println("x = {}", p.x);
        std::println("y = {}", p.y);
        std::println("z = {}", p.z);

        // Structured binding works on structs too
        auto [x, y, z] = getOrigin();
        std::println("structured: x={} y={} z={}", x, y, z);

        std::cout << "\nPros: named fields — self-documenting code\n";
        std::cout << "Cons: need to define struct outside function\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Struct with constructor and methods\n";
    std::cout << "=====================================================\n";

    {
        int data[] = {3, 1, 8, 5, 2, 9, 4, 7, 6};
        int size   = sizeof(data) / sizeof(data[0]);

        Stats s = computeStats(data, size);
        s.print();

        std::println("count:   {}", s.count);
        std::println("average: {:.2f}", s.average);
        std::println("min:     {}", s.min);
        std::println("max:     {}", s.max);

        std::cout << "\nPros: most readable, extensible, can add methods\n";
        std::cout << "Best choice for complex multi-value returns\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Comparison of all approaches\n";
    std::cout << "=====================================================\n";

    std::cout << "Method                | Syntax              | Readability | When to use\n";
    std::cout << "----------------------|---------------------|-------------|------------\n";
    std::cout << "Output refs           | fn(int& a, int& b)  | Low         | C legacy, perf-critical\n";
    std::cout << "std::pair             | pair<int,double>    | Medium      | Exactly 2 values\n";
    std::cout << "std::tuple            | tuple<int,char,...> | Low         | Quick multi-return\n";
    std::cout << "std::tie (C++11)      | tie(a,b) = fn()     | Medium      | Unpack into existing vars\n";
    std::cout << "Structured bind C++17 | auto [a,b] = fn()   | High        | Modern, any tuple/struct\n";
    std::cout << "struct (named fields) | struct Result {...} | Highest     | Best practice\n";
    std::cout << "struct + methods      | Result.compute()    | Highest     | Complex return types\n";
    std::cout << '\n';
    std::cout << "Recommendation:\n";
    std::cout << "  2 values:  std::pair  + structured binding\n";
    std::cout << "  3+ values: struct     + structured binding\n";
    std::cout << "  Complex:   struct with named fields and methods\n";

    std::cout << "\n=== Returning Multiple Values complete ===\n";
    return 0;
}
