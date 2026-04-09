#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <type_traits>
using namespace std;

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
// EXAMPLE 1: BASIC FUNCTION SPECIALIZATION
// ============================================

// Generic template
template<typename T>
bool isEqual(T a, T b) {
    cout << "  Generic comparison\n";
    return a == b;
}

// Specialization for double (epsilon comparison)
template<>
bool isEqual<double>(double a, double b) {
    cout << "  Specialized double comparison (epsilon)\n";
    const double epsilon = 0.0001;
    return abs(a - b) < epsilon;
}

// Specialization for C-strings
template<>
bool isEqual<const char*>(const char* a, const char* b) {
    cout << "  Specialized string comparison (strcmp)\n";
    return strcmp(a, b) == 0;
}

void demonstrateBasicFunction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: FUNCTION SPECIALIZATION║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Generic comparison ---\n";
    cout << "5 == 5: " << isEqual(5, 5) << "\n";
    cout << "5 == 10: " << isEqual(5, 10) << "\n";

    cout << "\n--- Specialized double (epsilon) ---\n";
    cout << "3.14159 == 3.14160: " << isEqual(3.14159, 3.14160) << "\n";

    cout << "\n--- Specialized string (strcmp) ---\n";
    cout << "\"hello\" == \"hello\": " << isEqual("hello", "hello") << "\n";
    cout << "\"hello\" == \"world\": " << isEqual("hello", "world") << "\n";

    cout << "\n💡 FUNCTION SPECIALIZATION:\n";
    cout << "   • Custom behavior for types\n";
    cout << "   • template<> syntax\n";
    cout << "   • Must specify type explicitly\n";
    cout << "   • Type-specific algorithms\n";
}

// ============================================
// EXAMPLE 2: BASIC CLASS SPECIALIZATION
// ============================================

// Generic template
template<typename T>
class Storage {
private:
    T data;

public:
    Storage(T d) : data(d) {
        cout << "  Generic Storage constructor\n";
    }

    void display() const {
        cout << "  Generic data: " << data << "\n";
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
        cout << "  Specialized Storage<const char*> constructor\n";
        data = new char[strlen(d) + 1];
        strcpy(data, d);
    }

    ~Storage() {
        delete[] data;
        cout << "  Specialized destructor (freed memory)\n";
    }

    // Copy constructor
    Storage(const Storage& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    void display() const {
        cout << "  String data: \"" << data << "\"\n";
    }

    const char* getData() const { return data; }
};

void demonstrateBasicClass() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: CLASS SPECIALIZATION   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Generic storage ---\n";
    Storage<int> intStorage(42);
    intStorage.display();

    Storage<double> doubleStorage(3.14);
    doubleStorage.display();

    cout << "\n--- Specialized storage (manages memory) ---\n";
    {
        Storage<const char*> strStorage("Hello World");
        strStorage.display();
        cout << "  Leaving scope...\n";
    }

    cout << "\n💡 CLASS SPECIALIZATION:\n";
    cout << "   • Complete class redefinition\n";
    cout << "   • Different data members\n";
    cout << "   • Different member functions\n";
    cout << "   • Can manage resources differently\n";
}

// ============================================
// EXAMPLE 3: PARTIAL SPECIALIZATION - POINTERS
// ============================================

// Primary template
template<typename T>
class Container {
public:
    void info() {
        cout << "  Container for regular type\n";
    }

    void use(T value) {
        cout << "  Using value directly: " << value << "\n";
    }
};

// Partial specialization for pointer types
template<typename T>
class Container<T*> {
public:
    void info() {
        cout << "  Container for POINTER type\n";
    }

    void use(T* ptr) {
        cout << "  Dereferencing pointer: " << *ptr << "\n";
    }
};

// Partial specialization for const pointer types
template<typename T>
class Container<const T*> {
public:
    void info() {
        cout << "  Container for CONST POINTER type\n";
    }

    void use(const T* ptr) {
        cout << "  Reading const pointer: " << *ptr << "\n";
    }
};

void demonstratePartialPointer() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: PARTIAL - POINTERS     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular type ---\n";
    Container<int> c1;
    c1.info();
    c1.use(42);

    cout << "\n--- Pointer type ---\n";
    Container<int*> c2;
    c2.info();
    int value = 100;
    c2.use(&value);

    cout << "\n--- Const pointer type ---\n";
    Container<const int*> c3;
    c3.info();
    c3.use(&value);

    cout << "\n💡 PARTIAL SPECIALIZATION:\n";
    cout << "   • Specialize for patterns (T*)\n";
    cout << "   • T remains generic\n";
    cout << "   • More specific than primary\n";
    cout << "   • Only for class templates\n";
}

