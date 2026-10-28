/*
 * compile_time_computation.cpp
 *
 * Compile-Time Computations with Template Metaprogramming
 *
 * Difficulty: 🔴 Advanced
 *
 * Key Concepts:
 * - Template recursion
 * - Compile-time values
 * - constexpr functions
 * - Template specialization for base cases
 * - Modern vs Classic TMP
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <type_traits>
#include <array>
#include <cstdint>
#include <print>

// ============================================================================
// PART 1: Classic Template Metaprogramming - Factorial
// ============================================================================

namespace ClassicTMP {

// Template recursive factorial
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

// Base case - template specialization
template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

void demonstrate() {
    std::println("");
    std::println("=== Classic TMP: Factorial ===");
    std::println("All computed at COMPILE-TIME!");
    std::println("Factorial<5>::value = {}", Factorial<5>::value);
    std::println("Factorial<10>::value = {}", Factorial<10>::value);
    std::println("Factorial<12>::value = {}", Factorial<12>::value);

    // This is a compile-time constant!
    constexpr int f5 = Factorial<5>::value;
    static_assert(f5 == 120, "Factorial<5> should be 120");

    std::println("");
    std::println("✅ Zero runtime cost - computed during compilation!");
}
}

// ============================================================================
// PART 2: Modern C++11 - constexpr Functions
// ============================================================================

namespace ModernConstexpr {

// Modern way - constexpr function (cleaner!)
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

void demonstrate() {
    std::println("");
    std::println("=== Modern C++11: constexpr Factorial ===");

    // Compile-time evaluation
    constexpr int f5 = factorial(5);
    constexpr int f10 = factorial(10);

    std::println("factorial(5) = {}", f5);
    std::println("factorial(10) = {}", f10);

    // Can also be used at runtime
    int n = 7;
    std::println("factorial(7) = {} (runtime)", factorial(n));

    std::println("");
    std::println("✅ Cleaner syntax, same zero-cost abstraction!");
}
}

// ============================================================================
// PART 3: Fibonacci - Both Styles
// ============================================================================

namespace Fibonacci {

// Classic TMP style
template<int N>
struct Fib {
    static constexpr int value = Fib<N-1>::value + Fib<N-2>::value;
};

template<>
struct Fib<0> {
    static constexpr int value = 0;
};

template<>
struct Fib<1> {
    static constexpr int value = 1;
};

// Modern constexpr style
constexpr int fib(int n) {
    return (n <= 1) ? n : fib(n - 1) + fib(n - 2);
}

void demonstrate() {
    std::println("");
    std::println("=== Fibonacci Sequence ===");

    std::println("Classic TMP style:");
    std::println("  Fib<10> = {}", Fib<10>::value);

    std::println("");
    std::println("Modern constexpr style:");
    std::println("  fib(10) = {}", fib(10));

    std::println("");
    std::print("First 12 Fibonacci numbers:\n  ");
    for (int i = 0; i < 12; ++i) {
        std::print("{} ", fib(i));
    }
    std::println("");
}
}

// ============================================================================
// PART 4: Power Calculation
// ============================================================================

namespace Power {

// Classic TMP
template<int Base, int Exp>
struct Pow {
    static constexpr int value = Base * Pow<Base, Exp - 1>::value;
};

template<int Base>
struct Pow<Base, 0> {
    static constexpr int value = 1;
};

// Modern constexpr
constexpr int pow(int base, int exp) {
    return (exp == 0) ? 1 : base * pow(base, exp - 1);
}

void demonstrate() {
    std::println("");
    std::println("=== Power Calculation ===");

    std::println("2^10 = {}", Pow<2, 10>::value);
    std::println("3^5 = {}", Pow<3, 5>::value);
    std::println("5^3 = {}", pow(5, 3));

    // Compile-time array sizing
    constexpr int size = pow(2, 5);  // 32
    std::array<int, size> arr;
    std::println("");
    std::println("Array size (compile-time): {}", arr.size());
}
}

// ============================================================================
// PART 5: GCD (Greatest Common Divisor)
// ============================================================================

namespace GCD {

// Euclidean algorithm at compile-time
template<int A, int B>
struct gcd {
    static constexpr int value = gcd<B, A % B>::value;
};

template<int A>
struct gcd<A, 0> {
    static constexpr int value = A;
};

// Modern version
constexpr int gcd_func(int a, int b) {
    return (b == 0) ? a : gcd_func(b, a % b);
}

void demonstrate() {
    std::println("");
    std::println("=== Greatest Common Divisor ===");
    std::println("gcd(48, 18) = {}", gcd<48, 18>::value);
    std::println("gcd(100, 35) = {}", gcd_func(100, 35));
    std::println("gcd(17, 19) = {} (coprime)", gcd_func(17, 19));
}
}

// ============================================================================
// PART 6: Prime Number Check
// ============================================================================

namespace Prime {

// Helper for checking divisibility
template<int N, int D>
struct is_prime_helper {
    static constexpr bool value =
        (N % D != 0) && is_prime_helper<N, D - 1>::value;
};

template<int N>
struct is_prime_helper<N, 1> {
    static constexpr bool value = true;
};

// Main prime checker
template<int N>
struct IsPrime {
    static constexpr bool value =
        (N < 2) ? false : is_prime_helper<N, N / 2>::value;
};

// Modern constexpr version
constexpr bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

void demonstrate() {
    std::println("");
    std::println("=== Prime Number Checking ===");
    std::println("IsPrime<17>::value = {}", IsPrime<17>::value);
    std::println("IsPrime<20>::value = {}", IsPrime<20>::value);
    std::println("isPrime(23) = {}", isPrime(23));
    std::println("isPrime(24) = {}", isPrime(24));

    std::println("");
    std::print("Primes up to 30: ");
    for (int i = 2; i <= 30; ++i) {
        if (isPrime(i)) std::print("{} ", i);
    }
    std::println("");
}
}

// ============================================================================
// PART 7: Compile-Time String Length
// ============================================================================

namespace StringLength {

// Classic TMP for string length
template<int N>
constexpr int strlen_recursive(const char (&str)[N], int i = 0) {
    return (str[i] == '\0') ? i : strlen_recursive(str, i + 1);
}

// Modern version
constexpr size_t strlen_constexpr(const char* str) {
    return (*str == '\0') ? 0 : 1 + strlen_constexpr(str + 1);
}

void demonstrate() {
    std::println("");
    std::println("=== Compile-Time String Length ===");

    constexpr char hello[] = "Hello, World!";
    constexpr size_t len = strlen_constexpr(hello);

    std::println("Length of \"{}\": {}", hello, len);

    // Can use as array size
    std::array<char, len> buffer;
    std::println("Buffer size: {}", buffer.size());
}
}

// ============================================================================
// PART 8: Compile-Time Array Sum
// ============================================================================

namespace ArraySum {

template<typename T, size_t N>
constexpr T sum_array(const T (&arr)[N], size_t i = 0) {
    return (i == N) ? T{} : arr[i] + sum_array(arr, i + 1);
}

void demonstrate() {
    std::println("");
    std::println("=== Compile-Time Array Sum ===");

    constexpr int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    constexpr int sum = sum_array(numbers);

    std::println("Sum of 1..10: {} (computed at compile-time)", sum);

    static_assert(sum == 55, "Sum should be 55");
}
}

// ============================================================================
// PART 9: Type Size Comparison
// ============================================================================

namespace TypeSize {

// Get larger type
template<typename T1, typename T2>
struct LargerType {
    using type = std::conditional_t<(sizeof(T1) > sizeof(T2)), T1, T2>;
};

void demonstrate() {
    std::println("");
    std::println("=== Type Size Comparison ===");

    using Larger = LargerType<int, long>::type;

    std::println("sizeof(int) = {}", sizeof(int));
    std::println("sizeof(long) = {}", sizeof(long));
    std::println("Larger type size: {}", sizeof(Larger));

    std::println("");
    std::print("Larger type is: ");
    if constexpr (std::is_same_v<Larger, long>) {
        std::println("long");
    } else {
        std::println("int");
    }
}
}

// ============================================================================
// PART 10: Compile-Time Selection
// ============================================================================

namespace CompileTimeSelection {

// Select value based on condition
template<bool Condition, int TrueVal, int FalseVal>
struct Select {
    static constexpr int value = Condition ? TrueVal : FalseVal;
};

void demonstrate() {
    std::println("");
    std::println("=== Compile-Time Selection ===");

    constexpr bool is_64bit = sizeof(void*) == 8;
    constexpr int buffer_size = Select<is_64bit, 1024, 512>::value;

    std::println("System is {}", (is_64bit ? "64-bit" : "32-bit"));
    std::println("Buffer size: {}", buffer_size);

    // Modern way with if constexpr
    if constexpr (sizeof(void*) == 8) {
        std::println("Using 64-bit optimized code path");
    } else {
        std::println("Using 32-bit code path");
    }
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║       COMPILE-TIME COMPUTATION WITH TMP             ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    ClassicTMP::demonstrate();
    ModernConstexpr::demonstrate();
    Fibonacci::demonstrate();
    Power::demonstrate();
    GCD::demonstrate();
    Prime::demonstrate();
    StringLength::demonstrate();
    ArraySum::demonstrate();
    TypeSize::demonstrate();
    CompileTimeSelection::demonstrate();

    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("🎓 KEY TAKEAWAYS:");
    std::println("{}", std::string(60, '='));
    std::println("{}", std::string(60, '='));
    std::println("");

    return 0;
}


//  * COMPILE AND RUN:
//  * ===============
//  * g++ -std=c++20 -Wall -Wextra compile_time_computation.cpp -o compile_time
//  * ./compile_time
//  *
//  * KEY CONCEPTS:
//  * ============
//  * 1. Template recursion with specialization for base cases
//  * 2. constexpr functions (modern, cleaner alternative)
//  * 3. All computations happen at COMPILE-TIME
//  * 4. Zero runtime overhead
//  * 5. Type-safe, compile-time validated
//  *
//  * CLASSIC TMP PATTERN:
//  * ===================
//  * template<int N>
//  * struct Compute {
//  *     static constexpr int value = /* recursive formula */;
//  * };
//  *
//  * template<>  // Base case
//  * struct Compute<0> {
//  *     static constexpr int value = /* base value */;
//  * };
//  *
//  * MODERN CONSTEXPR PATTERN:
//  * ========================
//  * constexpr int compute(int n) {
//  *     return (base_case) ? base_value : recursive_formula;
//  * }
//  *
//  * WHEN TO USE:
//  * ===========
//  * - Performance-critical code
//  * - Array sizing
//  * - Type selection
//  * - Compile-time validation
//  * - Generic library development
//  *
//  * NEXT STEPS:
//  * ===========
//  * - Study type_list.cpp for type manipulation
//  * - Learn SFINAE_examples.cpp for conditional compilation
//  * - Explore template_traits.cpp for type traits
//  * - Combine TMP with concepts in concepts_and_constraints.cpp
