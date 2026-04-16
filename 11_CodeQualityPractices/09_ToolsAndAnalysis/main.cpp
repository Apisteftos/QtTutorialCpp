#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cstring>
using namespace std;

// ============================================
// TOOLS AND ANALYSIS
// ============================================

/*
C++ TOOLS AND ANALYSIS:
    "The right tools make quality code easier to achieve and maintain.
     Use static analysis, sanitizers, and formatters to catch issues
     before they become bugs."
    — Modern C++ Best Practice

WHAT THEY ARE:
    • Automated code quality checkers
    • Runtime error detectors
    • Code formatting tools
    • Performance analyzers
    • Documentation generators

WHY THEY MATTER:
    ✅ Catch bugs before runtime
    ✅ Enforce coding standards
    ✅ Find memory leaks and UB
    ✅ Improve code consistency
    ✅ Reduce code review time
    ✅ Document code automatically

CATEGORIES:
    1. Linters (clang-tidy)
    2. Formatters (clang-format)
    3. Static Analyzers (cppcheck, PVS-Studio)
    4. Sanitizers (ASan, UBSan, TSan, MSan)
    5. Memory Analyzers (Valgrind)
    6. Coverage Tools (gcov, lcov)
    7. Profilers (gprof, perf)
    8. Documentation (Doxygen)

INTEGRATION:
    • CI/CD pipelines
    • Pre-commit hooks
    • IDE integration
    • Build systems (CMake)
    • Continuous monitoring
*/

// ============================================
// EXAMPLE 1: CLANG-TIDY (LINTER)
// ============================================

namespace clang_tidy_example {
// ❌ BAD: Code with issues clang-tidy would catch

class BadClass {
public:
    // 🚨 clang-tidy: use '= default'
    BadClass() {}

    // 🚨 clang-tidy: missing virtual destructor
    ~BadClass() {}

    // 🚨 clang-tidy: parameter should be const reference
    void setName(string name) {
        this->name = name;
    }

    // 🚨 clang-tidy: implicit conversion
    void setAge(int age) {
        this->age = age;
    }

private:
    string name;
    int age;
};

// 🚨 clang-tidy: C-style cast
void processData(void* data) {
    int* ptr = (int*)data;
    cout << "   Value: " << *ptr << endl;
}

// 🚨 clang-tidy: use auto
void iterate() {
    vector<int> numbers = {1, 2, 3, 4, 5};
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        cout << "   " << *it << endl;
    }
}
}

namespace clang_tidy_fixed {
// ✅ GOOD: Fixed based on clang-tidy suggestions

class GoodClass {
public:
    // ✅ Use = default for trivial constructors
    GoodClass() = default;

    // ✅ Virtual destructor for base class
    virtual ~GoodClass() = default;

    // ✅ Pass string by const reference
    void setName(const string& name) {
        this->name = name;
    }

    // ✅ Explicit constructor to prevent implicit conversion
    explicit GoodClass(int age) : age(age) {}

    void setAge(int age) {
        this->age = age;
    }

private:
    string name;
    int age = 0;  // ✅ In-class initialization
};

// ✅ Use static_cast instead of C-style cast
void processData(void* data) {
    auto* ptr = static_cast<int*>(data);
    cout << "   Value: " << *ptr << endl;
}

// ✅ Use auto and range-based for loop
void iterate() {
    vector<int> numbers = {1, 2, 3, 4, 5};
    for (auto value : numbers) {
        cout << "   " << value << endl;
    }
}
}

