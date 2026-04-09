#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

// ============================================
// WHAT ARE CLASS TEMPLATES?
// ============================================

/*
 * CLASS TEMPLATES:
 * - Generic classes that work with any type
 * - Foundation of STL containers (vector, list, etc.)
 * - Template parameters for type and values
 * - Member functions are also templates
 * - Instantiated on first use
 *
 * SYNTAX:
 * - template<typename T>
 * - class ClassName { T member; };
 * - ClassName<int> obj;  // Instantiation
 *
 * WHY CLASS TEMPLATES?
 * - Reusable container classes
 * - Type-safe generics
 * - Compile-time polymorphism
 * - No runtime overhead
 * - Foundation of modern C++
 *
 * MEMBER FUNCTIONS:
 * - Defined inside class (inline)
 * - Defined outside with template<typename T>
 * - Each instantiation gets own functions
 *
 * INSTANTIATION:
 * - Explicit: ClassName<Type> obj;
 * - Happens at compile time
 * - Each type creates separate class
 * - Dead code elimination
 *
 * FEATURES:
 * - Multiple template parameters
 * - Default template parameters
 * - Template specialization
 * - Partial specialization
 * - Non-type parameters
 * - Static members
 */

// ============================================
// EXAMPLE 1: BASIC CLASS TEMPLATE
// ============================================

template<typename T>
class Box {
private:
    T value;

public:
    Box(T v) : value(v) {
        cout << "  Box constructor\n";
    }

    void setValue(T v) {
        value = v;
    }

    T getValue() const {
        return value;
    }

    void display() const {
        cout << "  Box contains: " << value << "\n";
    }
};

void demonstrateBasic() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC CLASS TEMPLATE   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Box with different types ---\n";
    Box<int> intBox(42);
    intBox.display();

    Box<double> doubleBox(3.14);
    doubleBox.display();

    Box<string> stringBox("Hello");
    stringBox.display();

    cout << "\n--- Modifying values ---\n";
    intBox.setValue(100);
    intBox.display();

    cout << "\n✅ CLASS TEMPLATE:\n";
    cout << "   • One class for all types\n";
    cout << "   • Type-safe container\n";
    cout << "   • Each instantiation separate\n";
    cout << "   • Zero runtime overhead\n";
}

// ============================================
// EXAMPLE 2: MEMBER FUNCTIONS OUTSIDE CLASS
// ============================================

template<typename T>
class Pair {
private:
    T first;
    T second;

public:
    Pair(T f, T s);  // Declaration only

    T getFirst() const;
    T getSecond() const;
    void setFirst(T f);
    void setSecond(T s);
    void display() const;
};

// Definitions outside class
template<typename T>
Pair<T>::Pair(T f, T s) : first(f), second(s) {
    cout << "  Pair constructor\n";
}

template<typename T>
T Pair<T>::getFirst() const {
    return first;
}

template<typename T>
T Pair<T>::getSecond() const {
    return second;
}

template<typename T>
void Pair<T>::setFirst(T f) {
    first = f;
}

template<typename T>
void Pair<T>::setSecond(T s) {
    second = s;
}

template<typename T>
void Pair<T>::display() const {
    cout << "  Pair: (" << first << ", " << second << ")\n";
}

void demonstrateMemberFunctions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: MEMBER FUNCTIONS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating pairs ---\n";
    Pair<int> intPair(10, 20);
    intPair.display();

    Pair<string> strPair("Hello", "World");
    strPair.display();

    cout << "\n--- Accessing members ---\n";
    cout << "  First: " << intPair.getFirst() << "\n";
    cout << "  Second: " << intPair.getSecond() << "\n";

    cout << "\n💡 MEMBER FUNCTIONS:\n";
    cout << "   • Can define outside class\n";
    cout << "   • Need template<typename T> prefix\n";
    cout << "   • Use ClassName<T>::function syntax\n";
    cout << "   • All in header typically\n";
}

// ============================================
// EXAMPLE 3: MULTIPLE TEMPLATE PARAMETERS
// ============================================

template<typename K, typename V>
class KeyValue {
private:
    K key;
    V value;

public:
    KeyValue(K k, V v) : key(k), value(v) {
        cout << "  KeyValue constructor\n";
    }

