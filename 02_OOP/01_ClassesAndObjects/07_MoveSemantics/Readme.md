# 08_MoveSemantics - Efficient Resource Transfer (C++11)

## 📚 Overview

This folder introduces **Move Semantics** - a powerful C++11 feature that allows efficient transfer of resources from one object to another without expensive copying. Move semantics dramatically improves performance for classes managing dynamic memory, file handles, or other resources.

**Move Semantics** = Transfer ownership instead of copying

This is one of the most important features introduced in C++11 and is essential for modern C++ programming.

---

## 🎯 What You'll Learn

- What move semantics are and why they're needed
- Lvalues vs rvalues and rvalue references (&&)
- Move constructors
- Move assignment operators
- The Rule of Five
- `std::move` and when to use it
- When moves happen automatically
- `noexcept` and why it matters
- Performance benefits
- Real-world examples and best practices

---

## 📖 Concept Explanation

### What are Move Semantics?

**Move Semantics** allow an object to **transfer ownership** of its resources to another object instead of copying them.

**The Problem (Before C++11):**
```cpp
vector<int> v1(1000000);  // 1 million elements
vector<int> v2 = v1;      // Copies all 1 million elements! 😢
```

**The Solution (C++11+):**
```cpp
vector<int> v1(1000000);       // 1 million elements
vector<int> v2 = std::move(v1); // Just steals pointer! 😊
// v1 is now empty, v2 owns the data
```

### Simple Analogy
```
COPY (traditional):
  You: "Here's my book"
  Friend: *photocopies entire book*
  Result: Two books, expensive

MOVE (modern):
  You: "Here's my book, I'm done with it"
  Friend: *takes your book*
  Result: One book, just changed hands, cheap
```

---

## 💻 Basic Syntax

### Minimal Move Constructor
```cpp
class MyClass {
private:
    int* data;
    int size;
    
public:
    // Move Constructor
    MyClass(MyClass&& other) noexcept 
        : data(other.data), size(other.size) {
        
        // Steal resources (done in initializer list)
        
        // Leave source empty
        other.data = nullptr;
        other.size = 0;
    }
};
```

### Minimal Move Assignment
```cpp
class MyClass {
private:
    int* data;
    int size;
    
public:
    // Move Assignment Operator
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            // Clean up current resources
            delete[] data;
            
            // Steal resources
            data = other.data;
            size = other.size;
            
            // Leave source empty
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

### Usage
```cpp
MyClass obj1(1000);
MyClass obj2 = std::move(obj1);  // Move constructor
MyClass obj3(500);
obj3 = std::move(obj2);          // Move assignment
```

---

## 🔍 Lvalue vs Rvalue

Understanding lvalues and rvalues is crucial for move semantics.

### Lvalues

**Lvalue** = "Left value" - can appear on the left side of assignment

**Characteristics:**
- Has a name
- Persists beyond a single expression
- Can take its address with `&`
- Has identity

**Examples:**
```cpp
int x = 10;          // x is lvalue
int& ref = x;        // ref is lvalue reference
int* ptr = &x;       // Can take address of lvalue

x = 20;              // x on left side ✅
```

### Rvalues

**Rvalue** = "Right value" - temporary, about to be destroyed

**Characteristics:**
- No name (or about to die)
- Temporary, expires at end of expression
- Cannot take its address
- No persistent identity

**Examples:**
```cpp
int x = 42;          // 42 is rvalue
int y = x + 5;       // (x + 5) is rvalue
int z = func();      // return value is rvalue

42 = x;              // ❌ Error! Can't assign to rvalue
&(x + y);            // ❌ Error! Can't take address
```

### References
```cpp
// Lvalue reference (traditional)
int x = 10;
int& lref = x;       // ✅ Binds to lvalue
int& lref2 = 42;     // ❌ Error! Can't bind to rvalue

// Rvalue reference (C++11)
int&& rref = 42;     // ✅ Binds to rvalue
int&& rref2 = x + 5; // ✅ Binds to temporary

// Const lvalue reference (can bind to both)
const int& cref1 = x;      // ✅ Binds to lvalue
const int& cref2 = 42;     // ✅ Binds to rvalue
```

### Visual Representation
```
LVALUES:                 RVALUES:
┌────────────┐          ┌────────────┐
│  int x;    │          │    42      │
│  string s; │          │  x + y     │
│  arr[0]    │          │  func()    │
│  *ptr      │          │  MyClass() │
└────────────┘          └────────────┘
     │                       │
     ▼                       ▼
