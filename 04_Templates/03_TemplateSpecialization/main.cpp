#include <cmath>
#include <cstddef>
#include <cstring>
#include <print>
#include <string>

// ============================================
// WHAT IS TEMPLATE SPECIALIZATION?
// ============================================

/*
 * TEMPLATE SPECIALIZATION:
 * - Custom implementation for specific types
 * - Overrides generic template
 * - Full or partial specialization
 * - Enables type-specific optimizations
 *
 * TYPES OF SPECIALIZATION:
 * - Full (explicit) specialization
 * - Partial specialization (class templates only)
 * - Function template specialization
 * - Class template specialization
 *
 * WHY SPECIALIZE?
 * - Type-specific behavior needed
 * - Performance optimizations
 * - Different data structures for types
 * - Handle special cases (pointers, references)
 * - Custom algorithms for types
 *
 * SYNTAX:
 * - template<>  // Full specialization
 * - template<typename T>  // Partial specialization
 * - Must specialize entire class or function
 *
 * LIMITATIONS:
 * - No partial specialization for functions
 * - Specialization must match template signature
 * - Order matters (more specific preferred)
 * - Must be in same namespace
 *
 * COMMON USES:
 * - Pointer type handling
 * - String optimizations
 * - Boolean packing (vector<bool>)
 * - Type traits
 * - SFINAE techniques
 */

// ============================================
// === EXAMPLE 1: BASIC FUNCTION SPECIALIZATION ===
// ============================================

// Generic template
template<typename T>
bool isEqual(T a, T b) {
    std::println("  Generic comparison");
    return a == b;
}

// Specialization for double (epsilon comparison)
template<>
bool isEqual<double>(double a, double b) {
    std::println("  Specialized double comparison (epsilon)");
    const double epsilon = 0.0001;
    return std::abs(a - b) < epsilon;
}

// Specialization for C-strings
template<>
bool isEqual<const char*>(const char* a, const char* b) {
    std::println("  Specialized string comparison (strcmp)");
    return std::strcmp(a, b) == 0;
}

void demonstrateBasicFunction() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: FUNCTION SPECIALIZATION║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Generic comparison ---");
    bool r1 = isEqual(5, 5);
    std::println("5 == 5: {}", r1);
    bool r2 = isEqual(5, 10);
    std::println("5 == 10: {}", r2);

    std::println("");
    std::println("--- Specialized double (epsilon) ---");
    bool r3 = isEqual(3.14159, 3.14160);
    std::println("3.14159 == 3.14160: {}", r3);

    std::println("");
    std::println("--- Specialized string (strcmp) ---");
    bool r4 = isEqual("hello", "hello");
    std::println("\"hello\" == \"hello\": {}", r4);
    bool r5 = isEqual("hello", "world");
    std::println("\"hello\" == \"world\": {}", r5);
}

// ============================================
// === EXAMPLE 2: BASIC CLASS SPECIALIZATION ===
// ============================================

// Generic template
template<typename T>
class Storage {
private:
    T data;

public:
    Storage(T d) : data(d) {
        std::println("  Generic Storage constructor");
    }

    void display() const {
        std::println("  Generic data: {}", data);
    }

    T getData() const { return data; }
};

// Full specialization for const char*
template<>
class Storage<const char*> {
private:
    char* data;

public:
    Storage(const char* d) {
        std::println("  Specialized Storage<const char*> constructor");
        data = new char[std::strlen(d) + 1];
        std::strcpy(data, d);
    }

    ~Storage() {
        delete[] data;
        std::println("  Specialized destructor (freed memory)");
    }

    // Copy constructor
    Storage(const Storage& other) {
        data = new char[std::strlen(other.data) + 1];
        std::strcpy(data, other.data);
    }

    // Copy assignment would shallow-copy the pointer, so it is disabled
    Storage& operator=(const Storage&) = delete;

    void display() const {
        std::println("  String data: \"{}\"", data);
    }

    const char* getData() const { return data; }
};

void demonstrateBasicClass() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: CLASS SPECIALIZATION   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Generic storage ---");
    Storage<int> intStorage(42);
    intStorage.display();

    Storage<double> doubleStorage(3.14);
    doubleStorage.display();

    std::println("");
    std::println("--- Specialized storage (manages memory) ---");
    {
        Storage<const char*> strStorage("Hello World");
        strStorage.display();
        std::println("  Leaving scope...");
    }
}

// ============================================
// === EXAMPLE 3: PARTIAL SPECIALIZATION - POINTERS ===
// ============================================

// Primary template
template<typename T>
class Container {
public:
    void info() {
        std::println("  Container for regular type");
    }

    void use(T value) {
        std::println("  Using value directly: {}", value);
    }
};

// Partial specialization for pointer types
template<typename T>
class Container<T*> {
public:
    void info() {
        std::println("  Container for POINTER type");
    }

    void use(T* ptr) {
        std::println("  Dereferencing pointer: {}", *ptr);
    }
};

