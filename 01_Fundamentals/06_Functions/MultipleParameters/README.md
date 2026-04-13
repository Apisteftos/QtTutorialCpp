# Multiple Parameters — C++

A function can take any number of parameters of different types.

---

## Example from this file

```cpp
void myFunction(std::string fname, int age) {
    std::cout << fname << " Refsnes. " << age << " years old.\n";
}

myFunction("Liam",  3);
myFunction("Jenny", 14);
myFunction("Anja",  30);
```

---

## Important note from this file

```cpp
// This does NOT work — default param before non-default
void myFunction(std::string fname = "Konstantinos", int age);  // ERROR

// This works — all params have values supplied at call site
void myFunction(std::string fname, int age) { ... }            // OK
```

Default parameters must always be **at the end** of the parameter list.

---

## Parameter passing guidelines

```cpp
// Small types — pass by value
void fn(int x, double y, bool flag);

// Large objects read-only — pass by const reference (no copy!)
void fn(const std::string& name, const std::vector<int>& data);

// Large objects to modify — pass by reference
void fn(std::string& name, std::vector<int>& data);

// Mixed
void process(int id,
             const std::string& name,   // read-only string
             std::vector<int>& results, // output vector
             bool verbose = false);     // default param at end
```

---

## Parameter order convention

Put related parameters together and the most important first:

```cpp
// Good — id first, details after
void createUser(int id, const std::string& name, int age);

// Output parameters last (if unavoidable)
void compute(int input, int& result);
```

---

## Calling with named arguments (C++20 — not yet widely used)

Standard C++ does not have named arguments.
Use a struct for clarity when many params are needed:

```cpp
// Instead of this — hard to read at call site
void configure(bool logging, bool ssl, int timeout, int retries);
configure(true, false, 30, 3);   // what do these mean?

// Use a struct
struct Config { bool logging=false; bool ssl=false;
                int timeout=30; int retries=3; };
void configure(const Config& cfg);
configure({.logging=true, .retries=5});   // C++20 designated initializers
```