void example1_clang_tidy() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: CLANG-TIDY (LINTER)                ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS CLANG-TIDY?\n";
    cout << "   • Clang-based C++ linter\n";
    cout << "   • Checks for style violations\n";
    cout << "   • Suggests modernizations\n";
    cout << "   • Can auto-fix issues\n";
    cout << "   • Highly configurable\n";

    cout << "\n📋 COMMON CHECKS:\n";
    cout << "   • modernize-* (C++11/14/17/20 features)\n";
    cout << "   • readability-* (naming, complexity)\n";
    cout << "   • performance-* (efficiency issues)\n";
    cout << "   • bugprone-* (potential bugs)\n";
    cout << "   • cppcoreguidelines-* (C++ Core Guidelines)\n";

    cout << "\n💻 USAGE:\n";
    cout << "   # Run clang-tidy on a file\n";
    cout << "   $ clang-tidy file.cpp -- -std=c++17\n";
    cout << "   \n";
    cout << "   # With auto-fix\n";
    cout << "   $ clang-tidy -fix file.cpp -- -std=c++17\n";
    cout << "   \n";
    cout << "   # With specific checks\n";
    cout << "   $ clang-tidy -checks='modernize-*,readability-*' file.cpp\n";

    cout << "\n⚙️  .clang-tidy CONFIGURATION:\n";
    cout << "   Checks: 'modernize-*,readability-*,performance-*'\n";
    cout << "   CheckOptions:\n";
    cout << "     - key: readability-identifier-naming.ClassCase\n";
    cout << "       value: CamelCase\n";

    cout << "\n✨ EXAMPLE FIXES:\n";
    clang_tidy_fixed::iterate();

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Catch issues early\n";
    cout << "   • Enforce standards\n";
    cout << "   • Modernize codebase\n";
    cout << "   • Educational tool\n";
}

// ============================================
// EXAMPLE 2: CLANG-FORMAT (FORMATTER)
// ============================================

void example2_clang_format() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: CLANG-FORMAT (FORMATTER)           ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS CLANG-FORMAT?\n";
    cout << "   • Automatic code formatter\n";
    cout << "   • Enforces consistent style\n";
    cout << "   • Multiple style presets\n";
    cout << "   • Highly customizable\n";
    cout << "   • IDE integration\n";

    cout << "\n❌ BEFORE FORMATTING:\n";
    cout << "   void foo(int a,int b,int c){\n";
    cout << "   if(a>b){cout<<\"yes\"<<endl;}\n";
    cout << "   else{cout<<\"no\"<<endl;}\n";
    cout << "   }\n";

    cout << "\n✅ AFTER FORMATTING:\n";
    cout << "   void foo(int a, int b, int c) {\n";
    cout << "       if (a > b) {\n";
    cout << "           cout << \"yes\" << endl;\n";
    cout << "       } else {\n";
    cout << "           cout << \"no\" << endl;\n";
    cout << "       }\n";
    cout << "   }\n";

    cout << "\n💻 USAGE:\n";
    cout << "   # Format a file\n";
    cout << "   $ clang-format -i file.cpp\n";
    cout << "   \n";
    cout << "   # Use specific style\n";
    cout << "   $ clang-format -style=google file.cpp\n";
    cout << "   \n";
    cout << "   # Check formatting without modifying\n";
    cout << "   $ clang-format --dry-run file.cpp\n";

    cout << "\n⚙️  .clang-format CONFIGURATION:\n";
    cout << "   BasedOnStyle: LLVM\n";
    cout << "   IndentWidth: 4\n";
    cout << "   ColumnLimit: 100\n";
    cout << "   PointerAlignment: Left\n";
    cout << "   BreakBeforeBraces: Allman\n";

    cout << "\n📋 POPULAR STYLES:\n";
    cout << "   • LLVM - LLVM project style\n";
    cout << "   • Google - Google C++ style guide\n";
    cout << "   • Chromium - Chromium project style\n";
    cout << "   • Mozilla - Mozilla project style\n";
    cout << "   • WebKit - WebKit project style\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Consistent codebase\n";
    cout << "   • No style debates\n";
    cout << "   • Automatic enforcement\n";
    cout << "   • Focus on logic, not style\n";
}

// ============================================
// EXAMPLE 3: STATIC ANALYZERS
// ============================================

namespace static_analyzer_example {
// ❌ BAD: Issues static analyzers would catch

int* createArray() {
    int arr[10];  // 🚨 Returning pointer to local array!
    return arr;
}

void useAfterFree() {
    int* ptr = new int(42);
    delete ptr;
    // 🚨 Use after free!
    cout << "   Value: " << *ptr << endl;
}

void divisionByZero(int divisor) {
    // 🚨 Potential division by zero
    int result = 100 / divisor;
    cout << "   Result: " << result << endl;
}

void nullPointerDereference(int* ptr) {
    // 🚨 No null check!
    cout << "   Value: " << *ptr << endl;
}

void arrayOutOfBounds() {
    int arr[5];
    // 🚨 Out of bounds access!
    arr[10] = 42;
}
}

