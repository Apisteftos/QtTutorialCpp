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

### Example 2: Function Slicing
Slicing when passing to functions by value.

### Example 3: Container Slicing
Slicing in vectors and other containers.

### Example 4: Copy Constructor Slicing
How copy construction causes slicing.

### Example 5: Preventing Slicing
Using deleted copy operations.

### Example 6: Virtual Destructor
Relationship between destructors and slicing.

### Example 7: Return Value Slicing
Slicing when returning by value.

### Example 8: Assignment Slicing
Slicing through assignment operator.

### Example 9: Compile-Time Detection
Making slicing a compile error.

### Example 10: Best Practices
Summary of prevention techniques.

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
delete ptr;  // ⚠️ Only Base destructor called!
```

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
g++ -std=c++17 object_slicing.cpp -o object_slicing
```

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
- **12_Inheritance** - Inheritance basics
- **Virtual Functions** - Polymorphism
- **09_Constructors** - Copy constructors
- **10_Destructors** - Virtual destructors

### Coming Next:
- **29_VirtualFunctions** - Virtual function details
- **30_AbstractClasses** - Pure virtual functions
- **31_Polymorphism** - Polymorphic design

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
// process(dog);  // Error if process takes Animal by value
// vector<Animal> v;  // Would cause slicing
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
- **Don't ignore** slicing warnings
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

- [C++ FAQ - Object Slicing](https://isocpp.org/wiki/faq/proper-inheritance#slicing)
- "Effective C++" by Scott Meyers - Item 38
- [C++ Core Guidelines - C.67](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-copy-virtual)
- [cppreference.com - Object Slicing](https://en.cppreference.com/w/cpp/language/copy_constructor)

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