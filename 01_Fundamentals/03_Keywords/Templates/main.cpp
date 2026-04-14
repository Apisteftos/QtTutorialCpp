#include <iostream>
#include <vector>
#include <type_traits>
using namespace std;

// ===============================
// 1. FUNCTION TEMPLATE
// ===============================

template <typename T>
T add(T a, T b) {
    return a + b;
}

// ===============================
// 2. FUNCTION TEMPLATE (AUTO + DECLTYPE)
// ===============================

template <typename T1, typename T2>
auto multiply(T1 a, T2 b) -> decltype(a * b) {
    return a * b;
}

// ===============================
// 3. CLASS TEMPLATE
// ===============================

template <class T>
class Box {
private:
    T value;

public:
    Box(T v) : value(v) {}

    T getValue() const {
        return value;
    }

    void setValue(T v) {
        value = v;
    }
};

// ===============================
// 4. CLASS TEMPLATE WITH DEFAULT TYPE
// ===============================

template <typename T = int>
class Calculator {
public:
    T square(T x) {
        return x * x;
    }
};

// ===============================
// 5. TEMPLATE SPECIALIZATION
// ===============================

template <typename T>
class Printer {
public:
    void print(T value) {
        cout << "Generic: " << value << endl;
    }
};

// Specialization for const char*
template <>
class Printer<const char*> {
public:
    void print(const char* value) {
        cout << "String: " << value << endl;
    }
};

// ===============================
// 6. VARIADIC TEMPLATE (MULTIPLE ARGS)
// ===============================

template <typename T>
T sum(T t) {
    return t;
}

template <typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}

// ===============================
// 7. CONSTEXPR TEMPLATE USAGE
// ===============================

template <typename T>
constexpr T square_constexpr(T x) {
    return x * x;
}

// ===============================
// 8. STATIC ASSERT WITH TEMPLATE
// ===============================

template <typename T>
void checkType() {
    static_assert(is_integral<T>::value, "T must be an integer type!");
    cout << "Valid integer type" << endl;
}

// ===============================
// MAIN
// ===============================

int main() {

    // Function template
    cout << add<int>(3, 5) << endl;
    cout << add(2.5, 4.5) << endl;

    // multiply template
    cout << multiply(3, 2.5) << endl;

    // Class template
    Box<int> intBox(10);
    cout << intBox.getValue() << endl;

    Box<string> strBox("Hello Templates");
    cout << strBox.getValue() << endl;

    // Default template parameter
    Calculator<> calc;
    cout << calc.square(6) << endl;

    // Specialization
    Printer<int> p1;
    p1.print(100);

    Printer<const char*> p2;
    p2.print("Hello specialization");

    // Variadic template
    cout << sum(1, 2, 3, 4, 5) << endl;

    // constexpr
    cout << square_constexpr(8) << endl;

    // static_assert
    checkType<int>();
    // checkType<double>(); // would fail

    return 0;
}
