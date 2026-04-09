# FunctionTemplates - Generic Programming Foundation

## 📚 Overview

This folder introduces **Function Templates** - the foundation of generic programming in C++. Templates allow you to write code once that works with any type, providing type-safe generics with zero runtime overhead. They're the backbone of the Standard Template Library (STL) and essential for modern C++ development.

**Function Templates** = Write once, use with any type

This is **ESSENTIAL** for generic programming and understanding the STL.

---

## 🎯 What You'll Learn

- What function templates are and why they exist
- Template syntax and type parameters
- Type deduction (implicit and explicit)
- Multiple template parameters
- Template specialization
- Overloading with templates
- Non-type template parameters
- Return type deduction
- Template constraints (SFINAE)
- Real-world usage patterns
- Best practices and common pitfalls

---

## 📖 Concept Explanation

### What are Function Templates?

**Function templates** are blueprints for creating functions that work with any type. Instead of writing separate functions for int, double, string, etc., you write one template that the compiler uses to generate type-specific functions.

```cpp
// Without templates - need separate functions
int maxInt(int a, int b) { return (a > b) ? a : b; }
double maxDouble(double a, double b) { return (a > b) ? a : b; }
string maxString(string a, string b) { return (a > b) ? a : b; }
// Duplicated code! 💥

// With templates - one function for all types
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}
// Works with any type! ✅
```

**Key Properties:**
- Template is a blueprint, not actual code
- Compiler generates specific functions
- Type-safe (compile-time checking)
- Zero runtime overhead
- One template → many functions

### How Templates Work

```
COMPILATION PROCESS:

1. Template Definition
   ┌─────────────────────────┐
   │ template<typename T>    │
   │ T add(T a, T b) {      │
   │     return a + b;       │
   │ }                       │
   └─────────────────────────┘
         Blueprint only

2. Template Usage
   ┌─────────────────────────┐
   │ int r1 = add(5, 10);   │
   │ double r2 = add(3.1, 2.7);│
   └─────────────────────────┘
         Trigger instantiation

3. Compiler Generates
   ┌─────────────────────────┐
   │ int add(int a, int b) { │
   │     return a + b;       │
   │ }                       │
   │                         │
   │ double add(double a, double b) { │
   │     return a + b;       │
   │ }                       │
   └─────────────────────────┘
         Actual functions

4. Result
   One template → Multiple functions
   Generated at compile time
   Each type gets optimized version
```

### Real-World Analogy

```
Think of templates like a cookie cutter:

WITHOUT TEMPLATES (Cookie Cutters):
  - One cutter for chocolate cookies
  - One cutter for sugar cookies
  - One cutter for ginger cookies
  - Different cutter for each type!

WITH TEMPLATES (Universal Cutter):
  - One cutter shape (template)
  - Works with any dough (type)
  - Same shape, different flavors
  - Make cookies for any occasion!

The template is the shape,
the type is the dough,
the result is delicious cookies! 🍪
```

---

## 💻 Basic Syntax

### Simple Function Template

```cpp
// Template declaration
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Usage - type deduction
int m1 = maximum(10, 20);           // T = int
double m2 = maximum(3.14, 2.71);    // T = double
char m3 = maximum('a', 'z');        // T = char
```

### Multiple Type Parameters

```cpp
template<typename T1, typename T2>
void printPair(T1 first, T2 second) {
    cout << first << ", " << second << "\n";
}

// Different types
printPair(42, 3.14);        // T1=int, T2=double
printPair("Age", 25);       // T1=const char*, T2=int
```

### Explicit Type Specification

```cpp
// Implicit type deduction
maximum(10, 20);            // T deduced as int

// Explicit type specification
maximum<int>(10, 20);       // T explicitly int
maximum<double>(5, 10);     // T explicitly double (converts args)
```

---

## 🔍 Key Concepts

### 1. Type Deduction

The compiler automatically determines the type parameter based on arguments:

```cpp
template<typename T>
void print(T value) {
    cout << value << "\n";
}

print(42);          // T = int
print(3.14);        // T = double
print("Hello");     // T = const char*
print(string("Hi"));// T = string
```

**Rules:**
- Types must match exactly
- No implicit conversions in deduction
- Can specify explicitly when needed

