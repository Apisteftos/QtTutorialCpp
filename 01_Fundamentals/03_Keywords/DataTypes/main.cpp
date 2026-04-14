#include <iostream>
#include <string>

// enum (user-defined data type)
enum Color {
    RED,
    GREEN,
    BLUE
};

// function using void
void printSeparator() {
    std::cout << "---------------------\n";
}

int main() {

    // Integer types
    int a = 10;
    short b = 5;
    long c = 100000L;
    long long d = 123456789LL;

    // Floating point types
    float e = 3.14f;
    double f = 3.1415926535;
    long double g = 3.141592653589793238L;

    // Character types
    char h = 'A';
    wchar_t i = L'Ω';        // wide character
    char8_t j = u8'A';       // UTF-8 (C++20)
    char16_t k = u'A';       // UTF-16
    char32_t l = U'A';       // UTF-32

    // Boolean
    bool isTrue = true;
    bool isFalse = false;

    // void pointer (generic pointer)
    void* ptr = nullptr;

    // enum usage
    Color myColor = GREEN;

    // Printing values
    std::cout << "Integer types:\n";
    std::cout << a << ", " << b << ", " << c << ", " << d << "\n";
    printSeparator();

    std::cout << "Floating types:\n";
    std::cout << e << ", " << f << ", " << g << "\n";
    printSeparator();

    std::cout << "Character types:\n";
    std::cout << h << "\n";
    std::wcout << i << "\n"; // wide output
    std::cout << (char)j << ", " << (char)k << ", " << (char)l << "\n";
    printSeparator();

    std::cout << "Boolean:\n";
    std::cout << std::boolalpha << isTrue << ", " << isFalse << "\n";
    printSeparator();

    std::cout << "Enum:\n";
    if (myColor == GREEN) {
        std::cout << "Color is GREEN\n";
    }
    printSeparator();

    // void pointer usage (casting required)
    int x = 42;
    ptr = &x;
    std::cout << "Void pointer points to: " << *(static_cast<int*>(ptr)) << "\n";

    return 0;
}
