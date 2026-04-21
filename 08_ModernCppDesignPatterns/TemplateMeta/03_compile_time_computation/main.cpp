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
 * Compile: g++ -std=c++20 -Wall -Wextra compile_time_computation.cpp -o compile_time
 */

#include <iostream>
#include <type_traits>
#include <array>
#include <cstdint>

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
    std::cout << "\n=== Classic TMP: Factorial ===\n";
    std::cout << "All computed at COMPILE-TIME!\n";
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << "\n";
    std::cout << "Factorial<10>::value = " << Factorial<10>::value << "\n";
    std::cout << "Factorial<12>::value = " << Factorial<12>::value << "\n";

    // This is a compile-time constant!
    constexpr int f5 = Factorial<5>::value;
    static_assert(f5 == 120, "Factorial<5> should be 120");

    std::cout << "\n✅ Zero runtime cost - computed during compilation!\n";
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
    std::cout << "\n=== Modern C++11: constexpr Factorial ===\n";

    // Compile-time evaluation
    constexpr int f5 = factorial(5);
    constexpr int f10 = factorial(10);

    std::cout << "factorial(5) = " << f5 << "\n";
    std::cout << "factorial(10) = " << f10 << "\n";

    // Can also be used at runtime
    int n = 7;
    std::cout << "factorial(7) = " << factorial(n) << " (runtime)\n";

    std::cout << "\n✅ Cleaner syntax, same zero-cost abstraction!\n";
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
    std::cout << "\n=== Fibonacci Sequence ===\n";

    std::cout << "Classic TMP style:\n";
    std::cout << "  Fib<10> = " << Fib<10>::value << "\n";

    std::cout << "\nModern constexpr style:\n";
    std::cout << "  fib(10) = " << fib(10) << "\n";

    std::cout << "\nFirst 12 Fibonacci numbers:\n  ";
    for (int i = 0; i < 12; ++i) {
        std::cout << fib(i) << " ";
    }
    std::cout << "\n";
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
    std::cout << "\n=== Power Calculation ===\n";

    std::cout << "2^10 = " << Pow<2, 10>::value << "\n";
    std::cout << "3^5 = " << Pow<3, 5>::value << "\n";
    std::cout << "5^3 = " << pow(5, 3) << "\n";

    // Compile-time array sizing
    constexpr int size = pow(2, 5);  // 32
    std::array<int, size> arr;
    std::cout << "\nArray size (compile-time): " << arr.size() << "\n";
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
    std::cout << "\n=== Greatest Common Divisor ===\n";
    std::cout << "gcd(48, 18) = " << gcd<48, 18>::value << "\n";
    std::cout << "gcd(100, 35) = " << gcd_func(100, 35) << "\n";
    std::cout << "gcd(17, 19) = " << gcd_func(17, 19) << " (coprime)\n";
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
    std::cout << "\n=== Prime Number Checking ===\n";
    std::cout << "IsPrime<17>::value = " << IsPrime<17>::value << "\n";
    std::cout << "IsPrime<20>::value = " << IsPrime<20>::value << "\n";
    std::cout << "isPrime(23) = " << isPrime(23) << "\n";
    std::cout << "isPrime(24) = " << isPrime(24) << "\n";

    std::cout << "\nPrimes up to 30: ";
    for (int i = 2; i <= 30; ++i) {
        if (isPrime(i)) std::cout << i << " ";
    }
    std::cout << "\n";
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
    std::cout << "\n=== Compile-Time String Length ===\n";

    constexpr char hello[] = "Hello, World!";
    constexpr size_t len = strlen_constexpr(hello);

    std::cout << "Length of \"" << hello << "\": " << len << "\n";

    // Can use as array size
    std::array<char, len> buffer;
    std::cout << "Buffer size: " << buffer.size() << "\n";
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
    std::cout << "\n=== Compile-Time Array Sum ===\n";

    constexpr int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    constexpr int sum = sum_array(numbers);

    std::cout << "Sum of 1..10: " << sum << " (computed at compile-time)\n";

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
    std::cout << "\n=== Type Size Comparison ===\n";

    using Larger = LargerType<int, long>::type;

    std::cout << "sizeof(int) = " << sizeof(int) << "\n";
    std::cout << "sizeof(long) = " << sizeof(long) << "\n";
    std::cout << "Larger type size: " << sizeof(Larger) << "\n";

    std::cout << "\nLarger type is: ";
    if constexpr (std::is_same_v<Larger, long>) {
        std::cout << "long\n";
    } else {
        std::cout << "int\n";
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
    std::cout << "\n=== Compile-Time Selection ===\n";

    constexpr bool is_64bit = sizeof(void*) == 8;
    constexpr int buffer_size = Select<is_64bit, 1024, 512>::value;

    std::cout << "System is " << (is_64bit ? "64-bit" : "32-bit") << "\n";
    std::cout << "Buffer size: " << buffer_size << "\n";

    // Modern way with if constexpr
    if constexpr (sizeof(void*) == 8) {
        std::cout << "Using 64-bit optimized code path\n";
    } else {
        std::cout << "Using 32-bit code path\n";
    }
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║       COMPILE-TIME COMPUTATION WITH TMP             ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 KEY TAKEAWAYS:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "✅ Template metaprogramming = Compile-time computation\n";
    std::cout << "✅ Zero runtime cost - all work done by compiler\n";
    std::cout << "✅ Classic TMP uses template recursion + specialization\n";
    std::cout << "✅ Modern C++ uses constexpr (cleaner syntax)\n";
    std::cout << "✅ Both achieve same goal: zero-cost abstractions\n";
    std::cout << "✅ Use for: array sizes, type selection, optimization\n";
    std::cout << "\n💡 think-cell uses these techniques for:\n";
    std::cout << "   - Zero-cost generic algorithms\n";
    std::cout << "   - Compile-time type checking\n";
    std::cout << "   - Optimal code generation\n";
    std::cout << std::string(60, '=') << "\n\n";

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
