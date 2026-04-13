# constexpr — Compile-Time Evaluation in C++

`constexpr` tells the compiler that a variable or function **can be evaluated
at compile time**. The result is baked into the binary — zero runtime cost.

---

## Example from this file

```cpp
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    return (n <= 1) ? n : fibonacci(n-1) + fibonacci(n-2);
}

constexpr int fact5  = factorial(5);    // 120 — computed at compile time
constexpr int fib10  = fibonacci(10);   // 55  — computed at compile time
constexpr int arraySize = 5;
int myArray[arraySize];                 // valid — size is compile-time constant
```

---

## constexpr variable

```cpp
constexpr int SIZE   = 64;      // compile-time constant
constexpr double PI  = 3.14159; // compile-time constant

int arr[SIZE];                  // OK — SIZE known at compile time
```

---

## constexpr function

Can be evaluated at compile time **if** called with constant arguments.
Falls back to runtime evaluation if called with runtime values:

```cpp
constexpr int square(int x) { return x * x; }

constexpr int s1 = square(5);   // compile time → s1 = 25 baked in binary
int           n  = getInput();
int           s2 = square(n);   // runtime → n not known at compile time
```

---

## constexpr vs const

```cpp
const int a = getValue();   // runtime value — NOT a compile-time constant
constexpr int b = 64;       // compile-time constant

int arr1[a];   // ERROR — a is not constexpr
int arr2[b];   // OK    — b is constexpr
```

---

## Where constexpr is required

```cpp
// Array size
constexpr int N = 10;
int arr[N];                          // needs compile-time constant

// Template argument
std::array<int, factorial(4)> a;     // needs compile-time constant

// switch case labels
switch (val) { case factorial(3): break; }  // must be compile-time
```

---

## Use cases

✅ Mathematical lookup tables (factorial, Fibonacci, sine)
✅ Array sizes determined by formula
✅ Protocol constants and magic numbers
✅ Template arguments
✅ Hash functions evaluated at compile time
