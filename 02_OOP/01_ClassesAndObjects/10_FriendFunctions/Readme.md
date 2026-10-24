# 12_FriendFunctions - Breaking Encapsulation Carefully

## 📚 Overview

This folder introduces **Friend Functions** - non-member functions and classes that have special access to private and protected members of a class. Friend functions break encapsulation in a controlled way for specific use cases like operator overloading and tightly coupled classes.

**Friend Functions** = Non-member functions with access to private members

This is a powerful but potentially dangerous feature that must be used carefully and sparingly.

---

## 🎯 What You'll Learn

- What friend functions are and why they exist
- Friend functions vs member functions
- Friend classes
- Friend member functions
- Friendship rules (NOT mutual, NOT inherited, NOT transitive)
- When to use friend (and when not to!)
- Operator overloading with friend
- Testing and debugging with friend
- Best practices and common pitfalls
- Real-world patterns

---

## 📖 Concept Explanation

### What are Friend Functions?

**Friend functions** are non-member functions that have been granted access to the private and protected members of a class.

**Key Characteristics:**
- **Not member functions** - no `this` pointer
- **Can access private members** - break encapsulation
- **Declared with `friend` keyword** inside class
- **Defined outside class** without `friend` keyword (or defined right inside the class body, see below; it is still a non-member)

### Real-World Analogy
```
Think of your home:

MEMBER FUNCTIONS = Family members
  - Live in the house
  - Have natural access to everything
  - Part of the household

FRIEND FUNCTIONS = Close friends with a key
  - Don't live there
  - Have been given special access
  - Can come in and access private areas
  - Trust must be granted explicitly

REGULAR FUNCTIONS = Strangers
  - No access
  - Can only use public entrance (public methods)
```

### Visual Representation
```
class MyClass {
private:
    int secret = 42;           ◄─── Private
    
public:
    int getSecret() {          ◄─── Public interface
        return secret;
    }
    
    friend void friendFunc(const MyClass& obj);  ◄─── Friend
};

void friendFunc(const MyClass& obj) {
    cout << obj.secret;        ◄─── Direct access! ✅
}

void regularFunc(const MyClass& obj) {
    // cout << obj.secret;     ◄─── Error! ❌
    cout << obj.getSecret();   ◄─── Must use public ✅
}
```

---

## 💻 Basic Syntax

### Friend Function
```cpp
class MyClass {
private:
    int privateData;
    
public:
    // Declare friend inside class
    friend void friendFunc(const MyClass& obj);
};

// Define friend outside class (NO 'friend' keyword)
void friendFunc(const MyClass& obj) {
    // Can access private members!
    cout << obj.privateData;
}

// Usage
MyClass obj;
friendFunc(obj);  // Called like regular function
```

A friend can also be **defined inside the class body**. It is still a non-member (no `this`, no `obj.friendFunc()`); it is only found through argument-dependent lookup ("hidden friend"):
```cpp
class MyClass {
    int privateData = 0;
    friend void friendFunc(const MyClass& obj) { std::cout << obj.privateData; }
};
```
Writing `friend` on a definition *outside* the class is an error: *'friend' used outside of class* (tested).

### Friend Class
```cpp
class MyClass {
private:
    int privateData;
    
public:
    // Entire FriendClass is friend
    friend class FriendClass;
};

class FriendClass {
public:
    void accessMyClass(MyClass& obj) {
        // All members of FriendClass can access MyClass private
        obj.privateData = 10;
    }
};
```

### Friend Member Function
```cpp
class MyClass;  // Forward declaration

class OtherClass {
public:
    void specificFunc(MyClass& obj);
};

class MyClass {
private:
    int privateData;
    
public:
    // Only this specific function is friend
    friend void OtherClass::specificFunc(MyClass& obj);
};

void OtherClass::specificFunc(MyClass& obj) {
    // This function can access MyClass private
    obj.privateData = 10;
}
```
`OtherClass` must be a **complete** type when `MyClass` names its member, so it has to be defined first. The forward declaration of `MyClass` is only there so `OtherClass` can mention it in a parameter. Only the *definition* of `specificFunc` has to wait until `MyClass` is complete.

---

## 🔍 Friend vs Member Function

