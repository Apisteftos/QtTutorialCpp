# 01 – Data Types in C++23

This project demonstrates all fundamental C++ data types, including C++17 and C++23 additions, compiled with GCC 14 and Qt6.

## What is covered

### Signed types
| Type | Size | Example value |
|------|------|---------------|
| `int` | 4 bytes | `45` |
| `float` | 4 bytes | `23.78f` |
| `double` | 8 bytes | `456.89` |
| `char` | 1 byte | `'C'` |
| `bool` | 1 byte | `true` |
| `short` | 2 bytes | `12` |
| `long` | 8 bytes | `14L` |
| `long long` | 8 bytes | `35LL` |
| `long double` | 16 bytes | `3.141592653589793238L` |
| `wchar_t` | 4 bytes | `L'Ω'` |
| `std::wstring` | 32 bytes | `L"Με λένε Κώστα"` |

### Unsigned types
| Type | Max value |
|------|-----------|
| `unsigned int` | 4,294,967,295 |
| `unsigned short` | 65,535 |
| `unsigned long` | 4,294,967,295 |
| `unsigned long long` | 18,446,744,073,709,551,615 |
| `unsigned char` | 255 |

### Fixed-width integers (`<cstdint>`) — C++11
Guaranteed exact sizes regardless of platform. Essential for embedded, protocol buffers, and binary data.

| Type | Size | Range |
|------|------|-------|
| `int8_t` | 1 byte | -128 to 127 |
| `int16_t` | 2 bytes | -32,768 to 32,767 |
| `int32_t` | 4 bytes | -2,147,483,648 to 2,147,483,647 |
| `int64_t` | 8 bytes | -9.2e18 to 9.2e18 |
| `uint8_t` | 1 byte | 0 to 255 |
| `uint16_t` | 2 bytes | 0 to 65,535 |
| `uint32_t` | 4 bytes | 0 to 4,294,967,295 |
| `uint64_t` | 8 bytes | 0 to 18,446,744,073,709,551,615 |

> **Note:** `int8_t` and `uint8_t` are typedefs for `signed char` / `unsigned char`.
> Always cast them to `(int)` before printing with `wcout`, otherwise they print as a character symbol.

### Other types
| Type | Header | Notes |
|------|--------|-------|
| `std::byte` | `<cstddef>` | C++17. A semantic byte — not a number. Use `std::to_integer<int>()` to print. |
| `int*` / `nullptr_t` | — | Null pointer types |
| `auto` | — | Compiler-deduced type. `auto x = 42` → `int` |
| `decltype` | — | Deduces type from an expression |
| `std::complex<double>` | `<complex>` | Real + imaginary number |
| `void*` | — | Raw pointer to unknown type |

### C++23 additions
| Feature | Header | Notes |
|---------|--------|-------|
| `std::println` | `<print>` | Formatted output. Replaces `printf` and `cout`. |
| `std::expected<T,E>` | `<expected>` | Value-or-error type. Modern alternative to error codes. |
| `std::stacktrace` | `<stacktrace>` | Runtime call stack inspection. Requires linking `stdc++exp`. |

---

## Why `std::wcout` everywhere?

On Linux, `stdout` has an **orientation** — it is either narrow (`char`) or wide (`wchar_t`).
The orientation is locked on the **first write**.

```cpp
std::cout << "hello";   // stdout is now NARROW
std::wcout << L"Ω";     // FAILS — stdout is already narrow → garbled output
```

Once `std::cout` writes anything, `std::wcout` on the same terminal will produce garbage or nothing.
The fix is to **use only one stream throughout** — in this project `std::wcout` is used everywhere
so that Greek characters (`wchar_t`, `std::wstring`) print correctly.

Also required at the top of `main()`:
```cpp
std::setlocale(LC_ALL, "");          // tell the C runtime to use the system locale
std::locale::global(std::locale("")); // tell the C++ runtime the same
std::ios::sync_with_stdio(false);    // decouple C and C++ streams
```

### Why does `std::println` appear at the end despite being called last?
`std::wcout` and `std::println` write to different internal buffers.
`std::println` flushes immediately; `std::wcout` does not unless told to.
To guarantee ordering, flush `std::wcout` before calling `std::println`:

```cpp
std::wcout.flush();
std::println("std::println (C++23): int = {}, float = {:.2f}", intNum, floatNum);
```

---

## Build requirements

- Qt 6.x
- GCC 14+ (for `<print>`, `<expected>`, `<stacktrace>`)
- CMake 3.14+
- C++23 standard

## CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.14)
project(DataTypes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(DataTypes main.cpp)

target_link_libraries(DataTypes
    Qt${QT_VERSION_MAJOR}::Core
    stdc++exp   # required for <stacktrace> and <print> on GCC
)
```

> `stdc++exp` is the GCC experimental library that provides C++23 standard library features
> not yet fully stable in the main `libstdc++`. Required on GCC 13/14.
