#include <iostream>
#include <functional>

// 🔹 Simple function with return
int add(int a, int b) {
    return a + b;
}

// 🔹 Void function
void printHello() {
    std::cout << "Hello!\n";
}

// 🔹 Pass by reference
void increment(int& x) {
    x++;
}

// 🔹 Const reference (read-only, efficient)
void printValue(const int& x) {
    std::cout << "Value: " << x << "\n";
}

// 🔹 Default parameter
int multiply(int a, int b = 2) {
    return a * b;
}

// 🔹 Function overloading
int square(int x) {
    return x * x;
}

double square(double x) {
    return x * x;
}

// 🔹 Inline function
inline int cube(int x) {
    return x * x * x;
}

// 🔹 Recursive function
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// 🔹 constexpr (evaluated at compile time if possible)
constexpr int power(int base, int exp) {
    return (exp == 0) ? 1 : base * power(base, exp - 1);
}

// 🔹 noexcept (guarantees no exception)
int safeDivide(int a, int b) noexcept {
    return (b != 0) ? (a / b) : 0;
}

// 🔹 Function pointer
int operation(int a, int b, int (*func)(int, int)) {
    return func(a, b);
}

// 🔹 Function used for pointer
int subtract(int a, int b) {
    return a - b;
}

int main() {

    // Basic usage
    std::cout << "Add: " << add(3, 4) << "\n";

    // Void
    printHello();

    // Reference
    int x = 5;
    increment(x);
    std::cout << "Incremented: " << x << "\n";

    // Const reference
    printValue(x);

    // Default parameter
    std::cout << "Multiply default: " << multiply(5) << "\n";

    // Overloading
    std::cout << "Square int: " << square(4) << "\n";
    std::cout << "Square double: " << square(2.5) << "\n";

    // Inline
    std::cout << "Cube: " << cube(3) << "\n";

    // Recursion
    std::cout << "Factorial: " << factorial(5) << "\n";

    // Constexpr
    constexpr int p = power(2, 3);
    std::cout << "Power constexpr: " << p << "\n";

    // Noexcept
    std::cout << "Safe divide: " << safeDivide(10, 2) << "\n";

    // Function pointer
    std::cout << "Operation (subtract): "
              << operation(10, 3, subtract) << "\n";

    // 🔹 Lambda (modern C++)
    auto lambdaAdd = [](int a, int b) {
        return a + b;
    };

    std::cout << "Lambda add: " << lambdaAdd(5, 6) << "\n";

    // std::function (more flexible function wrapper)
    std::function<int(int, int)> func = add;
    std::cout << "std::function: " << func(7, 8) << "\n";

    return 0;
}
