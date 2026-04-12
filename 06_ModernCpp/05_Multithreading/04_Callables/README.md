# Callables — All Ways to Pass Work to std::thread

A **callable** is anything that can be called with `()`.
`std::thread` accepts any callable as its first argument.

---

## 1. Regular function

```cpp
void func(int id) { std::cout << id; }

std::thread t(func, 100);
t.join();
```

The simplest form — pass the function name (decays to function pointer).

---

## 2. Function with multiple parameters

```cpp
void func(int a, double b, std::string c) { ... }

std::thread t(func, 42, 3.14, "Hello");
t.join();
```

All arguments after the callable are forwarded to the function.
They are **copied** by default — use `std::ref()` for references.

---

## 3. Function with return value

```cpp
int compute(int x) { return x * 2; }

std::thread t(compute, 21);
t.join();
// Return value is IGNORED — std::thread discards it
```

To capture a return value use `std::async` / `std::future` instead:
```cpp
auto fut = std::async(compute, 21);
int result = fut.get();   // 42
```

---

## 4. Reference parameters — must use std::ref()

```cpp
void increment(int& counter) { counter++; }

int n = 10;
std::thread t(increment, std::ref(n));   // std::ref() required
t.join();
// n is now 11
```

Without `std::ref()`, the argument is copied and the original is unchanged.
This is a very common threading bug.

---

## 5. Function pointer

```cpp
void task() { std::cout << "task\n"; }

void (*fptr)() = task;
std::thread t(fptr);
t.join();
```

---

## 6. Lambda expressions

The most common and flexible way to pass work to a thread:

```cpp
// Simple lambda
std::thread t([]() { std::cout << "hello\n"; });
t.join();

// Lambda with parameters
std::thread t([](int x) { std::cout << x; }, 42);
t.join();

// Capture by value [=]
int val = 10;
std::thread t([val]() { std::cout << val; });   // copy of val
t.join();

// Capture by reference [&] — BE CAREFUL
int val = 10;
std::thread t([&val]() { val = 20; });           // modifies original
t.join();
// val is now 20

// Mutable lambda — modify captured copy
std::thread t([val]() mutable { val++; std::cout << val; });
t.join();
// original val unchanged
```

### Capture safety warning
```cpp
// DANGEROUS — val may be destroyed before thread uses it
void startThread() {
    int val = 10;
    std::thread t([&val]() { std::cout << val; });   // val on stack!
    t.detach();   // if detached, val may be gone when thread runs
}
```
Prefer capture by value `[=]` for detached threads to avoid dangling references.

---

## 7. Functor (callable object)

```cpp
class Functor {
public:
    void operator()(int id) const {
        std::cout << "Functor id: " << id << '\n';
    }
};

Functor f;
std::thread t(f, 42);   // a copy of f is made
t.join();
```

---

## 8. Stateful functor

```cpp
class Counter {
    int count;
public:
    Counter(int n) : count(n) {}
    void operator()() { std::cout << ++count; }
};

Counter c(100);
std::thread t(c);   // copy of c — original c unchanged after thread
t.join();
```

The thread gets a **copy** of the functor. If you need the thread to modify
the original, pass via `std::ref`:
```cpp
std::thread t(std::ref(c));
```

---

## 9. Class member function

```cpp
class MyClass {
public:
    void work(int x) { std::cout << x; }
};

MyClass obj;
std::thread t(&MyClass::work, &obj, 42);
//            ^ method ptr   ^ object ptr  ^ args
t.join();
```

Syntax: `&ClassName::method`, pointer to object, then arguments.
The object must remain alive for the duration of the thread.

---

## 10. Static member function

```cpp
class Util {
public:
    static void task(int id) { std::cout << id; }
};

std::thread t(Util::task, 42);   // no object needed
// or:
std::thread t(&Util::task, 42);
t.join();
```

---

## 11. std::function wrapper

```cpp
#include <functional>

std::function<void(int)> f = [](int x) { std::cout << x; };
std::thread t(f, 99);
t.join();
```

`std::function` adds a small overhead but allows storing any callable
in a variable, container, or passing it around.

---

## 12. std::bind

```cpp
void func(int a, int b, int c) { ... }

// Bind some args, leave others as placeholders
auto bound = std::bind(func, 1, std::placeholders::_1, 3);
std::thread t(bound, 2);   // 2 fills _1
t.join();
```

> Prefer lambdas over `std::bind` in modern C++ — lambdas are clearer,
> faster to compile, and easier to debug.

---

## 13. Move-only parameters — std::move()

```cpp
void process(std::unique_ptr<int> ptr) { std::cout << *ptr; }

auto p = std::make_unique<int>(42);
std::thread t(process, std::move(p));   // move into thread
// p is now nullptr
t.join();
```

`std::thread` normally copies arguments. For move-only types,
use `std::move()` to transfer ownership into the thread.

---

## Summary

| Callable type | Syntax | Notes |
|--------------|--------|-------|
| Regular function | `thread t(func, args...)` | Simplest |
| Lambda | `thread t([](){ }, args...)` | Most flexible, preferred |
| Functor | `thread t(obj, args...)` | Copied by default |
| Member function | `thread t(&Class::method, &obj, args...)` | Object must stay alive |
| Static method | `thread t(Class::method, args...)` | No object needed |
| `std::function` | `thread t(func_wrapper, args...)` | Slight overhead |
| `std::bind` | `thread t(bound, args...)` | Prefer lambda instead |
| Move-only args | `thread t(func, std::move(obj))` | unique_ptr etc. |
| Reference args | `thread t(func, std::ref(var))` | Modifies original |
