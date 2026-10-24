# 18_HeaderSourceSeparation - Organizing C++ Code Professionally

## 📚 Overview

This folder introduces **Header/Source Separation** - a fundamental practice in professional C++ development that separates class declarations (interface) from implementations (code). This separation is essential for building scalable, maintainable C++ projects and is the standard way to organize code in the industry.

**Header/Source Separation** = Splitting code into:
- **Headers (.h/.hpp)**: Class declarations, interfaces, prototypes (WHAT)
- **Sources (.cpp)**: Function implementations, definitions (HOW)

This is **ESSENTIAL** knowledge for professional C++ development and real-world projects.

---

## 🎯 What You'll Learn

- What header/source separation is and why it's critical
- Difference between .h, .hpp, and .cpp files
- How to properly organize C++ projects
- Include guards vs #pragma once
- Forward declarations to reduce dependencies
- When to inline functions in headers
- Static member initialization
- Template special case (must be in headers)
- Compilation process and linking
- Best practices for file organization
- Common mistakes and how to avoid them

---

## 📖 Concept Explanation

### What is Header/Source Separation?

**Header/Source separation** is the practice of dividing C++ code into two types of files:

1. **Header Files** (.h or .hpp): Contain declarations (the interface)
2. **Source Files** (.cpp): Contain implementations (the code)

This separation is fundamental to how C++ projects are organized and compiled.

**Real-World Analogy:**
```
Think of building a house:

HEADER (.h):
  - Blueprint/floor plan (what the house looks like)
  - Room layout, door locations
  - Everyone can see the plan
  
SOURCE (.cpp):
  - Actual construction (how to build it)
  - Electrical wiring details
  - Plumbing implementation
  - Workers know the details, residents don't need to
```

### Why Separate?

**1. Faster Compilation**
- Change implementation (.cpp) → recompile only that file
- Change header (.h) → recompile ALL files that include it
- Keep headers minimal = faster builds!

**2. Information Hiding**
- Users see the interface (header)
- Implementation details hidden (.cpp)
- Encapsulation at the file level

**3. Reusability**
- One interface, multiple implementations
- Platform-specific versions
- Different algorithms, same interface

**4. Modularity**
- Clear boundaries between modules
- Easier to create libraries
- Better organization for teams

**5. Compile Each Implementation Once**
- A header is re-read and re-parsed by every `.cpp` that includes it
- A function body in a `.cpp` is compiled exactly once
- (This is about build time. It does not make the executable smaller.)

---

## 💻 Basic Syntax

### Traditional Approach (All in One File)

```cpp
// main.cpp
class Person {
private:
    std::string name;
    int age;
    
public:
    Person(const std::string& n, int a) : name(n), age(a) {
        // Implementation right here
    }
    
    void display() const {
        std::cout << name << ", " << age << std::endl;
    }
};

int main() {
    Person p("Alice", 30);
    p.display();
}
```

**Problems:**
- ❌ Implementation exposed
- ❌ Change anything → recompile everything
- ❌ Doesn't scale to large projects
- ❌ Can't create libraries easily

### Modern Approach (Separated)

**Person.h (Header)**
```cpp
#pragma once
#include <string>

class Person {
private:
    std::string name;
    int age;
    
public:
    // Declarations only
    Person(const std::string& n, int a);
    ~Person();
    
    void display() const;
    void setAge(int a);
    int getAge() const;
};
```

**Person.cpp (Source)**
```cpp
#include "Person.h"
#include <iostream>

// Implementations
Person::Person(const std::string& n, int a) 
    : name(n), age(a) {
}

Person::~Person() {
}

void Person::display() const {
    std::cout << name << ", " << age << std::endl;
}

void Person::setAge(int a) {
    age = a;
}

int Person::getAge() const {
    return age;
}
```

**main.cpp (Usage)**
```cpp
#include "Person.h"

int main() {
    Person p("Alice", 30);
    p.display();
    return 0;
}
```

**Benefits:**
- ✅ Clear interface in header
- ✅ Hidden implementation in .cpp
- ✅ Change .cpp without affecting users
- ✅ Professional organization