**Example of deduction rules:**
```cpp
template<typename T>
T add(T a, T b) { return a + b; }

add(5, 10);       // ✅ OK - both int
add(5, 10.5);     // ❌ Error - int and double don't match
add<double>(5, 10.5);  // ✅ OK - explicit type
```

### 2. Template Specialization

Sometimes you need custom behavior for specific types:

```cpp
// Generic template
template<typename T>
T add(T a, T b) {
    return a + b;
}

// Specialization for const char*
template<>
string add<const char*>(const char* a, const char* b) {
    return string(a) + string(b);  // Custom behavior
}

// Usage
add(10, 20);           // Uses generic - returns 30
add("Hello", " World");// Uses specialization - concatenates
```

**Syntax:**
```cpp
template<>
ReturnType FunctionName<SpecificType>(parameters) {
    // Custom implementation
}
```

### 3. Overloading with Templates

You can have both template and regular functions with the same name:

```cpp
// Template version
template<typename T>
void display(T value) {
    cout << "Template: " << value << "\n";
}

// Regular version
void display(int value) {
    cout << "Regular: " << value << "\n";
}

display(42);      // Calls regular (exact match preferred)
display(3.14);    // Calls template (no double overload)
display("Hi");    // Calls template
```

**Overload Resolution Priority:**
1. Exact match with non-template function
2. Template with exact match
3. Template with conversion
4. Non-template with conversion

### 4. Non-Type Template Parameters

Templates can have compile-time constant parameters:

```cpp
template<typename T, int Size>
class Array {
    T data[Size];  // Size is compile-time constant
public:
    int size() const { return Size; }
};

Array<int, 5> arr1;     // int array, size 5
Array<double, 10> arr2; // double array, size 10
```

**Allowed Non-Type Parameters:**
- Integral types (int, char, bool)
- Enums
- Pointers
- References
- Must be compile-time constants!

### 5. Return Type Deduction

**C++11 - Trailing Return Type:**
```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```

**C++14 - Full Auto:**
```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) {
    return a + b;  // Type deduced from return
}
```

---

## 📋 Examples in This Folder

### Example 1: The Problem Without Templates
Shows code duplication with separate functions.

### Example 2: Basic Function Template
One template for all types.

### Example 3: Type Deduction
Implicit vs explicit type specification.

### Example 4: Multiple Template Parameters
Different types in one template.

### Example 5: Template Specialization
Custom behavior for specific types.

### Example 6: Overloading with Templates
Templates and regular functions together.

### Example 7: Non-Type Template Parameters
Compile-time constants in templates.

### Example 8: Return Type Deduction
Auto and decltype with templates.

### Example 9: Template Constraints
SFINAE and type restrictions.

### Example 10: Real-World Examples
Practical usage patterns.

---

## 🎓 When to Use Templates

### ✅ Use Templates When:

**Generic algorithms:**
```cpp
template<typename T>
void sort(T* array, int size) {
    // Works with any comparable type
}
```

**Type-independent operations:**
```cpp
template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}
```

**Container classes:**
```cpp
template<typename T>
class Vector {
    T* data;
    // Generic storage
};
```

**Code reuse:**
```cpp
template<typename T>
T min(T a, T b) { return (a < b) ? a : b; }
// One function for all types
```

### ❌ Don't Use Templates When:

- Type-specific behavior needed
- Adds unnecessary complexity
- Simple function used once
- Debugging is more important than generality
- Compile times are critical

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Type Mismatch in Deduction

**Bad:**
```cpp
template<typename T>
T add(T a, T b) { return a + b; }

add(5, 10.5);  // ❌ Error: int and double don't match
```

**Problem:** Type deduction requires exact match

**Fix:**
```cpp
add<double>(5, 10.5);  // ✅ Explicit type
// OR
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```

### ❌ Mistake 2: Forgetting Type Requirements

**Bad:**
```cpp
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;  // Assumes operator<
}

struct NoCompare { int x; };
maximum(NoCompare{1}, NoCompare{2});  // ❌ Error!
```

**Problem:** T must support operator>

**Fix:** Document requirements or use concepts (C++20)

### ❌ Mistake 3: Template in .cpp File

**Bad:**
```cpp
// header.h
template<typename T>
T maximum(T a, T b);

// implementation.cpp
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}
// ❌ Linker error!
```

**Problem:** Templates must be in header or explicitly instantiated

**Fix:** Keep template definition in header