### Comparison Table
```
┌─────────────────────────┬─────────────────────────┐
│ MEMBER FUNCTION         │ FRIEND FUNCTION         │
├─────────────────────────┼─────────────────────────┤
│ Part of class           │ Not part of class       │
│ Has 'this' pointer      │ No 'this' pointer       │
│ obj.func()              │ func(obj)               │
│ Access members directly │ Access via parameter    │
│ One object implicit     │ All objects explicit    │
│ Asymmetric             │ Can be symmetric        │
│ Inside class scope      │ Outside class scope     │
└─────────────────────────┴─────────────────────────┘
```

### Code Comparison
```cpp
class Point {
    double x, y;
    
public:
    // MEMBER FUNCTION
    double distanceFromOrigin() const {
        // Has 'this' pointer
        // Access x, y directly
        return sqrt(x * x + y * y);
    }
    
    // FRIEND FUNCTION (declared)
    friend double distanceBetween(const Point& p1, const Point& p2);
};

// FRIEND FUNCTION (defined)
double distanceBetween(const Point& p1, const Point& p2) {
    // No 'this' pointer
    // Access via p1, p2
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

// USAGE
Point p1, p2;
p1.distanceFromOrigin();      // Member (asymmetric)
distanceBetween(p1, p2);      // Friend (symmetric)
```

---

## 📋 Examples in This Folder

### Example 1: Basic Friend Function
Understanding friend function fundamentals.
- Not a member, no `this`, called like a regular function
- Can read the private data directly

### Example 2: Friend vs Member Function
Clear comparison of both approaches.
- `point.distanceFromOrigin()` is asymmetric (one object is implicit)
- `distanceBetween(p1, p2)` treats both points equally

### Example 3: Friend for Operator Overloading
Stream operators and symmetric operations.
- `<<`/`>>` need the stream on the left, so they must be non-member
- `2.0 * c1` needs a non-member too
- This is the one example that keeps `std::cout`: our own `operator<<` is the topic. It also reads a `Complex` from an `istringstream`

### Example 4: Friend Class
Granting access to entire class.
- ALL members of `Car` can access `Engine`'s private members
- For tightly coupled classes
- Friendship is NOT mutual

### Example 5: Friend Member Function
Granting access to specific function.
- Only ONE specific function is friend (finer control than a friend class)
- Needs the other class to be complete, plus a forward declaration of the first one

### Example 6: Friendship is NOT Mutual
If A friends B, B doesn't friend A.
- B can access A's private members, but A cannot access B's
- Grant it in both directions if both need it

### Example 7: Friendship is NOT Transitive
A friends B, B friends C ≠ A friends C.
- Alpha friends Beta, Beta friends Gamma, Gamma is NOT a friend of Alpha
- Each friendship must be explicit

### Example 8: Matrix and Vector Multiplication
Real-world use case with two classes.
- `Vector` is returned by value, so it has a deep-copy constructor now (see the Rule-of-Three note)
- Copy assignment is deleted in both classes: nothing here assigns

### Example 9: When to Use Friend
Good and bad use cases.
- A friend transfer between two accounts, then the same transfer through the public API only (`Account::transferViaMethods`)
- Shows that the public API was enough here

### Example 10: Testing with Friend
Using friend for unit testing.
- A `StackTester` reads capacity and top index to verify invariants
- White-box testing; see the note on whether to do this

---

## 🎓 Key Concepts

### The Three Types of Friends

#### 1. Friend Function
```cpp
class MyClass {
    int data;
    friend void func(const MyClass& obj);
};

void func(const MyClass& obj) {
    cout << obj.data;  // Access private
}
```

#### 2. Friend Class
```cpp
class MyClass {
    int data;
    friend class FriendClass;  // ALL members are friends
};

class FriendClass {
    void func1(MyClass& obj) { obj.data = 1; }  // ✅
    void func2(MyClass& obj) { obj.data = 2; }  // ✅
    void func3(MyClass& obj) { obj.data = 3; }  // ✅
};
```

#### 3. Friend Member Function
```cpp
class MyClass;

class OtherClass {
public:
    void friendFunc(MyClass& obj);      // Only this
    void notFriendFunc(MyClass& obj);   // Not this
};

class MyClass {
    int data;
    friend void OtherClass::friendFunc(MyClass& obj);
};
```

### Friendship Rules

#### Rule 1: Friendship is NOT Mutual
```cpp
class A {
    int secretA;
    friend class B;  // B can access A
};

class B {
    int secretB;
    // A CANNOT access B!
    
    void accessA(A& a) {
        a.secretA = 10;  // ✅ Works
    }
};

// (pseudo-code: A has no member tryAccessB in the real example)
void A::tryAccessB(B& b) {
    // b.secretB = 10;  // ❌ Error!
}
```

