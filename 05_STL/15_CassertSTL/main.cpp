#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <print>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

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
//   9.  C++23 [[assume]] attribute
// ============================================================

// ============================================================
// Example 3 — static_assert: compile-time checks
// ============================================================
static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
static_assert(sizeof(void*) == 8, "this code requires 64-bit platform");
static_assert(sizeof(std::uint32_t) == 4, "uint32_t must be exactly 4 bytes");

// ============================================================
// Example 7 — static_assert with type traits
// ============================================================
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

    // Postcondition: result is non-negative and result * result ≈ x
    assert(result >= 0.0 && "safeSqrt: result must be non-negative");
    assert(std::abs(result * result - x) <= 1e-9 * (1.0 + x)
           && "safeSqrt: result * result must be close to x");
    return result;
}

int binarySearch(const std::vector<int>& v, int target) {
    // Precondition: vector must be sorted (O(n) check, removed with NDEBUG)
    assert(std::is_sorted(v.begin(), v.end())
           && "binarySearch: input must be sorted!");

    int lo = 0, hi = static_cast<int>(v.size()) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (v[mid] == target) return mid;
        else if (v[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

namespace
{
    void banner(std::string_view title)
    {
        std::println("=====================================================");
        std::println("{}", title);
        std::println("=====================================================");
    }
}


int main()
{
    // === EXAMPLE 1: assert() — Runtime Assertion ===
    banner("Example 1: assert() — Runtime Assertion");

    [[maybe_unused]] int x = 42;   // only used inside assert(): silences -Wunused-variable with NDEBUG
    assert(x == 42);   // passes — does nothing
    std::println("assert(x == 42): passed");

    assert(x > 0);     // passes
    std::println("assert(x > 0):   passed");

    // assert(x == 0);  // would FAIL — prints message and calls abort()

    // With message (common idiom)
    assert(x > 0 && "x must be positive");
    std::println("assert with message: passed");
    std::println("");


    // === EXAMPLE 2: NDEBUG — Disabling Assertions ===
    banner("Example 2: NDEBUG — Disabling Assertions");

#ifdef NDEBUG
    std::println("NDEBUG is defined — assertions are DISABLED");
    std::println("assert() compiles to nothing — zero overhead");
#else
    std::println("NDEBUG is NOT defined — assertions are ENABLED");
    std::println("assert() is active — checking conditions");
#endif
    std::println("");


    // === EXAMPLE 3: static_assert — Compile-Time ===
    banner("Example 3: static_assert — Compile-Time");

    // These were declared at file scope above — if they fail, compilation stops
    std::println("static_assert(sizeof(int) >= 4): passed");
    std::println("static_assert(sizeof(void*) == 8): passed (64-bit)");
    std::println("static_assert(sizeof(uint32_t) == 4): passed");

    // Local static_assert
    constexpr int SIZE = 64;
    static_assert(SIZE > 0,   "SIZE must be positive");
    static_assert(SIZE <= 256,"SIZE must fit in a byte");
    std::println("local static_assert(SIZE): passed");
    std::println("");


    // === EXAMPLE 4: Custom Assert Messages ===
    banner("Example 4: Custom Assert Messages");

    // A hand-written check that, unlike assert(), stays active in release builds
    // and prints its own message.
    auto verify = [](bool condition, const std::string& msg) {
        if (!condition) {
            std::println(stderr, "ASSERTION FAILED: {}", msg);
            std::abort();
        }
    };

    int port = 8080;
    verify(port > 0 && port <= 65535, "port must be in valid range [1, 65535]");
    std::println("verify(port valid): passed");

    std::string name = "Kostas";
    verify(!name.empty(), "name must not be empty");
    std::println("verify(name not empty): passed");
    std::println("");


    // === EXAMPLE 5: Class Invariant Assertions ===
    banner("Example 5: Class Invariant Assertions");

    BoundedBuffer buf(3);
    buf.push(10);
    buf.push(20);
    buf.push(30);
    std::println("pushed 3 items, full: {}", buf.full());

    std::println("popped: {}", buf.pop());
    std::println("popped: {}", buf.pop());
    std::println("size after 2 pops: {}", buf.size());

    // buf.push(1); buf.push(2); buf.push(3); buf.push(4); // would assert: full!
    // buf.pop(); buf.pop();  // after empty: would assert!
    std::println("");


    // === EXAMPLE 6: Preconditions and Postconditions ===
    banner("Example 6: Preconditions and Postconditions");

    std::println("safeSqrt(16.0) = {}", safeSqrt(16.0));
    std::println("safeSqrt(2.0)  = {}", safeSqrt(2.0));
    std::println("safeSqrt(0.0)  = {}", safeSqrt(0.0));
    // safeSqrt(-1.0);  // would assert: precondition violated

    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13};
    std::println("binarySearch(7):  index {}", binarySearch(sorted, 7));
    std::println("binarySearch(10): index {}", binarySearch(sorted, 10));
    // std::vector<int> unsorted = {5,1,3}; binarySearch(unsorted, 3); // asserts!
    std::println("");


    // === EXAMPLE 7: static_assert with Type Traits ===
    banner("Example 7: static_assert with Type Traits");

    std::println("safeAdd<int>(3, 4)     = {}", safeAdd(3, 4));
    std::println("safeAdd<float>(1.5f, 2.5f) = {}", safeAdd(1.5f, 2.5f));
    std::println("safeAdd<double>(1.1, 2.2)  = {}", safeAdd(1.1, 2.2));
    // safeAdd(std::string("a"), std::string("b"));  // compile error: not arithmetic
    // safeAdd(true, false);                         // compile error: no bool

    // More type trait checks
    static_assert(std::is_integral_v<int>,    "int must be integral");
    static_assert(std::is_floating_point_v<double>, "double must be float");
    static_assert(std::is_signed_v<std::int32_t>,  "int32_t must be signed");
    static_assert(std::is_unsigned_v<std::uint32_t>,"uint32_t must be unsigned");
    std::println("type trait static_asserts: all passed");
    std::println("");


    // === EXAMPLE 8: static_assert with constexpr ===
    banner("Example 8: static_assert with constexpr");

    // Checked at compile time — no runtime cost
    static_assert(factorial(5) == 120, "factorial(5) == 120");
    static_assert(factorial(3) == 6,   "factorial(3) == 6");
    std::println("factorial static_asserts: passed at compile time");

    // Binary layout size checks
    struct RecordHeader { std::uint8_t ver; std::uint8_t type; std::uint16_t len; std::uint32_t id; };
    static_assert(sizeof(RecordHeader) == 8, "RecordHeader must be exactly 8 bytes");
    std::println("RecordHeader size static_assert: passed ({} bytes)", sizeof(RecordHeader));
    std::println("");


    // === EXAMPLE 9: C++23 [[assume]] attribute ===
    banner("Example 9: C++23 [[assume]] attribute");

    // [[assume]] tells the compiler to assume a condition is true
    // without checking it — enables optimizations
    // (does NOT check at runtime — UB if assumption is wrong!)

    int val = 42;
    [[assume(val > 0)]];   // C++23 — optimizer hint, not a check
    std::println("[[assume(val > 0)]] — hint to optimizer (not a runtime check)");

    std::println("");
    std::println("=== CassertSTL complete ===");
    return 0;
}
