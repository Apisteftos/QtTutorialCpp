# 09_OperatorOverloading - Custom Operator Behavior

## 📚 Overview

This folder introduces **Operator Overloading** - the ability to define custom behavior for operators when used with user-defined types. Operator overloading makes your classes behave like built-in types, leading to more intuitive and readable code.

**Operator Overloading** = Define how operators (+, -, *, ==, <<, etc.) work with your classes

This is a powerful C++ feature that enables natural syntax for custom types.

---

## 🎯 What You'll Learn

- What operator overloading is and why it's useful
- Syntax for overloading operators
- Member vs non-member operators
- Arithmetic operators (+, -, *, /, %)
- Comparison operators (==, !=, <, >, <=, >=)
- Assignment operators (=, +=, -=, etc.)
- Increment/decrement (++, --)
- Stream operators (<<, >>)
- Subscript operator ([])
- Function call operator (())
- Best practices and common mistakes

---

## 📖 Concept Explanation

### What is Operator Overloading?

**Operator Overloading** allows you to define custom behavior for operators when applied to your classes.

**Without operator overloading:**
```cpp
class Fraction {
public:
    Fraction add(const Fraction& other);
    bool isEqual(const Fraction& other);
};

Fraction f1, f2, f3;
f3 = f1.add(f2);           // Awkward
if (f1.isEqual(f2)) { }    // Verbose
```

**With operator overloading:**
```cpp
class Fraction {
public:
    Fraction operator+(const Fraction& other);
    bool operator==(const Fraction& other);
};

Fraction f1, f2, f3;
f3 = f1 + f2;              // Natural! ✅
if (f1 == f2) { }          // Intuitive! ✅
```

### Real-World Analogy
```
Think of operators as verbs in language:

Built-in types:
  5 + 3        "add 5 and 3"
  x == y       "x equals y"

Custom types (without overloading):
  f1.add(f2)      "f1, add f2" (awkward)
  f1.equals(f2)   "f1, equals f2" (verbose)

Custom types (with overloading):
  f1 + f2      "add f1 and f2" (natural!)
  f1 == f2     "f1 equals f2" (intuitive!)
```

---

## 💻 Basic Syntax

### General Form
```cpp
// As member function
ReturnType operator@(parameters) {
    // implementation
}

// As non-member function
ReturnType operator@(Type1 obj1, Type2 obj2) {
    // implementation
}
```

### Simple Example
```cpp
class Point {
private:
    int x, y;
    
public:
    Point(int x, int y) : x(x), y(y) { }
    
    // Operator overloading
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
};

// Usage
Point p1(1, 2);
Point p2(3, 4);
Point p3 = p1 + p2;  // Calls operator+
// p3 is now (4, 6)
```

---

## 🔍 Operators Overview

### Operators That CAN Be Overloaded

#### Arithmetic
```cpp
+  -  *  /  %
+=  -=  *=  /=  %=
```

#### Comparison
```cpp
==  !=  <  >  <=  >=
```

#### Logical
```cpp
&&  ||  !
```

#### Bitwise
```cpp
&  |  ^  ~  <<  >>
&=  |=  ^=  <<=  >>=
```

#### Increment/Decrement
```cpp
++  --  (prefix and postfix)
```

#### Assignment
```cpp
=
```

#### Memory
```cpp
new  delete  new[]  delete[]
```

#### Other
```cpp
[]  ()  ->  ,  ->*
```

### Operators That CANNOT Be Overloaded
```cpp
::   (scope resolution)
.    (member access)
.*   (member pointer access)
?:   (ternary conditional)
sizeof
typeid
```

---

## 📋 Examples in This Folder

### Example 1: Arithmetic Operators
Fraction class with +, -, *, / operations.

### Example 2: Comparison Operators
Point class with ==, !=, <, >, <=, >= comparisons.

### Example 3: Assignment Operators
Counter class with =, +=, -=, *=, /= operations.

### Example 4: Increment/Decrement
Iterator class with prefix and postfix ++/--.

### Example 5: Stream Operators
Complex class with << and >> for I/O.

### Example 6: Subscript Operator
Array class with [] for element access.

### Example 7: Function Call Operator
Multiplier class with () to make objects callable.

### Example 8: Member vs Non-Member
Vector class showing both approaches.

### Example 9: Matrix Class
Complete 2D matrix with multiple operators.

### Example 10: String Class
Full string implementation with all common operators.

---

## 🎓 Key Concepts

### Member vs Non-Member Operators

#### Member Operators