    K getKey() const { return key; }
    V getValue() const { return value; }

    void display() const {
        cout << "  Key: " << key << ", Value: " << value << "\n";
    }
};

void demonstrateMultipleParams() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: MULTIPLE PARAMETERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Different type combinations ---\n";
    KeyValue<int, string> kv1(1, "One");
    kv1.display();

    KeyValue<string, int> kv2("Age", 25);
    kv2.display();

    KeyValue<string, double> kv3("Pi", 3.14159);
    kv3.display();

    cout << "\n💡 MULTIPLE PARAMETERS:\n";
    cout << "   • Each can be different type\n";
    cout << "   • Flexible combinations\n";
    cout << "   • Like map<K, V> in STL\n";
}

// ============================================
// EXAMPLE 4: DEFAULT TEMPLATE PARAMETERS
// ============================================

template<typename T, typename Container = vector<T>>
class Stack {
private:
    Container data;

public:
    void push(const T& value) {
        data.push_back(value);
        cout << "  Pushed: " << value << "\n";
    }

    void pop() {
        if (!data.empty()) {
            cout << "  Popped: " << data.back() << "\n";
            data.pop_back();
        }
    }

    T top() const {
        return data.back();
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

void demonstrateDefaultParams() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: DEFAULT PARAMETERS     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Stack with default container (vector) ---\n";
    Stack<int> s1;  // Uses vector<int> by default
    s1.push(10);
    s1.push(20);
    s1.push(30);
    cout << "  Size: " << s1.size() << "\n";
    s1.pop();

    cout << "\n💡 DEFAULT PARAMETERS:\n";
    cout << "   • Provide sensible defaults\n";
    cout << "   • Can override when needed\n";
    cout << "   • Stack<int> uses vector<int>\n";
    cout << "   • Flexible and convenient\n";
}

// ============================================
// EXAMPLE 5: NON-TYPE TEMPLATE PARAMETERS
// ============================================

template<typename T, int Size>
class Array {
private:
    T data[Size];

public:
    Array() {
        cout << "  Array<T, " << Size << "> constructor\n";
    }

    int size() const {
        return Size;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    void fill(const T& value) {
        for (int i = 0; i < Size; ++i) {
            data[i] = value;
        }
    }

    void display() const {
        cout << "  Array[" << Size << "]: ";
        for (int i = 0; i < Size; ++i) {
            cout << data[i] << " ";
        }
        cout << "\n";
    }
};

void demonstrateNonType() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: NON-TYPE PARAMETERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Fixed-size arrays ---\n";
    Array<int, 5> arr1;
    arr1.fill(10);
    arr1.display();

    Array<double, 3> arr2;
    arr2[0] = 1.1;
    arr2[1] = 2.2;
    arr2[2] = 3.3;
    arr2.display();

    cout << "  Size known at compile time: " << arr1.size() << "\n";

    cout << "\n💡 NON-TYPE PARAMETERS:\n";
    cout << "   • Compile-time constants\n";
    cout << "   • Array sizes, dimensions\n";
    cout << "   • Part of type (Array<int,5> != Array<int,10>)\n";
    cout << "   • Zero runtime overhead\n";
}

// ============================================
// EXAMPLE 6: TEMPLATE SPECIALIZATION
// ============================================

// Generic template
template<typename T>
class Printer {
public:
    void print(const T& value) {
        cout << "  Generic: " << value << "\n";
    }
};

// Full specialization for bool
template<>
class Printer<bool> {
public:
    void print(const bool& value) {
        cout << "  Bool: " << (value ? "true" : "false") << "\n";
    }
};

// Full specialization for const char*
template<>
class Printer<const char*> {
public:
    void print(const char* const& value) {
        cout << "  String: \"" << value << "\"\n";
    }
};

void demonstrateSpecialization() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: SPECIALIZATION         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Generic template ---\n";
    Printer<int> p1;
    p1.print(42);

    Printer<double> p2;
    p2.print(3.14);

    cout << "\n--- Specialized versions ---\n";
    Printer<bool> p3;
    p3.print(true);
    p3.print(false);

    Printer<const char*> p4;
    p4.print("Hello World");

