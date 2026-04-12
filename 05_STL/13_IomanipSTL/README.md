# IomanipSTL — I/O Manipulation in C++23

`<iomanip>` provides stream manipulators for formatting output.
Most manipulators are **sticky** — they stay active until changed.
Exception: `setw` resets after each use.

---

## Header

```cpp
#include <iomanip>
#include <iostream>
```

---

## setw — field width

```cpp
std::cout << std::setw(10) << "hello" << '\n';  // "     hello"
std::cout << std::setw(10) << 42      << '\n';  // "        42"
```

`setw` affects **only the next output** — not sticky.

---

## setfill — fill character

```cpp
std::cout << std::setfill('0') << std::setw(6) << 42;   // "000042"
std::cout << std::setfill('*') << std::setw(8) << "hi"; // "******hi"
std::cout << std::setfill(' ');  // reset to default
```

`setfill` is sticky — stays until changed.

---

## Alignment

```cpp
std::cout << std::right    << std::setw(10) << "text";  // "      text"
std::cout << std::left     << std::setw(10) << "text";  // "text      "
std::cout << std::internal << std::setw(10) << -42;     // "-       42"
```

---

## setprecision — floating point digits

```cpp
double pi = 3.14159265;
std::cout << std::setprecision(2) << pi;   // 3.1
std::cout << std::setprecision(6) << pi;   // 3.14159 (default)
```

---

## fixed, scientific, defaultfloat

```cpp
double v = 123456.789;

std::cout << std::defaultfloat << v;                         // 123457
std::cout << std::fixed        << std::setprecision(2) << v; // 123456.79
std::cout << std::scientific   << std::setprecision(2) << v; // 1.23e+05

std::cout << std::defaultfloat << std::setprecision(6);  // reset
```

All are **sticky**.

---

## boolalpha

```cpp
std::cout << std::boolalpha   << true << " " << false;  // "true false"
std::cout << std::noboolalpha << true << " " << false;  // "1 0"
```

---

## Numeric bases

```cpp
int n = 255;
std::cout << std::dec      << n;   // 255
std::cout << std::hex      << n;   // ff
std::cout << std::oct      << n;   // 377
std::cout << std::showbase << std::hex << n;  // 0xff
std::cout << std::uppercase << std::hex << n; // FF
std::cout << std::showpos   << 42;            // +42
```

---

## Formatted table pattern

```cpp
std::cout << std::left  << std::setw(16) << "Name"
          << std::right << std::setw(8)  << "Qty"
          << std::fixed << std::setprecision(2)
          << std::setw(10) << "Price" << '\n';

for (const auto& p : products) {
    std::cout << std::left  << std::setw(16) << p.name
              << std::right << std::setw(8)  << p.qty
              << std::setw(10) << p.price << '\n';
}
```

---

## Zero-padded numbers

```cpp
for (int i = 1; i <= 99; ++i)
    std::cout << std::setfill('0') << std::setw(3) << i << '\n';
// 001, 002, ..., 099
```

---

## Hex dump

```cpp
for (unsigned char b : buffer)
    std::cout << std::uppercase << std::hex
              << std::setfill('0') << std::setw(2)
              << (int)b << ' ';
std::cout << std::dec << std::nouppercase;
```

---

## C++23 std::println — modern alternative

```cpp
#include <print>

std::println("{:.2f}", 3.14159);       // 3.14
std::println("{:10.4f}", 3.14159);     // "    3.1416"
std::println("{:<10.2f}|", 3.14);      // "3.14      |"
std::println("{:010.2f}", 3.14);       // "0000003.14"
std::println("{:+.2f}", 3.14);         // "+3.14"
std::println("{:x}", 255);             // "ff"
std::println("{:08b}", 42);            // "00101010"
```

`std::println` is preferred for new code — cleaner, type-safe, no sticky state issues.

---

## Sticky vs non-sticky manipulators

| Manipulator | Sticky? |
|-------------|---------|
| `setw` | ❌ resets after each use |
| `setfill` | ✅ |
| `setprecision` | ✅ |
| `fixed/scientific` | ✅ |
| `left/right/internal` | ✅ |
| `boolalpha` | ✅ |
| `hex/dec/oct` | ✅ |
| `showbase/showpos` | ✅ |
| `uppercase` | ✅ |
