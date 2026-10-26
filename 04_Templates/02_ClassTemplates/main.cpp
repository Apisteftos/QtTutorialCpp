#include <cstddef>
#include <ostream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

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
// === EXAMPLE 1: BASIC CLASS TEMPLATE ===
// ============================================

template<typename T>
class Box {
private:
    T value;

public:
    Box(T v) : value(v) {
        std::println("  Box constructor");
    }

    void setValue(T v) {
        value = v;
    }

    T getValue() const {
        return value;
    }

    void display() const {
        std::println("  Box contains: {}", value);
    }
};

void demonstrateBasic() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC CLASS TEMPLATE   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Box with different types ---");
    Box<int> intBox(42);
    intBox.display();

    Box<double> doubleBox(3.14);
    doubleBox.display();

    Box<std::string> stringBox("Hello");
    stringBox.display();

    std::println("");
    std::println("--- Modifying values ---");
    intBox.setValue(100);
    intBox.display();
}

// ============================================
// === EXAMPLE 2: MEMBER FUNCTIONS OUTSIDE CLASS ===
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
    std::println("  Pair constructor");
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
    std::println("  Pair: ({}, {})", first, second);
}

void demonstrateMemberFunctions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: MEMBER FUNCTIONS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating pairs ---");
    Pair<int> intPair(10, 20);
    intPair.display();

    Pair<std::string> strPair("Hello", "World");
    strPair.display();

    std::println("");
    std::println("--- Accessing members ---");
    std::println("  First: {}", intPair.getFirst());
    std::println("  Second: {}", intPair.getSecond());
}

// ============================================
// === EXAMPLE 3: MULTIPLE TEMPLATE PARAMETERS ===
// ============================================

template<typename K, typename V>
class KeyValue {
private:
    K key;
    V value;

public:
    KeyValue(K k, V v) : key(k), value(v) {
        std::println("  KeyValue constructor");
    }

    K getKey() const { return key; }
    V getValue() const { return value; }

    void display() const {
        std::println("  Key: {}, Value: {}", key, value);
    }
};

void demonstrateMultipleParams() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: MULTIPLE PARAMETERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Different type combinations ---");
    KeyValue<int, std::string> kv1(1, "One");
    kv1.display();

    KeyValue<std::string, int> kv2("Age", 25);
    kv2.display();

    KeyValue<std::string, double> kv3("Pi", 3.14159);
    kv3.display();
}

// ============================================
// === EXAMPLE 4: DEFAULT TEMPLATE PARAMETERS ===
// ============================================

template<typename T, typename Container = std::vector<T>>
class Stack {
private:
    Container data;

public:
    void push(const T& value) {
        data.push_back(value);
        std::println("  Pushed: {}", value);
    }

    void pop() {
        if (!data.empty()) {
            std::println("  Popped: {}", data.back());
            data.pop_back();
        }
    }

    T top() const {
        return data.back();
    }

    bool empty() const {
        return data.empty();
    }

    std::size_t size() const {
        return data.size();
    }
};

void demonstrateDefaultParams() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: DEFAULT PARAMETERS     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Stack with default container (vector) ---");
    Stack<int> s1;  // Uses std::vector<int> by default
    s1.push(10);
    s1.push(20);
    s1.push(30);
    std::println("  Size: {}", s1.size());
    s1.pop();
}

// ============================================
// === EXAMPLE 5: NON-TYPE TEMPLATE PARAMETERS ===
// ============================================

template<typename T, int Size>
class Array {
private:
    T data[Size];

public:
    Array() {
        std::println("  Array<T, {}> constructor", Size);
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
        std::print("  Array[{}]: ", Size);
        for (int i = 0; i < Size; ++i) {
            std::print("{} ", data[i]);
        }
        std::println("");
    }
};

void demonstrateNonType() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: NON-TYPE PARAMETERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Fixed-size arrays ---");
    Array<int, 5> arr1;
    arr1.fill(10);
    arr1.display();

    Array<double, 3> arr2;
    arr2[0] = 1.1;
    arr2[1] = 2.2;
    arr2[2] = 3.3;
    arr2.display();

    std::println("  Size known at compile time: {}", arr1.size());
}

// ============================================
// === EXAMPLE 6: TEMPLATE SPECIALIZATION ===
// ============================================

// Generic template
template<typename T>
class Printer {
public:
    void print(const T& value) {
        std::println("  Generic: {}", value);
    }
};

// Full specialization for bool
template<>
class Printer<bool> {
public:
    void print(const bool& value) {
        std::println("  Bool: {}", value ? "true" : "false");
    }
};

// Full specialization for const char*
template<>
class Printer<const char*> {
public:
    void print(const char* const& value) {
        std::println("  String: \"{}\"", value);
    }
};

void demonstrateSpecialization() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: SPECIALIZATION         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Generic template ---");
    Printer<int> p1;
    p1.print(42);

    Printer<double> p2;
    p2.print(3.14);

    std::println("");
    std::println("--- Specialized versions ---");
    Printer<bool> p3;
    p3.print(true);
    p3.print(false);

    Printer<const char*> p4;
    p4.print("Hello World");
}

