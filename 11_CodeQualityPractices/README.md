# CodeQualityExamples — C++ Code Quality Reference

> *"Any fool can write code that a computer can understand.*
> *Good programmers write code that humans can understand."*
> — Martin Fowler

> *"The ratio of time spent reading versus writing is well over 10 to 1.*
> *Making it easy to read makes it easier to write."*
> — Robert C. Martin

---

## What this repository covers

A complete reference for professional C++ development covering clean code,
design principles, performance, and tooling — all with before/after examples.

---

## Repository structure

```
CodeQualityExamples/
│
├── BestPractices/
│   ├── best_practices.cpp
│   └── README.md
│
├── CodeQualityBasics/
│   ├── code_quality_basics.cpp
│   └── README.md
│
├── CodeSmells/
│   ├── code_smells.cpp
│   └── README.md
│
├── CodeStyleFormatting/
│   ├── code_style_formatting.cpp
│   └── README.md
│
├── DesignPrinciples/
│   ├── 01_SingleResponsibility/
│   ├── 02_OpenClosed/
│   ├── 03_LiskovSubstitution/
│   ├── 04_InterfaceSegregation/
│   ├── 05_DependencyInversionPrinciple/
│   ├── 06_Dry/
│   ├── 07_Kiss/
│   ├── 08_Yagni/
│   ├── 09_SeparationConcerns/
│   ├── 10_LowCoupling/
│   ├── 11_HighCohesion/
│   └── README.md
│
├── Maintainability/
│   ├── maintainability.cpp
│   └── README.md
│
├── PerformanceConsiderations/
│   ├── 01_CodeComplexity/
│   ├── 02_CountComplexity/
│   ├── 03_PerformanceOptimization/
│   ├── 04_BenchmarkingAndProfiling/
│   └── README.md
│
├── Readability/
│   ├── readability.cpp
│   └── README.md
│
├── ToolsAndAnalysis/
│   ├── tools_and_analysis.cpp
│   └── README.md
│
└── README.md                ← this file
```

---

## Recommended learning order

```
1. CodeQualityBasics          ← what is quality? metrics, good vs bad
2. CodeSmells                 ← recognise anti-patterns early
3. CodeStyleFormatting        ← naming, indentation, consistency
4. Readability                ← clear names, short functions, no nesting
5. BestPractices              ← RAII, const correctness, smart pointers
6. DesignPrinciples           ← SOLID, DRY, KISS, YAGNI, cohesion
7. Maintainability            ← modularity, documentation, testability
8. PerformanceConsiderations  ← complexity, optimization, benchmarking
9. ToolsAndAnalysis           ← clang-tidy, sanitizers, coverage
```

---

## What each section covers

### 1. CodeQualityBasics
Foundation — what code quality means and how to measure it.

| Topic | Description |
|-------|-------------|
| Quality characteristics | Readability, maintainability, reliability, performance |
| Code metrics | Cyclomatic complexity, lines of code, coupling/cohesion |
| Technical debt | What it is, how it accumulates, how to pay it back |
| Good vs bad code | Side-by-side before/after comparisons |
| Quality checklist | Red flags to watch for in reviews |

---

### 2. CodeSmells
Anti-patterns that signal deeper problems — detect them before they grow.

| Smell | Problem |
|-------|---------|
| Long Method | Functions > 20–30 lines doing too much |
| Large Class | Class with too many responsibilities |
| Duplicate Code | Copy-pasted logic — change in one place, miss another |
| Long Parameter List | > 3–4 parameters — hard to call, hard to understand |
| Magic Numbers | `if (x == 42)` — what is 42? |
| God Class | One class that knows and does everything |
| Feature Envy | Method uses another class more than its own |
| Dead Code | Unused functions, variables, commented-out blocks |
| Deep Nesting | `if` inside `if` inside `for` inside `while` |

> *"A code smell is a surface indication that usually corresponds to a deeper problem."* — Martin Fowler

---

### 3. CodeStyleFormatting
Consistency and professional formatting.

| Topic | Examples |
|-------|---------|
| Naming conventions | `camelCase`, `snake_case`, `PascalCase`, `m_member`, `g_global` |
| Indentation | 4 spaces, consistent bracing style |
| Line length | Max ~100 characters |
| Comment style | `//` for inline, `/** */` for Doxygen |
| File organization | Include order, namespace usage |
| `clang-format` | Automated formatting — `.clang-format` config |

---

### 4. Readability
Code that reads like well-written prose.

| Topic | Rule |
|-------|------|
| Function names | Verb + noun: `calculateTotal()`, `sendEmail()` |
| Variable names | Descriptive: `userCount` not `n`, `isConnected` not `flag` |
| Short functions | One function = one task, fits on screen |
| Early returns | Guard clauses instead of deep nesting |
| Avoid abbreviations | `connectionTimeout` not `connTo` |
| Self-documenting | Code that explains itself without comments |

---

### 5. BestPractices
C++23 specific practices for safe, efficient code.

| Practice | Why |
|----------|-----|
| RAII | Destructor always runs — no leaks, exception-safe |
| Rule of Zero | Use `std::vector`/`std::string` — compiler handles everything |
| Rule of Five | If you write destructor, write all 5 special members |
| Const correctness | Compiler enforces immutability, enables optimizations |
| Initialize all variables | No undefined behavior, no garbage values |
| `std::unique_ptr` / `std::shared_ptr` | No raw `new`/`delete`, no leaks |
| Pass by `const&` | No copies for large objects |
| References over pointers | Can't be null, cleaner syntax |