**Conclusion:** Friendship must be granted in BOTH directions if needed.

#### Rule 2: Friendship is NOT Inherited
```cpp
class Base {
    int secretBase;
    friend class Friend;
};

class Derived : public Base {
    int secretDerived;
    // Friend is NOT automatically friend of Derived!
};

class Friend {
    void func(Base& b) {
        b.secretBase = 10;      // ✅ Works
    }
    
    void func(Derived& d) {
        d.secretBase = 10;      // ✅ Works: the member belongs to Base, and Friend is Base's friend
        // d.secretDerived = 10; // ❌ Error! Not friend of Derived
        //                          ('int Derived::secretDerived' is private within this context)
    }
};
```

**Conclusion:** Friendship must be granted at each class level.

#### Rule 3: Friendship is NOT Transitive
```cpp
class A {
    int secretA;
    friend class B;  // A friends B
};

class B {
    int secretB;
    friend class C;  // B friends C
};

class C {
    void func(A& a) {
        // a.secretA = 10;  // ❌ Error! C is NOT friend of A
    }
    
    void func(B& b) {
        b.secretB = 10;     // ✅ Works
    }
};
```

**Conclusion:** A friends B, B friends C does NOT mean A friends C.

---

## 💡 When to Use Friend

### ✅ Good Use Cases

#### 1. Operator Overloading (Stream Operators)

**Stream operators MUST be non-member. They are friends only when they need private access:**
```cpp
class Complex {
    double real, imag;
    
public:
    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << c.real << " + " << c.imag << "i";
        return os;
    }
};

// Natural syntax:
Complex c(3, 4);
cout << c;  // Works perfectly!
```

**Why friend?**
- Must be non-member (left operand is `ostream`), and that is the *only* hard requirement
- `friend` is needed only because this one reads `real`/`imag` directly
- With public getters, a plain non-friend `operator<<` works too (tested)
- The in-class definition above is a "hidden friend": it can only be found through ADL, which keeps the global namespace clean

#### 2. Symmetric Binary Operators
```cpp
class Vector {
    double x, y;
    
public:
    // Member: v * 2
    Vector operator*(double scalar) const {
        return Vector(x * scalar, y * scalar);
    }
    
    // Non-member: 2 * v (symmetry!). Declared as friend here, but it only calls the
    // public operator*, so it does not NEED friendship, only to be a non-member.
    friend Vector operator*(double scalar, const Vector& v) {
        return v * scalar;
    }
};

Vector v;
v * 2;  // Works (member)
2 * v;  // Works (non-member)
```
Tested: the same `operator*(double, const Vector&)` written as an ordinary free function (no `friend`) compiles and works. Use `friend` only if it reads `x`/`y` directly.

#### 3. Tightly Coupled Classes
```cpp
class Engine {
    int horsepower;
    friend class Car;  // Car and Engine tightly coupled
};

class Car {
    Engine engine;
    
    void tune() {
        engine.horsepower += 50;  // Direct access
    }
};
```

#### 4. Testing and Debugging
```cpp
class Stack {
    int* data;
    int top;
    friend class StackTester;  // For testing
};

class StackTester {
public:
    static void verifyInvariants(const Stack& s) {
        assert(s.top >= -1);
        assert(s.top < s.capacity);
    }
};
```

### ❌ Bad Use Cases

#### 1. When Public Interface is Sufficient
```cpp
// ❌ BAD - Unnecessary friend
class Bad {
    int value;
    friend void setValue(Bad& obj, int v);
};

void setValue(Bad& obj, int v) {
    obj.value = v;
}

// ✅ GOOD - Use public interface
class Good {
    int value;
public:
    void setValue(int v) { value = v; }
};
```

#### 2. Lazy Design
```cpp
// ❌ BAD - Making everything friend
class Bad {
    int data1, data2, data3;
    
    friend class A;
    friend class B;
    friend class C;
    friend class D;
    // ... defeats purpose of encapsulation!
};
```

#### 3. Breaking Encapsulation Unnecessarily
```cpp
// ❌ BAD - Exposing internals
class Bad {
    vector<int> internalBuffer;
    friend void directlyModifyBuffer(Bad& obj);
};

// ✅ GOOD - Provide controlled interface
class Good {
    vector<int> internalBuffer;
public:
    void addElement(int x) { 
        // Validation, invariants maintained
        internalBuffer.push_back(x);
    }
};
```

