#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <numeric>
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
    cout << "  Processing " << numbers.size() << " numbers: ";
    for (int n : numbers) {
        cout << n << " ";
    }
    cout << "\n";
}

void demonstrateBasic() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC USAGE            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Calling function with initializer_list ---\n";
    processNumbers({1, 2, 3, 4, 5});
    processNumbers({10, 20, 30});
    processNumbers({});  // Empty list

    cout << "\n--- Creating initializer_list directly ---\n";
    initializer_list<double> values = {1.1, 2.2, 3.3, 4.4};
    cout << "  Size: " << values.size() << "\n";
    cout << "  Values: ";
    for (double v : values) {
        cout << v << " ";
    }
    cout << "\n";

    cout << "\n💡 INITIALIZER_LIST:\n";
    cout << "   • Lightweight proxy\n";
    cout << "   • Points to array of const T\n";
    cout << "   • No ownership (shallow copy)\n";
    cout << "   • Used with braced init { }\n";
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
        cout << "  Point(" << x << ", " << y << ")\n";
    }

    void display() const {
        cout << "  Point: (" << x << ", " << y << ")\n";
    }
};

void demonstrateUniform() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: UNIFORM INIT           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Old style (parentheses) ---\n";
    int a(5);
    Point p1(10, 20);
    vector<int> v1(3, 100);  // 3 elements, each 100

    cout << "\n--- New style (braces) ---\n";
    int b{5};
    Point p2{10, 20};
    vector<int> v2{3, 100};  // 2 elements: 3 and 100

    cout << "\n--- Works everywhere! ---\n";
    int c{42};
    double d{3.14};
    string s{"Hello"};
    vector<int> v{1, 2, 3, 4, 5};

    cout << "  c = " << c << "\n";
    cout << "  d = " << d << "\n";
    cout << "  s = " << s << "\n";
    cout << "  v = {";
    for (int x : v) cout << x << " ";
    cout << "}\n";

    cout << "\n✅ UNIFORM INITIALIZATION:\n";
    cout << "   • Same syntax for all types\n";
    cout << "   • Consistent and predictable\n";
    cout << "   • Modern C++ style\n";
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
        cout << "  IntList with " << list.size() << " elements\n";
    }

    // Regular constructor
    IntList(int count, int value) {
        data.resize(count, value);
        cout << "  IntList: " << count << " elements of " << value << "\n";
    }

    void display() const {
        cout << "  Data: {";
        for (int x : data) cout << x << " ";
        cout << "}\n";
    }

    int sum() const {
        return accumulate(data.begin(), data.end(), 0);
    }
};

void demonstrateConstructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: CONSTRUCTOR            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using initializer_list constructor ---\n";
    IntList list1{1, 2, 3, 4, 5};
    list1.display();
    cout << "  Sum: " << list1.sum() << "\n";

    cout << "\n--- Using regular constructor ---\n";
    IntList list2(3, 100);  // Parentheses call regular constructor
    list2.display();
    cout << "  Sum: " << list2.sum() << "\n";

    cout << "\n--- Braces prefer initializer_list! ---\n";
    IntList list3{3, 100};  // Calls initializer_list constructor!
    list3.display();  // Two elements: 3 and 100
    cout << "  Sum: " << list3.sum() << "\n";

    cout << "\n⚠️  IMPORTANT:\n";
    cout << "   • Braces {} prefer initializer_list constructor\n";
    cout << "   • Parentheses () call other constructors\n";
    cout << "   • Can be confusing!\n";
}

// ============================================
// EXAMPLE 4: NARROWING CONVERSIONS
// ============================================

void demonstrateNarrowing() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: NARROWING              ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Old style allows narrowing ---\n";
    int a = 3.14;  // ✅ Compiles (truncates to 3)
    int b = 1000000;
    char c = b;    // ✅ Compiles (data loss!)
    cout << "  a = " << a << " (truncated from 3.14)\n";
    cout << "  c = " << (int)c << " (data loss from " << b << ")\n";

    cout << "\n--- New style prevents narrowing ---\n";
    // int d{3.14};     // ❌ ERROR: narrowing conversion
    // char e{1000000}; // ❌ ERROR: narrowing conversion
    cout << "  int d{3.14};     // ❌ Compilation error\n";
    cout << "  char e{1000000}; // ❌ Compilation error\n";

    cout << "\n--- Safe conversions OK ---\n";
    int f{42};        // ✅ OK
    double g{42};     // ✅ OK (widening)
    long h{42};       // ✅ OK
    cout << "  f = " << f << "\n";
    cout << "  g = " << g << "\n";
    cout << "  h = " << h << "\n";

    cout << "\n✅ NARROWING PREVENTION:\n";
    cout << "   • Braced init prevents data loss\n";
    cout << "   • Compile-time safety\n";
    cout << "   • Catches bugs early\n";
    cout << "   • Type safe!\n";
}