---

### 6. DesignPrinciples
Architecture principles that make code extensible and maintainable.

#### SOLID

| Principle | Short | Rule |
|-----------|-------|------|
| Single Responsibility | SRP | A class has ONE reason to change |
| Open/Closed | OCP | Open for extension, closed for modification |
| Liskov Substitution | LSP | Subtypes must be substitutable for their base types |
| Interface Segregation | ISP | No client depends on methods it doesn't use |
| Dependency Inversion | DIP | Depend on abstractions, not concretions |

#### Other principles

| Principle | Rule |
|-----------|------|
| DRY | Don't Repeat Yourself — every piece of knowledge has one place |
| KISS | Keep It Simple — simplest solution that works |
| YAGNI | You Aren't Gonna Need It — don't build for hypothetical futures |
| Separation of Concerns | UI, business logic, data — each in its own layer |
| Low Coupling | Modules depend on each other as little as possible |
| High Cohesion | Related things stay together, unrelated things stay apart |

---

### 7. Maintainability
Code that can be changed safely by anyone, including future-you.

| Topic | Practice |
|-------|---------|
| Modular design | Small, focused classes with clear interfaces |
| Documentation | Doxygen comments on public APIs |
| Testable code | Dependency injection, avoid global state |
| Avoiding god objects | Split large classes by responsibility |
| Clear interfaces | Headers as public API contracts |
| Boy Scout Rule | Leave code cleaner than you found it |

---

### 8. PerformanceConsiderations

Four sub-modules in learning order:

#### 01_CodeComplexity
Big-O notation — the language of algorithmic performance.

| Complexity | Name | Example |
|-----------|------|---------|
| O(1) | Constant | Array index, hash map lookup |
| O(log n) | Logarithmic | Binary search |
| O(n) | Linear | Linear scan |
| O(n log n) | Linearithmic | `std::sort` |
| O(n²) | Quadratic | Bubble sort, nested loops |

#### 02_CountComplexity
Practical analysis — counting actual operations in real code.
Loop analysis, nested loop patterns, amortized complexity.

#### 03_PerformanceOptimization
Apply complexity knowledge to write faster code.

| Technique | Benefit |
|-----------|---------|
| `vector::reserve()` | Eliminate reallocations |
| `const&` parameters | No unnecessary copies |
| Move semantics | Transfer instead of copy |
| Cache-friendly access | Sequential > random memory access |
| `unordered_map` vs `map` | O(1) vs O(log n) lookup |
| Stack vs heap | Stack allocation is ~4x faster |

#### 04_BenchmarkingAndProfiling
Measure — never guess. Uses **Google Benchmark**.

```bash
# Install
sudo apt-get install libbenchmark-dev

# Compile
g++ -std=c++23 -O3 -march=native main.cpp -lbenchmark -lpthread -o bench

# Run
./bench
./bench --benchmark_filter=Vector
./bench --benchmark_repetitions=10
```

> **Always build Release** — Debug benchmarks are meaningless.
> In Qt Creator: Projects → Build → **Release**

---

### 9. ToolsAndAnalysis
Automated quality enforcement.

| Tool | Purpose | Install |
|------|---------|---------|
| `clang-tidy` | Static analysis, style checks | `sudo apt install clang-tidy` |
| `clang-format` | Automatic code formatting | `sudo apt install clang-format` |
| AddressSanitizer | Detects memory errors | `-fsanitize=address` |
| UBSanitizer | Detects undefined behavior | `-fsanitize=undefined` |
| `valgrind` | Memory leak detection | `sudo apt install valgrind` |
| `gcov`/`lcov` | Code coverage | `sudo apt install lcov` |
| `perf` | CPU profiling | `sudo apt install linux-tools-generic` |

---

## Compiling the examples

### Standard examples (all except benchmarking)

```bash
# Qt Creator — just open the folder and build (CMakeLists.txt included)

# Terminal
g++ -std=c++23 -O2 main.cpp -lbenchmark -lpthread -o output && ./output
```

### BenchmarkingAndProfiling only

```bash
# Install library first
sudo apt-get install libbenchmark-dev

# Compile with benchmark flags
g++ -std=c++23 -O3 -march=native main.cpp -lbenchmark -lpthread -o bench

# Run
./bench
```

### With sanitizers (catch bugs)

```bash
# Memory errors
g++ -std=c++23 -O1 -fsanitize=address -fno-omit-frame-pointer main.cpp -o app

# Undefined behavior
g++ -std=c++23 -O1 -fsanitize=undefined main.cpp -o app
```

---

## Key takeaways

```
Clean code is not a luxury — it is a professional responsibility.

Write code for the next person who reads it.
That person is often you, six months from now.

Measure before you optimize.
Profile before you rewrite.
Test before you refactor.

The best code is code that clearly expresses intent,
handles errors gracefully, and is easy to change.
```

---

## References

- **Clean Code** — Robert C. Martin
- **Refactoring** — Martin Fowler
- **Effective Modern C++** — Scott Meyers
- **C++ Core Guidelines** — Bjarne Stroustrup & Herb Sutter
  https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
- **Google C++ Style Guide**
  https://google.github.io/styleguide/cppguide.html
