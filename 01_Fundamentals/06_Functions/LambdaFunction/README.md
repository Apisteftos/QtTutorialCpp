# Lambda Functions — C++

A **lambda** is an anonymous function defined inline where it is needed.
It can capture variables from the surrounding scope.

---

## Syntax

```
[ captures ] ( parameters ) -> return_type { body }
```

---

## Examples from this file

### No capture

```cpp
auto greet = []() {
    std::cout << "Hello from lambda!\n";
};
greet();
```

### With parameters

```cpp
auto add = [](int a, int b) { return a + b; };
std::cout << add(5, 3);   // 8
```

### Capture by value — owns a copy

```cpp
int multiplier = 10;
auto multiply = [multiplier](int x) { return x * multiplier; };
multiply(7);   // 70 — multiplier is a copy, outer unchanged
```

### Capture by reference — modifies original

```cpp
int counter = 0;
auto increment = [&counter]() { counter++; };
increment(); increment(); increment();
// counter = 3
```

### With STL — sort descending

```cpp
std::sort(numbers.begin(), numbers.end(), [](int a, int b) {
    return a > b;
});
```

### for_each callback

```cpp
std::for_each(values.begin(), values.end(), [](int x) {
    std::cout << x * x << " ";   // print squares
});
```

### Capture all by value `[=]`

```cpp
int a = 5, b = 10;
auto fn = [=]() { std::cout << a << " " << b; };   // copies of a and b
```

### Capture all by reference `[&]`

```cpp
auto fn = [&]() { a += 10; b += 10; };   // modifies originals
fn();
// a = 15, b = 20
```

### Explicit return type

```cpp
auto divide = [](double x, double y) -> double {
    if (y == 0) return 0;
    return x / y;
};
```

---

## Capture modes summary

| Capture | Meaning |
|---------|---------|
| `[]` | Nothing captured |
| `[=]` | All by value (copies) |
| `[&]` | All by reference |
| `[x]` | Only x, by value |
| `[&x]` | Only x, by reference |
| `[=, &y]` | All by value, except y by ref |

---

## Lambda vs function pointer

```cpp
// Function pointer — no capture allowed
void (*fp)(int) = [](int x) { std::cout << x; };   // OK (no capture)
void (*fp)(int) = [y](int x) { std::cout << x*y; }; // ERROR — has capture

// std::function — accepts capturing lambdas
std::function<void(int)> fn = [y](int x) { std::cout << x*y; };  // OK
```