Has persistent          Temporary,
identity                about to die
```

---

## 📋 Examples in This Folder

### Example 1: The Problem - Expensive Copies
Shows why we need move semantics (copying large objects is slow).

### Example 2: Move Constructor Basics
First implementation of move constructor.

### Example 3: Lvalue vs Rvalue
Detailed explanation of value categories.

### Example 4: Move Assignment Operator
Implementing move assignment.

### Example 5: The Rule of Five
All five special member functions together.

### Example 6: std::move Explained
How `std::move` works and when to use it.

### Example 7: Automatic Moves
When compiler moves automatically without `std::move`.

### Example 8: Copy vs Move Performance
Performance comparison with numbers.

### Example 9: String Class
Real-world string implementation with move support.

### Example 10: Unique Pointer
Move-only type (cannot be copied, only moved).

---

## 🎓 Key Concepts

### The Rule of Five (C++11+)

If you define **ONE** of these, define **ALL FIVE**:
```cpp
class MyClass {
public:
    // 1. Destructor
    ~MyClass();
    
    // 2. Copy Constructor
    MyClass(const MyClass& other);
    
    // 3. Copy Assignment Operator
    MyClass& operator=(const MyClass& other);
    
    // 4. Move Constructor (NEW in C++11)
    MyClass(MyClass&& other) noexcept;
    
    // 5. Move Assignment Operator (NEW in C++11)
    MyClass& operator=(MyClass&& other) noexcept;
};
```

**Or** use `= default` or `= delete` for all:
```cpp
class MyClass {
public:
    ~MyClass() = default;
    MyClass(const MyClass&) = default;
    MyClass& operator=(const MyClass&) = default;
    MyClass(MyClass&&) = default;
    MyClass& operator=(MyClass&&) = default;
};
```

### std::move

**`std::move` doesn't actually move anything!**

It's just a **cast** from lvalue to rvalue reference:
```cpp
template<typename T>
typename remove_reference<T>::type&& move(T&& arg) {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}
```

**What it does:**
```cpp
MyClass a;               // a is lvalue
MyClass b = a;           // Copy constructor (a is lvalue)
MyClass c = std::move(a); // Move constructor (std::move makes it rvalue)
```

**After `std::move`:**
- Source object is in **valid but unspecified state**
- Don't use it except to:
  - Destroy it
  - Assign a new value to it
```cpp
MyClass a;
MyClass b = std::move(a);

// ❌ DON'T:
a.doSomething();  // Undefined behavior!

// ✅ DO:
a = MyClass();    // OK - assign new value
// a destroyed     // OK - destructor runs
```

### noexcept

**Move operations should be `noexcept`!**
```cpp
MyClass(MyClass&& other) noexcept { }
MyClass& operator=(MyClass&& other) noexcept { }
```

**Why it matters:**

1. **STL containers check for `noexcept`:**
```cpp
   vector<MyClass> vec;
   vec.push_back(obj);
   // If move is noexcept → use move
   // If move can throw → use copy (safer!)
```

2. **Performance:**
   - `noexcept` moves are faster
   - No need for exception handling overhead

3. **Strong exception guarantee:**
   - If move can't fail, operations are safer

**When not to use `noexcept`:**
- If move operations can throw exceptions
- Better to omit than to lie about it

### When Moves Happen Automatically

The compiler automatically uses move in these situations:

#### 1. Returning Local Objects
```cpp
MyClass func() {
    MyClass temp;
    return temp;  // Automatic move (or RVO)
}

MyClass obj = func();  // No explicit std::move needed!
```

#### 2. Initializing from Temporary
```cpp
MyClass obj = MyClass();  // Temporary → automatic move
```

#### 3. Container Operations
```cpp
vector<MyClass> vec;
vec.push_back(MyClass());  // Temporary → move into vector
```

#### 4. Return Statements
```cpp
MyClass func(bool condition) {
    MyClass a, b;
    if (condition)
        return a;  // Automatic move (a is local)
    else
        return b;  // Automatic move (b is local)
}
```

**Important:** Don't use `std::move` on return values - it prevents Return Value Optimization (RVO)!
```cpp
// ❌ BAD - prevents RVO
MyClass func() {
    MyClass temp;
    return std::move(temp);  // DON'T DO THIS!
}