// ============================================
// EXAMPLE 5: MOST VEXING PARSE
// ============================================

class Widget {
public:
    Widget() {
        cout << "  Widget default constructor\n";
    }

    Widget(int x) {
        cout << "  Widget(" << x << ")\n";
    }
};

Widget createWidget() {
    return Widget();
}

void demonstrateMostVexingParse() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MOST VEXING PARSE      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- The problem with parentheses ---\n";
    // Widget w1();  // ❌ NOT a default construction!
    //               // It's a FUNCTION DECLARATION!
    cout << "  Widget w1();  // ❌ Function declaration, not object!\n";

    cout << "\n--- Solution 1: No parentheses ---\n";
    Widget w2;  // ✅ Default construction

    cout << "\n--- Solution 2: Braces (C++11) ---\n";
    Widget w3{};  // ✅ Default construction (uniform init)

    cout << "\n--- With arguments ---\n";
    Widget w4{42};  // ✅ Clear initialization

    cout << "\n--- Function-style initialization ---\n";
    Widget w5 = Widget();  // ✅ Works but verbose

    cout << "\n💡 MOST VEXING PARSE:\n";
    cout << "   • Widget w();  → function declaration\n";
    cout << "   • Widget w{};  → object creation\n";
    cout << "   • Braces solve the ambiguity!\n";
}

// ============================================
// EXAMPLE 6: CONTAINER INITIALIZATION
// ============================================

