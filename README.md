# QtTutorialCpp — C++23 Tutorial Series

A comprehensive C++23 tutorial repository covering fundamentals through
advanced topics, built on **Ubuntu 24.04** with **GCC 14.1.0** and **Qt Creator**.

---

## Environment

| Item | Value |
|------|-------|
| Standard | C++23 |
| Compiler | GCC 14.1.0 at `/usr/local/gcc-14.1.0/bin/g++-14.1.0` |
| Build system | CMake (Qt Creator wizard template) |
| OS | Ubuntu 24.04 |
| IDE | Qt Creator |
| Extra lib | `stdc++exp` — required for `<print>`, `<format>`, `<stacktrace>` |

---

## Repository structure

```
QtTutorialCpp/
├── 01_Fundamentals
├── 02_OOP
├── 03_MemoryManagement
├── 04_Templates
├── 05_STL
├── 06_ModernCpp
├── 07_DesignPatterns
├── 08_ModernCppDesignPatterns
├── 09_SoftwareArchitecture
├── 10_SystemProgramming
└── 11_CodeQualityPractices
```

---

## 01 — Fundamentals

```
01_Fundamentals/
├── 01_DataTypes
├── 02_Variables
├── 03_Keywords/
│   ├── ClassesAndOOP
│   ├── ControlFlow/
│   │   ├── BreakExample
│   │   ├── Continue
│   │   ├── DoWhile
│   │   ├── ForLoop
│   │   ├── GoToExample
│   │   ├── ifStatement
│   │   └── SwitchCase
│   ├── DataTypes
│   ├── Functions
│   ├── Memory
│   ├── ModernCpp17
│   ├── ModernCpp20
│   ├── ModernCpp23
│   ├── Namespaces
│   ├── Operators
│   ├── Others
│   ├── SpecialPurpose
│   ├── Templates
│   ├── TypeCasting
│   └── TypeModifiers/
│       ├── 01_GeneralTypeModifiers
│       └── 02_VolatileKeyword
├── 04_Operators
├── 05_ControlFlow
├── 06_Functions/
│   ├── CallByReference
│   ├── CallByValue
│   ├── ConstExpression
│   ├── DefaultParameter
│   ├── LambdaFunction
│   ├── MultipleParameters
│   ├── Overloading
│   ├── PassByReference
│   ├── Recursion
│   ├── ReturningSet
│   ├── ReturningTuples
│   ├── Scope
│   ├── SimpleFunctionExample
│   └── Swapping
└── 07_HeaderFiles/
    ├── 01_HeaderSourceFiles        (.h + .cpp — C-compatible)
    └── 02_HeaderHppSourceFiles     (.hpp — C++ only, templates, header-only)
```

---

## 02 — OOP

```
02_OOP/
├── 01_ClassesAndObjects/
│   ├── 01_BasicClass
│   ├── 02_Constructor
│   ├── 03_Destructor
│   ├── 04_MemberFunctions
│   ├── 05_GettersSetters
│   ├── 06_CopyConstructor
│   ├── 07_MoveSemantics
│   ├── 08_ThisPointer
│   ├── 09_StaticMembers
│   ├── 10_FriendFunctions
│   ├── 11_HeaderSourceSeparation
│   ├── 12_ConstMemberFunctions
│   ├── 13_ConstObjects
│   ├── 14_MutableKeyword
│   ├── 15_DefaultDelete
│   ├── 16_ExplicitKeyword
│   ├── 17_DelegatingConstructors
│   ├── 18_InheritingConstructors
│   ├── 19_NestedClasses
│   ├── 20_ConversionOperators
│   ├── 21_ObjectSlicing
│   └── 22_InitializerLists
├── 02_Encapsulation
├── 03_Abstraction
├── 04_Inheritance/
│   ├── 01_Inheritance
│   ├── 02_MultiInheritance
│   └── 03_VirtualInheritance
├── 05_Overloading
├── 06_Overriding
├── 07_Polymorphism
└── 08_VirtualDestructor
```

---

## 03 — Memory Management

