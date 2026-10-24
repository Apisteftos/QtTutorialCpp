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
// v2 owns the data. v1 is "valid but unspecified" (with GCC's vector: size 0)
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

**Lvalue** = historically "left value". The useful definition today: an expression with an **identity**, i.e. something you can take the address of.

**Characteristics:**
- Has a name
- Persists beyond a single expression
- Can take its address with `&`
- Has identity

(The "left of `=`" idea is only a rough hint: a `const` lvalue cannot be assigned to, see below.)

**Examples:**
```cpp
int x = 10;          // x is lvalue
int& ref = x;        // ref is lvalue reference
int* ptr = &x;       // Can take address of lvalue

x = 20;              // x on left side ✅

const int c = 1;
// c = 2;            // ❌ c is an lvalue too, but const: cannot be assigned
```

### Rvalues

**Rvalue** = historically "right value": a temporary, or something you have marked as "no longer needed" (`std::move`)

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

42 = x;              // ❌ Error! Can't assign to the int rvalue 42
&(x + y);            // ❌ Error! Can't take address
```

⚠️ "Rvalues can only be on the right of `=`" is **not** true for class types: `std::string("a") = "b";` compiles (tested), because that is just a call of `operator=` on a temporary. The reliable test is "does it have an identity / can I take its address?", not the side of the `=`.

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

### A named rvalue reference is an lvalue

```cpp
void whichRef(int&);    // overload 1
void whichRef(int&&);   // overload 2

int&& rr = 42;          // rr has TYPE "rvalue reference"...
whichRef(rr);           // ...but the EXPRESSION rr has a name, so it is an lvalue -> overload 1
whichRef(std::move(rr)); // -> overload 2
```
This is exactly why you need `std::move` (or `std::forward`) inside a function that takes `T&&`. Example 3 prints this (tested).

> C++11 actually has three basic categories: *lvalue*, *prvalue* (a pure temporary such as `42` or `x + y`) and *xvalue* (an "expiring" value, the result of `std::move(x)`). Together, xvalues and prvalues are "rvalues". Examples 3 and 6 show what you need in practice.

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
Copying a 1-million-element object (`ExpensiveClass`, no move constructor) allocates and copies 4 MB. Returning a local from a function does **not** copy here, because GCC applies NRVO. The standard does not guarantee that (see "Copy elision" below), and a move constructor is the safety net.

### Example 2: Move Constructor Basics
First move constructor. After `std::move(obj1)` the source is empty (`data == nullptr`) and the destination holds the same address.

### Example 3: Lvalue vs Rvalue
Two overloads, `whichRef(int&)` and `whichRef(int&&)`, show which one the compiler picks for `x`, `x + y`, `42`, `std::move(x)`, and for a named rvalue reference.

### Example 4: Move Assignment Operator
Copy assignment and move assignment in one class. Move assignment first frees the current data, then steals.

### Example 5: The Rule of Five
All five special member functions together, each printing its number.

### Example 6: std::move Explained
Copy vs move with a tracker class. The moved-from name shows `[moved]`.

### Example 7: Automatic Moves
Only `push_back(AutoMove("Vec1", 10))` really moves. Returning a local and initializing from a temporary print no copy and no move (see "When moves happen" below).

### Example 8: Copy vs Move Performance
**Measured** timing of copying and then moving a 1-million-int object. The numbers change on every run.

### Example 9: String Class
A small `char*` string with all copy and move operations.

### Example 10: Unique Pointer
A move-only type: copy operations are `= delete`, ownership moves.

---

## 🧾 Notes moved from main.cpp

### Moves and copies at a glance
| | Copy | Move |
|--|------|------|
| What happens | New memory, all elements copied | Pointer (and size) taken over |
| Source afterwards | Unchanged | Valid, contents unspecified (empty in our classes) |
| Cost | O(n) | O(1) for pointer-based types |
| Use when | You need **both** objects | The source is no longer needed |

### What triggers a move
- `std::move(x)` on an lvalue you are done with (`T b = std::move(a);`, `b = std::move(a);`)
- Passing `std::move(x)` to a by-value parameter (`f(std::move(x))`). A plain temporary, `f(T())`, is elided in C++17
- Container operations with a temporary: `vec.push_back(T())`
- A `return` of a local that is not elided (see below)

### Copy elision and implicit moves (all tested with GCC, C++17)
| Code | What happens |
|------|--------------|
| `T obj = T();` | **No copy, no move** (guaranteed elision since C++17) |
| `return T();` | **No copy, no move** (guaranteed) |
| `T f() { T t; return t; }` | Usually no copy, no move (**NRVO**, optional) |
| Two locals, `if (c) return a; return b;` | NRVO not possible: **implicit move** |
| `return std::move(t);` | Always a move, and **prevents** NRVO (GCC warns with `-Wpessimizing-move`) |
| `v.push_back(T())` | Move |
| `T b = std::move(constObj);` | **Copy** (a `const` object cannot be moved from) |

So the old advice "initializing from a temporary is an automatic move" is no longer right since C++17: nothing at all happens.

### Performance numbers (Example 8)
Measured in the sandbox, 1,000,000 ints (4 MB): copy about **3-5 milliseconds**, move about **70-700 nanoseconds** (three runs each at `-O0` and `-O2`). That is a factor of roughly 10,000 or more here, but the exact ratio depends on machine, allocator and optimization level. The count of "operations" is not a time ratio, so "1,000,000x faster" is not a measured result.

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

This is a guideline, and the reason is concrete: if you declare a destructor or a copy operation, the compiler does **not** generate the move operations. Then `std::move(x)` still compiles but silently **copies** (tested: a class with only a destructor and a copy constructor printed "copy" for `T b = std::move(a);`). Best of all is the **Rule of Zero**: use `std::string`, `std::vector`, `std::unique_ptr` as members and declare none of the five.

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
// simplified (the real one is also constexpr and noexcept)
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
- Source object is in a **valid but unspecified state**. It is *not* automatically undefined behavior to touch it. It is a normal object whose value you must not rely on.
- Safe: destroy it, assign a new value, or call functions that have no preconditions (e.g. `size()`, `clear()` on standard types).
```cpp
string s = "hello";
string t = std::move(s);

