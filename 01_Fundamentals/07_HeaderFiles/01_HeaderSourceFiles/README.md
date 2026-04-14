# Header Files — C++23

## Why header files exist

When a C++ project grows beyond a single file, functions and types need to
be **shared across multiple .cpp files**. A header file (`.h`) solves this
by declaring **what exists** — the compiler can then check your usage before
the actual implementation is compiled.

```
Without headers — problem:
  MathUtils.cpp defines add()
  main.cpp calls add()  ← compiler doesn't know add() exists!

With headers — solution:
  MathUtils.h   declares  int add(int a, int b);
  MathUtils.cpp defines   int add(int a, int b) { return a+b; }
  main.cpp      includes  MathUtils.h → compiler knows the signature
```

---

## Project structure

```
07_HeaderFiles/
├── CMakeLists.txt      ← lists all .cpp files to compile
├── main.cpp            ← includes headers, calls functions
├── MathUtils.h         ← math declarations + constants
├── MathUtils.cpp       ← math implementations
├── McxConfig.h         ← MCX struct, enum, constants
└── McxConfig.cpp       ← McxSessionConfig implementations
```

---

## #pragma once — include guard

Every header must have an **include guard** to prevent being included twice:

```cpp
#pragma once   // modern — preferred

// equivalent traditional style:
#ifndef MATHUTILS_H
#define MATHUTILS_H
// ... content ...
#endif
```

Without it, if two files both include `MathUtils.h`, the compiler sees
the declarations twice → **redefinition error**.

---

## Declaration vs definition

| | Where | What it looks like |
|--|-------|-------------------|
| Declaration | `.h` | `int add(int a, int b);` — just the signature |
| Definition | `.cpp` | `int add(int a, int b) { return a+b; }` — actual code |

```cpp
// MathUtils.h — declaration (WHAT)
int add(int a, int b);

// MathUtils.cpp — definition (HOW)
int add(int a, int b) {
    return a + b;
}
```

The header is your **public API** — it tells other files what they can use.

---

## Angle brackets vs quotes

```cpp
#include <iostream>      // system / library header  → search system paths
#include <vector>        // system / library header
#include <QThread>       // Qt framework header

#include "MathUtils.h"   // OUR header → search current directory first
#include "McxConfig.h"   // OUR header
```

**Rule:** use `""` for your own headers, `<>` for system and library headers.

---

## What belongs in a header

```cpp
// ✅ IN the header (.h)
#pragma once
constexpr int MAX = 100;            // compile-time constant
enum class Status { OK, ERROR };    // shared type
struct Config { int port; ... };    // shared data layout
int add(int a, int b);              // function declaration
inline int square(int x) { return x*x; }  // inline definition
template<typename T> T max(T a, T b);     // template (MUST be in header)

// ❌ NOT in the header
using namespace std;        // pollutes every file that includes this!
int add(int a, int b) { return a+b; }  // definition → multiple definition error
int globalVar = 0;          // definition → multiple definition error
```

---

## Namespaces in headers

Wrap header content in a namespace to avoid name collisions:

```cpp
// MathUtils.h
namespace MathUtils {
    int add(int a, int b);
    constexpr double PI = 3.14159;
}

// usage
MathUtils::add(3, 4);
MathUtils::PI;
```

---

## Struct / class in headers

```cpp
// McxConfig.h — declaration
struct McxSessionConfig {
    std::string serverHost;
    uint16_t    serverPort;

    McxSessionConfig();                 // constructor declaration
    std::string toString() const;       // method declaration
    bool        isValid()  const;
};

// McxConfig.cpp — definition
McxSessionConfig::McxSessionConfig()
    : serverHost("localhost"), serverPort(5060) {}

std::string McxSessionConfig::toString() const { ... }
```

---

## Forward declarations

Declare a type **exists** without including its full header:

```cpp
// In MyFile.h
class Logger;                  // forward declaration — just says Logger exists
void process(Logger* log);     // can use Logger* or Logger& in declarations

// In MyFile.cpp
#include "Logger.h"            // full definition needed to call methods
```

**When to use:** breaking circular dependencies, reducing compile times.
**Limitation:** can only use pointer (`Logger*`) or reference (`Logger&`) —
not by value, and cannot call any methods.

---

## CMakeLists.txt — multiple source files

```cmake
add_executable(07_HeaderFiles
    main.cpp
    MathUtils.cpp       # ← add each .cpp file here
    McxConfig.cpp       # ← header .h files are NOT listed
)
```

- **`.cpp` files** → listed in `add_executable` → compiled to object files
- **`.h` files** → NOT listed → included by `#include` at compile time
- **Linker** → combines all object files into the final executable

---

## Build process explained

```
Compile phase:
  main.cpp      → main.o       (uses declarations from headers)
  MathUtils.cpp → MathUtils.o  (implements the functions)
  McxConfig.cpp → McxConfig.o

Link phase:
  main.o + MathUtils.o + McxConfig.o → 07_HeaderFiles (executable)
```

If only `MathUtils.cpp` changes → only `MathUtils.o` recompiles.
`main.cpp` is NOT recompiled — this is why header-based design is fast.

---

## Common mistakes

```cpp
// MISTAKE 1: definition in header — causes "multiple definition" error
// If two .cpp files include this header, add() is defined twice!
int add(int a, int b) { return a+b; }  // ❌ in .h

// FIX: declaration in .h, definition in .cpp
int add(int a, int b);                 // ✅ in .h
int add(int a, int b) { return a+b; } // ✅ in .cpp

// MISTAKE 2: using namespace std in header — pollutes every includer
using namespace std;   // ❌ in .h

// FIX: always qualify in headers
std::string name;      // ✅

// MISTAKE 3: missing #pragma once — redefinition errors
// Add #pragma once as the very first line of every header

// MISTAKE 4: .h file not found
#include "MathUtils.h"  // ← file must be in same dir or include path
```

---

## Summary

| File | Role | Contains |
|------|------|----------|
| `.h` header | Interface — **WHAT** | Declarations, constants, enums, structs |
| `.cpp` source | Implementation — **HOW** | Definitions, method bodies |
| `CMakeLists.txt` | Build config | Lists all `.cpp` files |

**The header IS your API.** Anyone reading the `.h` file should understand
everything they need to use your code — without reading the `.cpp`.