namespace static_analyzer_fixed {
// ✅ GOOD: Issues fixed

vector<int> createArray() {
    // ✅ Return by value (move semantics)
    vector<int> arr(10);
    return arr;
}

void useAfterFree() {
    auto ptr = make_unique<int>(42);
    cout << "   Value: " << *ptr << endl;
    // ✅ Automatic cleanup, no use-after-free
}

void divisionByZero(int divisor) {
    // ✅ Check for zero
    if (divisor == 0) {
        cout << "   Error: Division by zero!" << endl;
        return;
    }
    int result = 100 / divisor;
    cout << "   Result: " << result << endl;
}

void nullPointerDereference(int* ptr) {
    // ✅ Null check
    if (ptr == nullptr) {
        cout << "   Error: Null pointer!" << endl;
        return;
    }
    cout << "   Value: " << *ptr << endl;
}

void arrayOutOfBounds() {
    vector<int> arr(5);
    // ✅ Bounds checking with at()
    try {
        arr.at(10) = 42;
    } catch (const out_of_range& e) {
        cout << "   Error: " << e.what() << endl;
    }
}
}

void example3_static_analyzers() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: STATIC ANALYZERS                    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 POPULAR STATIC ANALYZERS:\n";

    cout << "\n1. CPPCHECK:\n";
    cout << "   • Open source\n";
    cout << "   • Detects bugs and undefined behavior\n";
    cout << "   • Memory leaks, null pointer dereferences\n";
    cout << "   • Out of bounds access\n";
    cout << "   \n";
    cout << "   Usage: $ cppcheck --enable=all file.cpp\n";

    cout << "\n2. CLANG STATIC ANALYZER:\n";
    cout << "   • Part of Clang/LLVM\n";
    cout << "   • Deep path-sensitive analysis\n";
    cout << "   • Memory management issues\n";
    cout << "   • API usage errors\n";
    cout << "   \n";
    cout << "   Usage: $ clang --analyze file.cpp\n";

    cout << "\n3. PVS-STUDIO:\n";
    cout << "   • Commercial (free for open source)\n";
    cout << "   • Very comprehensive\n";
    cout << "   • Detects copy-paste errors\n";
    cout << "   • Security vulnerabilities\n";

    cout << "\n4. COVERITY:\n";
    cout << "   • Commercial (free for open source)\n";
    cout << "   • Enterprise-grade\n";
    cout << "   • Security vulnerabilities\n";
    cout << "   • Quality defects\n";

    cout << "\n5. SONARQUBE:\n";
    cout << "   • Platform for continuous inspection\n";
    cout << "   • Multiple language support\n";
    cout << "   • Technical debt tracking\n";
    cout << "   • Integration with CI/CD\n";

    cout << "\n📋 WHAT THEY DETECT:\n";
    cout << "   • Null pointer dereferences\n";
    cout << "   • Memory leaks\n";
    cout << "   • Buffer overflows\n";
    cout << "   • Use after free\n";
    cout << "   • Division by zero\n";
    cout << "   • Uninitialized variables\n";
    cout << "   • Dead code\n";
    cout << "   • Logic errors\n";

    cout << "\n✨ DEMONSTRATION:\n";
    static_analyzer_fixed::divisionByZero(0);
    static_analyzer_fixed::arrayOutOfBounds();

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Find bugs without running code\n";
    cout << "   • No performance overhead\n";
    cout << "   • Comprehensive analysis\n";
    cout << "   • Early bug detection\n";
}

// ============================================
// EXAMPLE 4: ADDRESS SANITIZER (ASan)
// ============================================