// ✅ GOOD - allows RVO or automatic move
MyClass func() {
    MyClass temp;
    return temp;  // Compiler optimizes
}
```

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Using Moved-From Object
```cpp
MyClass a;
MyClass b = std::move(a);
a.doSomething();  // ❌ Undefined behavior!
```

**Fix:**
```cpp
MyClass a;
MyClass b = std::move(a);
// Don't use 'a' anymore!
// Except to destroy or assign:
a = MyClass();  // ✅ OK - assign new value
```

### ❌ Mistake 2: Missing noexcept
```cpp
class Bad {
public:
    Bad(Bad&& other) {  // ❌ Missing noexcept
        // STL containers won't use this!
    }
};
```

**Fix:**
```cpp
class Good {
public:
    Good(Good&& other) noexcept {  // ✅ noexcept
        // STL will use this for moves
    }
};
```

### ❌ Mistake 3: std::move on Return Value
```cpp
MyClass func() {
    MyClass temp;
    return std::move(temp);  // ❌ Prevents RVO!
}
```

**Fix:**
```cpp
MyClass func() {
    MyClass temp;
    return temp;  // ✅ Compiler optimizes
}
```

### ❌ Mistake 4: Moving const Objects
```cpp
const MyClass a;
MyClass b = std::move(a);  // ❌ Won't move, will COPY!
```

**Explanation:** Can't move from `const` because moving modifies the source!

### ❌ Mistake 5: Not Leaving Source Valid
```cpp
class Bad {
public:
    Bad(Bad&& other) noexcept 
        : data(other.data), size(other.size) {
        // ❌ Didn't nullify source pointers!
        // Destructor will delete same memory twice!
    }
};
```

**Fix:**
```cpp
class Good {
public:
    Good(Good&& other) noexcept 
        : data(other.data), size(other.size) {
        other.data = nullptr;  // ✅ Leave source valid
        other.size = 0;
    }
};
```

---

## 🎯 Practice Exercises

### Exercise 1: String Class
Implement a `String` class with:
- Dynamic char buffer
- All five special member functions
- Move constructor and assignment
- Proper resource management

### Exercise 2: Vector Class
Create a `Vector` class with:
- Dynamic array
- Copy and move operations
- push_back that uses move when possible
- Verify moves are used for temporaries

### Exercise 3: Unique Pointer
Implement a `UniquePtr` template:
- Move-only (delete copy operations)
- Transfer ownership on move
- Automatic deletion
- Similar to `std::unique_ptr`

### Exercise 4: Matrix Class
Create a `Matrix` class with:
- 2D dynamic array
- Move operations for efficient return
- Verify performance improvement

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 move_semantics.cpp -o move_semantics
```

### Run:
```bash
./move_semantics
```

### Expected Output:
The program demonstrates:
1. The problem with expensive copies
2. Move constructor basics
3. Lvalue vs rvalue concepts
4. Move assignment operator
5. Rule of Five
6. std::move explained
7. Automatic moves
8. Performance comparison
9. String class with moves
10. Unique pointer (move-only)

---

## 📊 Visual Concept

### Copy vs Move
```
COPY OPERATION:
═══════════════

Source:                Destination:
┌──────────┐          ┌──────────┐
│  obj1    │          │  obj2    │
│          │          │          │
│ data ────┼──► [100] │ data ────┼──► [100]
└──────────┘  elements└──────────┘  elements
                                      (copied)
Cost: O(n) - expensive!


MOVE OPERATION:
═══════════════

Source:                Destination:
┌──────────┐          ┌──────────┐
│  obj1    │          │  obj2    │
│          │          │          │
│ data:null│          │ data ────┼──► [100]
└──────────┘          └──────────┘  elements
  (empty)                            (stolen)

Cost: O(1) - cheap!
```

### std::move Effect
```
BEFORE std::move:
┌─────────┐
│   obj   │  (lvalue)
└─────────┘

AFTER std::move(obj):
┌─────────┐
│   obj   │  (rvalue)
└─────────┘
     │
     ▼
Can now be moved from!
```

---

## 🔗 Performance Benefits

### Copy vs Move Performance
```cpp
// Large object with 1 million integers
class LargeObject {
    int* data;  // 1,000,000 integers
    // ...
};

// COPY: O(n)
LargeObject a;
LargeObject b = a;
// Cost: Allocate 4MB + copy 1M ints ≈ 1,000,000 operations

// MOVE: O(1)
LargeObject c;
LargeObject d = std::move(c);
// Cost: Copy pointer (8 bytes) ≈ 1 operation

// Speedup: ~1,000,000x faster!
```

### Real-World Impact
```cpp
// Returning large objects efficiently
vector<int> createLargeVector() {
    vector<int> v(1000000);
    // ... fill vector ...
    return v;  // Move, not copy!
}

auto result = createLargeVector();  // Fast! ✅
```

---

## 📚 Related Topics

### Prerequisites:
- **07_CopyConstructor** - Understanding copying
- **02_Constructor** - Constructors basics
- **03_Destructor** - Resource cleanup

### Coming Next:
- **09_OperatorOverloading** - Custom operators
- **12_SmartPointers** - unique_ptr, shared_ptr (use move semantics)
- **14_Templates** - Generic programming

### Related Concepts:
- **Perfect Forwarding** - `std::forward`
- **Universal References** - `T&&` in templates
- **RVO/NRVO** - Return Value Optimization

---

## 🎓 Summary

### What We Learned:

