# C++20 Modules

## What are Modules?

C++20 Modules are a **replacement for `#include`**. Instead of pasting
header file content everywhere via the preprocessor, modules are compiled
once and their interface is cached as a Binary Module Interface (BMI).

---

## #include vs import

```mermaid
flowchart TD
    subgraph Old ["Old — #include (preprocessor)"]
        H["math.h\n(header file)"]
        A["file_a.cpp\n#include 'math.h'\n→ preprocessor pastes entire header"]
        B["file_b.cpp\n#include 'math.h'\n→ preprocessor pastes again"]
        C["file_c.cpp\n#include 'math.h'\n→ preprocessor pastes again"]
        H --> A
        H --> B
        H --> C
        note1["math.h parsed 3 times\nMacros leak into all files\nSlow builds"]
        style note1 fill:#8B0000,color:#fff
    end

    subgraph New ["New — import (C++20 Modules)"]
        M["math.ixx\n(module file)"]
        BMI["math.pcm\nBinary Module Interface\ncompiled ONCE"]
        D["file_a.cpp\nimport Math;"]
        E["file_b.cpp\nimport Math;"]
        F["file_c.cpp\nimport Math;"]
        M --> BMI
        BMI --> D
        BMI --> E
        BMI --> F
        note2["BMI compiled once\nNo macro leakage\nFaster builds"]
        style note2 fill:#267326,color:#fff
    end
```

---

## Module file structure

```mermaid
flowchart LR
    subgraph ModuleFile ["math.ixx"]
        decl["export module Math;\n← declares this IS module 'Math'"]
        imp["import &lt;string&gt;;\n← imports std headers"]
        exp["export namespace Math {\n    export int add(...);\n    export double PI;\n}\n← EXPORTED: visible outside"]
        priv["namespace Math::Internal {\n    bool isValid(...);\n}\n← NOT exported: private to module"]
    end
```

---

## export keyword — what gets exported

```mermaid
flowchart TD
    module["math.ixx module"]

    module --> exported["export int add()\nexport double PI\nexport class MathHelper\n✅ Visible to import Math users"]
    module --> notexported["namespace Math::Internal\nbool isValidDivisor()\nstatic helpers\n❌ Invisible outside module"]

    user["main.cpp\nimport Math;"]
    exported --> user
    notexported -. "cannot access" .-> user
```

---

## Module dependency chain

```mermaid
flowchart LR
    math["math.ixx\nexport module Math"]
    geo["geometry.ixx\nexport module Geometry\nimport Math"]
    main["main.cpp\nimport Math\nimport Geometry"]

    math --> geo
    math --> main
    geo --> main
```

`geometry.ixx` imports `Math` — it can use `Math::PI` internally.
`main.cpp` imports both independently.

---

## Compilation flow

```mermaid
sequenceDiagram
    participant CMake
    participant Compiler
    participant BMI as BMI Cache (.pcm)
    participant Linker

    CMake->>Compiler: compile math.ixx
    Compiler->>BMI: produces math.pcm (Binary Module Interface)

    CMake->>Compiler: compile geometry.ixx
    Compiler->>BMI: reads math.pcm (for import Math)
    Compiler->>BMI: produces geometry.pcm

    CMake->>Compiler: compile main.cpp
    Compiler->>BMI: reads math.pcm + geometry.pcm
    Compiler->>Linker: main.o

    Linker->>Linker: link math.o + geometry.o + main.o
    Linker-->>CMake: executable ✅
```

---

## Key syntax

### Declaring a module

```cpp
// math.ixx
export module Math;       // this file IS module "Math"

export int add(int a, int b) { return a + b; }  // exported
int helper() { return 0; }                       // NOT exported
```

### Importing a module

```cpp
// main.cpp
import Math;              // replaces: #include "math.h"

Math::add(3, 4);          // use exported symbols
```

### Module partitions (splitting a large module)

```cpp
// math-core.ixx
export module Math:core;     // partition "core" of module "Math"
export int add(int a, int b) { return a + b; }

// math-trig.ixx
export module Math:trig;     // partition "trig" of module "Math"
import <cmath>;
export double sin(double x) { return std::sin(x); }

// math.ixx — assembles the partitions
export module Math;
export import :core;    // re-export core partition
export import :trig;    // re-export trig partition
```

---

## #include vs import — full comparison

| | `#include` | `import` (C++20) |
|--|:----------:|:----------------:|
| Mechanism | Preprocessor text paste | Compiled BMI |
| Parsed per file | ✅ Every time | ❌ Once, then cached |
| Macro leakage | ✅ Yes — macros escape | ❌ No — hard boundary |
| Include guards | Required (`#pragma once`) | Not needed |
| Build speed | Slower (O(n×m)) | Faster (O(n+m)) |
| Encapsulation | None — all visible | Explicit `export` |
| Tooling | Universal | CMake 3.28+, GCC 14+ |
| Availability | All C++ | C++20 only |

---

## Compiler support

| Compiler | Modules support | Version needed |
|----------|:--------------:|:--------------:|
| GCC | ✅ Experimental | 14+ (`-fmodules-ts`) |
| Clang | ✅ Experimental | 16+ |
| MSVC | ✅ Best support | VS 2019 16.8+ |
| CMake | ✅ | 3.28+ |

---

## Terminal compile (GCC 14)

```bash
# Compile modules first
g++ -std=c++23 -fmodules-ts -c math.ixx     -o math.o
g++ -std=c++23 -fmodules-ts -c geometry.ixx -o geometry.o

# Compile main
g++ -std=c++23 -fmodules-ts main.cpp math.o geometry.o -lstdc++exp -o modules_demo

# Run
./modules_demo
```

> **Important:** Module files must be compiled **before** any file that
> imports them. CMake 3.28+ handles this dependency ordering automatically.

---

## When to use Modules

✅ New C++20 projects from scratch
✅ Large projects with slow build times (`#include` bottleneck)
✅ Library authors — clean encapsulation, no macro leakage
✅ When MSVC is the primary compiler (best support)

❌ Legacy codebases — mixing headers and modules is complex
❌ If your team's toolchain doesn't support C++20 modules yet
❌ Small projects — `#include` is fine, overhead is minimal

---

## Relationship to Package Structure

```mermaid
flowchart LR
    subgraph HeaderBased ["Header-based (03_PackageStructure)"]
        H1["#include 'Package/API/package.h'"]
        H2["Works everywhere\nAll C++ standards\nPreprocessor-based"]
    end

    subgraph ModuleBased ["Module-based (07_Modules — C++20)"]
        M1["import Math;\nimport Geometry;"]
        M2["C++20 only\nFaster builds\nBetter encapsulation"]
    end

    note["Both organize code into\nlogical groups.\nModules are the modern\nevolution of headers."]
    style note fill:#2a2a2a,color:#aaa
```