namespace asan_example {
// ❌ BAD: Issues AddressSanitizer would catch

void heapBufferOverflow() {
    cout << "\n   Testing Heap Buffer Overflow:\n";
    int* arr = new int[5];
    // 🚨 Buffer overflow - ASan would catch this!
    // arr[10] = 42;  // Commented to prevent actual crash
    cout << "   (Would detect: heap-buffer-overflow)\n";
    delete[] arr;
}

void stackBufferOverflow() {
    cout << "\n   Testing Stack Buffer Overflow:\n";
    int arr[5];
    // 🚨 Stack buffer overflow
    // arr[10] = 42;  // Commented to prevent crash
    cout << "   (Would detect: stack-buffer-overflow)\n";
}

void useAfterFree() {
    cout << "\n   Testing Use After Free:\n";
    int* ptr = new int(42);
    delete ptr;
    // 🚨 Use after free
    // int value = *ptr;  // Commented to prevent crash
    cout << "   (Would detect: heap-use-after-free)\n";
}

void memoryLeak() {
    cout << "\n   Testing Memory Leak:\n";
    // 🚨 Memory leak
    int* ptr = new int(42);
    // Not deleted!
    cout << "   (Would detect: memory leak)\n";
}
}

void example4_address_sanitizer() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: ADDRESS SANITIZER (ASan)            ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS ADDRESS SANITIZER?\n";
    cout << "   • Runtime memory error detector\n";
    cout << "   • Built into Clang and GCC\n";
    cout << "   • Detects memory bugs at runtime\n";
    cout << "   • Minimal performance overhead (~2x)\n";
    cout << "   • Production-ready\n";

    cout << "\n📋 DETECTS:\n";
    cout << "   • Heap buffer overflow\n";
    cout << "   • Stack buffer overflow\n";
    cout << "   • Use after free\n";
    cout << "   • Use after return\n";
    cout << "   • Use after scope\n";
    cout << "   • Double free\n";
    cout << "   • Memory leaks\n";
    cout << "   • Invalid pointer arithmetic\n";

    cout << "\n💻 COMPILATION:\n";
    cout << "   # Compile with ASan\n";
    cout << "   $ clang++ -fsanitize=address -g file.cpp -o program\n";
    cout << "   $ g++ -fsanitize=address -g file.cpp -o program\n";
    cout << "   \n";
    cout << "   # Run the program\n";
    cout << "   $ ./program\n";

    cout << "\n⚙️  ENVIRONMENT VARIABLES:\n";
    cout << "   # Customize output\n";
    cout << "   $ export ASAN_OPTIONS=verbosity=1:log_path=/tmp/asan.log\n";
    cout << "   \n";
    cout << "   # Halt on error\n";
    cout << "   $ export ASAN_OPTIONS=halt_on_error=1\n";

    cout << "\n✨ EXAMPLE ISSUES (demonstrations):\n";
    asan_example::heapBufferOverflow();
    asan_example::stackBufferOverflow();
    asan_example::useAfterFree();
    asan_example::memoryLeak();

    cout << "\n📊 TYPICAL ASAN OUTPUT:\n";
    cout << "   =================================================================\n";
    cout << "   ==12345==ERROR: AddressSanitizer: heap-buffer-overflow\n";
    cout << "   WRITE of size 4 at 0x60300000eff4 thread T0\n";
    cout << "       #0 0x4a1234 in main file.cpp:10\n";
    cout << "   \n";
    cout << "   0x60300000eff4 is located 4 bytes to the right of\n";
    cout << "   20-byte region [0x60300000efe0,0x60300000eff4)\n";
    cout << "   =================================================================\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Catches memory bugs immediately\n";
    cout << "   • Detailed error reports\n";
    cout << "   • Minimal overhead\n";
    cout << "   • Easy to integrate\n";
    cout << "   • Works with existing code\n";
}

// ============================================
// EXAMPLE 5: UNDEFINED BEHAVIOR SANITIZER (UBSan)
// ============================================

