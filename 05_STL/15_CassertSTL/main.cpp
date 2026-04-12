#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>
#include <string>
#include <print>

// ============================================================
// cassert / static_assert — Assertions in C++23
// ============================================================
// Topics covered:
//   1.  assert() — runtime assertion
//   2.  NDEBUG — disabling assertions in release
//   3.  static_assert — compile-time assertion
//   4.  Custom assert messages
//   5.  Asserting class invariants
//   6.  Asserting function preconditions/postconditions
//   7.  static_assert with type traits
//   8.  static_assert with constexpr
//   9.  assert vs exceptions vs if
//  10.  C++23 [[assume]] attribute
// ============================================================

// ============================================================
// Example 3 — static_assert: compile-time checks
// ============================================================
static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
static_assert(sizeof(void*) == 8, "this code requires 64-bit platform");
static_assert(sizeof(uint32_t) == 4, "uint32_t must be exactly 4 bytes");

// ============================================================
// Example 7 — static_assert with type traits
// ============================================================
#include <type_traits>

template<typename T>
T safeAdd(T a, T b) {
    static_assert(std::is_arithmetic_v<T>,
                  "safeAdd requires arithmetic type (int, float, etc.)");
    static_assert(!std::is_same_v<T, bool>,
                  "safeAdd does not accept bool");
    return a + b;
}

// ============================================================
// Example 8 — static_assert with constexpr
// ============================================================
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}
static_assert(factorial(5) == 120, "factorial(5) must be 120");
static_assert(factorial(0) == 1,   "factorial(0) must be 1");


// ============================================================
// Example 5 — Class with invariant assertions
// ============================================================
class BoundedBuffer {
private:
    std::vector<int> m_data;
    std::size_t      m_maxSize;

    void checkInvariant() const {
        assert(m_data.size() <= m_maxSize
               && "BoundedBuffer: size exceeded maxSize!");
    }

public:
    explicit BoundedBuffer(std::size_t maxSize) : m_maxSize(maxSize) {
        assert(maxSize > 0 && "BoundedBuffer: maxSize must be > 0");
    }

    void push(int val) {
        assert(m_data.size() < m_maxSize && "BoundedBuffer: push on full buffer!");
        m_data.push_back(val);
        checkInvariant();
    }

    int pop() {
        assert(!m_data.empty() && "BoundedBuffer: pop on empty buffer!");
        int val = m_data.back();
        m_data.pop_back();
        checkInvariant();
        return val;
    }

    std::size_t size()    const { return m_data.size();  }
    bool        empty()   const { return m_data.empty(); }
    bool        full()    const { return m_data.size() == m_maxSize; }
};


// ============================================================
// Example 6 — Precondition / postcondition assertions
// ============================================================
double safeSqrt(double x) {
    // Precondition: x must be non-negative
    assert(x >= 0.0 && "safeSqrt: x must be >= 0");

    double result = 0.0;
    // Newton's method
    if (x > 0) {
        result = x;
        for (int i = 0; i < 100; ++i)
            result = 0.5 * (result + x / result);
    }

    // Postcondition: result * result ≈ x
    assert(result >= 0.0 && "safeSqrt: result must be non-negative");
    return result;
}

