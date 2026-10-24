# 28_ObjectSlicing - The Hidden Danger of Inheritance

## 📚 Overview

This folder introduces **Object Slicing** - a critical and dangerous issue in C++ inheritance that occurs when a derived class object is assigned to a base class object. The derived portion is "sliced off," causing silent data loss and breaking polymorphism. Understanding and preventing object slicing is essential for correct use of inheritance and polymorphism.

**Object Slicing** = Derived part is lost when assigned to base

This is **CRITICAL** for correct polymorphic code.

---

## 🎯 What You'll Learn

- What object slicing is and why it happens
- How slicing causes silent data loss
- Loss of polymorphic behavior
- Slicing in function parameters
- Slicing in containers
- Slicing in return values and assignments
- How to prevent slicing (pointers, references, smart pointers)
- Compile-time detection techniques
- Best practices for polymorphic types
- Real-world examples and patterns

---

## 📖 Concept Explanation

### What is Object Slicing?

**Object slicing** occurs when a derived class object is copied to a base class object. The derived portion is "sliced off," leaving only the base class portion.

```cpp
class Animal {
protected:
    string name;
public:
    Animal(const string& n) : name(n) { }
    virtual void speak() const {
        cout << name << " makes a sound\n";
    }
};

class Dog : public Animal {
private:
    string breed;  // Additional member
public:
    Dog(const string& n, const string& b) 
        : Animal(n), breed(b) { }
    
    void speak() const override {
        cout << name << " barks: Woof!\n";
    }
};

// ⚠️ SLICING HAPPENS HERE!
Dog dog("Buddy", "Golden Retriever");
Animal animal = dog;  // breed is LOST!

animal.speak();  // ⚠️ Calls Animal::speak, not Dog::speak!
```

