# Function Overloading — C++

Instead of defining two functions that do the same thing with different types,
you can **overload** one function name — the compiler picks the correct version
based on the argument types at the call site.

---

## Example from this file

```cpp
int    plusFunc(int x,    int y)    { return x + y; }
double plusFunc(double x, double y) { return x + y; }

int    r1 = plusFunc(8, 5);       // calls int version    → 13
double r2 = plusFunc(4.3, 6.26);  // calls double version → 10.56
```

---

## How the compiler chooses

```
plusFunc(8, 5)       → arguments are int, int   → calls int version
plusFunc(4.3, 6.26)  → arguments are double     → calls double version
```

The compiler matches argument types to parameter types at **compile time** —
no runtime overhead.

---

## Rules

```cpp
// Valid — different parameter TYPES
int    add(int a,    int b);
double add(double a, double b);
std::string add(std::string a, std::string b);

// Valid — different number of parameters
void draw(int x, int y);
void draw(int x, int y, int radius);

// INVALID — return type alone is NOT enough
int    getValue();
double getValue();   // ERROR — ambiguous, same parameters
```

---

## Why overloading instead of different names

```cpp
// Without overloading — repetitive names
int    addInt(int a, int b);
double addDouble(double a, double b);

// With overloading — clean, unified interface
int    add(int a, int b);
double add(double a, double b);
```

---

## Common uses

```cpp
// print anything
void print(int x);
void print(double x);
void print(const std::string& x);

// constructors are always overloaded
MyClass();
MyClass(int id);
MyClass(int id, const std::string& name);
```