### ❌ Mistake 4: Unnecessary Copies

**Bad:**
```cpp
template<typename T>
void process(T value) {  // Copies large objects!
    // Use value
}
```

**Fix:**
```cpp
template<typename T>
void process(const T& value) {  // ✅ Reference
    // Use value
}
```

---

## 🎯 Practice Exercises

### Exercise 1: Generic Swap
Implement a template swap function that works with any type.

### Exercise 2: Min of Three
Create a template that finds minimum of three values.

### Exercise 3: Generic Print
Write a template that prints any container.

### Exercise 4: Array Sum
Template function that sums array of any numeric type.

### Exercise 5: Type-Safe Compare
Create a comparison template with proper specializations.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++14 function_templates.cpp -o function_templates
```

Note: Requires C++11 minimum, C++14 for some features

### Run:
```bash
./function_templates
```

### Expected Output:
The program demonstrates:
1. Problem without templates
2. Basic template usage
3. Type deduction
4. Multiple parameters
5. Specialization
6. Overloading
7. Non-type parameters
8. Return type deduction
9. Constraints (SFINAE)
10. Real-world patterns

---

## 📊 Visual Concepts

### Template Instantiation

```
TEMPLATE:
┌──────────────────────┐
│ template<typename T> │
│ T max(T a, T b) {   │
│   return (a>b)?a:b; │
│ }                    │
└──────────────────────┘
         │
         │ Compiler sees usage
         │
    ┌────┴────┬────────┬────────┐
    │         │        │        │
    ▼         ▼        ▼        ▼
┌───────┐ ┌────────┐ ┌──────┐ ┌───────┐
│ max   │ │  max   │ │ max  │ │  max  │
│ int   │ │ double │ │ char │ │string │
└───────┘ └────────┘ └──────┘ └───────┘

One template → Multiple functions
Generated at compile time
Each type gets optimized version
```

### Type Deduction

```
FUNCTION CALL:
maximum(10, 20)
    │
    ├─ Argument 1: 10 → type int
    └─ Argument 2: 20 → type int
         │
         ▼
    T = int (deduced)
         │
         ▼
Instantiate: int maximum(int a, int b)
```

### Overload Resolution

```
display(42)
    │
    ├─ Check exact match non-template
    │  → void display(int) exists ✅
    │  → USE THIS
    │
    └─ Don't check template

display(3.14)
    │
    ├─ Check exact match non-template
    │  → No void display(double) ❌
    │
    └─ Check template
       → template<typename T>
       → T = double ✅
       → USE THIS
```

---

## 📚 Related Topics

### Prerequisites:
- **Functions** - Basic function concepts
- **Types** - Understanding C++ types
- **References** - Pass by reference

### Coming Next:
- **33_ClassTemplates** - Template classes
- **34_TemplateSpecialization** - Advanced specialization
- **35_VariadicTemplates** - Variable arguments

### Related Concepts:
- **Generic Programming** - Paradigm
- **STL** - Uses templates extensively
- **Type Traits** - Template metaprogramming

---

## 🎓 Summary

### What We Learned:

✅ **Templates** are blueprints for functions
✅ **Write once**, use with any type
✅ **Type deduction** is automatic
✅ **Compiler generates** specific functions
✅ **Specialization** for custom behavior
✅ **Overloading** with regular functions
✅ **Non-type parameters** for constants
✅ **Return type deduction** with auto/decltype
✅ **Zero overhead** - compile-time only
✅ **Foundation of STL** and generic programming

### The Complete Pattern:

```cpp
// ============================================
// FUNCTION TEMPLATE PATTERNS
// ============================================

// 1. BASIC TEMPLATE
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// 2. MULTIPLE PARAMETERS
template<typename T1, typename T2>
void printPair(T1 first, T2 second) {
    cout << first << ", " << second << "\n";
}

// 3. CONST REFERENCES (efficient)
template<typename T>
void process(const T& value) {
    // No copy for large objects
}

// 4. RETURN TYPE DEDUCTION
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}

// 5. SPECIALIZATION
template<typename T>
T func(T param) {
    // Generic implementation
}

template<>
string func<string>(string param) {
    // Custom for string
}

// 6. NON-TYPE PARAMETER
template<typename T, int Size>
void printArray(T (&arr)[Size]) {
    for (int i = 0; i < Size; ++i) {
        cout << arr[i] << " ";
    }
}