// ============================================
// EXAMPLE 4: PARTIAL SPECIALIZATION - ARRAYS
// ============================================

// Primary template
template<typename T>
class ArrayWrapper {
public:
    void describe() {
        cout << "  Not an array type\n";
    }
};

// Partial specialization for arrays
template<typename T>
class ArrayWrapper<T[]> {
public:
    void describe() {
        cout << "  Unbounded array type\n";
    }
};

// Partial specialization for arrays with known size
template<typename T, size_t N>
class ArrayWrapper<T[N]> {
public:
    void describe() {
        cout << "  Bounded array type, size: " << N << "\n";
    }
};

void demonstratePartialArray() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: PARTIAL - ARRAYS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Different array types ---\n";
    ArrayWrapper<int> w1;
    w1.describe();

    ArrayWrapper<int[]> w2;
    w2.describe();

    ArrayWrapper<int[5]> w3;
    w3.describe();

    ArrayWrapper<double[10]> w4;
    w4.describe();

    cout << "\n💡 ARRAY SPECIALIZATION:\n";
    cout << "   • Detect array types\n";
    cout << "   • Extract array size\n";
    cout << "   • Different handling for arrays\n";
}

// ============================================
// EXAMPLE 5: MULTIPLE PARAMETER SPECIALIZATION
// ============================================

// Primary template
template<typename T1, typename T2>
class Pair {
public:
    void describe() {
        cout << "  Generic: Two different types\n";
    }
};

// Specialization: both types same
template<typename T>
class Pair<T, T> {
public:
    void describe() {
        cout << "  Specialized: Both types SAME\n";
    }
};

// Specialization: first is int
template<typename T2>
class Pair<int, T2> {
public:
    void describe() {
        cout << "  Specialized: First type is INT\n";
    }
};

// Specialization: second is pointer
template<typename T1, typename T2>
class Pair<T1, T2*> {
public:
    void describe() {
        cout << "  Specialized: Second type is POINTER\n";
    }
};

// Full specialization: both int
template<>
class Pair<int, int> {
public:
    void describe() {
        cout << "  Full specialization: BOTH INT\n";
    }
};

void demonstrateMultipleParam() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MULTIPLE PARAMETERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Different combinations ---\n";
    Pair<double, string> p1;
    p1.describe();  // Generic

    Pair<int, string> p2;
    p2.describe();  // First is int

    Pair<double, double> p3;
    p3.describe();  // Both same

    Pair<double, int*> p4;
    p4.describe();  // Second is pointer

    Pair<int, int> p5;
    p5.describe();  // Full specialization (most specific)

    cout << "\n💡 SPECIALIZATION PRIORITY:\n";
    cout << "   1. Full specialization (most specific)\n";
    cout << "   2. Partial specialization\n";
    cout << "   3. Primary template (least specific)\n";
}

// ============================================
// EXAMPLE 6: TYPE TRAITS PATTERN
// ============================================

// Primary template - not a pointer
template<typename T>
struct IsPointer {
    static const bool value = false;

    static void check() {
        cout << "  Not a pointer type\n";
    }
};

// Specialization for pointer types
template<typename T>
struct IsPointer<T*> {
    static const bool value = true;

    static void check() {
        cout << "  IS a pointer type\n";
    }
};

// Primary template - get value type
template<typename T>
struct ValueType {
    using type = T;

    static void display() {
        cout << "  Regular type\n";
    }
};

// Specialization for pointers - extract pointed-to type
template<typename T>
struct ValueType<T*> {
    using type = T;

    static void display() {
        cout << "  Pointer type, pointing to base type\n";
    }
};

void demonstrateTypeTraits() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: TYPE TRAITS            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- IsPointer trait ---\n";
    cout << "int: " << IsPointer<int>::value << " - ";
    IsPointer<int>::check();

    cout << "int*: " << IsPointer<int*>::value << " - ";
    IsPointer<int*>::check();

    cout << "double**: " << IsPointer<double**>::value << " - ";
    IsPointer<double**>::check();

    cout << "\n--- ValueType trait ---\n";
    ValueType<int>::display();
    ValueType<int*>::display();

    cout << "\n💡 TYPE TRAITS:\n";
    cout << "   • Detect type properties\n";
    cout << "   • Extract type information\n";
    cout << "   • Compile-time decisions\n";
    cout << "   • Foundation of std::is_pointer, etc.\n";
}

