#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cstring>
#include <print>

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
    void setName(std::string name) {
        this->name = name;
    }

    // 🚨 clang-tidy: implicit conversion
    void setAge(int age) {
        this->age = age;
    }

private:
    std::string name;
    int age;
};

// 🚨 clang-tidy: C-style cast
void processData(void* data) {
    int* ptr = (int*)data;
    std::println("   Value: {}", *ptr);
}

// 🚨 clang-tidy: use auto
void iterate() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    for (std::vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        std::println("   {}", *it);
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
    void setName(const std::string& name) {
        this->name = name;
    }

    // ✅ Explicit constructor to prevent implicit conversion
    explicit GoodClass(int age) : age(age) {}

    void setAge(int age) {
        this->age = age;
    }

private:
    std::string name;
    int age = 0;  // ✅ In-class initialization
};

// ✅ Use static_cast instead of C-style cast
void processData(void* data) {
    auto* ptr = static_cast<int*>(data);
    std::println("   Value: {}", *ptr);
}

// ✅ Use auto and range-based for loop
void iterate() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    for (auto value : numbers) {
        std::println("   {}", value);
    }
}
}

void example1_clang_tidy() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: CLANG-TIDY (LINTER)                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    clang_tidy_fixed::iterate();

}

// ============================================
// EXAMPLE 2: CLANG-FORMAT (FORMATTER)
// ============================================

void example2_clang_format() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: CLANG-FORMAT (FORMATTER)            ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

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
    std::println("   Value: {}", *ptr);
}

void divisionByZero(int divisor) {
    // 🚨 Potential division by zero
    int result = 100 / divisor;
    std::println("   Result: {}", result);
}

void nullPointerDereference(int* ptr) {
    // 🚨 No null check!
    std::println("   Value: {}", *ptr);
}

void arrayOutOfBounds() {
    int arr[5];
    // 🚨 Out of bounds access!
    arr[10] = 42;
}
}

namespace static_analyzer_fixed {
// ✅ GOOD: Issues fixed

std::vector<int> createArray() {
    // ✅ Return by value (move semantics)
    std::vector<int> arr(10);
    return arr;
}

void useAfterFree() {
    auto ptr = std::make_unique<int>(42);
    std::println("   Value: {}", *ptr);
    // ✅ Automatic cleanup, no use-after-free
}

void divisionByZero(int divisor) {
    // ✅ Check for zero
    if (divisor == 0) {
        std::println("   Error: Division by zero!");
        return;
    }
    int result = 100 / divisor;
    std::println("   Result: {}", result);
}

void nullPointerDereference(int* ptr) {
    // ✅ Null check
    if (ptr == nullptr) {
        std::println("   Error: Null pointer!");
        return;
    }
    std::println("   Value: {}", *ptr);
}

void arrayOutOfBounds() {
    std::vector<int> arr(5);
    // ✅ Bounds checking with at()
    try {
        arr.at(10) = 42;
    } catch (const std::out_of_range& e) {
        std::println("   Error: {}", e.what());
    }
}
}

void example3_static_analyzers() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: STATIC ANALYZERS                    ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    static_analyzer_fixed::divisionByZero(0);
    static_analyzer_fixed::arrayOutOfBounds();

}

// ============================================
// EXAMPLE 4: ADDRESS SANITIZER (ASan)
// ============================================

namespace asan_example {
// ❌ BAD: Issues AddressSanitizer would catch

void heapBufferOverflow() {
    std::println("");
    std::println("   Testing Heap Buffer Overflow:");
    int* arr = new int[5];
    // 🚨 Buffer overflow - ASan would catch this!
    // arr[10] = 42;  // Commented to prevent actual crash
    std::println("   (Would detect: heap-buffer-overflow)");
    delete[] arr;
}

void stackBufferOverflow() {
    std::println("");
    std::println("   Testing Stack Buffer Overflow:");
    int arr[5];
    // 🚨 Stack buffer overflow
    // arr[10] = 42;  // Commented to prevent crash
    std::println("   (Would detect: stack-buffer-overflow)");
}

void useAfterFree() {
    std::println("");
    std::println("   Testing Use After Free:");
    int* ptr = new int(42);
    delete ptr;
    // 🚨 Use after free
    // int value = *ptr;  // Commented to prevent crash
    std::println("   (Would detect: heap-use-after-free)");
}

void memoryLeak() {
    std::println("");
    std::println("   Testing Memory Leak:");
    // 🚨 Memory leak
    int* ptr = new int(42);
    // Not deleted!
    std::println("   (Would detect: memory leak)");
}
}