---

## 🔍 Include Guards

### Problem: Multiple Inclusion

```cpp
// A.h
#include "Person.h"

// B.h
#include "Person.h"

// main.cpp
#include "A.h"
#include "B.h"
// Error! Person defined twice!
// GCC: error: redefinition of 'class Person'   (a compile error, tested)
```

### Solution 1: Traditional Include Guards

```cpp
// Person.h
#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
    // ... class definition ...
};

#endif // PERSON_H
```

**How it works:**
1. First inclusion: `PERSON_H` not defined → define it, include code
2. Second inclusion: `PERSON_H` defined → skip code
3. Prevents multiple definition errors

**Naming Convention:**
- Use `FILENAME_H` or `FILENAME_HPP`
- ALL CAPS
- Underscores instead of dots
- Project prefix for uniqueness: `MYPROJECT_PERSON_H`

### Solution 2: #pragma once (Modern)

```cpp
// Person.h
#pragma once

#include <string>

class Person {
    // ... class definition ...
};
```

**Benefits:**
- ✅ One line instead of three
- ✅ No guard names to invent, so no typos or two headers with the same guard name
- ✅ Less error-prone
- ✅ Supported by GCC, Clang and MSVC

**Drawback:** it is **not part of the C++ standard**. It also depends on the compiler recognising "the same file", which can go wrong with copies or links of one header in several places (from memory, not tested here).

**Comparison:**

| Feature | Include Guards | #pragma once |
|---------|---------------|--------------|
| Standard | Yes (C++98+) | No (but universal) |
| Lines of code | 3 | 1 |
| Naming conflicts | Possible (same guard name twice) | No guard name needed |
| Speed | About the same (compilers optimise both; not measured here) | About the same |
| Error-prone | Typos in the guard name | Less |
| **Recommendation** | Strictly portable | Simplest; **follow your project's convention** |

Both are fine; what matters is that **every header has one** and the project uses one style consistently. The C++ Core Guidelines (SF.8) and the Google style guide ask for `#include` guards, many other codebases use `#pragma once` (from memory, pages not opened here).

---

## 📋 Examples in This Folder

### Example 1: Single File (Before Separation)
Shows everything in one file - the old way.
- A change in any function body means recompiling this whole file
- No separate interface, implementation details are exposed, hard to turn into a library

### Example 2: Simulated Separation
Demonstrates proper separation structure.
- Class declaration first (this is what `Person.h` would hold), member definitions after it (`Person.cpp`)
- Users of the class only need the declaration

### Example 3: Include Guards (Traditional)
Classic `#ifndef`/`#define`/`#endif` pattern.
- The same "header" text is pasted twice; the guard makes the second copy disappear
- Without the guard GCC reports *redefinition of 'int guardDemoValue()'* (tested)