// ============================================
// EXAMPLE 7: OPTIMIZATION EXAMPLE
// ============================================

// Generic swap (works for all types)
template<typename T>
void mySwap(T& a, T& b) {
    cout << "  Generic swap (uses copy)\n";
    T temp = a;
    a = b;
    b = temp;
}

// Specialization for large arrays (memcpy optimization)
template<>
void mySwap<char[1024]>(char (&a)[1024], char (&b)[1024]) {
    cout << "  Optimized swap for large array (memcpy)\n";
    char temp[1024];
    memcpy(temp, a, 1024);
    memcpy(a, b, 1024);
    memcpy(b, temp, 1024);
}

void demonstrateOptimization() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: OPTIMIZATION           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular swap ---\n";
    int x = 10, y = 20;
    mySwap(x, y);
    cout << "  After swap: x=" << x << ", y=" << y << "\n";

    cout << "\n--- Optimized large array swap ---\n";
    char arr1[1024] = "Array 1";
    char arr2[1024] = "Array 2";
    mySwap(arr1, arr2);
    cout << "  arr1: " << arr1 << "\n";
    cout << "  arr2: " << arr2 << "\n";

    cout << "\n💡 OPTIMIZATION:\n";
    cout << "   • Type-specific optimizations\n";
    cout << "   • Better performance for special cases\n";
    cout << "   • memcpy for POD types\n";
    cout << "   • Bit manipulation for bools\n";
}

// ============================================
// EXAMPLE 8: VECTOR<BOOL> PATTERN
// ============================================

// Generic vector (simplified)
template<typename T>
class SimpleVector {
private:
    T* data;
    size_t size;

public:
    SimpleVector(size_t n) : size(n) {
        data = new T[n];
        cout << "  SimpleVector<T> allocated " << sizeof(T) * n << " bytes\n";
    }

    ~SimpleVector() {
        delete[] data;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    size_t getSize() const { return size; }
};

// Specialization for bool (bit packing)
template<>
class SimpleVector<bool> {
private:
    unsigned char* data;
    size_t size;

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
                byte &= ~mask;
            return *this;
        }
    };

    SimpleVector(size_t n) : size(n) {
        size_t bytes = (n + 7) / 8;  // Round up
        data = new unsigned char[bytes]();
        cout << "  SimpleVector<bool> allocated " << bytes << " bytes (bit packed!)\n";
    }

    ~SimpleVector() {
        delete[] data;
    }

    BitReference operator[](size_t index) {
        size_t byteIndex = index / 8;
        unsigned char mask = 1 << (index % 8);
        return BitReference(data[byteIndex], mask);
    }

    size_t getSize() const { return size; }
};

void demonstrateVectorBool() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: VECTOR<BOOL> PATTERN   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular vector ---\n";
    SimpleVector<int> intVec(100);
    intVec[0] = 42;
    cout << "  intVec[0]: " << intVec[0] << "\n";

    cout << "\n--- Bool vector (bit packed) ---\n";
    SimpleVector<bool> boolVec(100);
    boolVec[0] = true;
    boolVec[1] = false;
    boolVec[2] = true;
    cout << "  boolVec[0]: " << boolVec[0] << "\n";
    cout << "  boolVec[1]: " << boolVec[1] << "\n";
    cout << "  boolVec[2]: " << boolVec[2] << "\n";

    cout << "\n💡 VECTOR<BOOL> PATTERN:\n";
    cout << "   • Space optimization (1 bit per bool)\n";
    cout << "   • Different data structure\n";
    cout << "   • Proxy class for bit access\n";
    cout << "   • 8x memory savings!\n";
}

// ============================================
// EXAMPLE 9: MEMBER FUNCTION SPECIALIZATION
// ============================================

template<typename T>
class Calculator {
public:
    T add(T a, T b) {
        cout << "  Generic add\n";
        return a + b;
    }

    T multiply(T a, T b);  // Declare only
};

// Generic definition
template<typename T>
T Calculator<T>::multiply(T a, T b) {
    cout << "  Generic multiply\n";
    return a * b;
}

// Specialization of member function for int
template<>
int Calculator<int>::multiply(int a, int b) {
    cout << "  Specialized multiply for int (checks overflow)\n";
    // Could add overflow checking here
    return a * b;
}

