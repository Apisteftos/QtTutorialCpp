# Lambdas — C++23

A **lambda** is an anonymous function defined inline where it is needed.
It is the modern C++ replacement for function pointers when you need closures (captured state).

---

## Header

```cpp
#include <functional>   // for std::function (storing lambdas)
#include <algorithm>    // for sort, find_if etc. (using lambdas)
```

---

## Anatomy

```
[ captures ] ( parameters ) -> return_type { body }
      │             │               │           │
      │             │               │           └── code
      │             │               └── optional explicit type
      │             └── inputs (like a normal function)
      └── which outer variables are accessible inside
```

---

## Minimal examples

```cpp
// No parameters, no return
auto greet = []() { std::cout << "Hello!\n"; };
greet();

// With parameters
auto add = [](int a, int b) { return a + b; };
int r = add(3, 4);   // 7

// Explicit return type
auto divide = [](double a, double b) -> double {
    if (b == 0.0) return 0.0;
    return a / b;
};

// Called immediately (IILE)
int result = [](int x) { return x * x; }(5);   // 25
```

---

## Capture modes

```cpp
int x = 10, y = 20;

[]()     { /* no access to x or y */ };
[=]()    { use(x); use(y); };     // all by value (copies)
[&]()    { x++; y++; };           // all by reference (modifies outer!)
[x]()    { use(x); };             // only x, by value
[&x]()   { x++; };                // only x, by reference
[=, &y]() { use(x); y++; };       // all by value except y by ref
[&, x]()  { y++; use(x); };       // all by ref except x by value
```

**By value** — lambda owns a copy. Outer variable unaffected.
**By reference** — lambda accesses the original. Changes are visible outside.

---

## Mutable lambda

By default, captured-by-value variables are `const` inside the lambda.
`mutable` allows modifying the copy:

```cpp
int n = 0;

auto fn = [n]() mutable {
    ++n;   // modifies the COPY — outer n unchanged
    return n;
};

fn();   // returns 1
fn();   // returns 2
// outer n is still 0
```

Useful for stateful generators:

```cpp
auto counter = [n = 0]() mutable { return ++n; };
counter();  // 1
counter();  // 2
counter();  // 3
```

---

## Storing lambdas

```cpp
// auto — lambda's own unique type (fastest, preferred when stored locally)
auto fn = [](int x) { return x * 2; };

// std::function — type-erased, accepts anything callable
std::function<int(int)> fn2 = [](int x) { return x * 2; };

// std::function required when:
//   - storing in a class member
//   - storing in a vector
//   - passing to a function taking std::function
//   - reassigning at runtime

std::vector<std::function<int(int)>> pipeline;
pipeline.push_back([](int x) { return x + 1; });
pipeline.push_back([](int x) { return x * 2; });
pipeline.push_back([](int x) { return x * x; });
```

---

## Lambda with STL algorithms

```cpp
std::vector<int> v = {5, 3, 8, 1, 9, 2};

// sort
std::sort(v.begin(), v.end(), [](int a, int b) { return a < b; });

// find_if
auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 5; });

// count_if
int big = std::count_if(v.begin(), v.end(), [](int x) { return x > 5; });

// transform
std::transform(v.begin(), v.end(), out.begin(), [](int x) { return x*x; });

// remove_if + erase
v.erase(std::remove_if(v.begin(), v.end(), [](int x){ return x%2==0; }), v.end());

// for_each
std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ' '; });

// accumulate with custom op
int product = std::accumulate(v.begin(), v.end(), 1,
                              [](int acc, int x) { return acc * x; });
```

---

## Generic lambda (C++14)

```cpp
auto print = [](auto x) { std::cout << x; };
print(42);       // int
print(3.14);     // double
print("hello");  // const char*

auto add = [](auto a, auto b) { return a + b; };
add(3, 4);       // 7
add(1.5, 2.5);   // 4.0
```

---

## Templated lambda (C++20)

```cpp
// Explicit template parameter — more control than auto
auto typed = []<typename T>(T a, T b) -> T { return a + b; };
typed(3, 4);       // int — 7
typed(1.1, 2.2);   // double — 3.3

// With concepts — constrained
auto intOnly = []<std::integral T>(T a, T b) { return a + b; };
intOnly(5, 3);    // OK
// intOnly(1.5, 2.5); // compile error — not integral
```

---

## Immediately Invoked Lambda Expression (IILE)

```cpp
// Initialize const variable with complex logic
const std::string mode = []() -> std::string {
    int hour = 14;
    if (hour < 12) return "morning";
    if (hour < 18) return "afternoon";
    return "evening";
}();

// Complex const initialization in one expression
const int config = []() {
    int val = 100;
    val *= 2;
    val += 50;
    return val;
}();
```

---

## Lambda returning lambda

```cpp
// Factory pattern
auto multiplierFactory = [](int n) {
    return [n](int x) { return x * n; };
};
auto times3 = multiplierFactory(3);
times3(7);   // 21

// Composition: compose(f,g)(x) = f(g(x))
auto compose = [](auto f, auto g) {
    return [f, g](auto x) { return f(g(x)); };
};

auto doubleSquare = compose(
    [](int x){ return x*2; },
    [](int x){ return x*x; }
);
doubleSquare(3);   // 3*3=9, 9*2=18

// Partial application
auto add = [](int a) {
    return [a](int b) { return a + b; };
};
auto add10 = add(10);
add10(5);   // 15
add10(25);  // 35
```

---

## Lambda vs function pointer vs std::function

| | Lambda (`auto`) | Function pointer | `std::function` |
|--|----------------|-----------------|----------------|
| Captures state | ✅ | ❌ | ✅ |
| Inline definition | ✅ | ❌ | ✅ |
| Storable as member | ❌ (use `std::function`) | ✅ | ✅ |
| Reassignable | ❌ | ✅ | ✅ |
| Performance | Fastest | Fast | Small overhead |
| C-compatible | ❌ (no capture) | ✅ | ❌ |

---

## Common mistakes

```cpp
// 1. Dangling reference — capturing local by ref after it's destroyed
auto bad = [&x]() { return x; };   // OK if x outlives lambda
// DANGER if lambda outlives x (e.g. stored in vector, x goes out of scope)

// 2. Forgetting mutable — trying to modify by-value capture
int n = 5;
auto fn = [n]() { ++n; };   // ERROR — n is const copy
auto fn = [n]() mutable { ++n; };   // OK — mutable allows it

// 3. Thinking mutable modifies the outer variable
auto fn = [n]() mutable { ++n; };
fn(); fn(); fn();
// n in outer scope is STILL 5 — only the copy changed

// 4. auto parameter conflicts with overload resolution
auto fn = [](auto x) { return x * 2; };
fn(5);    // int
fn(5.0);  // double — both work, different instantiations

// 5. Capturing this in member functions
class MyClass {
    int m_val = 42;
    void method() {
        auto fn = [this]() { return m_val; };   // correct
        auto fn2 = [*this]() { return m_val; }; // C++17 — captures copy of *this
    }
};
```

---

## When to use lambdas

✅ Short, inline callbacks for STL algorithms
✅ Event handlers registered once
✅ When the function needs to capture local state
✅ Sorting with custom comparators
✅ Filtering and transforming collections
✅ IILE for complex const initialization
✅ Higher-order functions (factory, compose, partial application)

❌ Long complex logic → define a named function instead
❌ Reused in many places → named function is clearer
❌ C API callback (no capture) → use function pointer
❌ Virtual polymorphism → use virtual functions
