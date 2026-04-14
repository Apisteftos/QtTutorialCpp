#include <iostream>
#include <cmath>
#include <type_traits>

bool isValid(int x) {
    return x >= 0 && x <= 100;
}

template <typename T>
void checkType(T value) {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "Integer type\n";
    } else {
        std::cout << "Non-integer type\n";
    }
}

int main() {
    int x = 15;
    double a = 0.1 + 0.2;
    double b = 0.3;
    int* ptr = nullptr;
    bool isReady = true;

    // Basic if
    if (x > 0) {
        std::cout << "Positive\n";
    }

    // if - else
    if (x > 0) {
        std::cout << "Positive again\n";
    } else {
        std::cout << "Not positive\n";
    }

    // if - else if - else
    if (x > 0) {
        std::cout << "Positive (chain)\n";
    } else if (x == 0) {
        std::cout << "Zero\n";
    } else {
        std::cout << "Negative\n";
    }

    // Nested if
    if (x > 0) {
        if (x % 2 == 0) {
            std::cout << "Positive even\n";
        } else {
            std::cout << "Positive odd\n";
        }
    }

    // Logical operators
    if (x > 0 && x < 20) {
        std::cout << "Between 1 and 19\n";
    }

    if (x < 0 || x > 100) {
        std::cout << "Out of range\n";
    }

    if (!(x == 0)) {
        std::cout << "Not zero\n";
    }

    // Initialization inside if (C++17)
    if (int y = x * 2; y > 20) {
        std::cout << "y > 20\n";
    } else if (y == 20) {
        std::cout << "y == 20\n";
    } else {
        std::cout << "y < 20\n";
    }

    // Ternary operator
    std::string result = (x > 0) ? "Positive" : "Negative or zero";
    std::cout << result << "\n";

    // Pointer check
    if (ptr) {
        std::cout << "Pointer not null\n";
    } else {
        std::cout << "Pointer is null\n";
    }

    // Boolean check
    if (isReady) {
        std::cout << "Ready!\n";
    }

    // One-line if
    if (x > 0) std::cout << "One-line if\n";

    // Floating point comparison
    if (std::abs(a - b) < 0.0001) {
        std::cout << "Almost equal\n";
    }

    // Function call in if
    if (isValid(x)) {
        std::cout << "x is valid\n";
    }

    // if constexpr (compile-time)
    checkType(x);
    checkType(3.14);

    return 0;
}