---

## 💡 Operator Overloading with Friend

### Why Some Operators Need Friend

**Problem:**
```cpp
class Vector {
    double x, y;
public:
    // Member function - v * 2 works
    Vector operator*(double s) const;
};

Vector v;
v * 2;    // ✅ Works (v.operator*(2))
2 * v;    // ❌ Error! (2.operator*(v) - doesn't exist)
```

**Solution: a non-member (friend only if it needs private data):**
```cpp
class Vector {
    double x, y;
public:
    // Member: v * 2
    Vector operator*(double s) const {
        return Vector(x * s, y * s);
    }
    
    // Non-member: 2 * v
    friend Vector operator*(double s, const Vector& v) {
        return v * s;  // Reuse member operator, so no private access is needed
    }
};

Vector v;
v * 2;    // ✅ Works
2 * v;    // ✅ Works now!
```

### Stream Operators (MUST be non-member, friend only if they read private data)
```cpp
class MyClass {
    int data;
    
public:
    // Output operator
    friend ostream& operator<<(ostream& os, const MyClass& obj) {
        os << obj.data;
        return os;
    }
    
    // Input operator
    friend istream& operator>>(istream& is, MyClass& obj) {
        is >> obj.data;
        return is;
    }
};

// Usage
MyClass obj;
cout << obj;  // Natural!
cin >> obj;   // Natural!
```

**Why MUST they be non-member?**
- Left operand is `ostream`/`istream`, not our class
- Can't add member function to `ostream`
- Must be non-member for natural syntax: `cout << obj`

**Why friend, then?** Only because the example above touches `obj.data`. "Non-member" and "friend" are two separate decisions (tested: a non-friend `operator<<` that uses a public getter compiles and works).

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Repeating 'friend' in Definition
```cpp
class MyClass {
    friend void func(const MyClass& obj);
};

// ❌ WRONG
friend void func(const MyClass& obj) { }

// ✅ CORRECT
void func(const MyClass& obj) { }
```
(Error from GCC: *'friend' used outside of class*, tested. A definition written *inside* the class body does carry `friend`, that is the hidden-friend form.)

### ❌ Mistake 2: Assuming Mutual Friendship
```cpp
class A {
    int dataA;
    friend class B;  // B can access A
};

class B {
    int dataB;
    
    void accessA(A& a) {
        a.dataA = 10;  // ✅ Works
    }
};

// Assuming A can access B (pseudo-code: this redefines class A, which is not legal C++;
// in real code the method would be declared in the first class A)
class A {
    void accessB(B& b) {
        // b.dataB = 10;  // ❌ Error! A is NOT friend of B
    }
};
```

**Fix:** Grant friendship in both directions if needed:
```cpp
class A {
    int dataA;
    friend class B;
};

class B {
    int dataB;
    friend class A;  // Now mutual
};
```

### ❌ Mistake 3: Wrong Order (Friend Member Function)
```cpp
// ❌ WRONG - MyClass not yet defined
class OtherClass {
    void func(MyClass& obj);
};

class MyClass {
    friend void OtherClass::func(MyClass& obj);  // Error!
};
```

**Fix:** Define `OtherClass` first, with a forward declaration of `MyClass` for its parameter. A forward declaration of `OtherClass` is *not* enough: `friend void B::func(A&)` with `B` only forward-declared gives *invalid use of incomplete type* (tested).
```cpp
// ✅ CORRECT
class MyClass;  // Forward declare

class OtherClass {
    void func(MyClass& obj);
};

class MyClass {
    int data;
    friend void OtherClass::func(MyClass& obj);  // ✅
};

// Define after MyClass is complete
void OtherClass::func(MyClass& obj) {
    obj.data = 10;
}
```

### ❌ Mistake 4: Overusing Friend
```cpp
// ❌ BAD - Everything is friend!
class Bad {
    int data;
    
    friend class A;
    friend class B;
    friend class C;
    friend void func1();
    friend void func2();
    friend void func3();
    // Encapsulation completely broken!
};
```