void demonstrateContainers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: CONTAINERS             ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Vector initialization ---\n";
    vector<int> v1{1, 2, 3, 4, 5};
    vector<string> v2{"apple", "banana", "cherry"};

    cout << "  v1: ";
    for (int x : v1) cout << x << " ";
    cout << "\n";

    cout << "  v2: ";
    for (const auto& s : v2) cout << s << " ";
    cout << "\n";

    cout << "\n--- Note the difference! ---\n";
    vector<int> v3(3, 100);  // 3 elements, each 100
    vector<int> v4{3, 100};  // 2 elements: 3 and 100

    cout << "  v3 (parentheses): ";
    for (int x : v3) cout << x << " ";
    cout << "\n";

    cout << "  v4 (braces): ";
    for (int x : v4) cout << x << " ";
    cout << "\n";

    cout << "\n--- Nested initialization ---\n";
    vector<vector<int>> matrix{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    cout << "  Matrix:\n";
    for (const auto& row : matrix) {
        cout << "    ";
        for (int x : row) cout << x << " ";
        cout << "\n";
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

        cout << "  Matrix(" << rows << "×" << cols << ")\n";

        for (const auto& row : list) {
            data.push_back(vector<int>(row));
        }
    }

    void display() const {
        cout << "  Matrix " << rows << "×" << cols << ":\n";
        for (const auto& row : data) {
            cout << "    [";
            for (size_t i = 0; i < row.size(); ++i) {
                cout << row[i];
                if (i < row.size() - 1) cout << ", ";
            }
            cout << "]\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: CUSTOM CLASS           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating matrix with initializer_list ---\n";
    Matrix m1{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    m1.display();
    cout << "  Sum: " << m1.sum() << "\n";

    cout << "\n--- Different sized matrix ---\n";
    Matrix m2{
        {10, 20},
        {30, 40},
        {50, 60},
        {70, 80}
    };
    m2.display();
    cout << "  Sum: " << m2.sum() << "\n";

    cout << "\n✅ CUSTOM INITIALIZER_LIST:\n";
    cout << "   • Natural syntax for construction\n";
    cout << "   • Works with nested lists\n";
    cout << "   • Flexible initialization\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: RETURN VALUES          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Returning with initializer_list ---\n";
    auto numbers = createVector();
    cout << "  Numbers: ";
    for (int n : numbers) cout << n << " ";
    cout << "\n";

    auto names = getNames();
    cout << "  Names: ";
    for (const auto& name : names) cout << name << " ";
    cout << "\n";

    cout << "\n💡 RETURN INITIALIZATION:\n";
    cout << "   • Clean return syntax\n";
    cout << "   • Type deduction works\n";
    cout << "   • Efficient (move/copy elision)\n";
}

// ============================================
// EXAMPLE 9: MIN/MAX WITH INITIALIZER_LIST
// ============================================

void demonstrateMinMax() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: MIN/MAX                ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using std::min and std::max ---\n";
    int minimum = std::min({5, 2, 8, 1, 9, 3});
    int maximum = std::max({5, 2, 8, 1, 9, 3});

    cout << "  Min of {5, 2, 8, 1, 9, 3}: " << minimum << "\n";
    cout << "  Max of {5, 2, 8, 1, 9, 3}: " << maximum << "\n";

    cout << "\n--- With strings ---\n";
    string first = std::min({"zebra", "apple", "banana", "cherry"});
    string last = std::max({"zebra", "apple", "banana", "cherry"});

    cout << "  First alphabetically: " << first << "\n";
    cout << "  Last alphabetically: " << last << "\n";

    cout << "\n💡 STL FUNCTIONS:\n";
    cout << "   • Many STL functions accept initializer_list\n";
    cout << "   • Convenient for multiple arguments\n";
}

// ============================================
// EXAMPLE 10: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: BEST PRACTICES        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "✅ PREFER BRACES FOR:\n";
    cout << "   • Variable initialization\n";
    cout << "   • Container creation\n";
    cout << "   • Return values\n";
    cout << "   • When you want narrowing prevention\n";
    cout << "\n";
    cout << "   int x{42};           // ✅ Recommended\n";
    cout << "   vector<int> v{1,2};  // ✅ Clear intent\n";
    cout << "\n";

    cout << "✅ USE PARENTHESES FOR:\n";
    cout << "   • When you need specific constructor\n";
    cout << "   • When braces would be ambiguous\n";
    cout << "   • Traditional function calls\n";
    cout << "\n";
    cout << "   vector<int> v(100, 0);  // ✅ 100 zeros\n";
    cout << "   Widget w(42);           // ✅ Clear\n";
    cout << "\n";

    cout << "⚠️  WATCH OUT FOR:\n";
    cout << "   • vector<int> v{100, 0};  // 2 elements!\n";
    cout << "   • vector<int> v(100, 0);  // 100 elements!\n";
    cout << "   • Braces prefer initializer_list constructor\n";
    cout << "\n";

    cout << "💡 GENERAL GUIDELINES:\n";
    cout << "   • Use braces as default\n";
    cout << "   • Use parentheses when needed\n";
    cout << "   • Be aware of initializer_list priority\n";
    cout << "   • Test with both if unsure\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║          INITIALIZER LISTS IN C++                        ║\n";
    cout << "║          Uniform Initialization (C++11)                   ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

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

    // ============================================
    // COMPREHENSIVE EXPLANATION
    // ============================================

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              COMPREHENSIVE EXPLANATION                    ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n========================================\n";
    cout << "WHAT IS INITIALIZER_LIST?\n";
    cout << "========================================\n";
    cout << "std::initializer_list<T>:\n";
    cout << "  • Lightweight proxy to array of const T\n";
    cout << "  • Added in C++11\n";
    cout << "  • Enables uniform initialization\n";
    cout << "  • Used with braced init { }\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "// Function parameter\n";
    cout << "void func(initializer_list<int> list) {\n";
    cout << "    for (int x : list) { }\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Constructor\n";
    cout << "class MyClass {\n";
    cout << "    MyClass(initializer_list<int> list) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage\n";
    cout << "func({1, 2, 3, 4, 5});\n";
    cout << "MyClass obj{1, 2, 3};\n";
    cout << "vector<int> v{1, 2, 3, 4, 5};\n";

    cout << "\n========================================\n";
    cout << "UNIFORM INITIALIZATION\n";
    cout << "========================================\n";
    cout << "Before C++11 (inconsistent):\n";
    cout << "  int x = 5;              // Assignment\n";
    cout << "  int y(10);              // Direct init\n";
    cout << "  vector<int> v;          // Default\n";
    cout << "  Point p(1, 2);          // Constructor\n";
    cout << "  int arr[] = {1,2,3};    // Array\n";
    cout << "\n";
    cout << "C++11+ (uniform with braces):\n";
    cout << "  int x{5};               // ✅ Same syntax\n";
    cout << "  int y{10};              // ✅ Same syntax\n";
    cout << "  vector<int> v{1,2,3};   // ✅ Same syntax\n";
    cout << "  Point p{1, 2};          // ✅ Same syntax\n";
    cout << "  int arr[]{1,2,3};       // ✅ Same syntax\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • Uniform syntax everywhere\n";
    cout << "  • Prevents narrowing conversions\n";
    cout << "  • Solves most vexing parse\n";
    cout << "  • Type safe\n";
    cout << "  • Modern C++ style\n";

    cout << "\n========================================\n";
    cout << "NARROWING CONVERSIONS\n";
    cout << "========================================\n";
    cout << "Old style allows data loss:\n";
    cout << "  int x = 3.14;      // ✅ Compiles, truncates\n";
    cout << "  char c = 1000000;  // ✅ Compiles, overflow\n";
    cout << "  // Silent data loss! 💥\n";
    cout << "\n";
    cout << "Braced init prevents:\n";
    cout << "  int x{3.14};       // ❌ Compilation error\n";
    cout << "  char c{1000000};   // ❌ Compilation error\n";
    cout << "  // Catches bugs at compile time! ✅\n";
    cout << "\n";
    cout << "NARROWING RULES:\n";
    cout << "  • Floating to integer → ERROR\n";
    cout << "  • Integer to narrower type → ERROR (if overflow)\n";
    cout << "  • Double to float → ERROR (if out of range)\n";
    cout << "  • Widening conversions → OK\n";

    cout << "\n========================================\n";
    cout << "MOST VEXING PARSE\n";
    cout << "========================================\n";
    cout << "THE PROBLEM:\n";
    cout << "class Widget {\n";
    cout << "    Widget() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Widget w();  // What is this?\n";
    cout << "\n";
    cout << "// Is it:\n";
    cout << "// A) Default construction of Widget?\n";
    cout << "// B) Function declaration?\n";
    cout << "\n";
    cout << "// Answer: B! It's a function declaration!\n";
    cout << "// Widget w() declares a function named w\n";
    cout << "// that takes no parameters and returns Widget\n";
    cout << "\n";
    cout << "SOLUTIONS:\n";
    cout << "Widget w1;     // ✅ No parentheses\n";
    cout << "Widget w2{};   // ✅ Braces (C++11)\n";
    cout << "Widget w3 = Widget();  // ✅ Verbose but works\n";
    cout << "\n";
    cout << "Braces solve the ambiguity!\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR PRIORITY\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "    MyClass(int a, int b) { }  // Regular\n";
    cout << "    MyClass(initializer_list<int> list) { }  // List\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj1(3, 5);    // Calls regular constructor\n";
    cout << "MyClass obj2{3, 5};    // Calls initializer_list! ⚠️\n";
    cout << "\n";
    cout << "PRIORITY RULE:\n";
    cout << "  • Braces {} prefer initializer_list constructor\n";
    cout << "  • Even if other constructors match better\n";
    cout << "  • To avoid, use parentheses ()\n";
    cout << "\n";
    cout << "EXAMPLE:\n";
    cout << "vector<int> v1(100, 0);  // 100 zeros\n";
    cout << "vector<int> v2{100, 0};  // 2 elements: 100, 0\n";
    cout << "// Different results! ⚠️\n";

    cout << "\n========================================\n";
    cout << "INITIALIZER_LIST PROPERTIES\n";
    cout << "========================================\n";
    cout << "CHARACTERISTICS:\n";
    cout << "  • Lightweight proxy\n";
    cout << "  • Points to const array\n";
    cout << "  • No ownership (shallow copy)\n";
    cout << "  • Size known at compile time\n";
    cout << "  • Efficient (no allocation)\n";
    cout << "\n";
    cout << "MEMBERS:\n";
    cout << "  • size()   → number of elements\n";
    cout << "  • begin()  → iterator to first\n";
    cout << "  • end()    → iterator past last\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "void func(initializer_list<int> list) {\n";
    cout << "    cout << list.size();  // Number of elements\n";
    cout << "    for (int x : list) {  // Range-for works\n";
    cout << "        // Process x\n";
    cout << "    }\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE WHICH\n";
    cout << "========================================\n";
    cout << "USE BRACES { } FOR:\n";
    cout << "  • Default (uniform initialization)\n";
    cout << "  • Container initialization\n";
    cout << "  • When you want narrowing prevention\n";
    cout << "  • Most modern C++ code\n";
    cout << "\n";
    cout << "  int x{42};              // ✅ Default choice\n";
    cout << "  vector<int> v{1,2,3};   // ✅ Clear intent\n";
    cout << "  string s{\"hello\"};     // ✅ Uniform\n";
    cout << "\n";
    cout << "USE PARENTHESES ( ) FOR:\n";
    cout << "  • When braces would call wrong constructor\n";
    cout << "  • Traditional constructor calls\n";
    cout << "  • Function calls\n";
    cout << "  • When you need specific overload\n";
    cout << "\n";
    cout << "  vector<int> v(100, 0);  // ✅ 100 zeros\n";
    cout << "  Widget w(arg);          // ✅ Specific ctor\n";
    cout << "\n";
    cout << "USE ASSIGNMENT = FOR:\n";
    cout << "  • Copy initialization\n";
    cout << "  • Traditional style\n";
    cout << "  • Legacy code\n";
    cout << "\n";
    cout << "  int x = 42;             // ✅ Traditional\n";
    cout << "  string s = \"hello\";    // ✅ Readable\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use braces as default\n";
    cout << "  • Provide initializer_list constructors\n";
    cout << "  • Be aware of constructor priority\n";
    cout << "  • Use uniform initialization style\n";
    cout << "  • Take advantage of narrowing prevention\n";
    cout << "  • Use for container initialization\n";
    cout << "  • Use for return values\n";
    cout << "  • Document constructor choices\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Mix initialization styles randomly\n";
    cout << "  • Forget initializer_list priority\n";
    cout << "  • Assume braces always better\n";
    cout << "  • Use when parentheses clearer\n";
    cout << "  • Forget about narrowing\n";
    cout << "  • Use old-style arrays\n";
    cout << "  • Ignore most vexing parse\n";
    cout << "  • Be dogmatic about style\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Uniform initialization\n";
    cout << "int x{42};              // Variable\n";
    cout << "vector<int> v{1,2,3};   // Container\n";
    cout << "Point p{10, 20};        // Class\n";
    cout << "int arr[]{1,2,3};       // Array\n";
    cout << "\n";
    cout << "// Function with initializer_list\n";
    cout << "void func(initializer_list<int> list) {\n";
    cout << "    for (int x : list) { }\n";
    cout << "}\n";
    cout << "func({1, 2, 3, 4, 5});\n";
    cout << "\n";
    cout << "// Constructor\n";
    cout << "class MyClass {\n";
    cout << "    MyClass(initializer_list<int> list) { }\n";
    cout << "};\n";
    cout << "MyClass obj{1, 2, 3};\n";
    cout << "\n";
    cout << "// Return value\n";
    cout << "vector<int> create() {\n";
    cout << "    return {1, 2, 3, 4, 5};\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. initializer_list enables uniform initialization\n";
    cout << "2. Braces { } work for all types uniformly\n";
    cout << "3. Prevents narrowing conversions\n";
    cout << "4. Solves most vexing parse problem\n";
    cout << "5. Braces prefer initializer_list constructor\n";
    cout << "6. Use braces as default, parentheses when needed\n";
    cout << "7. Be aware of constructor priority\n";
    cout << "8. Modern C++ style uses braces\n";
    cout << "9. Safer than old-style initialization\n";
    cout << "10. Test when behavior unclear\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Prefer braces as default\n";
    cout << "   Modern uniform initialization!\n";
    cout << "   \n";
    cout << "   int x{42};  // Not int x = 42;\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Watch constructor priority\n";
    cout << "   Braces prefer initializer_list!\n";
    cout << "   \n";
    cout << "   vector<int> v{3, 5};  // 2 elements\n";
    cout << "   vector<int> v(3, 5);  // 3 elements of 5\n";
    cout << "\n";
    cout << "⚠️  RULE 3: Use for narrowing prevention\n";
    cout << "   Catches bugs at compile time!\n";
    cout << "   \n";
    cout << "   int x{3.14};  // ❌ Error\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Provide initializer_list constructors\n";
    cout << "   For container-like classes!\n";
    cout << "   \n";
    cout << "   MyClass(initializer_list<T> list) { }\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Be pragmatic\n";
    cout << "   Use parentheses when clearer!\n";
    cout << "   \n";
    cout << "   vector<int> v(100, 0);  // Clearer intent\n";

    return 0;
}