**Form:**
```cpp
class MyClass {
public:
    ReturnType operator@(parameters) const;
};
```

**Usage:** `obj @ arg` becomes `obj.operator@(arg)`

**Characteristics:**
- `this` is the left operand
- Can access private members directly
- More common for most operators

**Must be members:**
- `=` (assignment)
- `[]` (subscript)
- `()` (function call)
- `->` (member access)

**Example:**
```cpp
class Vector {
    double x, y;
public:
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }
};

Vector v1, v2, v3;
v3 = v1 + v2;  // v1.operator+(v2)
```

#### Non-Member Operators

**Form:**
```cpp
ReturnType operator@(Type1 obj1, Type2 obj2) {
    // implementation
}
```

**Usage:** `obj1 @ obj2` becomes `operator@(obj1, obj2)`

**Characteristics:**
- Both operands as parameters
- Often declared as `friend`
- Allows conversions on left operand
- Used for symmetric operations

**Example:**
```cpp
class Vector {
    double x, y;
public:
    // Member: v * 2
    Vector operator*(double scalar) const {
        return Vector(x * scalar, y * scalar);
    }
    
    // Non-member: 2 * v (symmetry!)
    friend Vector operator*(double scalar, const Vector& v) {
        return v * scalar;  // Reuse member operator
    }
};

Vector v;
Vector v2 = v * 2;   // Works (member)
Vector v3 = 2 * v;   // Works (non-member)
```

### When to Use Each

**Use Member:**
- Default choice for most operators
- When operator modifies left operand
- When you need access to private members

**Use Non-Member:**
- Stream operators (`<<`, `>>`) - must be non-member
- Symmetric operators (for commutativity)
- When left operand is not your class
- When you want implicit conversions on left side

---

## 💡 Specific Operators

### Arithmetic Operators (+, -, *, /, %)
```cpp
class Complex {
    double real, imag;
public:
    // Addition
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
    
    // Subtraction
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }
    
    // Multiplication
    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }
};
```