// ============================================
// === EXAMPLE 7: PARTIAL SPECIALIZATION ===
// ============================================

// Primary template
template<typename T1, typename T2>
class Holder {
public:
    void describe() {
        std::println("  Generic: Two different types");
    }
};

// Partial specialization: both types same
template<typename T>
class Holder<T, T> {
public:
    void describe() {
        std::println("  Specialized: Both types are same");
    }
};

// Partial specialization: second type is pointer
template<typename T1, typename T2>
class Holder<T1, T2*> {
public:
    void describe() {
        std::println("  Specialized: Second type is pointer");
    }
};

void demonstratePartialSpecialization() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: PARTIAL SPECIALIZATION ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Different combinations ---");
    Holder<int, double> h1;
    h1.describe();  // Generic

    Holder<int, int> h2;
    h2.describe();  // Both same

    Holder<int, double*> h3;
    h3.describe();  // Second is pointer
}

// ============================================
// === EXAMPLE 8: STATIC MEMBERS ===
// ============================================

template<typename T>
class Counter {
private:
    static int count;
    T value;

public:
    Counter(T v) : value(v) {
        count++;
        std::println("  Counter created (count={})", count);
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: STATIC MEMBERS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- int counters (separate count) ---");
    Counter<int> c1(10);
    Counter<int> c2(20);
    std::println("  Int count: {}", Counter<int>::getCount());

    std::println("");
    std::println("--- string counters (separate count) ---");
    Counter<std::string> s1("Hello");
    Counter<std::string> s2("World");
    std::println("  String count: {}", Counter<std::string>::getCount());

    std::println("");
    std::println("--- int count still separate ---");
    std::println("  Int count: {}", Counter<int>::getCount());
}

// ============================================
// === EXAMPLE 9: FRIEND FUNCTIONS ===
// ============================================

template<typename T>
class Value;

// Friend function declaration
template<typename T>
std::ostream& operator<<(std::ostream& os, const Value<T>& v);

template<typename T>
class Value {
private:
    T data;

public:
    Value(T d) : data(d) {}

    // Friend function
    friend std::ostream& operator<< <>(std::ostream& os, const Value<T>& v);

    // Friend function defined inside
    friend Value<T> operator+(const Value<T>& a, const Value<T>& b) {
        return Value<T>(a.data + b.data);
    }
};

// Friend function definition
template<typename T>
std::ostream& operator<<(std::ostream& os, const Value<T>& v) {
    os << "Value(" << v.data << ")";
    return os;
}

// std::println cannot format a type that only has operator<<,
// so we render it to a string first.
template<typename T>
std::string toString(const Value<T>& v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

void demonstrateFriend() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: FRIEND FUNCTIONS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using friend operators ---");
    Value<int> v1(10);
    Value<int> v2(20);

    std::println("  v1: {}", toString(v1));
    std::println("  v2: {}", toString(v2));

    Value<int> v3 = v1 + v2;
    std::println("  v1 + v2: {}", toString(v3));
}

// ============================================
// === EXAMPLE 10: REAL-WORLD CONTAINER ===
// ============================================

template<typename T>
class SimpleVector {
private:
    T* data;
    std::size_t capacity;
    std::size_t length;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (std::size_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        std::println("  Resized to capacity: {}", capacity);
    }

public:
    SimpleVector() : data(nullptr), capacity(4), length(0) {
        data = new T[capacity];
        std::println("  SimpleVector created");
    }

    ~SimpleVector() {
        delete[] data;
        std::println("  SimpleVector destroyed");
    }

    // Copy constructor
    SimpleVector(const SimpleVector& other)
        : capacity(other.capacity), length(other.length) {
        data = new T[capacity];
        for (std::size_t i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
        std::println("  SimpleVector copied");
    }

    // Copy assignment
    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            length = other.length;
            data = new T[capacity];
            for (std::size_t i = 0; i < length; ++i) {
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

    T& operator[](std::size_t index) {
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        return data[index];
    }

    std::size_t size() const {
        return length;
    }

    void display() const {
        std::print("  Vector[{}]: ", length);
        for (std::size_t i = 0; i < length; ++i) {
            std::print("{} ", data[i]);
        }
        std::println("");
    }
};

void demonstrateRealWorld() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: REAL-WORLD CONTAINER  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating vector ---");
    SimpleVector<int> vec;

    std::println("");
    std::println("--- Adding elements ---");
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.display();

    std::println("");
    std::println("--- Adding more (triggers resize) ---");
    vec.push_back(40);
    vec.push_back(50);
    vec.display();

    std::println("");
    std::println("--- Accessing elements ---");
    std::println("  vec[0]: {}", vec[0]);
    std::println("  vec[2]: {}", vec[2]);

    std::println("");
    std::println("--- With strings ---");
    SimpleVector<std::string> names;
    names.push_back("Alice");
    names.push_back("Bob");
    names.push_back("Charlie");
    names.display();
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║             CLASS TEMPLATES IN C++                       ║");
    std::println("║             Generic Classes and Containers                ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
