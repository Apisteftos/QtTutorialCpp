# Zero-Cost Abstractions

## 🎯 What Are Zero-Cost Abstractions?

**Zero-cost abstractions** mean that high-level, readable code compiles down to the **same machine code** as low-level, hand-written code. You get safety, readability, and maintainability **without sacrificing performance**.

> **"What you don't use, you don't pay for. And what you do use, you couldn't hand code any better."**  
> — Bjarne Stroustrup (Creator of C++)

This is a **fundamental principle** of modern C++ and a design goal of the standard library.

---

## 📚 Core Concept

### The Old Belief (WRONG!)
```cpp
// "I need to write ugly code for performance"
int sum = 0;
for (size_t i = 0; i < vec.size(); ++i) {
    sum += vec[i];
}
```

### The Modern Truth (CORRECT!)
```cpp
// Beautiful code with SAME performance!
int sum = std::accumulate(vec.begin(), vec.end(), 0);
```

**With optimizations enabled (`-O3`), both compile to IDENTICAL assembly code!**

---

## 🔬 What This Folder Contains

### `abstraction_performance.cpp`
Comprehensive examples demonstrating:

1. **Loop Abstractions** - Range-for vs manual loops
2. **Lambdas** - Modern syntax with zero overhead
3. **Smart Pointers** - `unique_ptr` vs raw pointers
4. **Ranges (C++20)** - Functional composition without cost
5. **Template Polymorphism** - Compile-time dispatch
6. **Type Erasure** - Abstraction when needed
7. **Compile-Time Computation** - `constexpr` magic

Each example shows:
- ❌ Low-level "manual" approach
- ✅ High-level abstraction approach
- 🎯 Proof they compile to **identical code**

---

## 🚀 How to Use These Examples

### Compile with Optimizations
```bash
g++ -std=c++20 -O3 -Wall -Wextra abstraction_performance.cpp -o abstraction_test
./abstraction_test
```

### View Assembly (Prove Zero-Cost!)
```bash
g++ -std=c++20 -O3 -S abstraction_performance.cpp -o abstraction.s
cat abstraction.s  # Compare manual vs abstracted code
```

### Compare Different Optimization Levels
```bash
# No optimization - abstractions DO have cost
g++ -std=c++20 -O0 abstraction_performance.cpp -o test_O0

# Full optimization - abstractions are FREE
g++ -std=c++20 -O3 abstraction_performance.cpp -o test_O3

# Time them and compare!
time ./test_O0
time ./test_O3
```

---

## 💡 Key Examples Explained

### Example 1: Loop Abstraction
```cpp
// Manual - what you think is "fast"
for (size_t i = 0; i < vec.size(); ++i) {
    sum += vec[i];
}

// Range-for - cleaner, SAME speed
for (int x : vec) {
    sum += x;
}

// Algorithm - cleanest, SAME speed
std::accumulate(vec.begin(), vec.end(), 0);
```
**All three compile to IDENTICAL assembly!**

### Example 2: Smart Pointers
```cpp
// Raw pointer - manual, error-prone
int* ptr = new int(42);
// ... use ptr ...
delete ptr;  // Easy to forget!

// unique_ptr - automatic, SAME cost!
auto ptr = std::make_unique<int>(42);
// ... use ptr ...
// Automatically deleted - no leaks!
```
**`unique_ptr` has ZERO overhead vs raw pointer!**

### Example 3: Templates vs Virtual Functions
```cpp
// Virtual function - runtime dispatch (slower)
class Base {
    virtual void draw() = 0;
};

// Template - compile-time dispatch (faster!)
template<typename T>
void draw(const T& shape) {
    shape.draw();  // Resolved at compile-time!
}
```
**Templates eliminate virtual function overhead!**

### Example 4: Compile-Time Computation
```cpp
// Runtime computation
int result = factorial(5);  // Computed when program runs

// Compile-time computation
constexpr int result = factorial(5);  // Computed at compile-time!
// Result is a LITERAL in the binary!
```
**`constexpr` moves computation from runtime to compile-time!**

---

## 🎓 Style: Generic and Functional

The examples follow these principles:

### 1. **Functional Style Over Imperative**
```cpp
// ❌ Imperative - hard to read
std::vector<int> result;
for (auto x : data) {
    if (x > 0) result.push_back(x * 2);
}

// ✅ Functional - clear intent, same speed
auto result = data 
    | std::views::filter([](int x) { return x > 0; })
    | std::views::transform([](int x) { return x * 2; });
```

