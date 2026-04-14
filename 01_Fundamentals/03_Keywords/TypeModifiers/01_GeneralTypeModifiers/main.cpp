#include <iostream>

int main() {

    // 🔹 Signed / Unsigned
    signed int a = -10;
    unsigned int b = 10;

    signed char c = -5;
    unsigned char d = 250;

    std::cout << "Signed / Unsigned:\n";
    std::cout << "a = " << a << ", b = " << b << "\n";
    std::cout << "c = " << (int)c << ", d = " << (int)d << "\n";

    // 🔹 Short / Long
    short int e = 32000;
    long int f = 100000L;
    long long g = 123456789LL;

    unsigned short int h = 65000;
    unsigned long int i = 4000000000UL;

    std::cout << "\nShort / Long:\n";
    std::cout << "e = " << e << ", f = " << f << ", g = " << g << "\n";
    std::cout << "h = " << h << ", i = " << i << "\n";

    // 🔹 Const (read-only)
    const int j = 42;
    // j = 10; // ❌ error: cannot modify const variable

    const int* ptr1 = &j;        // pointer to const int
    int k = 100;
    int* const ptr2 = &k;        // const pointer
    const int* const ptr3 = &j;  // const pointer to const int

    std::cout << "\nConst examples:\n";
    std::cout << "j = " << j << "\n";
    std::cout << "*ptr1 = " << *ptr1 << "\n";
    std::cout << "*ptr2 = " << *ptr2 << "\n";
    std::cout << "*ptr3 = " << *ptr3 << "\n";

    // 🔹 Volatile (can change unexpectedly, e.g., hardware or threads)
    volatile int sensorValue = 0;

    // Simulate change
    sensorValue = 5;

    std::cout << "\nVolatile example:\n";
    std::cout << "sensorValue = " << sensorValue << "\n";

    // 🔹 Combining modifiers
    const unsigned long long bigNumber = 123456789ULL;

    std::cout << "\nCombined modifiers:\n";
    std::cout << "bigNumber = " << bigNumber << "\n";

    return 0;
}
