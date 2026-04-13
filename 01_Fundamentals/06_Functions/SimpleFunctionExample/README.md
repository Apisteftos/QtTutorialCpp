# Functions Fundamentals — C++23

A **function** is a named, reusable block of code.
This file covers all fundamental function concepts in one place.

---

## Anatomy

```
returnType  functionName  ( parameters )  { body }
   int         add        ( int a, int b ) { return a + b; }
```

---

## 1. Basic function — no params, no return

```cpp
void sayHello() {
    std::cout << "Hello!\n";
}
sayHello();   // call — reusable
```

---

## 2. Function with parameters

```cpp
void greet(const std::string& name) {
    std::println("Hello, {}!", name);
}
greet("Kostas");
```

**Terminology:**
- **parameter** — variable in the function definition: `(const string& name)`
- **argument**  — value passed at the call site: `greet("Kostas")`

---

## 3. Function with return value

```cpp
int add(int a, int b) { return a + b; }
double divide(double a, double b) {
    if (b == 0.0) return 0.0;   // guard
    return a / b;
}

int sum = add(10, 25);                // 35
int nested = add(add(1,2), 3);        // nested calls — 6
```

---

## 4. void function — performs action, no return

```cpp
void printSeparator(char c, int n) {
    for (int i = 0; i < n; ++i) std::cout << c;
    std::cout << '\n';
}
printSeparator('-', 20);
```

---

## 5. Declaration (prototype) before main

```cpp
// Prototype — just the signature, defined later
int multiply(int a, int b);
double circleArea(double r);

int main() {
    multiply(6, 7);       // can call — prototype seen
    circleArea(5.0);
}

// Actual definitions — after main
int    multiply(int a, int b)    { return a * b; }
double circleArea(double r)      { return 3.14159 * r * r; }
```

In real projects, prototypes go in **header files** (`.h`).

---

## 6. Multiple parameters, mixed types

```cpp
void describeUser(const std::string& name, int age, bool active) {
    std::println("{} age={} active={}", name, age, active);
}

double calcBMI(double weight, double height) {
    return weight / (height * height);
}
```

Pass large objects by `const&` to avoid expensive copies.

---

## 7. Default parameters

```cpp
void connect(const std::string& host,
             int port    = 5060,
             bool secure = false) { ... }

connect("192.168.1.1");              // port=5060, secure=false
connect("192.168.1.1", 443);         // port=443,  secure=false
connect("192.168.1.1", 443, true);   // all specified
```

**Rule:** defaults must be at the **end** of the parameter list.

---

## 8. Function overloading

Same name, different parameter types or count:

```cpp
void print(int x)               { std::println("int: {}",    x); }
void print(double x)            { std::println("double: {}", x); }
void print(const std::string& x){ std::println("str: {}",    x); }

print(42);       // calls int version
print(3.14);     // calls double version
print("hello");  // calls string version
```

---

## 9. inline function

```cpp
inline int square(int x)                { return x * x; }
inline int clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}
```

- Compiler **may** replace the call with the function body (no call overhead)
- Best for small, frequently called functions
- Compiler may ignore the hint if it decides otherwise

---

## 10. Practical patterns

```cpp
// Greet with default title
void greet(const std::string& name, const std::string& title = "Mr") {
    std::println("Good day, {}. {}!", title, name);
}

// Calculate
double percentOf(double pct, double total) { return (pct/100.0)*total; }

// Validate — return bool
bool isValidPort(int port) { return port >= 1 && port <= 65535; }
bool isValidIP(const std::string& ip) {
    return !ip.empty() && ip.find('.') != std::string::npos;
}
```

---

## Summary

| Signature | Description |
|-----------|-------------|
| `void fn()` | No params, no return |
| `void fn(int x)` | With params, no return |
| `int fn(int x)` | With params, with return |
| `int fn(int x, int y=0)` | Default parameter |
| `inline int fn(int x)` | Inline hint |
| `int fn(int a, int b);` | Prototype (declaration) |

---

## Parameter passing quick guide

```
int, double, bool    → by value        fn(int x)
large object read    → const ref       fn(const string& s)
large object modify  → ref             fn(string& s)
```
