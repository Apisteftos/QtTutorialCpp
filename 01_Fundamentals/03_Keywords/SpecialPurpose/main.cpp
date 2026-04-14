#include <iostream>
#include <typeinfo>

// ===================== ALIGNMENT EXAMPLES =====================

// alignas: forces specific memory alignment for performance or hardware requirements
struct alignas(16) Vec4
{
    float x, y, z, w;
};

// alignof: checks alignment requirement of a type
struct MyStruct
{
    char a;
    int b;
};

// ===================== CONSTEXPR / CONSTEVAL / CONSTINIT =====================

// constexpr: evaluated at compile time if possible
constexpr int square(int x)
{
    return x * x;
}

// consteval: MUST be evaluated at compile time (C++20)
consteval int cube(int x)
{
    return x * x * x;
}

// constinit: ensures variable is initialized at compile time (no runtime init)
constinit int globalValue = 42;

// ===================== NOEXCEPT =====================

// noexcept: guarantees function will not throw exceptions (or helps optimization)
void safeFunction() noexcept
{
    std::cout << "This function will not throw exceptions\n";
}

// Example function that conditionally noexcept
int riskyFunction(int x) noexcept(noexcept(x > 0))
{
    return x > 0 ? x : 0;
}

// ===================== MAIN DEMONSTRATION =====================

int main()
{
    // ---------------- sizeof ----------------
    int arr[10];
    std::cout << "sizeof(int) = " << sizeof(int) << " bytes\n";
    std::cout << "sizeof(arr) = " << sizeof(arr) << " bytes\n";

    Vec4 v;
    std::cout << "sizeof(Vec4) = " << sizeof(Vec4) << " bytes\n";

    // ---------------- alignof ----------------
    std::cout << "alignof(int) = " << alignof(int) << "\n";
    std::cout << "alignof(Vec4) = " << alignof(Vec4) << "\n";
    std::cout << "alignof(MyStruct) = " << alignof(MyStruct) << "\n";

    // ---------------- typeid ----------------
    int a = 10;
    double b = 3.14;

    std::cout << "typeid(a).name() = " << typeid(a).name() << "\n";
    std::cout << "typeid(b).name() = " << typeid(b).name() << "\n";

    // ---------------- decltype ----------------
    decltype(a) newInt = 100;          // same type as a (int)
    decltype(b + a) result = 0.0;      // expression type (double)

    std::cout << "newInt = " << newInt << "\n";
    std::cout << "result = " << result << "\n";

    // ---------------- constexpr ----------------
    constexpr int val = square(5);
    std::cout << "constexpr square(5) = " << val << "\n";

    // ---------------- consteval ----------------
    constexpr int cubeVal = cube(3);
    std::cout << "consteval cube(3) = " << cubeVal << "\n";

    // ---------------- constinit ----------------
    std::cout << "constinit globalValue = " << globalValue << "\n";

    // ---------------- noexcept ----------------
    safeFunction();
    std::cout << "riskyFunction(-5) = " << riskyFunction(-5) << "\n";

    return 0;
}
