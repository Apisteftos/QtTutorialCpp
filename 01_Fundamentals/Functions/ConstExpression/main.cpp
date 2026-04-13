#include <iostream>

/*


In C++, constexpr is a keyword that is used to declare that an object or function can be
evaluated at compile time. This allows computations to be performed at compile time
rather than at runtime, potentially improving performance and allowing for more flexibility in code.


constexpr Variables: In C++11 and later, you can declare variables as constexpr.
This means that their value must be computable at compile time. These variables can
be used in contexts where a constant expression is required, such as array sizes,
template arguments, and switch statements.


constexpr Functions: Functions can also be declared as constexpr, indicating that
they can be evaluated at compile time when called with constant expressions as arguments.
constexpr functions can have multiple return statements and conditional expressions as
long as the conditions can be evaluated at compile time.


*/





// constexpr function to calculate the factorial of a number
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

// constexpr function to calculate the nth Fibonacci number
constexpr int fibonacci(int n) {
    return (n <= 1) ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    // Using constexpr variables
    constexpr int arraySize = 5;
    int myArray[arraySize]; // Valid array size

    // Using constexpr functions
    constexpr int fact5 = factorial(5); // Computed at compile time
    constexpr int fib10 = fibonacci(10); // Computed at compile time

    // Print results
    std::cout << "Factorial of 5: " << fact5 << std::endl;
    std::cout << "10th Fibonacci number: " << fib10 << std::endl;

    return 0;
}
