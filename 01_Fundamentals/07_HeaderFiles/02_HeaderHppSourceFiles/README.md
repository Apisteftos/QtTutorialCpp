# .h vs .hpp — Header File Extensions in C++

## The core truth

**The compiler treats `.h` and `.hpp` identically.**
Both are pasted into the source file by the preprocessor at `#include` time.
The extension is purely a **convention for human readers**.

```cpp
#include "MathUtils.h"   // processed identically
#include "Stack.hpp"     // processed identically
#include <vector>        // no extension — also processed identically
```

---

## What the convention means

| Extension | Convention | Signals |
|-----------|-----------|---------|
| `.h` | C or C++ header | "This could work in C too" |
| `.hpp` | C++ only header | "This is C++ only — templates, classes, std library" |
| none | Standard library | `<vector>`, `<string>`, `<algorithm>` |

---

## .h — C-compatible header

Use `.h` when the header **could be shared with C code**:

```c
// MathUtils.h — works in both C and C++
int   math_add(int a, int b);
float math_divide(float a, float b);
#define MATH_PI 3.14159f
```

Rules for `.h`:
- No `class` or `struct` with methods
- No templates
- No `std::vector`, `std::string` etc.
- No `auto`, no lambdas
- Uses `#define` for constants (not `constexpr`)

---

## .hpp — C++ only header

Use `.hpp` when the file uses **C++ specific features**:

```cpp
// Stack.hpp — C++ only
#pragma once
#include <vector>
#include <stdexcept>

template<typename T>
class Stack {
public:
    void push(const T& value) { m_data.push_back(value); }
    T&   top()                { return m_data.back(); }
    void pop()                { m_data.pop_back(); }
    bool empty() const        { return m_data.empty(); }
private:
    std::vector<T> m_data;
};
```

---

## Why templates MUST be in the header

Templates are **blueprints** — the compiler generates actual machine code
only when it sees a concrete type like `Stack<int>` or `Stack<std::string>`.

```cpp
// If Stack definition were in Stack.cpp:
// main.cpp uses Stack<int>
// Compiler compiles main.cpp — needs Stack<int> implementation
// But it only sees the declaration in Stack.hpp — ERROR!

// "undefined reference to Stack<int>::push(int const&)"
```

**Rule: template class/function definitions always go in the header** —
`.hpp` signals this to readers.

---

## Header-only library (.hpp)

Small utility classes can live entirely in a `.hpp` — no `.cpp` needed:

```cpp
// Logger.hpp — everything defined here
#pragma once
class Logger {
public:
    void info(const std::string& msg) {
        std::println("[INFO] {}", msg);
    }
};
```

Just `#include "Logger.hpp"` wherever you need it. Nothing to add to CMakeLists.txt.

Popular header-only libraries: **Eigen** (math), **nlohmann/json**, **stb_image**, **Catch2** (testing).

---

## Mixing .h and .hpp in one project

Completely normal — you will do this all the time:

```cpp
#include "MathUtils.h"      // .h — C-style functions + .cpp
#include "Stack.hpp"        // .hpp — template class, header-only
#include "Logger.hpp"       // .hpp — utility class, header-only
#include <iostream>         // system header, no extension
#include <QThread>          // Qt header — uses .h even for C++ classes
```

---

## CMakeLists.txt — what to list

```cmake
add_executable(MyApp
    main.cpp
    MathUtils.cpp       # .h + .cpp pair → list the .cpp

    # Stack.hpp         # header-only → DO NOT list
    # Logger.hpp        # header-only → DO NOT list
)
```

| Type | CMakeLists.txt |
|------|---------------|
| `.h` + `.cpp` pair | List the `.cpp` |
| `.hpp` header-only | Do NOT list — just `#include` it |
| `.hpp` template | Do NOT list — compiler instantiates from `#include` |

---

## File structure — this project

```
08_HppHeaders/
├── CMakeLists.txt      ← only lists main.cpp + MathUtils.cpp
│
├── MathUtils.h         ← .h  C-compatible declarations
├── MathUtils.cpp       ← .cpp implementations of MathUtils.h
│
├── Stack.hpp           ← .hpp template class (header-only)
├── Logger.hpp          ← .hpp utility class (header-only)
│
└── main.cpp            ← includes all three headers
```

---

## Convention across popular projects

| Project | Convention |
|---------|-----------|
| Qt | `.h` for everything — even C++ classes |
| Boost | `.hpp` for C++ headers |
| Eigen | `.hpp` for template math library |
| nlohmann/json | `.hpp` single header |
| OpenCV | `.hpp` for C++ API |
| Google Test | `.h` for everything |
| Standard library | no extension `<vector>` |

**Qt uses `.h` for everything** — so if you follow Qt convention in your
MCXApp project, using `.h` everywhere is perfectly valid.

---

## Decision guide

```
Does the header need to work with C code?
  YES → use .h (no classes, no templates, no std library)
  NO  →
    Is it a template class/function?
      YES → use .hpp (template definition MUST be in header)
      NO  →
        Is it a small header-only utility?
          YES → use .hpp (signals "no .cpp needed")
          NO  → use .h or .hpp (team/project convention)
```

---

## Summary

```cpp
// .h  — C-compatible, has a .cpp counterpart
#include "MathUtils.h"
// → compile MathUtils.cpp too (add to CMakeLists.txt)

// .hpp — C++ only template, header-only
#include "Stack.hpp"
// → no .cpp needed (do NOT add to CMakeLists.txt)

// .hpp — C++ only class, header-only
#include "Logger.hpp"
// → no .cpp needed (do NOT add to CMakeLists.txt)

// Both use #pragma once. Both work the same way.
// Extension = convention for readers, not the compiler.
```