```
03_MemoryManagement/
├── 01_Pointers/
│   ├── 01_NullPointer
│   ├── 02_SimplePointer
│   └── 03_VoidPointer
├── 02_References
├── 03_RAII/
│   ├── 01_RAII_Example
│   └── 02_RAII_Example
├── 04_SmartPointers/
│   ├── 01_SharedPointer/
│   │   ├── 01_SharedPointer_Example
│   │   └── 02_SharedPointer_Example
│   ├── 02_UniquePointer/
│   │   ├── 01_UniquePointer_Example
│   │   └── 02_UniquePointer_Example
│   ├── 03_WeakPointer/
│   │   ├── 01_WeakPointer_Example
│   │   └── 02_WeakPointer_Example
│   └── 04_CustomDeleters
├── 05_MemoryManagement
├── 06_MallocVsNew
├── 07_StorageClasses
└── 08_RuleOf3_5_0
```

---

## 04 — Templates

```
04_Templates/
├── 01_FunctionTemplates
├── 02_ClassTemplates
├── 03_TemplateSpecialization
├── 04_VariadicTemplates
└── 05_SFINAE
```

---

## 05 — STL

```
05_STL/
├── 01_VectorSTL          dynamic array
├── 02_ArraySTL           fixed-size array
├── 03_DequeSTL           double-ended queue
├── 04_ListSTL            doubly linked list
├── 05_StackSTL           LIFO adaptor
├── 06_QueueSTL           FIFO adaptor
├── 07_PriorityQueueSTL   heap-based priority queue
├── 08_SetSTL             sorted unique elements
├── 09_MapSTL             sorted key-value pairs
├── 10_MultimapSTL        sorted duplicate keys
├── 11_BitsetSTL          fixed-size bit array
├── 12_AlgorithmSTL       std::sort, find, transform...
├── 13_IomanipSTL         stream formatting
├── 14_CstdintSTL         fixed-width integer types
├── 15_CassertSTL         compile/runtime assertions
└── 16_StdexceptSTL       standard exceptions
```

---

## 06 — Modern C++

```
06_ModernCpp/
├── 01_FunctionPointers
├── 02_Callbacks/
│   ├── 01_CallbacksExample
│   └── 02_CallbackExample
├── 03_Lambdas
├── 04_Semantics             move semantics, rvalue references
├── 05_Multithreading/
│   ├── 01_SimpleThread
│   ├── 02_SimpleThreadOperations
│   ├── 03_ThreadAttributes
│   ├── 04_Callables
│   ├── 05_Mutex
│   ├── 06_DeadLock
│   ├── 07_ConditionVariable
│   ├── 08_Futures
│   ├── 09_Atomics
│   ├── 10_Semaphores
│   ├── 11_ThreadPool
│   ├── 12_ThreadSynchronization
│   ├── 13_ThreadManagement
│   ├── 14_Timer
│   ├── 15_CommonThreadIssues
│   └── 16_QThread
└── 06_Async
```

---

## 07 — Design Patterns

```
07_DesignPatterns/
├── 01_BehavioralPatterns/
│   ├── 01_TemplateMethod        (2 examples)
│   ├── 02_Strategy              (2 examples)
│   ├── 03_Command               (3 examples: basic, smart home, text editor)
│   ├── 04_Iterator              (3 examples: generic, restaurant, custom)
│   ├── 05_Observer              (2 examples)
│   ├── 06_State                 (2 examples)
│   ├── 07_Memento               (2 examples)
│   ├── 08_ChainOfResponsibility (3 examples)
│   ├── 09_Mediator              (2 examples)
│   └── 10_Visitor               (2 examples)
├── 02_CreationalPatterns/
│   ├── 01_Singleton             (4 variants: naive, thread-safe, eager, multiton)
│   ├── 02_FactoryMethod         (2 examples)
│   ├── 03_AbstractFactory       (2 examples)
│   ├── 04_Builder               (2 examples)
│   └── 05_Prototype             (2 examples)
└── 03_StructuralPatterns/
    ├── 01_Adapter
    ├── 02_Bridge
    ├── 03_Facade
    ├── 04_Decorator
    ├── 05_Proxy                 (3 variants: caching, protection, virtual)
    ├── 06_Composite
    └── 07_Flyweight             (2 examples)
```

---

## 08 — Modern C++ Design Patterns

```
08_ModernCppDesignPatterns/
├── FunctionalProgramming
├── TemplateMeta
├── ThinkCellPhilosophy
└── ZeroCostAbstractions
```

---

## 09 — Software Architecture

High-level architecture principles and patterns.