// Partial specialization for const pointer types
template<typename T>
class Container<const T*> {
public:
    void info() {
        std::println("  Container for CONST POINTER type");
    }

    void use(const T* ptr) {
        std::println("  Reading const pointer: {}", *ptr);
    }
};

void demonstratePartialPointer() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: PARTIAL - POINTERS     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular type ---");
    Container<int> c1;
    c1.info();
    c1.use(42);

    std::println("");
    std::println("--- Pointer type ---");
    Container<int*> c2;
    c2.info();
    int value = 100;
    c2.use(&value);

    std::println("");
    std::println("--- Const pointer type ---");
    Container<const int*> c3;
    c3.info();
    c3.use(&value);
}

// ============================================
// === EXAMPLE 4: PARTIAL SPECIALIZATION - ARRAYS ===
// ============================================

// Primary template
template<typename T>
class ArrayWrapper {
public:
    void describe() {
        std::println("  Not an array type");
    }
};

// Partial specialization for arrays
template<typename T>
class ArrayWrapper<T[]> {
public:
    void describe() {
        std::println("  Unbounded array type");
    }
};

// Partial specialization for arrays with known size
template<typename T, std::size_t N>
class ArrayWrapper<T[N]> {
public:
    void describe() {
        std::println("  Bounded array type, size: {}", N);
    }
};

void demonstratePartialArray() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: PARTIAL - ARRAYS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Different array types ---");
    ArrayWrapper<int> w1;
    w1.describe();

    ArrayWrapper<int[]> w2;
    w2.describe();

    ArrayWrapper<int[5]> w3;
    w3.describe();

    ArrayWrapper<double[10]> w4;
    w4.describe();
}

// ============================================
// === EXAMPLE 5: MULTIPLE PARAMETER SPECIALIZATION ===
// ============================================

// Primary template
template<typename T1, typename T2>
class Pair {
public:
    void describe() {
        std::println("  Generic: Two different types");
    }
};

// Specialization: both types same
template<typename T>
class Pair<T, T> {
public:
    void describe() {
        std::println("  Specialized: Both types SAME");
    }
};

// Specialization: first is int
template<typename T2>
class Pair<int, T2> {
public:
    void describe() {
        std::println("  Specialized: First type is INT");
    }
};

// Specialization: second is pointer
template<typename T1, typename T2>
class Pair<T1, T2*> {
public:
    void describe() {
        std::println("  Specialized: Second type is POINTER");
    }
};

// Full specialization: both int
// (needed: Pair<int, int> would otherwise be ambiguous between
//  Pair<T, T> and Pair<int, T2>)
template<>
class Pair<int, int> {
public:
    void describe() {
        std::println("  Full specialization: BOTH INT");
    }
};

void demonstrateMultipleParam() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MULTIPLE PARAMETERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Different combinations ---");
    Pair<double, std::string> p1;
    p1.describe();  // Generic

    Pair<int, std::string> p2;
    p2.describe();  // First is int

    Pair<double, double> p3;
    p3.describe();  // Both same

    Pair<double, int*> p4;
    p4.describe();  // Second is pointer

    Pair<int, int> p5;
    p5.describe();  // Full specialization (most specific)
}

// ============================================
// === EXAMPLE 6: TYPE TRAITS PATTERN ===
// ============================================

// Primary template - not a pointer
template<typename T>
struct IsPointer {
    static constexpr bool value = false;

    static void check() {
        std::println("  Not a pointer type");
    }
};

// Specialization for pointer types
template<typename T>
struct IsPointer<T*> {
    static constexpr bool value = true;

    static void check() {
        std::println("  IS a pointer type");
    }
};

// Primary template - get value type
template<typename T>
struct ValueType {
    using type = T;

    static void display() {
        std::println("  Regular type");
    }
};

// Specialization for pointers - extract pointed-to type
template<typename T>
struct ValueType<T*> {
    using type = T;

    static void display() {
        std::println("  Pointer type, pointing to base type");
    }
};

void demonstrateTypeTraits() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: TYPE TRAITS            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- IsPointer trait ---");
    std::print("int: {} - ", IsPointer<int>::value);
    IsPointer<int>::check();

    std::print("int*: {} - ", IsPointer<int*>::value);
    IsPointer<int*>::check();

    std::print("double**: {} - ", IsPointer<double**>::value);
    IsPointer<double**>::check();

    std::println("");
    std::println("--- ValueType trait ---");
    ValueType<int>::display();
    ValueType<int*>::display();
}

// ============================================
// === EXAMPLE 7: OPTIMIZATION EXAMPLE ===
// ============================================

// Generic swap (works for all types)
template<typename T>
void mySwap(T& a, T& b) {
    std::println("  Generic swap (uses copy)");
    T temp = a;
    a = b;
    b = temp;
}

