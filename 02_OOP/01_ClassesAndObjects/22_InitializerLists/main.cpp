#include <print>
#include <string>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <type_traits>
using namespace std;

// ============================================
// WHAT ARE INITIALIZER LISTS?
// ============================================

/*
 * INITIALIZER_LIST (C++11):
 * - std::initializer_list<T> container
 * - Lightweight proxy to array of const T
 * - Enables uniform initialization syntax
 * - Used with braced initialization { }
 *
 * UNIFORM INITIALIZATION:
 * - Type name{values} or Type{values}
 * - Works for all types uniformly
 * - Prevents narrowing conversions
 * - Solves "most vexing parse"
 * - Consistent syntax
 *
 * SYNTAX:
 * - vector<int> v{1, 2, 3, 4, 5};
 * - MyClass obj{arg1, arg2, arg3};
 * - auto list = {1, 2, 3, 4, 5};
 *
 * CONSTRUCTOR WITH INITIALIZER_LIST:
 * - MyClass(initializer_list<T> list)
 * - Called when using braced init
 * - Preferred over other constructors
 * - Enables flexible initialization
 *
 * BENEFITS:
 * - Uniform syntax for all types
 * - Prevents narrowing conversions
 * - Type safe
 * - Solves ambiguity problems
 * - Modern C++ style
 *
 * COMMON USES:
 * - Container initialization
 * - Array-like construction
 * - Variadic initialization
 * - Uniform initialization
 * - Factory functions
 */

// ============================================
// EXAMPLE 1: BASIC INITIALIZER_LIST
// ============================================

void processNumbers(initializer_list<int> numbers) {
    std::print("  Processing {} numbers: ", numbers.size());
    for (int n : numbers) {
        std::print("{} ", n);
    }
    std::println("");
}

void demonstrateBasic() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC USAGE            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Calling function with initializer_list ---");
    processNumbers({1, 2, 3, 4, 5});
    processNumbers({10, 20, 30});
    processNumbers({});  // Empty list

    std::println("");
    std::println("--- Creating initializer_list directly ---");
    initializer_list<double> values = {1.1, 2.2, 3.3, 4.4};
    std::println("  Size: {}", values.size());
    std::print("  Values: ");
    for (double v : values) {
        std::print("{} ", v);
    }
    std::println("");
}

// ============================================
// EXAMPLE 2: UNIFORM INITIALIZATION
// ============================================

class Point {
private:
    int x, y;

public:
    // Regular constructor
    Point(int x, int y) : x(x), y(y) {
        std::println("  Point({}, {})", x, y);
    }

    void display() const {
        std::println("  Point: ({}, {})", x, y);
    }
};

void demonstrateUniform() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: UNIFORM INIT           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Old style (parentheses) ---");
    int a(5);
    Point p1(10, 20);
    vector<int> v1(3, 100);  // 3 elements, each 100

    std::println("");
    std::println("--- New style (braces) ---");
    int b{5};
    Point p2{10, 20};
    vector<int> v2{3, 100};  // 2 elements: 3 and 100
    std::println("  a = {}, b = {}", a, b);
    std::println("  v1 (parentheses): {} elements, v2 (braces): {} elements", v1.size(), v2.size());

    std::println("");
    std::println("--- Works everywhere! ---");
    int c{42};
    double d{3.14};
    string s{"Hello"};
    vector<int> v{1, 2, 3, 4, 5};

    std::println("  c = {}", c);
    std::println("  d = {}", d);
    std::println("  s = {}", s);
    std::print("  v = {{");
    for (int x : v) std::print("{} ", x);
    std::println("}}");
}

// ============================================
// EXAMPLE 3: CONSTRUCTOR WITH INITIALIZER_LIST
// ============================================

class IntList {
private:
    vector<int> data;

public:
    // Constructor taking initializer_list
    IntList(initializer_list<int> list) : data(list) {
        std::println("  IntList with {} elements", list.size());
    }

    // Regular constructor
    IntList(int count, int value) {
        data.resize(count, value);
        std::println("  IntList: {} elements of {}", count, value);
    }

    void display() const {
        std::print("  Data: {{");
        for (int x : data) std::print("{} ", x);
        std::println("}}");
    }

    int sum() const {
        return accumulate(data.begin(), data.end(), 0);
    }
};

void demonstrateConstructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: CONSTRUCTOR            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using initializer_list constructor ---");
    IntList list1{1, 2, 3, 4, 5};
    list1.display();
    std::println("  Sum: {}", list1.sum());

    std::println("");
    std::println("--- Using regular constructor ---");
    IntList list2(3, 100);  // Parentheses call regular constructor
    list2.display();
    std::println("  Sum: {}", list2.sum());

    std::println("");
    std::println("--- Braces prefer initializer_list! ---");
    IntList list3{3, 100};  // Calls initializer_list constructor!
    list3.display();  // Two elements: 3 and 100
    std::println("  Sum: {}", list3.sum());
}