namespace ubsan_example {
// ❌ BAD: Undefined behavior examples

void integerOverflow() {
    cout << "\n   Testing Integer Overflow:\n";
    int max = 2147483647;  // INT_MAX
    // 🚨 Signed integer overflow (undefined behavior)
    // int overflow = max + 1;  // Commented to prevent UB
    cout << "   (Would detect: signed integer overflow)\n";
}

void divisionByZero() {
    cout << "\n   Testing Division by Zero:\n";
    int a = 10;
    int b = 0;
    // 🚨 Division by zero
    // int result = a / b;  // Commented to prevent UB
    cout << "   (Would detect: division by zero)\n";
}

void nullPointerDereference() {
    cout << "\n   Testing Null Pointer:\n";
    int* ptr = nullptr;
    // 🚨 Null pointer dereference
    // int value = *ptr;  // Commented to prevent crash
    cout << "   (Would detect: null pointer dereference)\n";
}

void shiftOutOfBounds() {
    cout << "\n   Testing Shift Out of Bounds:\n";
    int value = 1;
    // 🚨 Shift by 32 or more (UB for 32-bit int)
    // int result = value << 35;  // Commented to prevent UB
    cout << "   (Would detect: shift out of bounds)\n";
}
}

void example5_undefined_behavior_sanitizer() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: UNDEFINED BEHAVIOR SANITIZER (UBSan)║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS UBSAN?\n";
    cout << "   • Detects undefined behavior at runtime\n";
    cout << "   • Built into Clang and GCC\n";
    cout << "   • Complements other sanitizers\n";
    cout << "   • Low overhead (~20%)\n";
    cout << "   • Multiple check categories\n";

    cout << "\n📋 DETECTS:\n";
    cout << "   • Signed integer overflow\n";
    cout << "   • Division by zero\n";
    cout << "   • Null pointer dereference\n";
    cout << "   • Misaligned pointer access\n";
    cout << "   • Out of bounds array access\n";
    cout << "   • Invalid shift operations\n";
    cout << "   • Invalid enum values\n";
    cout << "   • Invalid bool values\n";

    cout << "\n💻 COMPILATION:\n";
    cout << "   # Compile with UBSan\n";
    cout << "   $ clang++ -fsanitize=undefined -g file.cpp -o program\n";
    cout << "   \n";
    cout << "   # Specific checks\n";
    cout << "   $ clang++ -fsanitize=integer,null -g file.cpp\n";
    cout << "   \n";
    cout << "   # Combined with other sanitizers\n";
    cout << "   $ clang++ -fsanitize=address,undefined -g file.cpp\n";

    cout << "\n⚙️  UBSAN CHECKS:\n";
    cout << "   • -fsanitize=alignment\n";
    cout << "   • -fsanitize=bool\n";
    cout << "   • -fsanitize=bounds\n";
    cout << "   • -fsanitize=enum\n";
    cout << "   • -fsanitize=float-divide-by-zero\n";
    cout << "   • -fsanitize=integer-divide-by-zero\n";
    cout << "   • -fsanitize=null\n";
    cout << "   • -fsanitize=shift\n";
    cout << "   • -fsanitize=signed-integer-overflow\n";

    cout << "\n✨ EXAMPLE ISSUES:\n";
    ubsan_example::integerOverflow();
    ubsan_example::divisionByZero();
    ubsan_example::nullPointerDereference();
    ubsan_example::shiftOutOfBounds();

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Catch subtle bugs\n";
    cout << "   • Prevent security vulnerabilities\n";
    cout << "   • Enforce language rules\n";
    cout << "   • Low overhead\n";
    cout << "   • Production-ready\n";
}

// ============================================
// EXAMPLE 6: THREAD SANITIZER (TSan)
// ============================================