`Animal animal = dog;` is copy-*initialization* (it calls `Animal`'s copy constructor); `animal = dog;` on an existing object is copy *assignment*. Both slice (Examples 1 and 8).

**What gets lost:**
- Derived class member variables (`breed`)
- Polymorphic behavior (calls `Animal::speak` instead of `Dog::speak`)
- Type information
- Virtual function dispatch

### Why Slicing is Dangerous

**1. Silent Data Loss:**
```cpp
Dog dog("Max", "German Shepherd");
Animal animal = dog;  // ⚠️ breed member is silently lost!
// No compiler warning!
```

✅ tested: GCC 13 with `-Wall -Wextra -Wpedantic` is silent. **clang-tidy does warn**: `clang-tidy -checks=-*,cppcoreguidelines-slicing main.cpp` reports e.g. `slicing object from type 'Dog' to 'Animal' discards 32 bytes of state` and `discards override 'speak'` - also for `Animal copy = *ptr;`, so use it (the old advice "don't ignore slicing warnings" assumed warnings that compilers do not give).

**2. Loss of Polymorphism:**
```cpp
Dog dog("Max", "German Shepherd");
Animal animal = dog;

animal.speak();  // ⚠️ Calls Animal::speak, not Dog::speak!
// Polymorphism doesn't work with sliced objects
```

**3. Incorrect Behavior:**
```cpp
class Circle : public Shape {
    double radius;
public:
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

Circle circle(5.0);
Shape shape = circle;  // ⚠️ Slicing!
cout << shape.area();  // ⚠️ Wrong! Returns 0, not circle area
```

### Real-World Analogy

```
Think of photocopying:

COPYING A FULL DOCUMENT (No Slicing):
  Original: Cover page + Content pages
  Photocopy: Cover page + Content pages
  ✅ Everything preserved

SLICING (Copying only first page):
  Original: Cover page + Content pages
  Photocopy: Only cover page
  ⚠️  Content pages LOST!
  ⚠️  Incomplete information
  ⚠️  Wrong conclusions possible

OBJECT SLICING = Copying only the base part
  • Derived members lost
  • Behavior changed
  • Silent problem
```

---

## 💻 Common Scenarios Where Slicing Occurs

### 1. Pass by Value

**The most common cause of slicing:**

```cpp
// ❌ BAD - Causes slicing!
void processAnimal(Animal a) {  // Pass by value
    a.speak();  // ⚠️ Always calls Animal::speak
}

Dog dog("Max", "Husky");
processAnimal(dog);  // ⚠️ Dog is sliced to Animal!

// ✅ GOOD - No slicing
void processAnimal(const Animal& a) {  // Pass by reference
    a.speak();  // ✅ Calls correct overridden function
}

processAnimal(dog);  // ✅ Polymorphism works!
```

### 2. Containers of Values

**Storing polymorphic objects by value:**

```cpp
// ❌ BAD - Causes slicing!
vector<Animal> animals;
animals.push_back(Dog("Buddy", "Beagle"));  // ⚠️ Sliced!

for (const auto& a : animals) {
    a.speak();  // ⚠️ Only calls Animal::speak
}

// ✅ GOOD - No slicing
vector<unique_ptr<Animal>> animals;
animals.push_back(make_unique<Dog>("Buddy", "Beagle"));

for (const auto& a : animals) {
    a->speak();  // ✅ Calls Dog::speak
}
```

### 3. Return by Value

**Returning polymorphic objects by value:**

```cpp
// ❌ BAD - Causes slicing!
Animal getAnimal() {
    Dog dog("Max", "Poodle");
    return dog;  // ⚠️ Sliced to Animal!
}

Animal a = getAnimal();
a.speak();  // ⚠️ Calls Animal::speak

// ✅ GOOD - No slicing
unique_ptr<Animal> getAnimal() {
    return make_unique<Dog>("Max", "Poodle");
}

auto a = getAnimal();
a->speak();  // ✅ Calls Dog::speak
```

### 4. Assignment

**Assigning derived to base variable:**

```cpp
Dog dog("Luna", "Husky");
Animal animal = dog;  // ⚠️ Slicing on assignment

animal.speak();  // ⚠️ Calls Animal::speak
```

---

## 🛡️ How to Prevent Object Slicing

### Solution 1: Use References

**Best for function parameters:**

```cpp
// ✅ Pass by reference - no slicing
void processAnimal(const Animal& a) {
    a.speak();  // ✅ Polymorphism works
}

Dog dog("Buddy", "Labrador");
processAnimal(dog);  // ✅ No slicing, dog stays a Dog
```

### Solution 2: Use Pointers

**Good for optional parameters and containers:**

```cpp
// ✅ Pass by pointer - no slicing
void processAnimal(const Animal* a) {
    if (a) {
        a->speak();  // ✅ Polymorphism works
    }
}

Dog dog("Max", "Boxer");
processAnimal(&dog);  // ✅ No slicing
```

### Solution 3: Smart Pointers (Best!)

**Modern C++ solution with automatic memory management:**

```cpp
// ✅ Smart pointers - no slicing + automatic cleanup
vector<unique_ptr<Animal>> animals;

animals.push_back(make_unique<Dog>("Buddy", "Beagle"));
animals.push_back(make_unique<Dog>("Luna", "Husky"));

for (const auto& a : animals) {
    a->speak();  // ✅ Polymorphism works
}
// Automatic cleanup, no memory leaks!
```

### Solution 4: Delete Copy Operations

**Prevent slicing at compile time:**

```cpp
class Animal {
public:
    Animal(const string& n) : name(n) { }
    
    // ✅ Delete copy operations
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;
    
    virtual void speak() const = 0;  // Pure virtual
    virtual ~Animal() = default;
    
protected:
    string name;
};

// Now slicing is impossible:
Dog dog("Max", "Terrier");
// Animal animal = dog;  // ❌ Compilation error!
```

⚠️ Downside: this also forbids copying a whole `Dog`. The usual compromise is **protected** copy operations plus a virtual `clone()`: `Dog d2 = d;` works, `Animal a = d;` is an error (`copy constructor is protected within this context`), and `animal->clone()` copies through a base pointer without slicing ✅ tested (Example 9).

### Solution 5: Abstract Base Class

**Can't create base objects directly:**

```cpp
class Animal {
public:
    virtual void speak() const = 0;  // Pure virtual
    virtual ~Animal() = default;
};

// Animal a;  // ❌ Can't create abstract class
// Forces use of pointers/references
```

---

## 📋 Examples in This Folder

### Example 1: The Problem
Demonstrates basic object slicing with data loss.
- `Animal animal = dog;` copies only the Animal part; `display()`/`speak()` now call the Animal versions

### Example 2: Function Slicing
Slicing when passing to functions by value.
- By value slices; `const Animal&` and `const Animal*` keep polymorphism

### Example 3: Container Slicing
Slicing in vectors and other containers.
- `vector<Animal>` slices, `vector<Animal*>` works but needs manual `delete`, `vector<unique_ptr<Animal>>` is the best option

### Example 4: Copy Constructor Slicing
How copy construction causes slicing.
- `Shape shape = circle;` loses the radius, so `area()` returns 0; a reference to the same circle gives the real area

### Example 5: Preventing Slicing
Using deleted copy operations.
- Deleted copy + pure virtual: slicing becomes a compile error

### Example 6: Virtual Destructor
Relationship between destructors and slicing.
- Virtual destructor for deleting through a base pointer; label fixed (the first demo is the *correct* case, not "WRONG"). Sliced copies leak nothing - the old comment "Derived destructor NOT called for sliced part" was misleading

### Example 7: Return Value Slicing
Slicing when returning by value.
- `return mgr;` as `Employee` loses the bonus; pointer / `unique_ptr` keep it

### Example 8: Assignment Slicing
Slicing through assignment operator.
- `book = ebook;` copies only the Book part; `book` stays a Book

### Example 9: Compile-Time Detection
Making slicing a compile error.
- Non-copyable base class; added: protected copy + `clone()` as the alternative that still lets you copy derived objects

### Example 10: Best Practices
Summary of prevention techniques.
- Was only printed text. Now real output: `typeid` of a sliced copy vs. a reference, `sizeof`, and `is_copy_constructible_v` / `is_abstract_v` of the protected types

---

## 🎓 Key Concepts

### Memory Layout

**Before slicing (Derived object):**
```
┌─────────────────────┐
│ Base part:          │
│   - vptr            │  Points to Derived vtable
│   - base members    │
├─────────────────────┤
│ Derived part:       │
│   - derived members │
└─────────────────────┘
```

**After slicing (Base object):**
```
┌─────────────────────┐
│ Base part:          │
│   - vptr            │  Points to Base vtable
│   - base members    │
└─────────────────────┘
  ⚠️ Derived part LOST!
```

### Virtual Function Dispatch

**With slicing (broken):**
```cpp
Dog dog("Max", "Husky");
Animal animal = dog;  // Sliced

animal.speak();  // ⚠️ vptr points to Animal vtable
                // Calls Animal::speak
```

**Without slicing (correct):**
```cpp
Dog dog("Max", "Husky");
Animal& animal = dog;  // Reference, no slicing

animal.speak();  // ✅ vptr points to Dog vtable
                // Calls Dog::speak
```

### The Golden Rule

**For polymorphic types, ALWAYS use:**
- References (`Animal&`)
- Pointers (`Animal*`)
- Smart pointers (`unique_ptr<Animal>`)

**NEVER use:**
- Pass by value (`void func(Animal a)`)
- Store by value (`vector<Animal>`)
- Return by value (`Animal func()`)
- Value assignment (`Animal a = dog`)

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Pass by Value

**Bad:**
```cpp
void process(Animal a) {  // ⚠️ Pass by value
    a.speak();
}

Dog dog("Max", "Poodle");
process(dog);  // Sliced!
```

**Fix:**
```cpp
void process(const Animal& a) {  // ✅ Pass by reference
    a.speak();
}
```

### ❌ Mistake 2: Container of Values

**Bad:**
```cpp
vector<Animal> animals;  // ⚠️ Stores by value
animals.push_back(dog);  // Sliced!
```

**Fix:**
```cpp
vector<unique_ptr<Animal>> animals;  // ✅ Stores pointers
animals.push_back(make_unique<Dog>("Max", "Poodle"));
```

### ❌ Mistake 3: Return by Value

**Bad:**
```cpp
Animal createAnimal() {  // ⚠️ Returns by value
    return Dog("Max", "Terrier");  // Sliced!
}
```

**Fix:**
```cpp
unique_ptr<Animal> createAnimal() {  // ✅ Returns pointer
    return make_unique<Dog>("Max", "Terrier");
}
```

### ❌ Mistake 4: Forgetting Virtual Destructor

**Bad:**
```cpp
class Base {
public:
    ~Base() { }  // ⚠️ Not virtual!
};

Base* ptr = new Derived();
delete ptr;  // ⚠️ Undefined behavior! (usually only ~Base runs, so Derived's resources leak)
```

✅ tested: AddressSanitizer aborts this with `new-delete-type-mismatch`. Note that this is a *different* bug from slicing: a sliced copy is a complete `Base` object and does not leak anything ✅ tested.

**Fix:**
```cpp
class Base {
public:
    virtual ~Base() { }  // ✅ Virtual destructor
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Identify Slicing
Given code with slicing issues, identify all locations.

### Exercise 2: Fix Slicing
Refactor code to prevent slicing using references/pointers.

### Exercise 3: Container Design
Implement a container system without slicing.

### Exercise 4: Abstract Base
Create an abstract base class that prevents slicing.

### Exercise 5: Smart Pointers
Convert raw pointer code to use smart pointers.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o object_slicing
```

(`<print>` / `std::println` needs GCC 14 or newer.)

### Run:
```bash
./object_slicing
```

### Expected Output:
The program demonstrates:
1. Basic object slicing with data loss
2. Slicing in function parameters
3. Slicing in containers
4. Copy constructor and assignment slicing
5. Prevention using deleted operations
6. Virtual destructors and slicing
7. Return value slicing
8. Assignment operator slicing
9. Compile-time detection
10. Best practices summary

---

## 📊 Visual Concepts

### Slicing Process

```
BEFORE SLICING
┌──────────────────────────────┐
│ Dog object                   │
│ ┌──────────────────────────┐ │
│ │ Animal part:             │ │
│ │   name = "Buddy"         │ │
│ │   vptr → Dog vtable      │ │
│ └──────────────────────────┘ │
│ ┌──────────────────────────┐ │
│ │ Dog part:                │ │
│ │   breed = "Labrador"     │ │
│ └──────────────────────────┘ │
└──────────────────────────────┘

        ↓ Animal animal = dog;

AFTER SLICING
┌──────────────────────────────┐
│ Animal object                │
│ ┌──────────────────────────┐ │
│ │ Animal part:             │ │
│ │   name = "Buddy"         │ │
│ │   vptr → Animal vtable   │ │
│ └──────────────────────────┘ │
└──────────────────────────────┘
    ⚠️ breed is LOST!
    ⚠️ vptr changed!
```

### Comparison: Value vs Reference

```
PASS BY VALUE (Slicing)
┌─────────────┐       ┌─────────────┐
│ Dog         │ copy  │ Animal      │
│ • name      │ ────→ │ • name      │
│ • breed     │       │             │
│ • vptr→Dog  │       │ • vptr→Animal
└─────────────┘       └─────────────┘
                          ⚠️ Sliced!

PASS BY REFERENCE (No Slicing)
┌─────────────┐       ┌─────────────┐
│ Dog         │       │ Animal&     │
│ • name      │ ←───── │ reference   │
│ • breed     │       │ to Dog      │
│ • vptr→Dog  │       │             │
└─────────────┘       └─────────────┘
                          ✅ No slicing!
```

---

## 📚 Related Topics

### Prerequisites:
- **14_VirtualFunctions** - Polymorphism
- **02_Constructor** - Constructors (copy constructors)
- **03_Destructor** and **17_VirtualDestructor** - Virtual destructors

### Coming Next:
- (virtual functions, abstract classes and polymorphism are your existing folders 14, 15 and 16)

### Related Concepts:
- **Smart Pointers** - Memory management
- **RAII** - Resource management
- **Rule of Five** - Special member functions

---

## 🎓 Summary

### What We Learned:

✅ **Object slicing** loses derived class data  
✅ **Happens with** pass-by-value, assignments  
✅ **Polymorphism breaks** when sliced  
✅ **Silent bug** - no compiler warning usually  
✅ **Use references** to prevent slicing  
✅ **Use pointers** (especially smart pointers)  
✅ **Delete copy operations** for safety  
✅ **Abstract base classes** help prevent  
✅ **Virtual destructors** are essential  
✅ **NEVER pass** polymorphic types by value

### The Complete Pattern:

```cpp
// ✅ Modern C++ polymorphic class design
class Animal {
protected:
    string name;
    
public:
    Animal(const string& n) : name(n) { }
    
    // ✅ Delete copy operations (prevent slicing)
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;
    
    // ✅ Pure virtual function (abstract class)
    virtual void speak() const = 0;
    
    // ✅ Virtual destructor
    virtual ~Animal() = default;
};

class Dog : public Animal {
private:
    string breed;
    
public:
    Dog(const string& n, const string& b) 
        : Animal(n), breed(b) { }
    
    void speak() const override {
        cout << name << " barks!\n";
    }
};

// ✅ CORRECT USAGE:

// Pass by reference
void process(const Animal& a) {
    a.speak();
}

// Store in containers with smart pointers
vector<unique_ptr<Animal>> animals;
animals.push_back(make_unique<Dog>("Buddy", "Lab"));

// Return smart pointers
unique_ptr<Animal> createAnimal() {
    return make_unique<Dog>("Max", "Poodle");
}

// Use with references
Dog dog("Luna", "Husky");
const Animal& ref = dog;  // ✅ No slicing
ref.speak();  // ✅ Calls Dog::speak

// ❌ THESE ARE IMPOSSIBLE (compilation errors):
// Animal animal = dog;  // Error - copy deleted
// void process(Animal a);  // Error already at the declaration: abstract parameter type
// vector<Animal> v;  // declaring it compiles, but push_back(dog) does not (copy deleted)
```

---

## ✨ Quick Reference Card

```cpp
// === CAUSES OF SLICING ===
❌ Pass by value:     void func(Base b);
❌ Value assignment:  Base b = derived;
❌ Value container:   vector<Base> v;
❌ Return by value:   Base func();

// === PREVENTION ===
✅ Pass by reference: void func(const Base& b);
✅ Pass by pointer:   void func(Base* b);
✅ Reference:         Base& b = derived;
✅ Pointer:           Base* b = &derived;
✅ Smart pointer:     unique_ptr<Base> ptr;
✅ Pointer container: vector<unique_ptr<Base>> v;

// === DETECTION ===
// Delete copy operations
Base(const Base&) = delete;
Base& operator=(const Base&) = delete;

// Make base abstract
virtual void func() = 0;

// === ALWAYS ===
virtual ~Base() = default;  // Virtual destructor
```

---

## 📝 Best Practices

### ✅ DO:

- **Use references/pointers** for polymorphic types
- **Prefer smart pointers** (unique_ptr, shared_ptr)
- **Make destructors virtual**
- **Delete copy operations** if appropriate
- **Use abstract base classes**
- **Store pointers in containers**
- **Pass by reference** or pointer
- **Return smart pointers**
- **Document non-copyable classes**
- **Test for slicing issues**

### ❌ DON'T:

- **Never pass** polymorphic objects by value
- **Never use** `vector<Base>` for derived objects
- **Never return** polymorphic objects by value
- **Never assign** derived to base variables
- **Don't forget** virtual destructors
- **Don't copy** polymorphic objects carelessly
- **Run clang-tidy** (`cppcoreguidelines-slicing`) - compilers do not warn
- **Don't assume** copy will work correctly
- **Don't use** value semantics for polymorphism
- **Don't make** base classes copyable without reason

---

## 🌟 Real-World Applications

### GUI Frameworks
```cpp
vector<unique_ptr<Widget>> widgets;
widgets.push_back(make_unique<Button>());
widgets.push_back(make_unique<Textbox>());

for (const auto& w : widgets) {
    w->draw();  // ✅ Correct widget drawn
}
```

### Game Entities
```cpp
vector<unique_ptr<Entity>> entities;
entities.push_back(make_unique<Player>());
entities.push_back(make_unique<Enemy>());

for (const auto& e : entities) {
    e->update();  // ✅ Polymorphism works
}
```

### Document Processing
```cpp
vector<unique_ptr<Document>> docs;
docs.push_back(make_unique<PDFDocument>());
docs.push_back(make_unique<WordDocument>());

for (const auto& d : docs) {
    d->save();  // ✅ Correct save method
}
```

---

## 📖 Further Reading

- [C++ FAQ - proper inheritance](https://isocpp.org/wiki/faq/proper-inheritance) (the slicing anchor could not be verified)
- [clang-tidy: cppcoreguidelines-slicing](https://releases.llvm.org/13.0.0/tools/clang/tools/extra/docs/clang-tidy/checks/cppcoreguidelines-slicing.html) (found via search; the check itself ✅ tested)
- "Effective C++" by Scott Meyers - Item 20 (Prefer pass-by-reference-to-const to pass-by-value; from memory, not checked)
- [C++ Core Guidelines - C.67](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-copy-virtual): A polymorphic class should suppress public copy/move ✅ anchor checked

---

## 🎯 Key Takeaways

1. **Object slicing** loses derived class data
2. **Happens silently** - usually no warning
3. **Breaks polymorphism** completely
4. **Pass by value** is the main cause
5. **Use references/pointers** to prevent
6. **Smart pointers** are the best solution
7. **Delete copy operations** for safety
8. **Virtual destructors** are essential
9. **Abstract base** classes help prevent
10. **NEVER use value** semantics for polymorphism

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Never pass polymorphic types by value
   ALWAYS use references or pointers!
   
   ❌ void func(Animal a);          // NEVER!
   ✅ void func(const Animal& a);   // Always!

⚠️  RULE 2: Never store polymorphic types by value
   Use containers of pointers!
   
   ❌ vector<Animal> animals;       // NEVER!
   ✅ vector<unique_ptr<Animal>> v; // Always!

⚠️  RULE 3: Always virtual destructor
   For any class used polymorphically
   
   ✅ virtual ~Base() = default;

⚠️  RULE 4: Consider deleting copy operations
   Prevent slicing at compile time
   
   Base(const Base&) = delete;
   Base& operator=(const Base&) = delete;

⚠️  RULE 5: Prefer smart pointers
   Automatic memory management + no slicing
   
   unique_ptr<Animal> and shared_ptr<Animal>

   NO SLICING - NO EXCEPTIONS!
```

---

**Previous Topic:** [27_ConversionOperators](../27_ConversionOperators/) - Conversion Operators  
**Next Topic:** [29_VirtualFunctions](../29_VirtualFunctions/) - Virtual Functions Deep Dive

---

*Part of the C++ Classes and Objects series*  
*Difficulty: Intermediate to Advanced*  
*Prerequisites: Inheritance, virtual functions, pointers*  
*CRITICAL for Correct Polymorphism*
---

## 🧾 Notes moved from main.cpp

- **Slicing is not a memory leak.** The old lecture listed "Destructor might not be called / memory leaks possible" among the dangers of slicing. A sliced copy is a complete base object with its own base destructor, and the original derived object is destroyed normally ✅ tested (AddressSanitizer: no leak). The real destructor hazard is `delete basePtr` without a virtual destructor, which is undefined behavior (see Mistake 4).
- **Other places where slicing hides** ✅ tested:
  - `Animal copy = *ptr;` - copying through a pointer slices (flagged by clang-tidy).
  - `catch (std::exception e)` catches a `std::runtime_error("disk full")` by value and `e.what()` returns `std::exception` instead of `disk full`; GCC warns with `-Wcatch-value`. Always `catch (const std::exception& e)`.
- **What the sliced object really is:** `typeid(sliced) == typeid(Animal)` and `sizeof(Dog) > sizeof(Animal)` are both true (Example 10). The copy constructor never copies the vptr; the new object gets its own class's vptr.
- **Decision tree:** polymorphic type? No → values are fine. Yes → function parameter: reference/pointer; container: `vector<unique_ptr<Base>>`; return value: `unique_ptr<Base>`; assignment: reference or pointer. Always use pointers/references for polymorphic types.
- **Corrected comparison table:**

| Approach | Slicing? | Recommended |
|---|---|---|
| Pass by value | Yes | Never |
| Pass by reference / pointer | No | Yes |
| `vector<Base>` | Yes | Never |
| `vector<Base*>` | No | OK (manual `delete`) |
| `vector<unique_ptr<Base>>` | No | Best |
| Return by value as `Base` | Yes | Never |
| Return pointer | No | OK |
| Return `unique_ptr<Base>` | No | Best |
| `Base b = derived;` / `b = derived;` | Yes | Avoid |
| `Base& b = derived;` | No | Yes |

- **Code changes in main.cpp:**
  - Example 6: wrong "WRONG" label and the misleading destructor comment fixed; `Derived` (owns a raw array) got its copy operations deleted.
  - Example 9: unused variable `ref` (GCC warning) silenced; added the protected-copy + `clone()` alternative; Example 10 is a real demo now.
  - One-argument constructors (`Animal`, `Shape`, `Vehicle`, `SafeDerived`) made `explicit` (see 23_ExplicitKeyword).
  - ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer.