### 2. **Generic Programming**
```cpp
// Write once, works with any type
template<typename Container>
auto sum(const Container& c) {
    return std::accumulate(c.begin(), c.end(), 0);
}
```

### 3. **Template Metaprogramming**
```cpp
// Compute types and values at compile-time
template<typename... Types>
struct TypeList { /* ... */ };
```

### 4. **Always Use Latest C++ Features**
- C++11: Lambdas, move semantics
- C++14: Generic lambdas
- C++17: Structured bindings, `std::optional`
- C++20: Ranges, concepts, coroutines

---

## 📊 Performance Comparison

| Abstraction | Runtime Cost | Readability | Safety |
|-------------|-------------|-------------|--------|
| Raw loops | 0% | ⭐⭐ | ⭐ |
| Range-for | **0%** ✅ | ⭐⭐⭐⭐ | ⭐⭐⭐ |
| Algorithms | **0%** ✅ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| Lambdas | **0%** ✅ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| Smart ptrs | **0%** ✅ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Templates | **0%** ✅ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| Ranges | **0%** ✅ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |

**Key Insight:** Modern abstractions are both safer AND as fast as low-level code!

---

## ⚠️ When Abstractions DO Have Cost

Not all abstractions are zero-cost. These have **minimal overhead**:

### 1. Virtual Functions
```cpp
class Base {
    virtual void func() = 0;  // vtable lookup (small cost)
};
```
**Cost:** One pointer dereference per call  
**Use when:** You need runtime polymorphism

### 2. std::function
```cpp
std::function<int(int)> f = [](int x) { return x * 2; };
```
**Cost:** Type erasure overhead  
**Use when:** You need to store different callables

### 3. Exceptions
```cpp
try {
    throw std::runtime_error("error");
} catch (...) { }
```
**Cost:** Exception table lookups  
**Use when:** Handling exceptional situations

### 4. RTTI
```cpp
dynamic_cast<Derived*>(base);  // Runtime type checking
```
**Cost:** Type information lookups  
**Use when:** You need runtime type inspection

**But even these overheads are TINY and worth it for the safety!**

---

## 🛠️ Compiler Magic

### How Does It Work?

The compiler performs these optimizations:

1. **Inlining** - Replaces function calls with function body
2. **Dead Code Elimination** - Removes unused code
3. **Loop Unrolling** - Expands loops for speed
4. **Constant Folding** - Computes constants at compile-time
5. **Template Instantiation** - Generates specialized code

Example:
```cpp
// Your code
auto sum = std::accumulate(vec.begin(), vec.end(), 0);

// After inlining and optimization
int sum = 0;
for (size_t i = 0; i < vec.size(); ++i) {
    sum += vec[i];
}

// Further optimization
// (Compiler may use SIMD instructions, loop unrolling, etc.)
```

---

## 🎯 Key Takeaways

### ✅ DO:
- Use algorithms (`std::sort`, `std::find`, `std::transform`)
- Use range-based for loops
- Use lambdas instead of functors
- Use smart pointers instead of raw pointers
- Use templates for compile-time polymorphism
- Use `constexpr` for compile-time computation
- Use ranges (C++20) for composable operations

### ❌ DON'T:
- Write manual loops when algorithms exist
- Use raw pointers when smart pointers work
- Avoid abstractions for "performance" (myth!)
- Write low-level code thinking it's faster
- Sacrifice readability for premature optimization

### 🔑 Golden Rule:
**"Write clear, high-level code. Let the compiler optimize it."**

---

## 📖 Further Reading

### Online Resources:
- [Compiler Explorer (godbolt.org)](https://godbolt.org/) - See assembly in real-time
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)

### Books:
- "Effective Modern C++" by Scott Meyers
- "C++ Templates: The Complete Guide" by Vandevoorde & Josuttis
- "From Mathematics to Generic Programming" by Stepanov

### Conference Talks:
- CppCon talks on zero-cost abstractions
- Bjarne Stroustrup's talks on C++ philosophy

---

## 🧪 Experiments to Try

### Experiment 1: Optimization Levels
Compile the same code with different optimization levels and measure:
```bash
g++ -O0 -o test0 abstraction_performance.cpp
g++ -O1 -o test1 abstraction_performance.cpp
g++ -O2 -o test2 abstraction_performance.cpp
g++ -O3 -o test3 abstraction_performance.cpp

time ./test0
time ./test1
time ./test2
time ./test3
```