// ============================================
// EXAMPLE 4: NARROWING CONVERSIONS
// ============================================

void demonstrateNarrowing() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: NARROWING              ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Old style allows narrowing ---");
    int a = 3.14;  // ✅ Compiles (truncates to 3)
    int b = 1000000;
    char c = b;    // ✅ Compiles (data loss!)
    std::println("  a = {} (truncated from 3.14)", a);
    std::println("  c = {} (data loss from {})", static_cast<int>(c), b);

    std::println("");
    std::println("--- New style prevents narrowing ---");
    // int d{3.14};     // ❌ ERROR: narrowing conversion
    // char e{1000000}; // ❌ ERROR: narrowing conversion
    std::println("  int d{{3.14}};     // ❌ Compilation error");
    std::println("  char e{{1000000}}; // ❌ Compilation error");

    std::println("");
    std::println("--- Safe conversions OK ---");
    int f{42};        // ✅ OK
    double g{42};     // ✅ OK (widening)
    long h{42};       // ✅ OK
    const int k = 42;
    char kc{k};       // ✅ OK: a CONSTANT that fits is not narrowing
    std::println("  f = {}", f);
    std::println("  g = {}", g);
    std::println("  h = {}", h);
    std::println("  kc = {}", static_cast<int>(kc));
}

// ============================================
// EXAMPLE 5: MOST VEXING PARSE
// ============================================

class Widget {
public:
    Widget() {
        std::println("  Widget default constructor");
    }

    explicit Widget(int x) {
        std::println("  Widget({})", x);
    }
};

void demonstrateMostVexingParse() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MOST VEXING PARSE      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- The problem with parentheses ---");
    // Widget w1();  // ❌ NOT a default construction!
    //               // It's a FUNCTION DECLARATION!
    std::println("  Widget w1();  // ❌ Function declaration, not object!");

    std::println("");
    std::println("--- Solution 1: No parentheses ---");
    Widget w2;  // ✅ Default construction

    std::println("");
    std::println("--- Solution 2: Braces (C++11) ---");
    Widget w3{};  // ✅ Default construction (uniform init)

    std::println("");
    std::println("--- With arguments ---");
    Widget w4{42};  // ✅ Clear initialization

    std::println("");
    std::println("--- Function-style initialization ---");
    Widget w5 = Widget();  // ✅ Works but verbose
    (void)w5;
}

// ============================================
// EXAMPLE 6: CONTAINER INITIALIZATION
// ============================================

void demonstrateContainers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: CONTAINERS             ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Vector initialization ---");
    vector<int> v1{1, 2, 3, 4, 5};
    vector<string> v2{"apple", "banana", "cherry"};

    std::print("  v1: ");
    for (int x : v1) std::print("{} ", x);
    std::println("");

    std::print("  v2: ");
    for (const auto& s : v2) std::print("{} ", s);
    std::println("");

    std::println("");
    std::println("--- Note the difference! ---");
    vector<int> v3(3, 100);  // 3 elements, each 100
    vector<int> v4{3, 100};  // 2 elements: 3 and 100

    std::print("  v3 (parentheses): ");
    for (int x : v3) std::print("{} ", x);
    std::println("");

    std::print("  v4 (braces): ");
    for (int x : v4) std::print("{} ", x);
    std::println("");

    std::println("");
    std::println("--- Nested initialization ---");
    vector<vector<int>> matrix{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::println("  Matrix:");
    for (const auto& row : matrix) {
        std::print("    ");
        for (int x : row) std::print("{} ", x);
        std::println("");
    }
}

// ============================================
// EXAMPLE 7: CUSTOM CLASS WITH INITIALIZER_LIST
// ============================================

class Matrix {
private:
    vector<vector<int>> data;
    int rows, cols;

public:
    // Constructor with initializer_list
    Matrix(initializer_list<initializer_list<int>> list) {
        rows = list.size();
        cols = rows > 0 ? list.begin()->size() : 0;

        for (const auto& row : list) {
            if (row.size() != static_cast<size_t>(cols)) {
                throw invalid_argument("all rows must have the same size");
            }
        }

        std::println("  Matrix({}×{})", rows, cols);

        for (const auto& row : list) {
            data.push_back(vector<int>(row));
        }
    }

    void display() const {
        std::println("  Matrix {}×{}:", rows, cols);
        for (const auto& row : data) {
            std::print("    [");
            for (size_t i = 0; i < row.size(); ++i) {
                std::print("{}", row[i]);
                if (i < row.size() - 1) std::print(", ");
            }
            std::println("]");
        }
    }

