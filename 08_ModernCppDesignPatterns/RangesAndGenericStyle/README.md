# Ranges and Generic Style

Two examples in a style that prefers **generic algorithms and ranges over hand-written loops**, and uses templates as a tool where they remove duplication or move checks to compile time — not as a goal in themselves.

## Files in this folder

1. **`README.md`** (this file) — overview of the style
2. **`01_generic_templates/`** — compile-time polymorphism, generic algorithms, type traits / concepts, `constexpr`, type-safe units, variadic templates, perfect forwarding
3. **`02_range_pipelines/`** — range pipelines, lazy evaluation, reusable named operations, range algorithms

Each example has its own `main.cpp`, `Readme.md` and `CMakeLists.txt`.

## Learning objectives

- Understand why a functional style (algorithms and views) can be clearer than raw loops
- Use range-based programming
- Apply templates where they help
- Know when an abstraction is free and when you must measure

---

## Key techniques

### Technique 1: Ranges over loops

❌ **Traditional imperative:**
```cpp
std::vector<int> result;
for (const auto& item : data) {
    if (item > 10) {
        result.push_back(item * 2);
    }
}
```

✅ **Pipeline:**
```cpp
auto result = data
    | std::views::filter([](int x) { return x > 10; })
    | std::views::transform([](int x) { return x * 2; });
```

Why: composable, lazy, clear intent. Optimizing compilers often produce comparable code, but this should be measured for the case at hand.

### Technique 2: Compile-time instead of runtime polymorphism

❌ **Runtime polymorphism:**
```cpp
class Shape {
    virtual double area() = 0;  // virtual call
};
```

✅ **Templates:**
```cpp
template<typename Shape>
double calculate_area(const Shape& shape) {
    return shape.area();  // resolved at compile time
}
```

Why: no virtual call, better inlining, compile-time errors. The cost is that the type must be known at compile time.

### Technique 3: Type lists

```cpp
template<typename... Types>
struct TypeList {};

// Filter types at compile time
template<typename List, template<typename> class Pred>
struct Filter;
```

Useful for type-safe variants, generic visitors and compile-time dispatch. See `../TemplateMeta/04_type_list/`.

### Technique 4: Generic algorithms

❌ **Specific implementation:**
```cpp
int sum_vector(const std::vector<int>& vec) {
    int sum = 0;
    for (int x : vec) sum += x;
    return sum;
}
// Only works with vector<int>
```

✅ **Generic:**
```cpp
template<typename Container>
auto sum(const Container& c) {
    return std::accumulate(c.begin(), c.end(),
                           typename Container::value_type{});
}
// Works with any container of addable values
```

---

## Key principles

### 1. Readability first
Code should read like **what it does**, not **how it does it**.

```cpp
// ❌ How (imperative)
for (size_t i = 0; i < vec.size(); ++i) {
    if (vec[i] % 2 == 0) {
        result.push_back(vec[i] * vec[i]);
    }
}

// ✅ What (functional)
auto result = vec
    | filter(is_even)
    | transform(square);
```

### 2. Composability
Build complex operations from simple, reusable parts.

```cpp
auto pipeline = filter(positive)
              | transform(square)
              | take(10);

auto result1 = data1 | pipeline;
auto result2 = data2 | pipeline;  // reuse
```

(The snippet is schematic; with the standard library you write `std::views::filter(...)` and so on.)

### 3. Type safety
Catch errors at **compile time**, not at run time.

```cpp
template<typename T>
requires std::integral<T>
T square(T x) { return x * x; }

square(5);      // ✅ OK
square(5.0);    // ❌ compile error
```

### 4. Low runtime cost
Abstractions should compile to efficient code.

```cpp
// High-level
auto result = std::accumulate(vec.begin(), vec.end(), 0);

// Usually compiles to code equivalent to:
int result = 0;
for (size_t i = 0; i < vec.size(); ++i) {
    result += vec[i];
}
```

### 5. Clarity over cleverness

```cpp
// ❌ Nested calls
auto x = f(g(h(data)));

// ✅ Reads in the order of execution
auto x = data | h() | g() | f();
```

---

## Performance mindset

1. Write clear code first
2. Profile to find bottlenecks
3. Optimize only what matters
4. Prefer abstractions that cost nothing at run time — and check with a measurement

```
1. Algorithm complexity    (O(n) vs O(n²))    ← most important
2. Cache locality          (data structures)
3. Compiler optimizations  (abstraction removal)
4. Micro-optimizations     (rarely needed)    ← least important
```

---

## C++ standards used

- C++11: lambdas, move semantics
- C++14: generic lambdas
- C++17: structured bindings, `if constexpr`
- C++20: ranges, concepts
- C++23: `std::print` / `std::println` (used by the examples)

## Recommended reading

### Books
- **"Effective Modern C++"** — Scott Meyers
- **"C++ Templates: The Complete Guide"** — Vandevoorde, Josuttis, Gregor
- **"Functional Programming in C++"** — Ivan Čukić

### Online
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Compiler Explorer](https://godbolt.org/) — look at the generated assembly
- CppCon talks on ranges and generic programming

---

## Learning path

### Beginner → intermediate
1. Start with `../FunctionalProgramming/`
2. Master ranges and algorithms
3. Understand lambda composition

### Intermediate → advanced
4. Study `../TemplateMeta/`
5. Learn type traits, SFINAE and concepts
6. Build type lists

### Advanced
7. Study the two examples in this folder
8. Combine the techniques
9. Focus on API design and maintainability

## Practice projects

1. **Data pipeline** — a composable data-processing system
2. **Type-safe units** — compile-time dimensional analysis
3. **Generic algorithms** — implement STL-style algorithms
4. **Range adaptor** — a simplified `views::filter`
5. **Expression templates** — for mathematical operations

## Mindset

- ✅ Use algorithms, not raw loops
- ✅ Use smart pointers, not raw owning pointers
- ✅ Use ranges, not manual iteration
- ✅ Use templates, not runtime polymorphism (when the type is known at compile time)
- ✅ Use `constexpr`, not macros (when possible)

But also:
- ⚠️ Do not be dogmatic — use the right tool for the job
- ⚠️ Readability > cleverness
- ⚠️ Profile before optimizing
- ⚠️ A simple solution beats a complex abstraction when it works

## Checklist

**Core concepts**
- [ ] Functional style in C++
- [ ] C++20 ranges
- [ ] Practical use of templates
- [ ] Generic, reusable code

**Advanced techniques**
- [ ] Custom range adaptors
- [ ] Type lists
- [ ] Compile-time DSLs
- [ ] Optimizing compile times

---

**Remember:** quality code is not just about making it work — it is about making it **right**.