✅ **Move semantics** = Transfer ownership, not copy
✅ **Rvalue references** (`&&`) enable moves
✅ **Move constructor** steals resources
✅ **Move assignment** cleans up, then steals
✅ **Rule of Five** - add move ops to Rule of Three
✅ **std::move** casts lvalue to rvalue
✅ **noexcept** essential for STL optimization
✅ **Automatic moves** for temporaries
✅ **Don't use** moved-from objects
✅ **Much faster** than copying

### The Complete Pattern:
```cpp
class MyClass {
    int* data;
    int size;
    
public:
    // Constructor
    MyClass(int s) : size(s) {
        data = new int[size];
    }
    
    // Destructor
    ~MyClass() {
        delete[] data;
    }
    
    // Copy constructor
    MyClass(const MyClass& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }
    
    // Copy assignment
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }
    
    // Move constructor
    MyClass(MyClass&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    
    // Move assignment
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

### When to Use:

**Define move operations when:**
- Class manages resources (memory, files, etc.)
- Objects are large/expensive to copy
- Want maximum performance
- Following Rule of Five

**Use std::move when:**
- Done with an object and want to transfer it
- Returning by value (but let compiler optimize first!)
- Passing to functions that take ownership
- Working with containers of move-only types

---

## ✨ Quick Reference Card
```cpp
// MOVE CONSTRUCTOR
MyClass(MyClass&& other) noexcept
    : member(other.member) {
    other.member = nullptr;
}

// MOVE ASSIGNMENT
MyClass& operator=(MyClass&& other) noexcept {
    if (this != &other) {
        delete resource;
        resource = other.resource;
        other.resource = nullptr;
    }
    return *this;
}

// USAGE
MyClass a;
MyClass b = std::move(a);     // Move construct
MyClass c;
c = std::move(b);             // Move assign

// AUTOMATIC MOVES
MyClass func() {
    MyClass temp;
    return temp;  // Automatic move
}

// RVALUE REFERENCES
int&& rref = 42;              // Binds to rvalue
MyClass&& rref2 = MyClass();  // Binds to temporary
```

---

## 🔍 Best Practices

### ✅ DO:

- **Implement move** for resource-managing classes
- **Mark noexcept** on move operations
- **Leave source valid** but empty after move
- **Use std::move** when done with lvalues
- **Follow Rule of Five**
- **Let compiler** auto-move temporaries
- **Test** that moves actually happen
- **Use move-only types** when appropriate (unique_ptr)

### ❌ DON'T:

- **Use moved-from objects** (except destroy/assign)
- **Forget noexcept** on move operations
- **std::move** on return values (prevents RVO)
- **Move const objects** (won't work)
- **std::move** unnecessarily
- **Assume move is free** (still has cost)
- **Break exception safety**
- **Leave moved-from in invalid state**

---

## 🌟 Real-World Applications

### Container Classes
```cpp
class Vector {
    int* data;
    size_t size;
public:
    Vector(Vector&& other) noexcept {
        // Fast transfer of large array
    }
};
```

### String Classes
```cpp
class String {
    char* buffer;
public:
    String(String&& other) noexcept {
        // Steal buffer instead of copying
    }
};
```

### Smart Pointers
```cpp
template<typename T>
class unique_ptr {
    T* ptr;
public:
    unique_ptr(unique_ptr&& other) noexcept {
        // Transfer ownership
        // Can't be copied!
    }
};
```

### File Handles
```cpp
class File {
    FILE* handle;
public:
    File(File&& other) noexcept {
        // Transfer file handle
        // Original loses access
    }
};
```

---

## 📖 Further Reading

- [cppreference.com - Move Constructor](https://en.cppreference.com/w/cpp/language/move_constructor)
- [cppreference.com - std::move](https://en.cppreference.com/w/cpp/utility/move)
- "Effective Modern C++" by Scott Meyers - Items 23-25
- [C++ Core Guidelines - Move Semantics](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Move semantics **transfer ownership** instead of copying
2. Rvalue references (`&&`) enable move semantics
3. Move constructor: `ClassName(ClassName&& other) noexcept`
4. Move assignment: `ClassName& operator=(ClassName&& other) noexcept`
5. **Always mark `noexcept`** for STL optimization
6. `std::move` **casts** lvalue to rvalue
7. **Don't use** moved-from objects
8. **Automatic** for temporaries and return values
9. **Rule of Five** extends Rule of Three
10. **Dramatically faster** than copying large objects

---

**Previous Topic:** [07_CopyConstructor](../07_CopyConstructor/) - Copying Objects

**Next Topic:** [09_OperatorOverloading](../09_OperatorOverloading/) - Custom Operators

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate to Advanced*
*Prerequisites: 07_CopyConstructor*
*C++11 Feature - Modern C++ Essential*