**Fix:** Use friend sparingly, prefer public interface:
```cpp
// ✅ GOOD
class Good {
    int data;
    
public:
    int getData() const { return data; }
    void setData(int d) { data = d; }
    
    // Only truly necessary friends
    friend ostream& operator<<(ostream& os, const Good& obj);
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Complex Number Class
Create a `Complex` class with:
- Friend operators: +, -, *, /
- Friend stream operators: <<, >>
- Test all operations

### Exercise 2: Fraction Class
Create a `Fraction` class with:
- Friend comparison operators
- Friend arithmetic operators
- Automatic simplification

### Exercise 3: Matrix Class
Create `Matrix` and `Vector` classes with:
- Friend operator for matrix * vector
- Friend operator for vector * matrix
- Display operators
- Hint: if they own memory with `new[]`, follow the Rule of Three (see the note below)

### Exercise 4: Linked List
Create `Node` and `LinkedList` classes with:
- LinkedList as friend of Node
- Friend iterator class
- Test insertion and traversal

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o friend_functions
```

(`main.cpp` uses `std::println` from `<print>`, so it needs GCC 14+ (or another compiler with C++23 `<print>`).)

### Run:
```bash
./friend_functions
```

### Expected Output:
The program demonstrates:
1. Basic friend function
2. Friend vs member comparison
3. Friend for operators
4. Friend class
5. Friend member function
6. Non-mutual friendship
7. Non-transitive friendship
8. Matrix-vector multiplication
9. When to use friend
10. Testing with friend

---

## 📊 Visual Concepts

### Access Control Comparison
```
REGULAR FUNCTION:
┌──────────────┐
│  MyClass     │
│ ┌──────────┐ │
│ │ private  │ │ ◄─── Cannot access ❌
│ └──────────┘ │
│ ┌──────────┐ │
│ │ public   │ │ ◄─── Can access ✅
│ └──────────┘ │
└──────────────┘

FRIEND FUNCTION:
┌──────────────┐
│  MyClass     │
│ ┌──────────┐ │
│ │ private  │ │ ◄─── Can access ✅
│ └──────────┘ │
│ ┌──────────┐ │
│ │ public   │ │ ◄─── Can access ✅
│ └──────────┘ │
└──────────────┘
```

### Friendship Flow
```
Declaration (inside class):
┌────────────────────────┐
│ class MyClass {        │
│   friend void func();  │ ◄─── Grant access
│ };                     │
└────────────────────────┘
           │
           ▼
Definition (outside class):
┌────────────────────────┐
│ void func() {          │
│   // Access private    │ ◄─── Use access
│ }                      │
└────────────────────────┘
```

---

## 📚 Related Topics

### Prerequisites:
- **04_MemberFunctions** - Member functions
- **05_Encapsulation** - Private members
- **09_OperatorOverloading** - Operator basics

### Coming Next:
- **13_Inheritance** - Friend in inheritance
- **16_Polymorphism** - Friend with virtual
- Friend templates (a later topic)

### Related Concepts:
- **Encapsulation** - Friend breaks it
- **Access Control** - Friend bypasses it
- **Operator Overloading** - Often needs friend

---

## 🎓 Summary

### What We Learned:

✅ **Friend functions** can access private members
✅ **Not member functions** - no `this` pointer
✅ **Declared with `friend`** inside class
✅ **Defined without `friend`** outside class
✅ **Three types**: function, class, member function
✅ **NOT mutual** - must grant both ways
✅ **NOT inherited** - each class grants separately
✅ **NOT transitive** - no chain effect
✅ **Common for** stream operators (they must be non-member; friend only if they read private data)
✅ **Use sparingly** - breaks encapsulation

### The Complete Pattern:
```cpp
class MyClass {
private:
    int privateData;
    
public:
    // Friend function
    friend void friendFunc(const MyClass& obj);
    
    // Friend class
    friend class FriendClass;
    
    // Friend member function
    friend void OtherClass::specificFunc(MyClass& obj);
    
    // Friend operator
    friend ostream& operator<<(ostream& os, const MyClass& obj);
};

// Define friend function (NO 'friend' keyword)
void friendFunc(const MyClass& obj) {
    cout << obj.privateData;  // Can access private
}

ostream& operator<<(ostream& os, const MyClass& obj) {
    os << obj.privateData;
    return os;
}

// Usage
MyClass obj;
friendFunc(obj);  // Friend function
cout << obj;      // Friend operator
```

---