### Example 4: #pragma once (Modern)
Modern one-line solution.
- Works per file, so it cannot be shown in a single `main.cpp` (GCC warns *#pragma once in main file*, tested)

### Example 5: Inline Functions
When to put functions in headers vs .cpp.
- Small getters/setters in the class, loops and algorithms outside it

### Example 6: Forward Declarations
Reducing dependencies with forward declarations.
- `Car` only holds an `Engine*`, so `class Engine;` is enough in its declaration
- The member-initializer list now matches the declaration order, and copying `Car` is deleted

### Example 7: Static Member Initialization
How to handle static members correctly.
- Declaration in the class, definition (`int BankAccount::accountCount = 0;`) in exactly one `.cpp`
- Copying is deleted, otherwise a copy would decrement the counter without ever incrementing it

### Example 8: Template Classes
Special case - templates must be in headers.
- `Box<int>`, `Box<std::string>` and `Box<double>` come from one template definition

### Example 9: Constants and constexpr
Handling constants in headers vs sources.
- `static constexpr` can be initialised in the class; the `std::string` constant needs a `.cpp` definition (or `static inline const` in C++17)

### Example 10: Proper File Organization
Professional project structure.
- Small and medium layouts with their build commands (more in the sections below)

---

## 🎓 Key Concepts

### What Goes in Headers (.h/.hpp)

**Always:**
- ✅ Class declarations
- ✅ Function prototypes
- ✅ Template definitions
- ✅ Inline functions (small ones)
- ✅ `constexpr` constants
- ✅ Enums and type aliases
- ✅ Include guards or `#pragma once`
- ✅ Forward declarations

**Never:**
- ❌ Function implementations (except inline/template)
- ❌ `using namespace` directives
- ❌ Static member initialization (except `constexpr` and `static inline`, C++17)
- ❌ Plain variable definitions (`int counter = 0;`). `const`, `constexpr` and `inline` variables are fine
- ❌ Large amounts of code

### What Goes in Sources (.cpp)

**Always:**
- ✅ Function implementations
- ✅ Member function definitions
- ✅ Static member initialization
- ✅ Complex algorithms
- ✅ Private helper functions
- ✅ File-local utilities

### Inline Functions

**When to inline (in header):**
```cpp
class Person {
public:
    // ✅ Simple getter - can inline
    int getAge() const { return age; }
    
    // ✅ Simple setter - can inline
    void setAge(int a) { age = a; }
    
    // ❌ Complex logic - DON'T inline
    void complexCalculation();  // Put in .cpp
};
```

**Rule of Thumb** (a common heuristic, not a language rule):
- ≤ 3 lines → Can inline in header
- \> 3 lines → Put in .cpp file

**What `inline` really means:** permission to have the *same definition in many .cpp files* (one-definition rule). Whether the compiler actually inlines a call is its own decision. Tested with two `.cpp` files that include one header:
```cpp
// f.h
int twice(int x) { return 2 * x; }          // ❌ linker: multiple definition of `twice(int)`
inline int twice(int x) { return 2 * x; }   // ✅ OK
struct S { int g(int x) { return 2 * x; } };// ✅ OK: functions defined inside the class are implicitly inline

int counter = 0;                            // ❌ linker: multiple definition of `counter`
const int K = 5;                            // ✅ OK: namespace-scope const has internal linkage
inline int counter = 0;                     // ✅ OK in C++17 (inline variable)
```
**Why limit inlines?**
- Code in header → included everywhere
- Changes → recompile all users
- Slower compilation for large functions

### Forward Declarations

**Purpose:** Reduce header dependencies

```cpp
// Car.h
#pragma once
#include <string>

class Engine;  // Forward declaration (no #include needed!)

class Car {
private:
    Engine* engine;  // Pointer OK with forward declaration
    std::string model;
    
public:
    Car(const std::string& m);
    ~Car();
    Car(const Car&) = delete;             // Car owns the raw Engine*: a copy would delete it twice
    Car& operator=(const Car&) = delete;
    void start();
};
```

**Careful with the destructor:** define `~Car()` in `Car.cpp`, where `Engine` is complete. Tested with GCC:
- inline `~Car() { delete engine; }` in the header with only `class Engine;` → warning *possible problem detected in invocation of 'operator delete'* / *invalid use of incomplete type 'class Engine'*
- `std::unique_ptr<Engine>` member and no declared destructor → error *invalid application of 'sizeof' to incomplete type 'Engine'*. Declare `~Car();` in the header and define it in the `.cpp` and it compiles.

**When to use:**
```cpp
// ✅ CAN use forward declaration:
class Engine;
Engine* ptr = nullptr;    // Pointer
void func(Engine& e);     // Reference parameter (declaring it is enough)
void func(Engine* e);     // Pointer parameter
// class Car { Engine& ref; };   // Reference member is fine too

// ❌ CANNOT use forward declaration:
Engine obj;               // By value - needs full definition
class Car : public Engine {};   // Inheritance - needs full definition
// ptr->start();          // Calling a member also needs the full definition
```

**Benefits:**
- Faster compilation
- Breaks circular dependencies
- Reduces coupling
- `Car.h` doesn't need `#include "Engine.h"`

**Not everyone agrees:** the Google style guide says to *avoid* forward declarations and include the header instead, because they can hide dependencies and break when a type changes (from memory, page not opened here). Use them to break cycles and where the build time really matters.

### Static Members

**Declaration vs Definition:**

```cpp
// BankAccount.h
class BankAccount {
private:
    static int accountCount;  // Declaration in header
    
public:
    BankAccount();
    static int getCount();
};
```

```cpp
// BankAccount.cpp
#include "BankAccount.h"

// Definition MUST be in .cpp!
int BankAccount::accountCount = 0;

BankAccount::BankAccount() {
    accountCount++;
}

int BankAccount::getCount() {
    return accountCount;
}
```

**Exception (C++17):**
```cpp
// Can initialize in header with inline
class BankAccount {
    static inline int accountCount = 0;  // C++17
};
```
Tested: with `-std=c++17` this links without any definition in a `.cpp`. With `-std=c++14` GCC only warns *inline variables are only available with '-std=c++17'* and accepts it as an extension.

**`static constexpr` members:** since C++17 they are implicitly `inline`, so no `.cpp` definition is needed. Before C++17, binding one to a `const&` (for example `std::max(C::N, 3)`) failed at link time with *undefined reference to `C::N`* (tested with `-std=c++14`).

**Constants of class type (C++17):** `static inline const std::string VERSION = "1.0.0";` in the header works and replaces the `.cpp` definition (tested).

### Templates (Special Case)

**In practice, templates are fully defined in headers.** The compiler needs the definition at the point where the template is instantiated:

```cpp
// Box.h
#pragma once

template<typename T>
class Box {
private:
    T value;
    
public:
    // Implementation MUST be in header
    Box(T v) : value(v) { }
    
    T getValue() const {
        return value;
    }
    
    void setValue(T v) {
        value = v;
    }
};

// NO Box.cpp file! Everything in header!
```

**Why?**
- The compiler needs the full definition to instantiate (generate code for) a template
- Each new use (`Box<int>`, `Box<double>`) creates new code
- A declaration in `.h` and the body in `.cpp` compiles, but the linker cannot find the instance

**Tested** with `Box.h` (declaration), `Box.cpp` (member definitions) and a `main.cpp` using `Box<int>`:
- split like a normal class → `undefined reference to 'Box<int>::Box(int)'`
- add `template class Box<int>;` at the end of `Box.cpp` (explicit instantiation) → links and works, but `Box<double>` then gives `undefined reference` again, because only the listed types exist
- `#include "Box.cpp"` at the end of `Box.h` → works when only `main.cpp` is compiled (do **not** also compile `Box.cpp` on its own)

**Workarounds (if needed):**
1. Include `.cpp` at end of `.h` (rename it `.tpp`/`.ipp` so nobody compiles it by mistake)
2. Use `.hpp` extension for clarity
3. Explicit template instantiation, only for a fixed set of types (advanced)

---

## 🔧 Compilation Process

### How it Works

```
Step 1: PREPROCESSING
  - #include copies header contents
  - Macros expanded
  - Conditional compilation

Step 2: COMPILATION (separate for each .cpp)
  Person.cpp → Person.o
  Car.cpp → Car.o
  main.cpp → main.o

Step 3: LINKING
  Person.o + Car.o + main.o → program
```

### Commands

**Compile separately:**
```bash
# Compile each .cpp to object file
g++ -c Person.cpp -o Person.o
g++ -c Car.cpp -o Car.o
g++ -c main.cpp -o main.o

# Link object files
g++ Person.o Car.o main.o -o program
```

**Compile all at once:**
```bash
g++ Person.cpp Car.cpp main.cpp -o program
```

**With separate include directory:**
```bash
mkdir -p obj          # the output folder must exist
g++ -c -Iinclude src/Person.cpp -o obj/Person.o
g++ -c -Iinclude src/Car.cpp -o obj/Car.o
g++ -c -Iinclude main.cpp -o obj/main.o
g++ obj/*.o -o program
```

---

## 📊 File Organization Patterns

### Pattern 1: Small Projects

```
project/
├── Person.h
├── Person.cpp
├── Car.h
├── Car.cpp
└── main.cpp
```

**Compilation:**
```bash
g++ *.cpp -o program
```

**Pros:**
- Simple
- Easy to navigate
- Good for learning

**Cons:**
- Gets messy with many files
- Harder to organize features

### Pattern 2: Medium Projects

```
project/
├── include/
│   ├── Person.h
│   ├── Car.h
│   └── Engine.h
├── src/
│   ├── Person.cpp
│   ├── Car.cpp
│   └── Engine.cpp
└── main.cpp
```

**Compilation:**
```bash
g++ -Iinclude src/*.cpp main.cpp -o program
```

**Pros:**
- Clear separation
- Cleaner directory structure
- Professional organization

**Cons:**
- More complex paths
- Need `-I` flag

### Pattern 3: Large Projects

```
project/
├── include/
│   └── myproject/
│       ├── Person.h
│       ├── Car.h
│       ├── Engine.h
│       └── Utils.h
├── src/
│   ├── core/
│   │   ├── Person.cpp
│   │   └── Car.cpp
│   ├── utils/
│   │   └── Utils.cpp
│   └── engine/
│       └── Engine.cpp
├── tests/
│   ├── test_Person.cpp
│   └── test_Car.cpp
├── build/
├── docs/
└── main.cpp
```

**Compilation:** Use build system (CMake, Make, etc.)

**Pros:**
- Highly organized
- Scalable
- Team-friendly
- Module isolation

**Cons:**
- More complex
- Needs build system

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Forgetting Include Guards

**Bad:**
```cpp
// Person.h
class Person {
    // ... no guard!
};
```

**Problem:** if one `.cpp` ends up including it twice (directly or through other headers): *error: redefinition of 'class Person'* (tested). This is a **compile** error. The *linker* error *multiple definition of ...* appears for non-inline functions and variables defined in a header that two `.cpp` files include (see the inline section).

**Fix:**
```cpp
// Person.h
#pragma once
class Person {
    // ...
};
```

### ❌ Mistake 2: Implementation in Header

**Bad:**
```cpp
// Person.h
class Person {
public:
    void complexFunction() {
        // 50 lines of complex code!
        for (int i = 0; i < 1000; ++i) {
            // lots of logic
        }
    }
};
```

**Problems:**
- Every file that includes the header re-parses and re-compiles the body
- Slow compilation
- Changes require recompiling everything

(No link error here: functions defined inside the class are implicitly inline, tested. A *non-inline* function defined in a header would fail to link.)

**Fix:**
```cpp
// Person.h
class Person {
public:
    void complexFunction();  // Declaration only
};

// Person.cpp
void Person::complexFunction() {
    // Implementation here
}
```

### ❌ Mistake 3: Using namespace in Header

**Bad:**
```cpp
// Person.h
#pragma once
#include <string>
using namespace std;  // ❌ BAD!

class Person {
    string name;  // Pollutes global namespace
};
```

**Problem:**
- Pollutes namespace for all files that include this header
- Name conflicts
- Unexpected behavior

**Fix:**
```cpp
// Person.h
#pragma once
#include <string>

class Person {
    std::string name;  // ✅ Explicit namespace
};
```

### ❌ Mistake 4: Not Initializing Static Members

**Bad:**
```cpp
// BankAccount.h
class BankAccount {
    static int count;
};

// BankAccount.cpp
// ❌ Forgot to initialize!
```

**Problem:** Linker error: *undefined reference to `BankAccount::count`* (tested), as soon as the member is used

**Fix:**
```cpp
// BankAccount.cpp
int BankAccount::count = 0;  // ✅ Must initialize
```

### ❌ Mistake 5: Circular Dependencies

**Bad:**
```cpp
// A.h
#include "B.h"
class A {
    B* b;
};

// B.h
#include "A.h"  // ❌ Circular!
class B {
    A* a;
};
```

**Problem:** compilation fails: *error: 'A' does not name a type* (tested, even with guards in both headers)

**Fix:** (the `.cpp` files then include the real headers)
```cpp
// A.h
class B;  // Forward declaration
class A {
    B* b;
};

// B.h
class A;  // Forward declaration
class B {
    A* a;
};
```

### ❌ Mistake 6: Including .cpp Files

**Bad:**
```cpp
#include "Person.cpp"  // ❌ NEVER do this!
```

**Problem:**
- *multiple definition of `f()`* when the `.cpp` is also compiled on its own (tested)
- Breaks compilation model
- Confuses linker

**Fix:**
```cpp
#include "Person.h"  // ✅ Only include headers
```

---

## 🎯 Practice Exercises

### Exercise 1: Basic Separation
Take a single-file class and separate it into .h and .cpp files. Compile and run.

### Exercise 2: Multiple Classes
Create a project with 3 classes (Person, Address, Company). Organize with proper separation.

### Exercise 3: Forward Declarations
Create two classes that reference each other. Use forward declarations to avoid circular dependencies.

### Exercise 4: Static Members
Create a class with static members. Practice proper initialization in .cpp file.

### Exercise 5: Template Class
Create a template class and understand why it must be in the header.

### Exercise 6: Complex Project
Build a project with include/ and src/ directories. Practice proper compilation commands.

---

## 🚀 How to Run

Since this example demonstrates concepts using simulated separation in a single file for educational purposes, you can compile and run it directly:

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o header_source_separation
```
(`main.cpp` uses `std::println` from `<print>`, so it needs GCC 14+ or another compiler with C++23 `<print>`.)

### Run:
```bash
./header_source_separation
```

### Expected Output:
The program demonstrates:
1. Single file approach (before separation)
2. Separated approach (proper way)
3. Include guards (pasting the same "header" twice)
4. #pragma once (why it needs separate files)
5. Inline functions
6. Forward declarations
7. Static member initialization
8. Template special case
9. Constants handling
10. Professional file organization

---

## 📊 Visual Concepts

### Compilation Flow

```
HEADERS (.h/.hpp)           SOURCES (.cpp)
┌──────────────┐            ┌──────────────┐
│  Person.h    │←─include───│  Person.cpp  │
│              │            │              │
│ Declarations │            │ Implements   │
│ Interface    │            │ Details      │
└──────────────┘            └──────────────┘
       ↑                           ↓
       │                      [Compile]
       │                           ↓
       │                    ┌──────────────┐
       │                    │  Person.o    │
       │                    │ (object file)│
       │                    └──────────────┘
       │                           ↓
       └───include───────┐         │
                        │         │
                   ┌────▼─────────▼──┐
                   │    LINKER        │
                   │  Combines all    │
                   │  object files    │
                   └─────────┬────────┘
                             ↓
                      ┌──────────────┐
                      │   program    │
                      │ (executable) │
                      └──────────────┘
```

### Header vs Source

```
HEADER FILE (.h)              SOURCE FILE (.cpp)
┌─────────────────┐          ┌─────────────────┐
│ INTERFACE       │          │ IMPLEMENTATION  │
│                 │          │                 │
│ What it does:   │          │ How it works:   │
│                 │          │                 │
│ • Declarations  │          │ • Definitions   │
│ • Prototypes    │          │ • Logic         │
│ • Public API    │          │ • Algorithms    │
│ • Types         │          │ • Details       │
│                 │          │                 │
│ Visible to all  │          │ Hidden from     │
│ users           │          │ users           │
└─────────────────┘          └─────────────────┘
```

### Include Guard Flow

```
WITHOUT GUARD:                  WITH GUARD:

main.cpp                        main.cpp
  ↓ #include "A.h"                ↓ #include "A.h"
  ↓ #include "B.h"                ↓ #include "B.h"
  ↓                               ↓
A.h includes Person.h            A.h includes Person.h
  ↓                               ↓ (Person.h content)
B.h includes Person.h            B.h includes Person.h
  ↓                               ↓ (Person.h SKIPPED - already defined)
Person.h defined TWICE! ❌       Person.h defined ONCE! ✅
  ↓                               ↓
COMPILATION ERROR               SUCCESS
```

---

## 📚 Related Topics

### Prerequisites:
- **03_Destructor** - Understanding class lifecycle
- **07_CopyConstructor** - Copy semantics
- **11_StaticMembers** - Static member usage

### Coming Next:
- **19_ConstMemberFunctions** - Const correctness
- **20_DefaultDelete** - Controlling special functions
- **Templates** - Generic programming

### Related Concepts:
- **Compilation Model** - How C++ builds
- **Linkage** - External vs internal
- **Build Systems** - CMake, Make, etc.

---

## 🎓 Summary

### What We Learned:

✅ **Header files** contain declarations (interface)
✅ **Source files** contain implementations (code)
✅ **#pragma once** is simpler than include guards (but not standard C++)
✅ **Forward declarations** reduce dependencies
✅ **Inline functions** only for simple code
✅ **Static members** initialized in .cpp
✅ **Templates** must be fully in headers
✅ **Separation** enables faster compilation
✅ **Organization** is key to scalability
✅ **Professional** C++ projects usually use separation (header-only libraries are the exception)

### The Complete Pattern:

```cpp
// Person.h
#pragma once
#include <string>

class Person {
private:
    std::string name;
    int age;
    static int count;
    
public:
    Person(const std::string& n, int a);
    ~Person();
    
    // Simple - can inline
    int getAge() const { return age; }
    
    // Complex - in .cpp
    void display() const;
    
    static int getCount();
};
```

```cpp
// Person.cpp
#include "Person.h"
#include <iostream>

// Static initialization
int Person::count = 0;

Person::Person(const std::string& n, int a)
    : name(n), age(a) {
    count++;
}

Person::~Person() {
    count--;
}

void Person::display() const {
    std::cout << name << ", " << age << std::endl;
}

int Person::getCount() {
    return count;
}
```

```cpp
// main.cpp
#include "Person.h"

int main() {
    Person p("Alice", 30);
    p.display();
    return 0;
}
```

---

## ✨ Quick Reference Card

```cpp
// === HEADER FILE (.h/.hpp) ===
#pragma once                        // Modern include guard
#include <string>                   // System includes

class MyClass {
private:
    int value;
    static int count;               // Declaration
    
public:
    MyClass(int v);                 // Constructor
    ~MyClass();                     // Destructor
    
    int getValue() const { return value; }  // Inline OK
    void complexFunc();             // In .cpp
    
    static int getCount();          // In .cpp
};

// === SOURCE FILE (.cpp) ===
#include "MyClass.h"

int MyClass::count = 0;             // Static initialization

MyClass::MyClass(int v) : value(v) {
    count++;
}

MyClass::~MyClass() {
    count--;
}

void MyClass::complexFunc() {
    // Implementation here
}

int MyClass::getCount() {
    return count;
}
```

---

## 🔍 Best Practices

### ✅ DO:

- **Put an include guard in every header** (`#pragma once` is simplest, `#ifndef` is strictly portable; use one style)
- **Forward declare** when possible
- **Keep headers minimal** (fast compilation)
- **Include what you use** (IWYU principle)
- **Separate interface from implementation**
- **Organize by feature/module**
- **Document headers** well
- **Use consistent naming**
- **Initialize static members** in .cpp
- **Put complex code** in .cpp files

### ❌ DON'T:

- **Implement in headers** (except inline/template)
- **Use 'using namespace'** in headers
- **Forget include guards** or #pragma once
- **Include unnecessary headers**
- **Create circular dependencies**
- **Mix declarations and definitions**
- **Put large functions inline**
- **Include .cpp files**
- **Forget static initialization**
- **Expose implementation details**

---

## 🌟 Real-World Applications

### Library Development
```cpp
// Public API in headers
// Implementation hidden in .cpp
// Users link against compiled library
```

### Large Codebases
```cpp
// Organized by modules
// Clear dependencies
// Parallel compilation
// Team collaboration
```

### Cross-Platform Development
```cpp
// Common interface in headers
// Platform-specific .cpp files
// Same API, different implementations
```

### Plugin Systems
```cpp
// Interface defined in headers
// Plugins implement in .cpp
// Dynamic loading
```

---

## 📖 Further Reading

- [cppreference.com - Phases of translation](https://en.cppreference.com/cpp/language/translation_phases)
- [C++ Core Guidelines - Source files (SF.7 no `using namespace` in headers, SF.8 include guards)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#sf-source-files)
- "Large-Scale C++ Software Design" by John Lakos
- "C++ Coding Standards" by Herb Sutter
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Include What You Use](https://include-what-you-use.org/)

---

## 🎯 Key Takeaways

1. **Separate** interface (header) from implementation (source)
2. **Every header needs an include guard**: `#pragma once` is the simple, widely supported way (not part of the standard)
3. **Forward declare** to reduce compilation time
4. **Inline** only simple functions (≤3 lines)
5. **Static members** need .cpp initialization
6. **Templates** are exception - must be in headers
7. **Headers** should be self-contained (a header includes everything it needs itself, so it compiles when included first)
8. **Never** 'using namespace' in headers
9. **Organize** by feature for scalability
10. **Professional** C++ projects usually use separation (header-only libraries are the exception)

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Always use an include guard (#pragma once or #ifndef)
   Prevent multiple inclusion errors!
   
   #pragma once
   // Your code here

⚠️  RULE 2: Headers = Interface, Source = Implementation
   Keep them separate!
   
   Header: What the class does
   Source: How it does it

⚠️  RULE 3: Forward declare when possible
   Reduce dependencies and compile time!
   
   class Engine;  // Instead of #include "Engine.h"

⚠️  RULE 4: Templates are the exception
   They MUST be fully in headers!
   
   template<typename T>
   class Box {
       // Everything here in header
   };

⚠️  RULE 5: Never 'using namespace' in headers
   Don't pollute user's namespace!
   
   Use std::string, not 'using namespace std'

   FOLLOW THESE RULES FOR PROFESSIONAL C++ CODE!
```

---

**Previous Topic:** [17_VirtualDestructor](../17_VirtualDestructor/) - Virtual Destructors

**Next Topic:** [19_ConstMemberFunctions](../19_ConstMemberFunctions/) - Const Correctness

---

*Part of the C++ Classes and Objects series*
*Difficulty: Fundamental*
*Prerequisites: Basic C++ knowledge*
*CRITICAL for Professional Development*

---

## 🧾 Notes moved from main.cpp

The long lecture output (the "COMPREHENSIVE EXPLANATION" at the end of `main()` and the code listings printed inside Examples 3, 4 and 10) repeated this README, so it is gone from `main.cpp`. What it said that is not already above:

- **Inline is not a speed switch.** The old output said "Faster execution (no function call overhead)". `inline` is about allowing the same definition in many files; the compiler inlines (or not) on its own, and across `.cpp` files only with link-time optimisation (from memory, not measured here).
- **`#pragma once` "faster compilation"** was dropped as a claim: it is not measured here, and compilers also optimise include guards.
- **"Templates CANNOT be separated"** is now precise: not like normal classes, but explicit instantiation and the `.tpp` include trick work (tested, see Templates).
- **"static const (integral) can be initialised in the header"** is true for the declaration with an initializer, but using it where a reference is needed (for example `std::println`, `std::max`) still needs a definition. Prefer `static constexpr`.
- **Header-only libraries** (for example Eigen and many Boost libraries; from memory) skip the `.cpp` on purpose; the trade-off is longer builds.
- **Code fixes in `main.cpp`:** `Car`'s initializer list followed the wrong order (`-Wreorder` warning in the original); `Car` and `BankAccount` owned state but could be copied (copy deleted); the unused `<vector>` and `<memory>` includes are removed, since the topic is "include what you use"; `using namespace std;` is kept in the `.cpp` with a comment (never in a header).
- **How the claims were tested:** a scratch project with real `Person.h`/`Person.cpp`/`main.cpp` files, compiled separately and linked, then broken on purpose (no guard, non-inline function in a header, missing static definition, split template, circular includes, forward-declared type with `delete`/`unique_ptr`, `#include` of a `.cpp`). Every error text quoted above comes from GCC 13.