    cout << "\n💡 SPECIALIZATION:\n";
    cout << "   • Custom implementation for type\n";
    cout << "   • Complete class redefinition\n";
    cout << "   • template<> syntax\n";
    cout << "   • Different behavior per type\n";
}

// ============================================
// EXAMPLE 7: PARTIAL SPECIALIZATION
// ============================================

// Primary template
template<typename T1, typename T2>
class Holder {
public:
    void describe() {
        cout << "  Generic: Two different types\n";
    }
};

// Partial specialization: both types same
template<typename T>
class Holder<T, T> {
public:
    void describe() {
        cout << "  Specialized: Both types are same\n";
    }
};

// Partial specialization: second type is pointer
template<typename T1, typename T2>
class Holder<T1, T2*> {
public:
    void describe() {
        cout << "  Specialized: Second type is pointer\n";
    }
};

void demonstratePartialSpecialization() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: PARTIAL SPECIALIZATION ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Different combinations ---\n";
    Holder<int, double> h1;
    h1.describe();  // Generic

    Holder<int, int> h2;
    h2.describe();  // Both same

    Holder<int, double*> h3;
    h3.describe();  // Second is pointer

    cout << "\n💡 PARTIAL SPECIALIZATION:\n";
    cout << "   • Specialize based on pattern\n";
    cout << "   • Some parameters remain generic\n";
    cout << "   • More flexible than full specialization\n";
    cout << "   • Only for class templates (not functions)\n";
}

// ============================================
// EXAMPLE 8: STATIC MEMBERS
// ============================================

template<typename T>
class Counter {
private:
    static int count;
    T value;

public:
    Counter(T v) : value(v) {
        count++;
        cout << "  Counter created (count=" << count << ")\n";
    }

    ~Counter() {
        count--;
    }

    static int getCount() {
        return count;
    }

    T getValue() const {
        return value;
    }
};

// Static member definition
template<typename T>
int Counter<T>::count = 0;

void demonstrateStatic() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: STATIC MEMBERS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- int counters (separate count) ---\n";
    Counter<int> c1(10);
    Counter<int> c2(20);
    cout << "  Int count: " << Counter<int>::getCount() << "\n";

    cout << "\n--- string counters (separate count) ---\n";
    Counter<string> s1("Hello");
    Counter<string> s2("World");
    cout << "  String count: " << Counter<string>::getCount() << "\n";

    cout << "\n--- int count still separate ---\n";
    cout << "  Int count: " << Counter<int>::getCount() << "\n";

    cout << "\n💡 STATIC MEMBERS:\n";
    cout << "   • Each instantiation has own static\n";
    cout << "   • Counter<int> separate from Counter<string>\n";
    cout << "   • Must define outside class\n";
    cout << "   • template<typename T> prefix needed\n";
}

// ============================================
// EXAMPLE 9: FRIEND FUNCTIONS
// ============================================

template<typename T>
class Value;

// Friend function declaration
template<typename T>
ostream& operator<<(ostream& os, const Value<T>& v);

template<typename T>
class Value {
private:
    T data;

public:
    Value(T d) : data(d) {}

    // Friend function
    friend ostream& operator<< <>(ostream& os, const Value<T>& v);

    // Friend function defined inside
    friend Value<T> operator+(const Value<T>& a, const Value<T>& b) {
        return Value<T>(a.data + b.data);
    }
};

// Friend function definition
template<typename T>
ostream& operator<<(ostream& os, const Value<T>& v) {
    os << "Value(" << v.data << ")";
    return os;
}

void demonstrateFriend() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: FRIEND FUNCTIONS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using friend operators ---\n";
    Value<int> v1(10);
    Value<int> v2(20);

    cout << "  v1: " << v1 << "\n";
    cout << "  v2: " << v2 << "\n";

    Value<int> v3 = v1 + v2;
    cout << "  v1 + v2: " << v3 << "\n";

    cout << "\n💡 FRIEND FUNCTIONS:\n";
    cout << "   • Can access private members\n";
    cout << "   • Template friend needs forward declaration\n";
    cout << "   • Define inside or outside\n";
    cout << "   • Useful for operators\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD CONTAINER
// ============================================

template<typename T>
class SimpleVector {
private:
    T* data;
    size_t capacity;
    size_t length;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cout << "  Resized to capacity: " << capacity << "\n";
    }