---

## 10 — System Programming

```
10_SystemProgramming/
└── 04_IPC/
    └── MessageQueues
```

---

## 11 — Code Quality Practices

```
11_CodeQualityPractices/
├── 01_CodeStyleFormatting      naming, indentation, clang-format
├── 02_Readability              clear names, short functions, no nesting
├── 03_CodeQualityBasics        what is quality, metrics, good vs bad
├── 04_CodeSmells               anti-patterns: long method, god class, duplication
├── 05_Maintainability          modularity, documentation, testability
├── 06_BestPractices            RAII, const correctness, smart pointers
├── 07_DesignPrinciples/
│   ├── 01_SingleResponsibility
│   ├── 02_OpenClosed
│   ├── 03_LiskovSubstitution
│   ├── 04_InterfaceSegregation
│   ├── 05_DependencyInversionPrinciple
│   ├── 06_Dry
│   ├── 07_Kiss
│   ├── 08_Yagni
│   ├── 09_SeparationConcerns
│   ├── 10_LowCoupling
│   └── 11_HighCohesion
├── 08_PerformanceConsiderations/
│   ├── 01_CodeComplexity       Big-O notation, O(1) to O(n²)
│   ├── 02_CountComplexity      counting operations in real code
│   ├── 03_PerformanceOptimization  reserve, move, cache, avoid copies
│   └── 04_BenchmarkingAndProfiling Google Benchmark — measure everything
└── 09_ToolsAndAnalysis         clang-tidy, sanitizers, valgrind, perf
```

Recommended order inside this module:
```
03 → 04 → 01 → 02 → 06 → 07 → 05 → 08 → 09
Basics → Smells → Style → Readability → Practices → Design → Maintainability → Performance → Tools
```

---

## GCC 14 Setup — Qt Creator on Ubuntu 24.04

Ubuntu 24.04 ships with **GCC 13.3** which is missing `<print>` and other C++23
library headers. GCC 14 must be installed and registered manually.

### Step 1 — Install GCC 14

```bash
# Ubuntu toolchain PPA (easiest)
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install gcc-14 g++-14
```

If you have a manual build at a custom path, verify it:
```bash
/usr/local/gcc-14.1.0/bin/g++-14.1.0 --version
# g++ (GCC) 14.1.0
```

### Step 2 — Fix PATH if broken

```bash
export PATH=/usr/bin:/bin:$PATH
# Make permanent:
echo 'export PATH=/usr/bin:/bin:$PATH' >> ~/.bashrc && source ~/.bashrc
```

### Step 3 — Register GCC 14 in Qt Creator

**Tools → Preferences → Kits → Compilers → Add → GCC → C++**

| Field | Value |
|-------|-------|
| Name | `GCC 14.1.0` |
| Compiler path | `/usr/local/gcc-14.1.0/bin/g++-14.1.0` |
| ABI | `x86 - linux - gcc - elf - 64bit` |

**Tools → Preferences → Kits → Compilers → Add → GCC → C**

| Field | Value |
|-------|-------|
| Name | `GCC 14.1.0` |
| Compiler path | `/usr/local/gcc-14.1.0/bin/gcc-14.1.0` |
| ABI | `x86 - linux - generic - elf - 64bit` |

### Step 4 — Assign to Kit

**Tools → Preferences → Kits → Desktop kit**

| Field | Value |
|-------|-------|
| Compiler C | GCC 14.1.0 |
| Compiler C++ | GCC 14.1.0 |

Click **Apply → OK**.

### Step 5 — Set C++23 as default for all new projects

**Kit CMake configuration** — Tools → Preferences → Kits → Desktop → CMake Configuration → Change

Add at the bottom:
```
-DCMAKE_CXX_STANDARD:STRING=23
-DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON
```

**Patch wizard templates** — the Kit setting alone is not enough:

```bash
sudo sed -i 's/CMAKE_CXX_STANDARD 17/CMAKE_CXX_STANDARD 23/g' \
  /usr/share/qtcreator/templates/wizards/projects/consoleapp/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/plaincpp/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/qtwidgetsapplication/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/cpplibrary/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/qtquickapplication/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/qtquickapplication_compat/CMakeLists.txt
```

Verify:
```bash
grep -r "CXX_STANDARD" /usr/share/qtcreator/templates/wizards/projects/
# Every line should show CMAKE_CXX_STANDARD 23
```