// Specialization for large arrays (memcpy optimization)
template<>
void mySwap<char[1024]>(char (&a)[1024], char (&b)[1024]) {
    std::println("  Optimized swap for large array (memcpy)");
    char temp[1024];
    std::memcpy(temp, a, 1024);
    std::memcpy(a, b, 1024);
    std::memcpy(b, temp, 1024);
}

void demonstrateOptimization() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: OPTIMIZATION           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular swap ---");
    int x = 10, y = 20;
    mySwap(x, y);
    std::println("  After swap: x={}, y={}", x, y);

    std::println("");
    std::println("--- Optimized large array swap ---");
    char arr1[1024] = "Array 1";
    char arr2[1024] = "Array 2";
    mySwap(arr1, arr2);
    std::println("  arr1: {}", static_cast<const char*>(arr1));
    std::println("  arr2: {}", static_cast<const char*>(arr2));
}

// ============================================
// === EXAMPLE 8: VECTOR<BOOL> PATTERN ===
// ============================================

// Generic vector (simplified)
template<typename T>
class SimpleVector {
private:
    T* data;
    std::size_t size;

public:
    SimpleVector(std::size_t n) : size(n) {
        data = new T[n];
        std::println("  SimpleVector<T> allocated {} bytes", sizeof(T) * n);
    }

    ~SimpleVector() {
        delete[] data;
    }

    T& operator[](std::size_t index) {
        return data[index];
    }

    std::size_t getSize() const { return size; }
};

// Specialization for bool (bit packing)
template<>
class SimpleVector<bool> {
private:
    unsigned char* data;
    std::size_t size;

public:
    // Proxy class for bit access
    class BitReference {
        unsigned char& byte;
        unsigned char mask;
    public:
        BitReference(unsigned char& b, unsigned char m) : byte(b), mask(m) {}

        operator bool() const {
            return (byte & mask) != 0;
        }

        BitReference& operator=(bool value) {
            if (value)
                byte |= mask;
            else
                byte &= static_cast<unsigned char>(~mask);
            return *this;
        }
    };

    SimpleVector(std::size_t n) : size(n) {
        std::size_t bytes = (n + 7) / 8;  // Round up
        data = new unsigned char[bytes]();
        std::println("  SimpleVector<bool> allocated {} bytes (bit packed!)", bytes);
    }

    ~SimpleVector() {
        delete[] data;
    }

    BitReference operator[](std::size_t index) {
        std::size_t byteIndex = index / 8;
        unsigned char mask = static_cast<unsigned char>(1u << (index % 8));
        return BitReference(data[byteIndex], mask);
    }

    std::size_t getSize() const { return size; }
};

void demonstrateVectorBool() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: VECTOR<BOOL> PATTERN   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular vector ---");
    SimpleVector<int> intVec(100);
    intVec[0] = 42;
    std::println("  intVec[0]: {}", intVec[0]);

    std::println("");
    std::println("--- Bool vector (bit packed) ---");
    SimpleVector<bool> boolVec(100);
    boolVec[0] = true;
    boolVec[1] = false;
    boolVec[2] = true;
    // The proxy is not directly formattable: convert it to bool first
    std::println("  boolVec[0]: {}", static_cast<bool>(boolVec[0]));
    std::println("  boolVec[1]: {}", static_cast<bool>(boolVec[1]));
    std::println("  boolVec[2]: {}", static_cast<bool>(boolVec[2]));
}

// ============================================
// === EXAMPLE 9: MEMBER FUNCTION SPECIALIZATION ===
// ============================================

template<typename T>
class Calculator {
public:
    T add(T a, T b) {
        std::println("  Generic add");
        return a + b;
    }

    T multiply(T a, T b);  // Declare only
};

// Generic definition
template<typename T>
T Calculator<T>::multiply(T a, T b) {
    std::println("  Generic multiply");
    return a * b;
}

// Specialization of member function for int
template<>
int Calculator<int>::multiply(int a, int b) {
    std::println("  Specialized multiply for int");
    return a * b;
}

void demonstrateMemberSpecialization() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: MEMBER SPECIALIZATION  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Generic calculator ---");
    Calculator<double> calcDouble;
    double sum = calcDouble.add(3.14, 2.86);
    std::println("Add: {}", sum);
    double product = calcDouble.multiply(2.5, 4.0);
    std::println("Multiply: {}", product);

    std::println("");
    std::println("--- Specialized multiply for int ---");
    Calculator<int> calcInt;
    int intSum = calcInt.add(10, 20);
    std::println("Add: {}", intSum);
    int intProduct = calcInt.multiply(5, 7);
    std::println("Multiply: {}", intProduct);
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║          TEMPLATE SPECIALIZATION IN C++                  ║");
    std::println("║          Custom Behavior for Specific Types               ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

    demonstrateBasicFunction();
    demonstrateBasicClass();
    demonstratePartialPointer();
    demonstratePartialArray();
    demonstrateMultipleParam();
    demonstrateTypeTraits();
    demonstrateOptimization();
    demonstrateVectorBool();
    demonstrateMemberSpecialization();

    return 0;
}