## ✨ Quick Reference Card
```cpp
// FRIEND FUNCTION
class MyClass {
    int data;
    friend void func(const MyClass& obj);
};
void func(const MyClass& obj) { cout << obj.data; }

// FRIEND CLASS
class MyClass {
    int data;
    friend class FriendClass;
};

// FRIEND MEMBER FUNCTION
class MyClass;
class Other {
    void func(MyClass& obj);
};
class MyClass {
    int data;
    friend void Other::func(MyClass& obj);
};

// FRIEND OPERATOR
class MyClass {
    int data;
    friend ostream& operator<<(ostream& os, const MyClass& obj);
};
ostream& operator<<(ostream& os, const MyClass& obj) {
    return os << obj.data;
}

// USAGE
MyClass obj;
func(obj);    // Friend function
cout << obj;  // Friend operator
```

---

## 🔍 Best Practices

### ✅ DO:

- **Make stream operators (<<, >>) non-member**; use friend if they read private data
- **Make symmetric binary operators non-member**; use friend only if they read private data
- **Document** why friendship is needed
- **Keep friend count** minimal
- **Use friend** for tightly coupled classes
- **Grant friendship explicitly** where needed
- **Think twice** before using friend for testing access (see notes)
- **Prefer public interface** when possible

### ❌ DON'T:

- **Overuse friend** - breaks encapsulation
- **Make everything friend** - defeats purpose
- **Repeat 'friend'** in function definition
- **Assume mutual** friendship
- **Assume inherited** friendship
- **Assume transitive** friendship
- **Use friend** as lazy design
- **Forget forward declarations** for friend members

---

## 🌟 Real-World Applications

### Stream I/O
```cpp
class Date {
    int day, month, year;
    friend ostream& operator<<(ostream& os, const Date& d);
};
```

### Coupled Classes
```cpp
class Iterator {
    friend class Container;
};
class Container {
    friend class Iterator;
};
```

### Mathematical Operations
```cpp
class Matrix {
    friend Vector operator*(const Matrix& m, const Vector& v);
};
```

### Testing Frameworks
```cpp
class MyClass {
    friend class MyClassTest;
};
```

---

## 📖 Further Reading

- [cppreference.com - Friend declaration](https://en.cppreference.com/cpp/language/friend)
- "Effective C++" (3rd ed.) by Scott Meyers - Item 23 "Prefer non-member non-friend functions to member functions" and Item 46 (non-member functions for type conversions)
- "C++ Primer" by Lippman - Chapter on Friends
- [C++ Core Guidelines - Friend](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Friend functions **access private members**
2. **Not member functions** - no `this`
3. Declared with **`friend`** inside class
4. Defined **without `friend`** outside class
5. Friendship is **NOT mutual**
6. Friendship is **NOT inherited**
7. Friendship is **NOT transitive**
8. **Common** for stream operators (non-member is required, friend only if they need private data)
9. **Use sparingly** - breaks encapsulation
10. **Must be explicitly granted**

---

**Previous Topic:** [11_StaticMembers](../11_StaticMembers/) - Static Members

**Next Topic:** [13_Inheritance](../13_Inheritance/) - Class Inheritance (or continue with other topics)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: 05_Encapsulation, 09_OperatorOverloading*
*Use Wisely - Breaks Encapsulation*

---

## 🧾 Notes moved from main.cpp

The long lecture output at the end of `main()` is gone (it repeated this README). What it said that is not already above:

- **Three kinds of friend:** function (standalone), class (all members), member function (one specific function).
- **Performance is not a reason.** The old output claimed direct access is faster than getters. A trivial getter is inlined by the compiler, so there is normally no difference (the claim was not benchmarked here). Use friend for design reasons, not speed.
- **Tests with friend:** `friend class StackTester` gives white-box access, but many teams prefer testing the public behaviour, because tests that read internals break whenever the internals change. Test frameworks provide a helper for it (GoogleTest has `FRIEND_TEST`, not tested here).
- **Rule of Three in Example 8:** `Vector` owns a `new double[]`, and `operator*` returns it by value. Without a copy constructor, two objects point to the same array and the destructor frees it twice. Tested: the original code crashes with AddressSanitizer (*heap-use-after-free*) when built with `-fno-elide-constructors`. A normal build gets away with it only because the compiler elides the copy (NRVO). The fixed code adds a deep-copy constructor and deletes the assignment.
- **`transferViaMethods` in Example 9** used to be a friend-style direct access under a "public interface" name. It now uses only `deposit()` so the comparison is honest.
- **Unused parameter** in `Gamma::tryAccessAlpha` is commented out, so `-Wall -Wextra` is clean.
- **Don't mix up two ideas:** *non-member* (required for `<<` and `2 * v`) and *friend* (required only for private access).