public:
    SimpleVector() : data(nullptr), capacity(4), length(0) {
        data = new T[capacity];
        cout << "  SimpleVector created\n";
    }

    ~SimpleVector() {
        delete[] data;
        cout << "  SimpleVector destroyed\n";
    }

    // Copy constructor
    SimpleVector(const SimpleVector& other)
        : capacity(other.capacity), length(other.length) {
        data = new T[capacity];
        for (size_t i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
        cout << "  SimpleVector copied\n";
    }

    // Copy assignment
    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            length = other.length;
            data = new T[capacity];
            for (size_t i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        if (length >= capacity) {
            resize();
        }
        data[length++] = value;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const {
        return length;
    }

    void display() const {
        cout << "  Vector[" << length << "]: ";
        for (size_t i = 0; i < length; ++i) {
            cout << data[i] << " ";
        }
        cout << "\n";
    }
};

void demonstrateRealWorld() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: REAL-WORLD CONTAINER  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating vector ---\n";
    SimpleVector<int> vec;

    cout << "\n--- Adding elements ---\n";
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.display();

    cout << "\n--- Adding more (triggers resize) ---\n";
    vec.push_back(40);
    vec.push_back(50);
    vec.display();

    cout << "\n--- Accessing elements ---\n";
    cout << "  vec[0]: " << vec[0] << "\n";
    cout << "  vec[2]: " << vec[2] << "\n";

    cout << "\n--- With strings ---\n";
    SimpleVector<string> names;
    names.push_back("Alice");
    names.push_back("Bob");
    names.push_back("Charlie");
    names.display();

    cout << "\n💡 REAL-WORLD CONTAINER:\n";
    cout << "   • Dynamic array implementation\n";
    cout << "   • Automatic resizing\n";
    cout << "   • Works with any type\n";
    cout << "   • Like std::vector!\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║             CLASS TEMPLATES IN C++                       ║\n";
    cout << "║             Generic Classes and Containers                ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateBasic();
    demonstrateMemberFunctions();
    demonstrateMultipleParams();
    demonstrateDefaultParams();
    demonstrateNonType();
    demonstrateSpecialization();
    demonstratePartialSpecialization();
    demonstrateStatic();
    demonstrateFriend();
    demonstrateRealWorld();

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
    cout << "WHAT ARE CLASS TEMPLATES?\n";
    cout << "========================================\n";
    cout << "Class templates:\n";
    cout << "  • Generic classes for any type\n";
    cout << "  • Blueprint for creating classes\n";
    cout << "  • Foundation of STL containers\n";
    cout << "  • Compile-time polymorphism\n";
    cout << "  • Zero runtime overhead\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "template<typename T>\n";
    cout << "class Box {\n";
    cout << "    T value;\n";
    cout << "public:\n";
    cout << "    Box(T v) : value(v) { }\n";
    cout << "    T get() const { return value; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "Box<int> intBox(42);        // T = int\n";
    cout << "Box<string> strBox(\"Hi\");   // T = string\n";
    cout << "Box<double> dblBox(3.14);   // T = double\n";

    cout << "\n========================================\n";
    cout << "INSTANTIATION\n";
    cout << "========================================\n";
    cout << "EXPLICIT INSTANTIATION:\n";
    cout << "Box<int> b1;      // Creates Box for int\n";
    cout << "Box<double> b2;   // Creates Box for double\n";
    cout << "// Each type = separate class\n";
    cout << "\n";
    cout << "WHAT HAPPENS:\n";
    cout << "1. Template Definition:\n";
    cout << "   template<typename T>\n";
    cout << "   class Box { T value; };\n";
    cout << "\n";
    cout << "2. Instantiation:\n";
    cout << "   Box<int> b;  // Compiler generates:\n";
    cout << "   class Box_int {\n";
    cout << "       int value;\n";
    cout << "   };\n";
    cout << "\n";
    cout << "3. Result:\n";
    cout << "   • One template → many classes\n";
    cout << "   • Generated at compile time\n";
    cout << "   • Each type is separate class\n";
    cout << "   • No code until instantiated\n";

    cout << "\n========================================\n";
    cout << "MEMBER FUNCTIONS\n";
    cout << "========================================\n";
    cout << "INSIDE CLASS (inline):\n";
    cout << "template<typename T>\n";
    cout << "class Box {\n";
    cout << "public:\n";
    cout << "    T get() const { return value; }  // Inline\n";
    cout << "};\n";
    cout << "\n";
    cout << "OUTSIDE CLASS:\n";
    cout << "template<typename T>\n";
    cout << "class Box {\n";
    cout << "public:\n";
    cout << "    T get() const;  // Declaration\n";
    cout << "};\n";
    cout << "\n";
    cout << "template<typename T>  // Definition\n";
    cout << "T Box<T>::get() const {\n";
    cout << "    return value;\n";
    cout << "}\n";
    cout << "\n";
    cout << "IMPORTANT:\n";
    cout << "  • Need template<typename T> prefix\n";
    cout << "  • Use ClassName<T>:: syntax\n";
    cout << "  • Usually in header file\n";
    cout << "  • All definitions need to be visible\n";

    cout << "\n========================================\n";
    cout << "MULTIPLE TEMPLATE PARAMETERS\n";
    cout << "========================================\n";
    cout << "template<typename K, typename V>\n";
    cout << "class Map {\n";
    cout << "    K key;\n";
    cout << "    V value;\n";
    cout << "public:\n";
    cout << "    Map(K k, V v) : key(k), value(v) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "Map<int, string> m1(1, \"One\");\n";
    cout << "Map<string, double> m2(\"Pi\", 3.14);\n";
    cout << "Map<char, int> m3('A', 65);\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • Flexible type combinations\n";
    cout << "  • Like std::map, std::pair\n";
    cout << "  • Each parameter independent\n";

    cout << "\n========================================\n";
    cout << "DEFAULT TEMPLATE PARAMETERS\n";
    cout << "========================================\n";
    cout << "template<typename T, typename Alloc = allocator<T>>\n";
    cout << "class Vector {\n";
    cout << "    // Use Alloc for memory\n";
    cout << "};\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "Vector<int> v1;              // Uses default allocator\n";
    cout << "Vector<int, MyAlloc> v2;     // Custom allocator\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • Sensible defaults\n";
    cout << "  • Override when needed\n";
    cout << "  • Simplifies common usage\n";
    cout << "  • std::vector uses this!\n";

    cout << "\n========================================\n";
    cout << "NON-TYPE TEMPLATE PARAMETERS\n";
    cout << "========================================\n";
    cout << "template<typename T, int Size>\n";
    cout << "class Array {\n";
    cout << "    T data[Size];\n";
    cout << "};\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "Array<int, 5> arr1;    // int[5]\n";
    cout << "Array<double, 10> arr2; // double[10]\n";
    cout << "// Array<int,5> != Array<int,10>\n";
    cout << "\n";
    cout << "ALLOWED TYPES:\n";
    cout << "  • Integral types (int, char, bool)\n";
    cout << "  • Enumerations\n";
    cout << "  • Pointers\n";
    cout << "  • References\n";
    cout << "  • Must be compile-time constant\n";
    cout << "\n";
    cout << "USE CASES:\n";
    cout << "  • Fixed-size arrays (std::array)\n";
    cout << "  • Matrix dimensions\n";
    cout << "  • Buffer sizes\n";

    cout << "\n========================================\n";
    cout << "TEMPLATE SPECIALIZATION\n";
    cout << "========================================\n";
    cout << "FULL SPECIALIZATION:\n";
    cout << "// Generic\n";
    cout << "template<typename T>\n";
    cout << "class Container { };\n";
    cout << "\n";
    cout << "// Specialization for bool\n";
    cout << "template<>\n";
    cout << "class Container<bool> {\n";
    cout << "    // Different implementation\n";
    cout << "};\n";
    cout << "\n";
    cout << "PARTIAL SPECIALIZATION:\n";
    cout << "// Generic\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "class Pair { };\n";
    cout << "\n";
    cout << "// Both types same\n";
    cout << "template<typename T>\n";
    cout << "class Pair<T, T> { };\n";
    cout << "\n";
    cout << "// Second is pointer\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "class Pair<T1, T2*> { };\n";
    cout << "\n";
    cout << "WHY SPECIALIZE?\n";
    cout << "  • Type-specific optimizations\n";
    cout << "  • Different data structures\n";
    cout << "  • Special handling (vector<bool>)\n";

    cout << "\n========================================\n";
    cout << "STATIC MEMBERS\n";
    cout << "========================================\n";
    cout << "template<typename T>\n";
    cout << "class Counter {\n";
    cout << "    static int count;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Definition (in .cpp or header)\n";
    cout << "template<typename T>\n";
    cout << "int Counter<T>::count = 0;\n";
    cout << "\n";
    cout << "IMPORTANT:\n";
    cout << "  • Each instantiation has own static\n";
    cout << "  • Counter<int>::count separate from Counter<double>::count\n";
    cout << "  • Must define outside class\n";
    cout << "  • template<typename T> prefix\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Keep templates in headers\n";
    cout << "  • Use meaningful names (Value, Container)\n";
    cout << "  • Document type requirements\n";
    cout << "  • Provide default parameters\n";
    cout << "  • Use const references\n";
    cout << "  • Follow Rule of 3/5/0\n";
    cout << "  • Test with multiple types\n";
    cout << "  • Use type traits\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Put definitions in .cpp files\n";
    cout << "  • Over-complicate templates\n";
    cout << "  • Forget copy/move semantics\n";
    cout << "  • Ignore const-correctness\n";
    cout << "  • Make assumptions about T\n";
    cout << "  • Create circular dependencies\n";
    cout << "  • Forget friend function syntax\n";
    cout << "  • Ignore compilation errors\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Container\n";
    cout << "template<typename T>\n";
    cout << "class Vector {\n";
    cout << "    T* data;\n";
    cout << "    size_t size;\n";
    cout << "    // Dynamic array of T\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Pair/Tuple\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "class Pair {\n";
    cout << "    T1 first;\n";
    cout << "    T2 second;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: Smart Pointer\n";
    cout << "template<typename T>\n";
    cout << "class UniquePtr {\n";
    cout << "    T* ptr;\n";
    cout << "    // RAII wrapper\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic class template\n";
    cout << "template<typename T>\n";
    cout << "class Box {\n";
    cout << "    T value;\n";
    cout << "public:\n";
    cout << "    Box(T v) : value(v) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Member outside class\n";
    cout << "template<typename T>\n";
    cout << "T Box<T>::getValue() const {\n";
    cout << "    return value;\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Multiple parameters\n";
    cout << "template<typename K, typename V>\n";
    cout << "class Map { };\n";
    cout << "\n";
    cout << "// Default parameter\n";
    cout << "template<typename T, typename C = vector<T>>\n";
    cout << "class Stack { };\n";
    cout << "\n";
    cout << "// Non-type parameter\n";
    cout << "template<typename T, int Size>\n";
    cout << "class Array { };\n";
    cout << "\n";
    cout << "// Specialization\n";
    cout << "template<>\n";
    cout << "class Box<bool> { };\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Class templates = generic classes\n";
    cout << "2. Foundation of STL containers\n";
    cout << "3. Instantiated at compile time\n";
    cout << "4. Each type = separate class\n";
    cout << "5. Keep definitions in headers\n";
    cout << "6. Support multiple parameters\n";
    cout << "7. Can specialize for types\n";
    cout << "8. Static members per instantiation\n";
    cout << "9. Zero runtime overhead\n";
    cout << "10. Essential for generic containers\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Keep in headers\n";
    cout << "   Full definition must be visible!\n";
    cout << "   \n";
    cout << "   Template code in .h file\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Each type = separate class\n";
    cout << "   Box<int> != Box<double>\n";
    cout << "   \n";
    cout << "   Different classes entirely\n";
    cout << "\n";
    cout << "⚠️  RULE 3: Member functions syntax\n";
    cout << "   Outside class needs template prefix!\n";
    cout << "   \n";
    cout << "   template<typename T>\n";
    cout << "   T Box<T>::get() const { }\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Document requirements\n";
    cout << "   What must T support?\n";
    cout << "   \n";
    cout << "   T needs operator<, copy ctor, etc.\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Test with multiple types\n";
    cout << "   Don't assume it works!\n";
    cout << "   \n";
    cout << "   Test int, double, string, custom\n";

    return 0;
}