void example4_address_sanitizer() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: ADDRESS SANITIZER (ASan)            ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    asan_example::heapBufferOverflow();
    asan_example::stackBufferOverflow();
    asan_example::useAfterFree();
    asan_example::memoryLeak();

    std::println("   =================================================================");
    std::println("   ");
    std::println("   =================================================================");

}

// ============================================
// EXAMPLE 5: UNDEFINED BEHAVIOR SANITIZER (UBSan)
// ============================================

namespace ubsan_example {
// ❌ BAD: Undefined behavior examples

void integerOverflow() {
    std::println("");
    std::println("   Testing Integer Overflow:");
    int max = 2147483647;  // INT_MAX
    // 🚨 Signed integer overflow (undefined behavior)
    // int overflow = max + 1;  // Commented to prevent UB
    std::println("   (Would detect: signed integer overflow)");
}

void divisionByZero() {
    std::println("");
    std::println("   Testing Division by Zero:");
    int a = 10;
    int b = 0;
    // 🚨 Division by zero
    // int result = a / b;  // Commented to prevent UB
    std::println("   (Would detect: division by zero)");
}

void nullPointerDereference() {
    std::println("");
    std::println("   Testing Null Pointer:");
    int* ptr = nullptr;
    // 🚨 Null pointer dereference
    // int value = *ptr;  // Commented to prevent crash
    std::println("   (Would detect: null pointer dereference)");
}

void shiftOutOfBounds() {
    std::println("");
    std::println("   Testing Shift Out of Bounds:");
    int value = 1;
    // 🚨 Shift by 32 or more (UB for 32-bit int)
    // int result = value << 35;  // Commented to prevent UB
    std::println("   (Would detect: shift out of bounds)");
}
}

void example5_undefined_behavior_sanitizer() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: UNDEFINED BEHAVIOR SANITIZER (UBSan)║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    ubsan_example::integerOverflow();
    ubsan_example::divisionByZero();
    ubsan_example::nullPointerDereference();
    ubsan_example::shiftOutOfBounds();

}

// ============================================
// EXAMPLE 6: THREAD SANITIZER (TSan)
// ============================================

void example6_thread_sanitizer() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: THREAD SANITIZER (TSan)             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    std::println("   ");

}

// ============================================
// EXAMPLE 7: MEMORY SANITIZER (MSan)
// ============================================

void example7_memory_sanitizer() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: MEMORY SANITIZER (MSan)             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

}

// ============================================
// EXAMPLE 8: VALGRIND
// ============================================

void example8_valgrind() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: VALGRIND                            ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    std::println("   ");

    std::println("   ");

    std::println("   ");

    std::println("   ");
    std::println("   ");
}

// ============================================
// EXAMPLE 9: CODE COVERAGE (gcov/lcov)
// ============================================

void example9_code_coverage() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: CODE COVERAGE (gcov/lcov)           ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    std::println("   ");

}

// ============================================
// EXAMPLE 10: INTEGRATION STRATEGIES
// ============================================

void example10_integration() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: INTEGRATION STRATEGIES             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");
    std::println("   ");

    std::println("   ");
    std::println("   ");
    std::println("   ");
    std::println("   ");
    std::println("   ");

    std::println("   ");
    std::println("   ");
    std::println("   ");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║           C++ TOOLS AND ANALYSIS GUIDE                ║");
    std::println("║                                                       ║");
    std::println("║  \"The right tools make quality code easier to        ║");
    std::println("║   achieve and maintain. Use static analysis,          ║");
    std::println("║   sanitizers, and formatters to catch issues          ║");
    std::println("║   before they become bugs.\"                          ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

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

    return 0;
}