// 7. OVERLOADING
template<typename T>
void display(T value) { }  // Template

void display(int value) { }  // Regular (preferred for int)

// 8. GENERIC ALGORITHMS
template<typename Container>
void printAll(const Container& c) {
    for (const auto& item : c) {
        cout << item << " ";
    }
}
```

---

## ✨ Quick Reference Card

```cpp
// === BASIC SYNTAX ===
template<typename T>
T func(T param) { return param; }

// === USAGE ===
func(42);           // T = int
func<int>(42);      // Explicit

// === MULTIPLE TYPES ===
template<typename T1, typename T2>
void func(T1 a, T2 b) { }

// === NON-TYPE PARAMETER ===
template<typename T, int N>
void func(T (&arr)[N]) { }

// === SPECIALIZATION ===
template<>
void func<int>(int x) { }

// === RETURN TYPE DEDUCTION ===
template<typename T1, typename T2>
auto func(T1 a, T2 b) -> decltype(a+b) {
    return a + b;
}

// === CONST REFERENCE (efficient) ===
template<typename T>
void func(const T& param) { }

// === TYPE REQUIREMENTS ===
// T must support:
// - Copy constructor
// - operator< (for comparisons)
// - operator+ (for addition)
// Document these!
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use meaningful names** (T, U, V or Value, Key)
- **Prefer const references** for parameters
- **Document type requirements** 
- **Keep templates simple**
- **Test with multiple types**
- **Put templates in headers**
- **Use auto for return types** when appropriate
- **Specialize when needed**
- **Consider concepts** (C++20)
- **Provide good error messages**

### ❌ DON'T:

- **Over-complicate templates**
- **Forget type requirements**
- **Ignore compiler errors**
- **Use templates unnecessarily**
- **Forget const-correctness**
- **Make too general**
- **Put in .cpp files** (usually)
- **Assume all types work**
- **Create circular dependencies**
- **Ignore compilation time**

---

## 🌟 Real-World Applications

### STL Algorithms
```cpp
template<typename Iter, typename Func>
void for_each(Iter begin, Iter end, Func f) {
    for (auto it = begin; it != end; ++it) {
        f(*it);
    }
}
```

### Generic Swap
```cpp
template<typename T>
void swap(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}
```

### Container Operations
```cpp
template<typename Container>
typename Container::value_type sum(const Container& c) {
    typename Container::value_type result{};
    for (const auto& item : c) {
        result += item;
    }
    return result;
}
```

### Generic Factories
```cpp
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(forward<Args>(args)...));
}
```

---

## 📖 Further Reading

- [cppreference.com - Function templates](https://en.cppreference.com/w/cpp/language/function_template)
- "C++ Templates: The Complete Guide" by Vandevoorde & Josuttis
- "Effective C++" by Scott Meyers - Items on templates
- [Template Tutorial](https://isocpp.org/wiki/faq/templates)

---

## 🎯 Key Takeaways

1. **Templates** are blueprints, not code
2. **Write once**, use with any type
3. **Type deduction** happens automatically
4. **Compiler generates** specific functions
5. **Zero runtime overhead** - compile-time feature
6. **Type requirements** must be met
7. **Specialization** for custom behavior
8. **Keep in headers** (usually)
9. **Foundation of STL** algorithms
10. **Essential for** modern C++

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Template is a blueprint
   Not actual code until used!
   
   Compiler generates functions on demand

⚠️  RULE 2: Type must support operations
   Document requirements!
   
   max(a,b) needs operator>
   sort needs operator<

⚠️  RULE 3: Prefer const references
   For efficiency!
   
   template<typename T>
   void func(const T& param)

⚠️  RULE 4: Keep in headers
   Or explicitly instantiate!
   
   Templates need full definition visible

⚠️  RULE 5: Test with multiple types
   Don't assume it works!
   
   Test int, double, string, custom types
   
   USE TEMPLATES FOR CODE REUSE AND TYPE SAFETY!
```

---

**Previous Topic:** [31_InitializerLists](../31_InitializerLists/) - Uniform Initialization

**Next Topic:** [33_ClassTemplates](../33_ClassTemplates/) - Template Classes

---

*Part of the Templates and Generic Programming series*
*Difficulty: Intermediate*
*Prerequisites: Functions, types, references*
*CRITICAL for Generic Programming*