> Re-run after any Qt Creator upgrade — updates may overwrite the templates.

### Step 6 — Fix false red errors in editor (clangd)

Qt Creator uses clangd for IntelliSense which may show false C++23 errors.

**Simplest fix:** Tools → Preferences → C++ → Clangd → uncheck **Use clangd**

**Alternative:** add to CMakeLists.txt:
```cmake
target_compile_options(MyTarget PRIVATE
    --gcc-toolchain=/usr/local/gcc-14.1.0
)
```

---

## CMakeLists.txt templates

### Standard project (all examples except QThread/Benchmarking)

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(MyProject main.cpp)

target_link_libraries(MyProject
    Qt${QT_VERSION_MAJOR}::Core
    stdc++exp
)

include(GNUInstallDirs)
install(TARGETS MyProject
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
```

### QThread / QtConcurrent projects

```cmake
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Concurrent)

target_link_libraries(MyProject
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Concurrent
    stdc++exp
)
```

> `main.cpp` must end with `#include "main.moc"` as the very last line.

### BenchmarkingAndProfiling

```bash
# Install first
sudo apt-get install libbenchmark-dev
```

```cmake
find_package(benchmark REQUIRED)

target_link_libraries(MyProject
    Qt${QT_VERSION_MAJOR}::Core
    benchmark::benchmark
    pthread
    stdc++exp
)
```

Terminal compile:
```bash
g++ -std=c++23 -O3 -march=native main.cpp -lbenchmark -lpthread -o bench && ./bench
```

> **Always build Release for benchmarks** — Debug timings are meaningless.
> Qt Creator: Projects → Build → Build Configuration → **Release**

---

## C++23 header availability by GCC version

| Header | GCC 12 | GCC 13 | GCC 14 |
|--------|--------|--------|--------|
| `<expected>` | ✅ | ✅ | ✅ |
| `<stacktrace>` | ✅ needs `stdc++exp` | ✅ | ✅ |
| `<print>` | ❌ | ❌ | ✅ |
| `<flat_map>` | ❌ | ❌ | ✅ |
| `<mdspan>` | ❌ | ✅ | ✅ |

---

## Common issues and fixes

| Error | Cause | Fix |
|-------|-------|-----|
| `<print>` not found | GCC 13 used instead of 14 | Check Kit compiler assignment |
| `stdc++exp` linker error | Missing library link | Add `stdc++exp` to `target_link_libraries` |
| False red errors in editor | clangd using wrong headers | Disable clangd or add `--gcc-toolchain` |
| `cmake_check_build_system Error 1` | Stale Makefile after moving project | Delete build folder, reconfigure |
| `CMAKE_CXX_STANDARD 17` in new projects | Wizard template not patched | Run `sed` patch from Step 5 |
| `#include "main.moc"` errors | Not last line of file | Move to very last line |
| `undefined reference to benchmark::...` | Missing `-lbenchmark -lpthread` | Add both flags after source file |

---

## Moving projects to new subdirectories

1. Create the project normally in Qt Creator
2. Kit already has GCC 14 — no compiler paths needed in CMakeLists.txt
3. After moving: **Build → Clear CMake Configuration → Run CMake**
4. Build with **Ctrl+B**

> If you see `[Makefile:283: cmake_check_build_system] Error 1` — delete the
> build folder and reconfigure from scratch.

---

## Quick reference — learning path

```
Beginner:     01_Fundamentals → 02_OOP → 03_MemoryManagement
Intermediate: 04_Templates → 05_STL → 06_ModernCpp
Advanced:     07_DesignPatterns → 08_ModernCppDesignPatterns
Professional: 09_SoftwareArchitecture → 10_SystemProgramming → 11_CodeQualityPractices
```



---

## License

Copyright © 2024–2026 **Konstantinos Angeloulis**

This tutorial series is licensed under the
**[Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License (CC BY-NC-ND 4.0)](https://creativecommons.org/licenses/by-nc-nd/4.0/)**.

[![CC BY-NC-ND 4.0](https://licensebuttons.net/l/by-nc-nd/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc-nd/4.0/)

You may read and share this material with attribution, but you may **not** use it
commercially or publish modified versions of it.

See [LICENSE.md](LICENSE.md) for full terms.