void demonstrateMemberSpecialization() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: MEMBER SPECIALIZATION  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Generic calculator ---\n";
    Calculator<double> calcDouble;
    cout << "Add: " << calcDouble.add(3.14, 2.86) << "\n";
    cout << "Multiply: " << calcDouble.multiply(2.5, 4.0) << "\n";

    cout << "\n--- Specialized multiply for int ---\n";
    Calculator<int> calcInt;
    cout << "Add: " << calcInt.add(10, 20) << "\n";
    cout << "Multiply: " << calcInt.multiply(5, 7) << "\n";

    cout << "\n💡 MEMBER SPECIALIZATION:\n";
    cout << "   • Specialize individual members\n";
    cout << "   • Rest of class unchanged\n";
    cout << "   • Type-specific behavior\n";
}

// ============================================
// EXAMPLE 10: WHEN NOT TO SPECIALIZE
// ============================================

// ❌ BAD: Overloading is better here
template<typename T>
void process(T value) {
    cout << "  Template version\n";
}

// Instead of specializing, overload
void process(int value) {
    cout << "  Overload for int (BETTER than specialization)\n";
}

void process(double value) {
    cout << "  Overload for double\n";
}

void demonstrateAlternatives() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: ALTERNATIVES          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "✅ WHEN TO USE SPECIALIZATION:\n";
    cout << "   • Type-specific data structures\n";
    cout << "   • Optimization opportunities\n";
    cout << "   • Fundamentally different implementations\n";
    cout << "   • Type traits and metaprogramming\n";
    cout << "\n";
    cout << "❌ WHEN NOT TO USE:\n";
    cout << "   • Simple behavior differences → Use overloading\n";
    cout << "   • Function templates → Consider overloading\n";
    cout << "   • Can use if constexpr (C++17)\n";
    cout << "   • Can use concepts (C++20)\n";
    cout << "\n";
    cout << "💡 ALTERNATIVES:\n";
    cout << "   1. Function overloading (preferred for functions)\n";
    cout << "   2. if constexpr (C++17)\n";
    cout << "   3. Concepts (C++20)\n";
    cout << "   4. Tag dispatch\n";
    cout << "   5. SFINAE/enable_if\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║          TEMPLATE SPECIALIZATION IN C++                  ║\n";
    cout << "║          Custom Behavior for Specific Types               ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateBasicFunction();
    demonstrateBasicClass();
    demonstratePartialPointer();
    demonstratePartialArray();
    demonstrateMultipleParam();
    demonstrateTypeTraits();
    demonstrateOptimization();
    demonstrateVectorBool();
    demonstrateMemberSpecialization();
    demonstrateAlternatives();

    // ============================================
    // COMPREHENSIVE EXPLANATION
    // ============================================

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              COMPREHENSIVE EXPLANATION                    ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n========================================\n";
    cout << "WHAT IS TEMPLATE SPECIALIZATION?\n";
    cout << "========================================\n";
    cout << "Template specialization:\n";
    cout << "  • Custom implementation for specific types\n";
    cout << "  • Overrides generic template\n";
    cout << "  • Type-specific optimizations\n";
    cout << "  • Different data structures per type\n";
    cout << "  • Compile-time selection\n";
    cout << "\n";
    cout << "TWO TYPES:\n";
    cout << "1. Full (Explicit) Specialization\n";
    cout << "   template<>\n";
    cout << "   class Box<int> { };  // Specific type\n";
    cout << "\n";
    cout << "2. Partial Specialization\n";
    cout << "   template<typename T>\n";
    cout << "   class Box<T*> { };  // Pattern (pointers)\n";

    cout << "\n========================================\n";
    cout << "FULL SPECIALIZATION\n";
    cout << "========================================\n";
    cout << "FUNCTION:\n";
    cout << "// Generic\n";
    cout << "template<typename T>\n";
    cout << "void func(T param) { }\n";
    cout << "\n";
    cout << "// Full specialization\n";
    cout << "template<>\n";
    cout << "void func<int>(int param) {  // Specific for int\n";
    cout << "    // Custom implementation\n";
    cout << "}\n";
    cout << "\n";
    cout << "CLASS:\n";
    cout << "// Generic\n";
    cout << "template<typename T>\n";
    cout << "class Box { };\n";
    cout << "\n";
    cout << "// Full specialization\n";
    cout << "template<>\n";
    cout << "class Box<bool> {  // Completely different class\n";
    cout << "    // Custom implementation\n";
    cout << "};\n";
    cout << "\n";
    cout << "NOTES:\n";
    cout << "  • template<> indicates specialization\n";
    cout << "  • Must specify exact type\n";
    cout << "  • Complete redefinition\n";
    cout << "  • No partial specialization for functions\n";

    cout << "\n========================================\n";
    cout << "PARTIAL SPECIALIZATION\n";
    cout << "========================================\n";
    cout << "CLASSES ONLY! (Not functions)\n";
    cout << "\n";
    cout << "POINTER PATTERN:\n";
    cout << "// Primary\n";
    cout << "template<typename T>\n";
    cout << "class Container { };\n";
    cout << "\n";
    cout << "// Partial: any pointer type\n";
    cout << "template<typename T>\n";
    cout << "class Container<T*> {  // T is still generic\n";
    cout << "    // Special handling for pointers\n";
    cout << "};\n";
    cout << "\n";
    cout << "MULTIPLE PARAMETERS:\n";
    cout << "// Primary\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "class Pair { };\n";
    cout << "\n";
    cout << "// Both same type\n";
    cout << "template<typename T>\n";
    cout << "class Pair<T, T> { };\n";
    cout << "\n";
    cout << "// Second is pointer\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "class Pair<T1, T2*> { };\n";
    cout << "\n";
    cout << "ARRAY PATTERN:\n";
    cout << "// Unbounded array\n";
    cout << "template<typename T>\n";
    cout << "class Wrapper<T[]> { };\n";
    cout << "\n";
    cout << "// Bounded array\n";
    cout << "template<typename T, size_t N>\n";
    cout << "class Wrapper<T[N]> { };\n";

    cout << "\n========================================\n";
    cout << "SPECIALIZATION PRIORITY\n";
    cout << "========================================\n";
    cout << "When multiple specializations match:\n";
    cout << "1. Most specific wins\n";
    cout << "2. Full > Partial > Primary\n";
    cout << "\n";
    cout << "EXAMPLE:\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "class Pair { };                    // Primary\n";
    cout << "\n";
    cout << "template<typename T>\n";
    cout << "class Pair<T, T> { };              // Partial\n";
    cout << "\n";
    cout << "template<>\n";
    cout << "class Pair<int, int> { };          // Full\n";
    cout << "\n";
    cout << "Pair<double, string> p1;  → Primary\n";
    cout << "Pair<double, double> p2;  → Partial (both T)\n";
    cout << "Pair<int, int> p3;        → Full (most specific)\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Pointer Handling\n";
    cout << "template<typename T>\n";
    cout << "class SmartPtr<T*> {\n";
    cout << "    // Dereference, arrow operator\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Type Traits\n";
    cout << "template<typename T>\n";
    cout << "struct IsPointer { static const bool value = false; };\n";
    cout << "\n";
    cout << "template<typename T>\n";
    cout << "struct IsPointer<T*> { static const bool value = true; };\n";
    cout << "\n";
    cout << "PATTERN 3: Optimization\n";
    cout << "template<typename T>\n";
    cout << "void copy(T* dest, T* src, size_t n) {\n";
    cout << "    // Element-wise copy\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Optimized for POD types\n";
    cout << "template<>\n";
    cout << "void copy<char>(char* dest, char* src, size_t n) {\n";
    cout << "    memcpy(dest, src, n);  // Fast!\n";
    cout << "}\n";
    cout << "\n";
    cout << "PATTERN 4: Different Data Structure\n";
    cout << "template<typename T>\n";
    cout << "class Vector { T* data; };  // Regular array\n";
    cout << "\n";
    cout << "template<>\n";
    cout << "class Vector<bool> {\n";
    cout << "    unsigned char* data;  // Bit array (8x smaller)\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHEN TO SPECIALIZE\n";
    cout << "========================================\n";
    cout << "✅ USE SPECIALIZATION FOR:\n";
    cout << "  • Different data structures needed\n";
    cout << "  • Performance optimizations\n";
    cout << "  • Type-specific algorithms\n";
    cout << "  • Pointer/reference handling\n";
    cout << "  • Type traits implementation\n";
    cout << "  • Const/volatile variations\n";
    cout << "  • Array types\n";
    cout << "\n";
    cout << "❌ DON'T USE FOR:\n";
    cout << "  • Simple behavior differences\n";
    cout << "  • Function templates (use overloading)\n";
    cout << "  • When if constexpr works (C++17)\n";
    cout << "  • When concepts work (C++20)\n";
    cout << "  • Minor tweaks (consider SFINAE)\n";

    cout << "\n========================================\n";
    cout << "ALTERNATIVES TO SPECIALIZATION\n";
    cout << "========================================\n";
    cout << "1. FUNCTION OVERLOADING:\n";
    cout << "   template<typename T>\n";
    cout << "   void func(T x) { }  // Template\n";
    cout << "   \n";
    cout << "   void func(int x) { }  // Overload (better!)\n";
    cout << "\n";
    cout << "2. IF CONSTEXPR (C++17):\n";
    cout << "   template<typename T>\n";
    cout << "   void func(T x) {\n";
    cout << "       if constexpr (is_pointer<T>::value) {\n";
    cout << "           // Pointer handling\n";
    cout << "       } else {\n";
    cout << "           // Regular handling\n";
    cout << "       }\n";
    cout << "   }\n";
    cout << "\n";
    cout << "3. CONCEPTS (C++20):\n";
    cout << "   template<typename T>\n";
    cout << "   requires Integral<T>\n";
    cout << "   void func(T x) { }  // Only for integral types\n";
    cout << "\n";
    cout << "4. TAG DISPATCH:\n";
    cout << "   template<typename T>\n";
    cout << "   void func_impl(T x, true_type) { }  // Pointer\n";
    cout << "   \n";
    cout << "   template<typename T>\n";
    cout << "   void func_impl(T x, false_type) { }  // Not pointer\n";
    cout << "   \n";
    cout << "   template<typename T>\n";
    cout << "   void func(T x) {\n";
    cout << "       func_impl(x, is_pointer<T>());\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use for different data structures\n";
    cout << "  • Document why specializing\n";
    cout << "  • Keep specializations in same file\n";
    cout << "  • Test all specializations\n";
    cout << "  • Consider alternatives first\n";
    cout << "  • Use for type traits\n";
    cout << "  • Optimize when beneficial\n";
    cout << "  • Keep consistent interface\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Over-specialize\n";
    cout << "  • Specialize functions (overload instead)\n";
    cout << "  • Forget partial > full priority\n";
    cout << "  • Create inconsistent interfaces\n";
    cout << "  • Specialize unnecessarily\n";
    cout << "  • Put in different namespaces\n";
    cout << "  • Ignore compiler warnings\n";
    cout << "  • Forget to test edge cases\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Full specialization (function)\n";
    cout << "template<typename T>\n";
    cout << "void func(T x) { }  // Generic\n";
    cout << "\n";
    cout << "template<>\n";
    cout << "void func<int>(int x) { }  // Specialized\n";
    cout << "\n";
    cout << "// Full specialization (class)\n";
    cout << "template<typename T>\n";
    cout << "class Box { };  // Generic\n";
    cout << "\n";
    cout << "template<>\n";
    cout << "class Box<bool> { };  // Specialized\n";
    cout << "\n";
    cout << "// Partial specialization (pointer)\n";
    cout << "template<typename T>\n";
    cout << "class Box<T*> { };  // For all pointers\n";
    cout << "\n";
    cout << "// Partial specialization (two params)\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "class Pair { };  // Primary\n";
    cout << "\n";
    cout << "template<typename T>\n";
    cout << "class Pair<T, T> { };  // Both same\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Specialization = custom implementation for types\n";
    cout << "2. Full specialization = specific type\n";
    cout << "3. Partial specialization = pattern (T*, T[])\n";
    cout << "4. Classes support partial, functions don't\n";
    cout << "5. More specific wins (Full > Partial > Primary)\n";
    cout << "6. Use for optimization and type traits\n";
    cout << "7. Consider alternatives (overloading, if constexpr)\n";
    cout << "8. vector<bool> is famous example\n";
    cout << "9. Document why specializing\n";
    cout << "10. Test all specializations thoroughly\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Prefer overloading for functions\n";
    cout << "   Function specialization is tricky!\n";
    cout << "   \n";
    cout << "   void func(int x) { }  // Better than specialization\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Partial only for classes\n";
    cout << "   Functions don't support partial!\n";
    cout << "   \n";
    cout << "   template<typename T>\n";
    cout << "   class Box<T*> { };  // ✅ OK for classes\n";
    cout << "\n";
    cout << "⚠️  RULE 3: More specific wins\n";
    cout << "   Compiler picks most specialized!\n";
    cout << "   \n";
    cout << "   Full > Partial > Primary\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Keep interface consistent\n";
    cout << "   Specializations should have same interface!\n";
    cout << "   \n";
    cout << "   Same public methods across specializations\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Document the reason\n";
    cout << "   Why are you specializing?\n";
    cout << "   \n";
    cout << "   // Specialized for optimization\n";
    cout << "   // Specialized for different data structure\n";

    return 0;
}