    int sum() const {
        int total = 0;
        for (const auto& row : data) {
            total += accumulate(row.begin(), row.end(), 0);
        }
        return total;
    }
};

void demonstrateCustomClass() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: CUSTOM CLASS           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating matrix with initializer_list ---");
    Matrix m1{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    m1.display();
    std::println("  Sum: {}", m1.sum());

    std::println("");
    std::println("--- Different sized matrix ---");
    Matrix m2{
        {10, 20},
        {30, 40},
        {50, 60},
        {70, 80}
    };
    m2.display();
    std::println("  Sum: {}", m2.sum());

    std::println("");
    std::println("--- Rows of different sizes are rejected ---");
    try {
        Matrix bad{{1, 2, 3}, {4, 5}};
    } catch (const invalid_argument& e) {
        std::println("  Caught: {}", e.what());
    }
}

// ============================================
// EXAMPLE 8: RETURN VALUE INITIALIZATION
// ============================================

vector<int> createVector() {
    return {1, 2, 3, 4, 5};  // Return with initializer_list
}

vector<string> getNames() {
    return {"Alice", "Bob", "Charlie"};
}

void demonstrateReturn() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: RETURN VALUES          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Returning with initializer_list ---");
    auto numbers = createVector();
    std::print("  Numbers: ");
    for (int n : numbers) std::print("{} ", n);
    std::println("");

    auto names = getNames();
    std::print("  Names: ");
    for (const auto& name : names) std::print("{} ", name);
    std::println("");
}

// ============================================
// EXAMPLE 9: MIN/MAX WITH INITIALIZER_LIST
// ============================================

void demonstrateMinMax() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: MIN/MAX                ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using std::min and std::max ---");
    int minimum = std::min({5, 2, 8, 1, 9, 3});
    int maximum = std::max({5, 2, 8, 1, 9, 3});

    std::println("  Min of {{5, 2, 8, 1, 9, 3}}: {}", minimum);
    std::println("  Max of {{5, 2, 8, 1, 9, 3}}: {}", maximum);

    std::println("");
    std::println("--- With strings ---");
    // Without <string> the list would be initializer_list<const char*> and min/max would
    // compare the POINTERS (unspecified result). <string> makes it compare the texts.
    string first = std::min<string>({"zebra", "apple", "banana", "cherry"});
    string last = std::max<string>({"zebra", "apple", "banana", "cherry"});

    std::println("  First alphabetically: {}", first);
    std::println("  Last alphabetically: {}", last);
}

// ============================================
// EXAMPLE 10: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: BEST PRACTICES        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- auto and braces ---");
    auto a = {1, 2, 3};  // std::initializer_list<int>
    auto b{1};           // int (C++17 rules)
    auto c = {1};        // std::initializer_list<int>
    // auto d{1, 2};     // ❌ Error: direct-list-initialization of 'auto' requires exactly one element
    std::println("  auto a = {{1, 2, 3}} is initializer_list<int>: {}", is_same_v<decltype(a), initializer_list<int>>);
    std::println("  auto b{{1}}          is int:                   {}", is_same_v<decltype(b), int>);
    std::println("  auto c = {{1}}       is initializer_list<int>: {}", is_same_v<decltype(c), initializer_list<int>>);

    std::println("");
    std::println("--- Braces fall back to other constructors ---");
    vector<string> s1(2, "x");  // 2 elements
    vector<string> s2{2, "x"};  // 2 cannot become a string -> the initializer_list constructor is not viable
    std::println("  vector<string>(2, \"x\"): {} elements, vector<string>{{2, \"x\"}}: {} elements", s1.size(), s2.size());

    vector<int> i1(3, 7);
    vector<int> i2{3, 7};  // here 3 and 7 ARE ints: the initializer_list constructor wins
    std::println("  vector<int>(3, 7):     {} elements, vector<int>{{3, 7}}:     {} elements", i1.size(), i2.size());

    std::println("");
    std::println("--- Empty braces ---");
    int zero{};          // value-initialized: 0
    IntList emptyList{}; // no default constructor: the initializer_list constructor gets an empty list
    std::println("  int zero{{}} = {}", zero);
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║          INITIALIZER LISTS IN C++                        ║");
    std::println("║          Uniform Initialization (C++11)                   ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

    demonstrateBasic();
    demonstrateUniform();
    demonstrateConstructor();
    demonstrateNarrowing();
    demonstrateMostVexingParse();
    demonstrateContainers();
    demonstrateCustomClass();
    demonstrateReturn();
    demonstrateMinMax();
    demonstrateBestPractices();

    return 0;
}