**Key points:**
- Return by value (new object)
- Should be `const` (doesn't modify)
- Consider implementing compound assignment first

### Comparison Operators (==, !=, <, >, <=, >=)
```cpp
class Point {
    int x, y;
public:
    // Equality
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    // Inequality (reuse ==)
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    
    // Less than
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    // Greater than (reuse <)
    bool operator>(const Point& other) const {
        return other < *this;
    }
    
    // Less than or equal (reuse >)
    bool operator<=(const Point& other) const {
        return !(*this > other);
    }
    
    // Greater than or equal (reuse <)
    bool operator>=(const Point& other) const {
        return !(*this < other);
    }
};
```

**Pattern:** Implement `==` and `<`, derive others!

### Assignment Operators (=, +=, -=, etc.)
```cpp
class MyClass {
    int* data;
    int size;
public:
    // Copy assignment
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {  // Self-assignment check
            delete[] data;
            size = other.size;
            data = new int[size];
            // Copy data...
        }
        return *this;  // Return reference for chaining
    }
    
    // Compound assignment
    MyClass& operator+=(const MyClass& other) {
        // Add logic...
        return *this;
    }
};

// Usage
MyClass a, b, c;
a = b = c;      // Chaining works!
a += b;         // Modify a
```

**Key points:**
- Return reference to `*this`
- Check for self-assignment
- Enables chaining: `a = b = c`

### Increment/Decrement (++, --)
```cpp
class Counter {
    int value;
public:
    // Prefix increment: ++obj
    Counter& operator++() {
        ++value;
        return *this;  // Return new value
    }
    
    // Postfix increment: obj++
    Counter operator++(int) {  // int = dummy parameter
        Counter temp = *this;  // Save old value
        ++value;               // Increment
        return temp;           // Return old value
    }
    
    // Prefix decrement: --obj
    Counter& operator--() {
        --value;
        return *this;
    }
    
    // Postfix decrement: obj--
    Counter operator--(int) {
        Counter temp = *this;
        --value;
        return temp;
    }
};

// Usage
Counter c;
++c;    // Prefix: returns new value
c++;    // Postfix: returns old value
```

**Differences:**
- **Prefix:** Returns reference, more efficient
- **Postfix:** Returns copy, less efficient
- **Dummy `int`:** Differentiates postfix

**Prefer prefix when return value not used!**

### Stream Operators (<<, >>)
```cpp
class Point {
    int x, y;
public:
    // Output operator (MUST be non-member)
    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;  // For chaining
    }
    
    // Input operator (MUST be non-member)
    friend istream& operator>>(istream& is, Point& p) {
        is >> p.x >> p.y;
        return is;  // For chaining
    }
};

// Usage
Point p;
cout << p;           // Output
cin >> p;            // Input
cout << p1 << p2;    // Chaining
```

**Key points:**
- **Must** be non-member (for natural syntax)
- Declared as `friend` to access private members
- Return stream reference for chaining
- Left operand is stream, not your object

### Subscript Operator ([])
```cpp
class Array {
    int* data;
    int size;
public:
    // Non-const version (read/write)
    int& operator[](int index) {
        return data[index];
    }
    
    // Const version (read-only)
    const int& operator[](int index) const {
        return data[index];
    }
};

// Usage
Array arr(10);
arr[5] = 42;        // Uses non-const version
int x = arr[5];     // Uses non-const version

const Array carr(10);
int y = carr[3];    // Uses const version
// carr[3] = 10;    // Error! Can't modify
```

**Key points:**
- **Must** be member function
- Provide both const and non-const versions
- Non-const returns reference for assignment
- Const version for read-only access

### Function Call Operator (())
```cpp
class Multiplier {
    int factor;
public:
    Multiplier(int f) : factor(f) { }
    
    // Makes object callable like a function
    int operator()(int value) const {
        return value * factor;
    }
    
    // Can be overloaded
    int operator()(int a, int b) const {
        return (a + b) * factor;
    }
};

// Usage
Multiplier times2(2);
int result = times2(5);      // Calls operator()(5)
int sum = times2(3, 4);      // Calls operator()(3, 4)
```

**Use cases:**
- Functors (function objects)
- Callbacks
- STL algorithms
- Custom predicates

---

## 🔧 Common Patterns

### Pattern 1: Implement Compound, Derive Simple
```cpp
class MyClass {
public:
    // Implement += first
    MyClass& operator+=(const MyClass& other) {
        // Add logic
        return *this;
    }
    
    // Derive + from +=
    MyClass operator+(const MyClass& other) const {
        MyClass temp = *this;
        temp += other;
        return temp;
    }
};
```

### Pattern 2: Implement One Comparison, Derive Others
```cpp
class MyClass {
public:
    // Implement == and 
    bool operator==(const MyClass& o) const { /* ... */ }
    bool operator<(const MyClass& o) const { /* ... */ }
    
    // Derive others
    bool operator!=(const MyClass& o) const { return !(*this == o); }
    bool operator>(const MyClass& o) const { return o < *this; }
    bool operator<=(const MyClass& o) const { return !(*this > o); }
    bool operator>=(const MyClass& o) const { return !(*this < o); }
};
```

### Pattern 3: Symmetric Operators
```cpp
class Vector {
    double x, y;
public:
    // Member for v * scalar
    Vector operator*(double s) const {
        return Vector(x * s, y * s);
    }
    
    // Non-member for scalar * v
    friend Vector operator*(double s, const Vector& v) {
        return v * s;  // Reuse member operator
    }
};
```

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Wrong Return Type
```cpp
class Bad {
public:
    // ❌ Should return by value, not void
    void operator+(const Bad& other) { }
    
    // ❌ Should return reference, not copy
    Bad operator=(const Bad& other) { }
};
```

**Fix:**
```cpp
class Good {
public:
    Bad operator+(const Bad& other) const { }  // ✅ By value
    Bad& operator=(const Bad& other) { }       // ✅ By reference
};
```

### ❌ Mistake 2: Returning Reference to Temporary
```cpp
class Bad {
public:
    const Bad& operator+(const Bad& other) const {
        Bad temp;  // Local variable
        return temp;  // ❌ Returns reference to destroyed object!
    }
};
```

**Fix:**
```cpp
class Good {
public:
    Bad operator+(const Bad& other) const {
        Bad temp;
        return temp;  // ✅ Returns copy
    }
};
```

### ❌ Mistake 3: Missing const
```cpp
class Bad {
public:
    // ❌ Should be const (doesn't modify object)
    bool operator==(const Bad& other) {
        return true;
    }
};
```

**Fix:**
```cpp
class Good {
public:
    bool operator==(const Good& other) const {  // ✅ const
        return true;
    }
};
```

### ❌ Mistake 4: Not Checking Self-Assignment
```cpp
class Bad {
    int* data;
public:
    Bad& operator=(const Bad& other) {
        delete[] data;  // ❌ If this == &other, deleted!
        data = new int[other.size];
        // Copy from deleted memory!
    }
};
```

**Fix:**
```cpp
class Good {
    int* data;
public:
    Good& operator=(const Good& other) {
        if (this != &other) {  // ✅ Check first
            delete[] data;
            data = new int[other.size];
            // Copy...
        }
        return *this;
    }
};
```

### ❌ Mistake 5: Member Stream Operators
```cpp
class Bad {
public:
    // ❌ As member, syntax would be: obj << cout
    ostream& operator<<(ostream& os) {
        return os;
    }
};
```

**Fix:**
```cpp
class Good {
public:
    // ✅ Non-member, syntax is: cout << obj
    friend ostream& operator<<(ostream& os, const Good& obj) {
        return os;
    }
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Complex Number Class
Create a `Complex` class with:
- Real and imaginary parts
- Arithmetic: +, -, *, /
- Comparison: ==, !=
- Stream: <<, >>
- Test with: (3+4i) * (1+2i)

### Exercise 2: Rational Number Class
Create a `Rational` class with:
- Numerator and denominator
- Automatic simplification
- All arithmetic operators
- Comparison operators
- Test: 1/2 + 1/3 = 5/6

### Exercise 3: 2D Vector Class
Create a `Vector2D` class with:
- x, y components
- +, -, * (scalar and dot product)
- Magnitude and normalization
- Stream operators

### Exercise 4: Matrix Class
Create a `Matrix` class with:
- Dynamic 2D array
- +, -, * (matrix multiplication)
- [] for row access
- Stream operators

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 operator_overloading.cpp -o operator_overloading
```

### Run:
```bash
./operator_overloading
```

### Expected Output:
The program demonstrates:
1. Arithmetic operators (Fraction)
2. Comparison operators (Point)
3. Assignment operators (Counter)
4. Increment/decrement (Iterator)
5. Stream operators (Complex)
6. Subscript operator (Array)
7. Function call operator (Multiplier)
8. Member vs non-member (Vector2D)
9. Matrix class
10. String class

---

## 📊 Visual Concept

### How Operators Work
```
USER CODE:          WHAT COMPILER SEES:
═════════════       ═══════════════════

a + b        →      a.operator+(b)           (member)
                    operator+(a, b)          (non-member)

cout << obj  →      operator<<(cout, obj)    (non-member)

arr[5]       →      arr.operator[](5)        (member)

++obj        →      obj.operator++()         (prefix)
obj++        →      obj.operator++(0)        (postfix)

obj(args)    →      obj.operator()(args)     (functor)
```

### Operator Categories
```
┌─────────────────────────────────────┐
│         OPERATORS                   │
├─────────────────────────────────────┤
│ ARITHMETIC: +, -, *, /, %           │
│   Return: By value                  │
│   Type:   Usually member            │
├─────────────────────────────────────┤
│ COMPARISON: ==, !=, <, >, <=, >=    │
│   Return: bool                      │
│   Type:   Member (const)            │
├─────────────────────────────────────┤
│ ASSIGNMENT: =, +=, -=, *=, /=       │
│   Return: Reference to *this        │
│   Type:   MUST be member            │
├─────────────────────────────────────┤
│ INCREMENT: ++, --                   │
│   Prefix:  Return reference         │
│   Postfix: Return copy              │
├─────────────────────────────────────┤
│ STREAM: <<, >>                      │
│   Return: Stream reference          │
│   Type:   MUST be non-member        │
├─────────────────────────────────────┤
│ SUBSCRIPT: []                       │
│   Return: Reference                 │
│   Type:   MUST be member            │
└─────────────────────────────────────┘
```

---

## 📚 Related Topics

### Prerequisites:
- **04_MemberFunctions** - Member functions
- **05_Encapsulation** - Private members
- **07_CopyConstructor** - Copy semantics

### Coming Next:
- **10_ThisPointer** - Using `this` pointer
- **11_StaticMembers** - Class-level members
- **13_Inheritance** - Operator inheritance

### Related Concepts:
- **Friend Functions** - For non-member operators
- **Const Correctness** - Essential for operators
- **Return Value Optimization** - For arithmetic operators

---

## 🎓 Summary

### What We Learned:

✅ **Operator overloading** = Custom operator behavior
✅ **Syntax**: `ReturnType operator@(parameters)`
✅ **Member** operators for most cases
✅ **Non-member** for stream and symmetric ops
✅ **Arithmetic** returns by value, const
✅ **Assignment** returns reference, checks self
✅ **Comparison** returns bool, const
✅ **Stream** must be non-member
✅ **Subscript** must be member
✅ **Prefix/postfix** differ by dummy int

### Operator Categories Summary:
```cpp
// ARITHMETIC (return value, const)
MyClass operator+(const MyClass& o) const;

// COMPARISON (return bool, const)
bool operator==(const MyClass& o) const;

// ASSIGNMENT (return reference)
MyClass& operator=(const MyClass& o);
MyClass& operator+=(const MyClass& o);

// INCREMENT/DECREMENT
MyClass& operator++();      // Prefix
MyClass operator++(int);    // Postfix

// STREAM (non-member, friend)
friend ostream& operator<<(ostream& os, const MyClass& obj);

// SUBSCRIPT (member, both versions)
Type& operator[](int index);
const Type& operator[](int index) const;

// FUNCTION CALL (member)
ReturnType operator()(Parameters...);
```

### When to Overload:

**Good reasons:**
- Makes code more natural
- Matches mathematical notation
- Consistent with built-in types
- Improves readability

**Bad reasons:**
- Just because you can
- Unexpected behavior
- Confusing semantics

---

## ✨ Quick Reference Card
```cpp
// MEMBER OPERATORS
class MyClass {
public:
    // Arithmetic
    MyClass operator+(const MyClass& o) const { return MyClass(); }
    
    // Comparison
    bool operator==(const MyClass& o) const { return true; }
    
    // Assignment
    MyClass& operator=(const MyClass& o) { return *this; }
    MyClass& operator+=(const MyClass& o) { return *this; }
    
    // Increment/Decrement
    MyClass& operator++() { return *this; }      // Prefix
    MyClass operator++(int) { return *this; }    // Postfix
    
    // Subscript
    Type& operator[](int i) { return data[i]; }
    
    // Function call
    ReturnType operator()(Params...) { }
};

// NON-MEMBER OPERATORS
class MyClass {
    friend ostream& operator<<(ostream& os, const MyClass& obj);
    friend MyClass operator*(double s, const MyClass& obj);
};

ostream& operator<<(ostream& os, const MyClass& obj) {
    return os << "data";
}

MyClass operator*(double s, const MyClass& obj) {
    return obj * s;
}
```

---

## 🔍 Best Practices

### ✅ DO:

- **Keep behavior intuitive** - match expectations
- **Return appropriate types**:
  - Arithmetic: by value
  - Assignment: by reference
  - Comparison: bool
- **Use const** for non-modifying operators
- **Implement related operators** together
- **Check self-assignment** in operator=
- **Return *this** for chaining
- **Make stream operators non-member**
- **Provide both const/non-const** for []

### ❌ DON'T:

- **Change standard meanings** (+ for subtraction?)
- **Overload &&, ||, comma** (breaks short-circuit)
- **Make behavior surprising**
- **Return reference to temporary**
- **Forget const correctness**
- **Ignore self-assignment** in operator=
- **Make stream operators members**
- **Overload just for fun**

---

## 🌟 Real-World Applications

### Mathematical Types
```cpp
Complex c1(3, 4), c2(1, 2);
Complex c3 = c1 + c2;  // Natural math syntax
```

### Container Classes
```cpp
MyArray arr(100);
arr[50] = 42;  // Array-like access
```

### String Classes
```cpp
String s1 = "Hello";
String s2 = " World";
String s3 = s1 + s2;  // String concatenation
```

### Smart Pointers
```cpp
SmartPtr<int> ptr(new int(42));
*ptr = 100;      // Dereference
ptr->func();     // Member access
```

---

## 📖 Further Reading

- [cppreference.com - Operator Overloading](https://en.cppreference.com/w/cpp/language/operators)
- "Effective C++" by Scott Meyers - Items 23-27
- "C++ Primer" by Lippman - Chapter on Overloaded Operators
- [C++ Core Guidelines - Operator Overloading](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Operator overloading makes **code more natural**
2. Syntax: `ReturnType operator@(parameters)`
3. Can be **member or non-member**
4. Some **must be member** (=, [], (), ->)
5. Stream operators **must be non-member**
6. Return **appropriate types** (value, reference, bool)
7. Always use **const** for non-modifying ops
8. Implement **related operators together**
9. **Pattern**: implement compound (+=), derive simple (+)
10. Keep behavior **consistent and intuitive**

---

**Previous Topic:** [08_MoveSemantics](../08_MoveSemantics/) - Efficient Resource Transfer

**Next Topic:** [10_ThisPointer](../10_ThisPointer/) - The this Pointer (or continue with other topics)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: 04_MemberFunctions, 05_Encapsulation*
*Essential for Natural C++ Code*