void example6_thread_sanitizer() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: THREAD SANITIZER (TSan)             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS THREAD SANITIZER?\n";
    cout << "   • Detects data races\n";
    cout << "   • Finds threading issues\n";
    cout << "   • Built into Clang and GCC\n";
    cout << "   • Moderate overhead (5-15x)\n";
    cout << "   • Essential for multithreaded code\n";

    cout << "\n📋 DETECTS:\n";
    cout << "   • Data races\n";
    cout << "   • Deadlocks\n";
    cout << "   • Thread leaks\n";
    cout << "   • Improper synchronization\n";
    cout << "   • Lock order violations\n";

    cout << "\n💻 COMPILATION:\n";
    cout << "   # Compile with TSan\n";
    cout << "   $ clang++ -fsanitize=thread -g file.cpp -o program\n";
    cout << "   $ g++ -fsanitize=thread -g file.cpp -o program\n";
    cout << "   \n";
    cout << "   # Note: Cannot combine with ASan!\n";

    cout << "\n❌ DATA RACE EXAMPLE:\n";
    cout << "   int counter = 0;\n";
    cout << "   \n";
    cout << "   void thread1() { counter++; }  // 🚨 Race!\n";
    cout << "   void thread2() { counter++; }  // 🚨 Race!\n";

    cout << "\n✅ FIXED WITH MUTEX:\n";
    cout << "   int counter = 0;\n";
    cout << "   std::mutex mtx;\n";
    cout << "   \n";
    cout << "   void thread1() {\n";
    cout << "       std::lock_guard<std::mutex> lock(mtx);\n";
    cout << "       counter++;  // ✅ Safe!\n";
    cout << "   }\n";

    cout << "\n📊 TYPICAL TSAN OUTPUT:\n";
    cout << "   WARNING: ThreadSanitizer: data race (pid=12345)\n";
    cout << "     Write of size 4 at 0x7fff12345678 by thread T1:\n";
    cout << "       #0 thread1() file.cpp:10\n";
    cout << "     Previous write of size 4 at 0x7fff12345678 by thread T2:\n";
    cout << "       #0 thread2() file.cpp:15\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Find hard-to-reproduce bugs\n";
    cout << "   • Ensure thread safety\n";
    cout << "   • Prevent data corruption\n";
    cout << "   • Essential for concurrent code\n";
}

// ============================================
// EXAMPLE 7: MEMORY SANITIZER (MSan)
// ============================================

void example7_memory_sanitizer() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: MEMORY SANITIZER (MSan)             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS MEMORY SANITIZER?\n";
    cout << "   • Detects uninitialized memory reads\n";
    cout << "   • Tracks memory initialization\n";
    cout << "   • Clang only (not in GCC)\n";
    cout << "   • Moderate overhead (3x)\n";
    cout << "   • Requires instrumented libraries\n";

    cout << "\n📋 DETECTS:\n";
    cout << "   • Use of uninitialized values\n";
    cout << "   • Use of uninitialized memory\n";
    cout << "   • Reading uninitialized stack\n";
    cout << "   • Reading uninitialized heap\n";

    cout << "\n💻 COMPILATION:\n";
    cout << "   # Compile with MSan (Clang only)\n";
    cout << "   $ clang++ -fsanitize=memory -g file.cpp -o program\n";
    cout << "   \n";
    cout << "   # Track origins for better debugging\n";
    cout << "   $ clang++ -fsanitize=memory -fsanitize-memory-track-origins -g file.cpp\n";

    cout << "\n❌ UNINITIALIZED MEMORY EXAMPLE:\n";
    cout << "   int getRandomValue() {\n";
    cout << "       int x;  // 🚨 Uninitialized!\n";
    cout << "       return x;\n";
    cout << "   }\n";

    cout << "\n✅ FIXED:\n";
    cout << "   int getRandomValue() {\n";
    cout << "       int x = 0;  // ✅ Initialized!\n";
    cout << "       return x;\n";
    cout << "   }\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Catch initialization bugs\n";
    cout << "   • Prevent undefined behavior\n";
    cout << "   • Find subtle bugs\n";
    cout << "   • Deterministic detection\n";
}

// ============================================
// EXAMPLE 8: VALGRIND
// ============================================