int binarySearch(const std::vector<int>& v, int target) {
    // Precondition: vector must be sorted
    assert(std::is_sorted(v.begin(), v.end())
           && "binarySearch: input must be sorted!");

    int lo = 0, hi = (int)v.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (v[mid] == target) return mid;
        else if (v[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: assert() — Runtime Assertion\n";
    std::cout << "=====================================================\n";

    int x = 42;
    assert(x == 42);   // passes — does nothing
    std::cout << "assert(x == 42): passed\n";

    assert(x > 0);     // passes
    std::cout << "assert(x > 0):   passed\n";

    // assert(x == 0);  // would FAIL — prints message and calls abort()

    // With message (common idiom)
    assert(x > 0 && "x must be positive");
    std::cout << "assert with message: passed\n";

    std::cout << "\nassert behavior:\n";
    std::cout << "  DEBUG build:   assert fires → prints error + aborts program\n";
    std::cout << "  RELEASE build: assert is removed → zero overhead\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: NDEBUG — Disabling Assertions\n";
    std::cout << "=====================================================\n";

#ifdef NDEBUG
    std::cout << "NDEBUG is defined — assertions are DISABLED\n";
    std::cout << "assert() compiles to nothing — zero overhead\n";
#else
    std::cout << "NDEBUG is NOT defined — assertions are ENABLED\n";
    std::cout << "assert() is active — checking conditions\n";
#endif

    std::cout << "\nTo disable in CMake (Release build):\n";
    std::cout << "  set(CMAKE_BUILD_TYPE Release)  — sets NDEBUG automatically\n";
    std::cout << "  or: add_compile_definitions(NDEBUG)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: static_assert — Compile-Time\n";
    std::cout << "=====================================================\n";

    // These were declared at file scope above — if they fail, compilation stops
    std::cout << "static_assert(sizeof(int) >= 4): passed\n";
    std::cout << "static_assert(sizeof(void*) == 8): passed (64-bit)\n";
    std::cout << "static_assert(sizeof(uint32_t) == 4): passed\n";

    // Local static_assert
    constexpr int SIZE = 64;
    static_assert(SIZE > 0,   "SIZE must be positive");
    static_assert(SIZE <= 256,"SIZE must fit in a byte");
    std::cout << "local static_assert(SIZE): passed\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Custom Assert Messages\n";
    std::cout << "=====================================================\n";

    // C++23 style — cleaner messages with [[assume]] or custom macro
    auto verify = [](bool condition, const std::string& msg) {
        if (!condition) {
            std::cerr << "ASSERTION FAILED: " << msg << '\n';
            std::abort();
        }
    };

    int port = 5060;
    verify(port > 0 && port <= 65535, "port must be in valid range [1, 65535]");
    std::cout << "verify(port valid): passed\n";

    std::string name = "Kostas";
    verify(!name.empty(), "name must not be empty");
    std::cout << "verify(name not empty): passed\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Class Invariant Assertions\n";
    std::cout << "=====================================================\n";

    BoundedBuffer buf(3);
    buf.push(10);
    buf.push(20);
    buf.push(30);
    std::cout << "pushed 3 items, full: " << buf.full() << '\n';

    std::cout << "popped: " << buf.pop() << '\n';
    std::cout << "popped: " << buf.pop() << '\n';
    std::cout << "size after 2 pops: " << buf.size() << '\n';

    // buf.push(1); buf.push(2); buf.push(3); buf.push(4); // would assert: full!
    // buf.pop(); buf.pop();  // after empty: would assert!
    std::cout << "(overflow/underflow assert demos — commented out to not abort)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Preconditions and Postconditions\n";
    std::cout << "=====================================================\n";

    std::cout << "safeSqrt(16.0) = " << safeSqrt(16.0) << '\n';
    std::cout << "safeSqrt(2.0)  = " << safeSqrt(2.0)  << '\n';
    std::cout << "safeSqrt(0.0)  = " << safeSqrt(0.0)  << '\n';
    // safeSqrt(-1.0);  // would assert: precondition violated

    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13};
    std::cout << "binarySearch(7):  index " << binarySearch(sorted, 7)  << '\n';
    std::cout << "binarySearch(10): index " << binarySearch(sorted, 10) << '\n';
    // std::vector<int> unsorted = {5,1,3}; binarySearch(unsorted, 3); // asserts!


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: static_assert with Type Traits\n";
    std::cout << "=====================================================\n";

    std::cout << "safeAdd<int>(3, 4)     = " << safeAdd(3, 4)       << '\n';
    std::cout << "safeAdd<float>(1.5f, 2.5f) = " << safeAdd(1.5f, 2.5f) << '\n';
    std::cout << "safeAdd<double>(1.1, 2.2)  = " << safeAdd(1.1, 2.2)   << '\n';
    // safeAdd<std::string>("a","b");  // compile error: not arithmetic
    // safeAdd(true, false);           // compile error: no bool

    // More type trait checks
    static_assert(std::is_integral_v<int>,    "int must be integral");
    static_assert(std::is_floating_point_v<double>, "double must be float");
    static_assert(std::is_signed_v<int32_t>,  "int32_t must be signed");
    static_assert(std::is_unsigned_v<uint32_t>,"uint32_t must be unsigned");
    std::cout << "type trait static_asserts: all passed\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: static_assert with constexpr\n";
    std::cout << "=====================================================\n";

    // Checked at compile time — no runtime cost
    static_assert(factorial(5) == 120, "factorial(5) == 120");
    static_assert(factorial(3) == 6,   "factorial(3) == 6");
    std::cout << "factorial static_asserts: passed at compile time\n";

    // Protocol field size checks
    struct McxHeader { uint8_t ver; uint8_t type; uint16_t len; uint32_t id; };
    static_assert(sizeof(McxHeader) == 8, "McxHeader must be exactly 8 bytes");
    std::cout << "McxHeader size static_assert: passed (" << sizeof(McxHeader) << " bytes)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: assert vs exception vs if\n";
    std::cout << "=====================================================\n";

    std::cout << "assert(condition):\n";
    std::cout << "  + Zero cost in release\n";
    std::cout << "  + Documents programmer intent\n";
    std::cout << "  - Disabled in release — not for production error handling\n";
    std::cout << "  USE FOR: programming errors, invariants, preconditions\n\n";

    std::cout << "throw std::exception(msg):\n";
    std::cout << "  + Works in release\n";
    std::cout << "  + Caller can catch and recover\n";
    std::cout << "  - Cost (stack unwinding)\n";
    std::cout << "  USE FOR: recoverable errors, bad user input, I/O failures\n\n";

    std::cout << "if (!condition) return false:\n";
    std::cout << "  + Zero cost, always active\n";
    std::cout << "  + Works everywhere (embedded, no-exception)\n";
    std::cout << "  - Verbose, must propagate manually\n";
    std::cout << "  USE FOR: performance-critical, expected failure paths\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: C++23 [[assume]] attribute\n";
    std::cout << "=====================================================\n";

    // [[assume]] tells the compiler to assume a condition is true
    // without checking it — enables optimizations
    // (does NOT check at runtime — UB if assumption is wrong!)

    int val = 42;
    [[assume(val > 0)]];   // C++23 — optimizer hint, not a check
    std::cout << "[[assume(val > 0)]] — hint to optimizer (not a runtime check)\n";

    std::cout << "\nassert vs [[assume]]:\n";
    std::cout << "  assert(x > 0)    — DEBUG: checks + aborts. RELEASE: removed\n";
    std::cout << "  [[assume(x > 0)]] — never checks, tells optimizer to assume\n";
    std::cout << "  Use [[assume]] only when you are 100% certain condition holds\n";

    std::cout << "\n=== CassertSTL complete ===\n";
    return 0;
}