### Experiment 2: Assembly Comparison
Generate assembly for manual vs abstracted code:
```bash
# Create two files with equivalent logic
g++ -O3 -S manual.cpp -o manual.s
g++ -O3 -S abstracted.cpp -o abstracted.s
diff manual.s abstracted.s  # Should be identical!
```

### Experiment 3: Profiling
Use a profiler to see where time is actually spent:
```bash
g++ -O3 -g abstraction_performance.cpp -o test
perf record ./test
perf report
```

---

## 🚀 Next Steps

After mastering zero-cost abstractions:

1. **Study Functional Programming** → See `../FunctionalProgramming/`
2. **Learn Template Metaprogramming** → See `../TemplateMeta/`
3. **Explore the ranges and generic style** → See `../RangesAndGenericStyle/`
4. **Read C++20 Ranges** → Modern, composable abstractions
5. **Practice on Real Projects** → Apply these principles!

---

## 💭 Remember

> **"Premature optimization is the root of all evil."**  
> — Donald Knuth

Write clear code first. Profile later. Optimize only what matters. Modern C++ gives you the best of both worlds: **beautiful code that's also blazing fast**.

---

**🎓 Master zero-cost abstractions and you'll understand why C++ is the king of performance without sacrificing expressiveness!**

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### Notes

1. High-level code compiles to same assembly as low-level
2. Abstractions are optimized away by the compiler
3. No runtime overhead for safety and convenience
4. Templates enable compile-time polymorphism
5. Modern C++ is both safe AND fast

- 💡 This is why code like this is possible:
  - Beautiful, readable code
  - That's also extremely performant
  - Using functional-style abstractions
  - With zero runtime cost!

**🚀 Key Takeaway:**

Don't write C in C++!  
Use high-level abstractions - they're FREE!

## Notes moved from the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### LoopAbstraction

- Manual loop, range-`for` and `std::accumulate` are expected to compile to equivalent code with optimization. The program prints the measured times; this was not checked in assembly here.

### FunctionObjects

- A functor and a lambda are expected to compile to the same machine code; the lambda is shorter.

### SmartPointers

- `std::unique_ptr` is expected to have no overhead compared with a raw pointer, and it cannot leak.

### RangesExample

 and lazy-evaluated
- Ranges are composable and lazy-evaluated
- ✅ Compile to same code as manual loops!
- ✅ Much more readable and maintainable

### TemplateAbstraction

- ✅ Template version has ZERO virtual function overhead!
- ✅ Function call is inlined at compile-time

### TypeErasure

- ✅ Type erasure: Flexibility when you need it
- ✅ Still better than traditional inheritance
- ✅ Used in std::function, std::any

### CompileTime

- ✅ Compile-time result is a LITERAL in the binary!
- ✅ ZERO runtime cost - computed before program runs
- ✅ This is template metaprogramming power

## Notes moved from the end of main.cpp

The closing comment block of `main.cpp` (compile commands, optimization levels, assembly inspection, summary lists) is lecture material and lives here. These statements about generated code were not checked in this session.

### Compile and run

```bash
g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o zero_cost
./zero_cost
```

View the assembly to compare manual and high-level code:

```bash
g++ -std=c++23 -O3 -S main.cpp -o zero_cost.s
```

### Optimization levels

- `-O0`: no optimization (abstractions have a cost)
- `-O1`: basic optimization
- `-O2`: full optimization
- `-O3`: aggressive optimization (abstractions typically disappear)

### Assembly inspection

1. Compile with `-S` (generates assembly).
2. Look at `sum_manual`, `sum_range_for` and `sum_algorithm`.
3. With optimization they are expected to produce equivalent code — compare them yourself.

### Zero-cost examples

- Range-based `for` loops
- Lambdas vs functors
- Smart pointers vs raw pointers
- `std::` algorithms vs manual loops
- Templates vs virtual functions
- `constexpr` vs runtime computation

### When abstractions have a cost

- Virtual functions (vtable lookup)
- `std::function` (type-erasure overhead)
- Exceptions (table lookups)
- RTTI (`dynamic_cast`, `typeid`)

These costs are usually small and worth it for the flexibility and safety.

### Next steps

- Experiment with `-O0` vs `-O3`.
- Compare assembly output.
- Profile your own abstractions.
- Use Compiler Explorer (godbolt.org) to see the assembly live.

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output matches the original `std::cout` version except that the lecture text moved into *Notes moved from main.cpp* is no longer printed (and the remaining example titles were renumbered where an example was removed).
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer. UBSan reports signed `int` overflow in the benchmark sum loops (this is how the original code is written; the overflow is not fixed here).

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