s.size();         // ✅ allowed, but the value is unspecified (0 with GCC)
s = "new";        // ✅ assign a new value, then it is fine again

// ❌ DON'T rely on the old content, or call something that needs a particular state
//    (for YOUR class: whatever you left behind; a nullptr member is a crash waiting to happen)
```
Tested: a moved-from `std::vector<int>` reports `size() == 0` and a moved-from `std::string` too.

### noexcept

**Move operations should be `noexcept`!**
```cpp
MyClass(MyClass&& other) noexcept { }
MyClass& operator=(MyClass&& other) noexcept { }
```

**Why it matters:**

1. **`std::vector` checks it when it grows (reallocates):**
```cpp
   vector<MyClass> vec;
   vec.push_back(obj);   // when the buffer is full, elements are moved to the new buffer
   // If the move constructor is noexcept          → elements are moved
   // If it can throw AND the class is copyable    → elements are COPIED (safer)
   // If it can throw but the class is move-only   → moved anyway
```
   Tested with three small classes: noexcept move → "move"; non-noexcept move + copy constructor → "copy"; non-noexcept move-only → "move". The reason is the strong exception guarantee: if a move halfway through reallocation threw, the old buffer would already be damaged, while a copy leaves it intact.

2. **Other library code** uses the same check (`std::move_if_noexcept`, `std::is_nothrow_move_constructible`).

**When not to use `noexcept`:**
- If a move operation can really throw. Omit it rather than lie: if a `noexcept` function throws, the program calls `std::terminate`.

### When Moves Happen Automatically

The compiler uses a move (or skips the copy entirely) in these situations. Results below are tested with GCC in C++17 mode, using a class that prints from its copy and move constructors.

#### 1. Returning a Local Object
```cpp
MyClass func() {
    MyClass temp;
    return temp;  // NRVO: normally no copy and no move; otherwise an implicit move
}

MyClass obj = func();  // No explicit std::move needed!
```

#### 2. Initializing from a Temporary (C++17: nothing happens)
```cpp
MyClass obj = MyClass();  // Guaranteed elision: the temporary IS obj. No copy, no move.
```
(Before C++17 this was a move, or an elided move.)

#### 3. Container Operations
```cpp
vector<MyClass> vec;
vec.push_back(MyClass());  // Temporary → move into vector (tested)
```

#### 4. Return Statements With Several Candidates
```cpp
MyClass func(bool condition) {
    MyClass a, b;
    if (condition)
        return a;  // NRVO is not possible with two candidates → implicit move (tested)
    else
        return b;
}
```

**Important:** Don't use `std::move` on a returned local. It prevents NRVO, and GCC warns about it (`-Wpessimizing-move`, tested):
```cpp
// ❌ BAD - prevents NRVO
MyClass func() {
    MyClass temp;
    return std::move(temp);  // DON'T DO THIS!
}

// ✅ GOOD - allows NRVO or implicit move
MyClass func() {
    MyClass temp;
    return temp;  // Compiler optimizes
}
```
`std::move` **is** needed when you return something that is not a plain local or by-value parameter, for example a member: `return std::move(member_);` inside a function that is allowed to give it away.

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Using Moved-From Object
```cpp
MyClass a;
MyClass b = std::move(a);
a.doSomething();  // ❌ a's state is unspecified: may crash if you left a nullptr behind
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
    Bad(const Bad&);                  // copyable...
    Bad(Bad&& other) {                // ❌ Missing noexcept
        // vector reallocation will COPY instead of using this!
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
const MyClass a(1);          // (a const object needs an initializer / user-provided constructor)
MyClass b = std::move(a);    // ❌ Won't move, will COPY!
```

**Explanation:** `std::move(a)` gives a `const MyClass&&`, which cannot bind to `MyClass&&` (moving modifies the source). It binds to the copy constructor's `const MyClass&` instead. For a move-only type this is a compile error. Tested: the copy constructor ran.

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
g++ -std=c++23 -Wall -Wextra main.cpp -o move_semantics
```
`main.cpp` uses `std::println`, so it needs GCC 14 or newer (or another compiler with `<print>`).

### Run:
```bash
./move_semantics
```
The pointer addresses and the timings in Example 8 are different on every run.

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
// Cost: allocate 4MB + copy 1M ints (measured in the sandbox: about 3-5 ms)

// MOVE: O(1)
LargeObject c;
LargeObject d = std::move(c);
// Cost: copy one pointer and one int (measured: about 70-700 ns)

// Speedup: very large (10,000x or more here), but it depends on size, machine and
// optimization level. Example 8 prints the numbers for your machine.
```

### Real-World Impact
```cpp
// Returning large objects efficiently
vector<int> createLargeVector() {
    vector<int> v(1000000);
    // ... fill vector ...
    return v;  // no copy: NRVO, or at worst an implicit move
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
- Returning a **member** or something that is not a plain local (`return std::move(member_);`). For a plain local just write `return local;`
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

- [cppreference.com - Move constructors](https://en.cppreference.com/cpp/language/move_constructor)
- [cppreference.com - std::move](https://en.cppreference.com/cpp/utility/move)
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