void example8_valgrind() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: VALGRIND                            ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS VALGRIND?\n";
    cout << "   • Dynamic analysis framework\n";
    cout << "   • Multiple tools included\n";
    cout << "   • No recompilation needed\n";
    cout << "   • Significant overhead (10-50x)\n";
    cout << "   • Linux/macOS (limited Windows)\n";

    cout << "\n📋 VALGRIND TOOLS:\n";

    cout << "\n1. MEMCHECK (Default):\n";
    cout << "   • Memory leak detection\n";
    cout << "   • Invalid memory access\n";
    cout << "   • Use of uninitialized memory\n";
    cout << "   \n";
    cout << "   Usage: $ valgrind --leak-check=full ./program\n";

    cout << "\n2. CACHEGRIND:\n";
    cout << "   • Cache profiler\n";
    cout << "   • Cache miss analysis\n";
    cout << "   • Branch prediction analysis\n";
    cout << "   \n";
    cout << "   Usage: $ valgrind --tool=cachegrind ./program\n";

    cout << "\n3. CALLGRIND:\n";
    cout << "   • Call graph profiler\n";
    cout << "   • Function call analysis\n";
    cout << "   • Performance profiling\n";
    cout << "   \n";
    cout << "   Usage: $ valgrind --tool=callgrind ./program\n";

    cout << "\n4. HELGRIND:\n";
    cout << "   • Thread error detector\n";
    cout << "   • Data race detection\n";
    cout << "   • Lock ordering problems\n";
    cout << "   \n";
    cout << "   Usage: $ valgrind --tool=helgrind ./program\n";

    cout << "\n5. MASSIF:\n";
    cout << "   • Heap profiler\n";
    cout << "   • Memory usage over time\n";
    cout << "   • Stack and heap analysis\n";
    cout << "   \n";
    cout << "   Usage: $ valgrind --tool=massif ./program\n";

    cout << "\n💻 COMMON USAGE:\n";
    cout << "   # Basic memory check\n";
    cout << "   $ valgrind ./program\n";
    cout << "   \n";
    cout << "   # Detailed leak check\n";
    cout << "   $ valgrind --leak-check=full --show-leak-kinds=all ./program\n";
    cout << "   \n";
    cout << "   # Track origins\n";
    cout << "   $ valgrind --track-origins=yes ./program\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Comprehensive analysis\n";
    cout << "   • No recompilation\n";
    cout << "   • Multiple tools\n";
    cout << "   • Detailed reports\n";
}

// ============================================
// EXAMPLE 9: CODE COVERAGE (gcov/lcov)
// ============================================

void example9_code_coverage() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: CODE COVERAGE (gcov/lcov)           ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 WHAT IS CODE COVERAGE?\n";
    cout << "   • Measures test coverage\n";
    cout << "   • Shows which code is tested\n";
    cout << "   • Identifies untested paths\n";
    cout << "   • Essential for quality assurance\n";

    cout << "\n📋 COVERAGE TYPES:\n";
    cout << "   • Line Coverage - Lines executed\n";
    cout << "   • Branch Coverage - Branches taken\n";
    cout << "   • Function Coverage - Functions called\n";
    cout << "   • Condition Coverage - Boolean conditions\n";

    cout << "\n💻 USING GCOV:\n";
    cout << "   # Compile with coverage flags\n";
    cout << "   $ g++ -fprofile-arcs -ftest-coverage file.cpp -o program\n";
    cout << "   \n";
    cout << "   # Run the program\n";
    cout << "   $ ./program\n";
    cout << "   \n";
    cout << "   # Generate coverage report\n";
    cout << "   $ gcov file.cpp\n";

    cout << "\n💻 USING LCOV (HTML Reports):\n";
    cout << "   # Capture coverage data\n";
    cout << "   $ lcov --capture --directory . --output-file coverage.info\n";
    cout << "   \n";
    cout << "   # Generate HTML report\n";
    cout << "   $ genhtml coverage.info --output-directory out\n";

    cout << "\n📊 COVERAGE GOALS:\n";
    cout << "   • 80-100%: Excellent ✅\n";
    cout << "   • 60-79%:  Good ⚠️\n";
    cout << "   • 40-59%:  Fair 🔶\n";
    cout << "   • <40%:    Poor 🚨\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Ensure code is tested\n";
    cout << "   • Find untested code\n";
    cout << "   • Improve test quality\n";
    cout << "   • Track progress\n";
}

// ============================================
// EXAMPLE 10: INTEGRATION STRATEGIES
// ============================================

void example10_integration() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: INTEGRATION STRATEGIES             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔧 CMAKE INTEGRATION:\n";
    cout << "   # CMakeLists.txt\n";
    cout << "   \n";
    cout << "   # Enable sanitizers\n";
    cout << "   if(ENABLE_ASAN)\n";
    cout << "       add_compile_options(-fsanitize=address)\n";
    cout << "       add_link_options(-fsanitize=address)\n";
    cout << "   endif()\n";
    cout << "   \n";
    cout << "   # Enable clang-tidy\n";
    cout << "   set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=*)\n";
    cout << "   \n";
    cout << "   # Enable coverage\n";
    cout << "   if(ENABLE_COVERAGE)\n";
    cout << "       add_compile_options(--coverage)\n";
    cout << "       add_link_options(--coverage)\n";
    cout << "   endif()\n";

    cout << "\n🔄 CI/CD PIPELINE:\n";
    cout << "   # .github/workflows/ci.yml\n";
    cout << "   \n";
    cout << "   - name: Run clang-format\n";
    cout << "     run: clang-format --dry-run --Werror src/**/*.cpp\n";
    cout << "   \n";
    cout << "   - name: Run clang-tidy\n";
    cout << "     run: clang-tidy src/**/*.cpp\n";
    cout << "   \n";
    cout << "   - name: Build with sanitizers\n";
    cout << "     run: |\n";
    cout << "       cmake -DENABLE_ASAN=ON -DENABLE_UBSAN=ON\n";
    cout << "       make\n";
    cout << "   \n";
    cout << "   - name: Run tests\n";
    cout << "     run: ./run_tests\n";
    cout << "   \n";
    cout << "   - name: Generate coverage\n";
    cout << "     run: lcov --capture --directory . --output coverage.info\n";

    cout << "\n🪝 PRE-COMMIT HOOKS:\n";
    cout << "   # .git/hooks/pre-commit\n";
    cout << "   #!/bin/bash\n";
    cout << "   \n";
    cout << "   # Format check\n";
    cout << "   clang-format -i $(git diff --cached --name-only | grep -E '\\.(cpp|h)$')\n";
    cout << "   \n";
    cout << "   # Lint check\n";
    cout << "   clang-tidy $(git diff --cached --name-only | grep '\\.cpp$')\n";
    cout << "   \n";
    cout << "   # Exit if checks fail\n";
    cout << "   if [ $? -ne 0 ]; then\n";
    cout << "       echo \"Pre-commit checks failed!\"\n";
    cout << "       exit 1\n";
    cout << "   fi\n";

    cout << "\n🎯 RECOMMENDED WORKFLOW:\n";
    cout << "\n1. DEVELOPMENT:\n";
    cout << "   • Use clang-format in IDE\n";
    cout << "   • Enable clang-tidy warnings\n";
    cout << "   • Run with sanitizers locally\n";

    cout << "\n2. PRE-COMMIT:\n";
    cout << "   • Auto-format code\n";
    cout << "   • Run static analysis\n";
    cout << "   • Quick tests\n";

    cout << "\n3. CI/CD:\n";
    cout << "   • Full static analysis\n";
    cout << "   • Multiple sanitizer builds\n";
    cout << "   • Coverage reports\n";
    cout << "   • Performance tests\n";

    cout << "\n4. REGULAR:\n";
    cout << "   • Weekly deep analysis\n";
    cout << "   • Review coverage trends\n";
    cout << "   • Update tools\n";
    cout << "   • Technical debt review\n";

    cout << "\n💡 TOOL COMBINATION:\n";
    cout << "   Development:  clang-format + clang-tidy + ASan\n";
    cout << "   Testing:      ASan + UBSan + Coverage\n";
    cout << "   CI/CD:        All static analyzers + All sanitizers\n";
    cout << "   Performance:  Valgrind Callgrind + Profilers\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║           C++ TOOLS AND ANALYSIS GUIDE                ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"The right tools make quality code easier to         ║\n";
    cout << "║   achieve and maintain. Use static analysis,          ║\n";
    cout << "║   sanitizers, and formatters to catch issues          ║\n";
    cout << "║   before they become bugs.\"                          ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_clang_tidy();
    example2_clang_format();
    example3_static_analyzers();
    example4_address_sanitizer();
    example5_undefined_behavior_sanitizer();
    example6_thread_sanitizer();
    example7_memory_sanitizer();
    example8_valgrind();
    example9_code_coverage();
    example10_integration();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 TOOLS AND ANALYSIS COMPLETE! 🎉            ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Integrate these tools into your       ║\n";
    cout << "║  development workflow. They catch bugs early, enforce ║\n";
    cout << "║  standards, and make your code more maintainable!     ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
