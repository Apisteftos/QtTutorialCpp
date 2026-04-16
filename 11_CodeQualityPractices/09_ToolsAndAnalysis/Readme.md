# C++ Tools and Analysis

## 📚 Overview

Modern C++ development requires a comprehensive toolchain to ensure code quality, catch bugs early, and maintain consistency:

> **"The right tools make quality code easier to achieve and maintain. Use static analysis, sanitizers, and formatters to catch issues before they become bugs."**  
> — Modern C++ Best Practice

### What They Are

- **Automated quality checkers** - Find issues without human review
- **Runtime error detectors** - Catch bugs during execution
- **Code formatters** - Enforce consistent style
- **Performance analyzers** - Identify bottlenecks
- **Documentation generators** - Create docs from code

### Why They Matter

✅ **Catch bugs before runtime** - Find issues during development  
✅ **Enforce coding standards** - Consistent codebase automatically  
✅ **Find memory leaks** - Detect resource management issues  
✅ **Improve consistency** - Same style everywhere  
✅ **Reduce review time** - Automated checks free up reviewers  
✅ **Document automatically** - Generate docs from code  

---

## 🛠️ Tool Categories

### Quick Reference

| Category | Tools | Purpose | When to Use |
|----------|-------|---------|-------------|
| **Linters** | clang-tidy | Code quality & modernization | Always |
| **Formatters** | clang-format | Consistent code style | Always |
| **Static Analyzers** | cppcheck, PVS-Studio, Coverity | Bug detection | CI/CD |
| **Sanitizers** | ASan, UBSan, TSan, MSan | Runtime error detection | Testing |
| **Memory Analyzers** | Valgrind | Memory profiling | Performance |
| **Coverage** | gcov, lcov | Test coverage | Testing |
| **Profilers** | gprof, perf, Callgrind | Performance analysis | Optimization |
| **Documentation** | Doxygen | API documentation | Release |

---

## 🔍 1. Clang-Tidy (Linter)

### What It Is

**Clang-tidy** is a clang-based C++ linter that provides:
- Style checking and enforcement
- Bug-prone pattern detection
- Modernization suggestions
- Performance improvements
- Automatic fixes

### Installation

```bash
# Ubuntu/Debian
sudo apt install clang-tidy

# macOS
brew install llvm

# From source
git clone https://github.com/llvm/llvm-project.git
```

### Basic Usage

```bash
# Check a single file
clang-tidy file.cpp -- -std=c++17

# With compilation database
clang-tidy file.cpp

# Auto-fix issues
clang-tidy -fix file.cpp -- -std=c++17

# Specific checks only
clang-tidy -checks='modernize-*,readability-*' file.cpp

# List all available checks
clang-tidy -list-checks
```

### Configuration (.clang-tidy)

```yaml
---
Checks: >
  modernize-*,
  readability-*,
  performance-*,
  bugprone-*,
  cppcoreguidelines-*,
  -modernize-use-trailing-return-type

CheckOptions:
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  - key: readability-identifier-naming.FunctionCase
    value: camelCase
  - key: readability-identifier-naming.VariableCase
    value: camelCase
  - key: readability-identifier-naming.ConstantCase
    value: UPPER_CASE
  - key: readability-function-cognitive-complexity.Threshold
    value: 25
```

### Common Check Categories

**modernize-*** - C++11/14/17/20 features
```cpp
// Before (C++03)
for (std::vector<int>::iterator it = vec.begin(); 
     it != vec.end(); ++it) {
    std::cout << *it << std::endl;
}

// After (Modern C++)
for (auto value : vec) {
    std::cout << value << std::endl;
}
```

**readability-*** - Code readability
```cpp
// Before
void f(int a, int b, int c, int d, int e, int f) { }  // Too many params

// After
struct Params {
    int a, b, c, d, e, f;
};
void f(const Params& params) { }
```

**performance-*** - Performance issues
```cpp
// Before
std::string getName() { return name; }  // Expensive copy

// After
const std::string& getName() const { return name; }
```

**bugprone-*** - Potential bugs
```cpp
// Before
if (ptr = nullptr) { }  // Assignment instead of comparison!

// After
if (ptr == nullptr) { }
```

**cppcoreguidelines-*** - C++ Core Guidelines
```cpp
// Before
int* createArray() {
    return new int[100];  // Raw pointer!
}

// After
std::unique_ptr<int[]> createArray() {
    return std::make_unique<int[]>(100);
}
```

### Examples

**❌ Bad Code (Issues clang-tidy catches):**

```cpp
class Widget {
public:
    Widget() {}  // Use = default
    ~Widget() {}  // Missing virtual
    
    void setName(string name) {  // Pass by const ref
        this->name = name;
    }
    
private:
    string name;
    int* data;  // Raw pointer
};

void processData(void* ptr) {
    int* p = (int*)ptr;  // C-style cast
}
```

**✅ Good Code (Fixed):**

```cpp
class Widget {
public:
    Widget() = default;
    virtual ~Widget() = default;
    
    void setName(const std::string& name) {
        this->name = name;
    }
    
private:
    std::string name;
    std::unique_ptr<int> data;  // Smart pointer
};

void processData(void* ptr) {
    auto* p = static_cast<int*>(ptr);  // Static cast
}
```

### CMake Integration

```cmake
# Enable clang-tidy for all targets
set(CMAKE_CXX_CLANG_TIDY 
    clang-tidy;
    -checks=*;
    -warnings-as-errors=*;
)

# Per-target configuration
set_target_properties(myapp PROPERTIES
    CXX_CLANG_TIDY "clang-tidy;-checks=-*,readability-*"
)
```

---

## 🎨 2. Clang-Format (Formatter)

### What It Is

**Clang-format** automatically formats C++ code according to style guidelines:
- Enforces consistent indentation
- Manages whitespace and braces
- Aligns code elements
- Supports multiple style presets
- IDE integration available

### Installation

```bash
# Ubuntu/Debian
sudo apt install clang-format

# macOS
brew install clang-format

# Specific version
sudo apt install clang-format-14
```

### Basic Usage

```bash
# Format a file (in-place)
clang-format -i file.cpp

# Format multiple files
clang-format -i src/*.cpp include/*.h

# Check formatting without modifying
clang-format --dry-run --Werror file.cpp

# Use specific style
clang-format -style=google file.cpp

# Output to stdout
clang-format file.cpp > formatted.cpp
```

### Configuration (.clang-format)

```yaml
---
# Base style
BasedOnStyle: LLVM

# Indentation
IndentWidth: 4
TabWidth: 4
UseTab: Never
IndentCaseLabels: true

# Line length
ColumnLimit: 100

# Braces
BreakBeforeBraces: Allman
# Options: Attach, Linux, Mozilla, Stroustrup, Allman, Whitesmiths, GNU, WebKit, Custom

# Pointers and references
PointerAlignment: Left
# Options: Left, Right, Middle

# Spaces
SpaceAfterCStyleCast: false
SpaceBeforeParens: ControlStatements
SpacesInParentheses: false
SpacesInAngles: false

# Function calls
AllowShortFunctionsOnASingleLine: Empty
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false

# Include sorting
SortIncludes: true
IncludeBlocks: Regroup

# Alignment
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
AlignOperands: true
AlignTrailingComments: true

# Breaking
AlwaysBreakAfterReturnType: None
AlwaysBreakTemplateDeclarations: Yes
BreakBeforeBinaryOperators: None
BreakConstructorInitializers: BeforeColon
```

### Popular Style Presets

**LLVM Style:**
```cpp
void function(int param)
{
    if (condition) {
        doSomething();
    }
}
```

**Google Style:**
```cpp
void Function(int param) {
  if (condition) {
    DoSomething();
  }
}
```

**Mozilla Style:**
```cpp
void
Function(int param)
{
  if (condition) {
    DoSomething();
  }
}
```

**WebKit Style:**
```cpp
void function(int param)
{
    if (condition)
        doSomething();
}
```

### Example

**❌ Before Formatting:**

```cpp
void foo(int a,int b,int c){
if(a>b){cout<<"yes"<<endl;}
else{cout<<"no"<<endl;}}

class MyClass{public:int value;void setValue(int v){value=v;}};
```

**✅ After Formatting (LLVM style):**

```cpp
void foo(int a, int b, int c) {
    if (a > b) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
}

class MyClass {
public:
    int value;
    void setValue(int v) { value = v; }
};
```

### Git Integration

```bash
# Format changed files before commit
git diff --name-only --cached | grep -E '\.(cpp|h)$' | xargs clang-format -i

# Pre-commit hook (.git/hooks/pre-commit)
#!/bin/bash
for file in $(git diff --cached --name-only | grep -E '\.(cpp|h)$')
do
    clang-format -i "$file"
    git add "$file"
done
```

### IDE Integration

**VSCode:**
```json
{
    "editor.formatOnSave": true,
    "C_Cpp.clang_format_style": "file"
}
```

**CLion/IntelliJ:**
- Settings → Editor → Code Style → C/C++
- Enable "Enable ClangFormat"

**Vim:**
```vim
map <C-K> :py3f /usr/share/clang/clang-format.py<cr>
imap <C-K> <c-o>:py3f /usr/share/clang/clang-format.py<cr>
```

---

## 🔬 3. Static Analyzers

### 3.1 Cppcheck

**What it does:**
- Detects bugs and undefined behavior
- Memory leaks
- Null pointer dereferences
- Out of bounds access
- Dead code detection

**Installation:**
```bash
sudo apt install cppcheck  # Ubuntu/Debian
brew install cppcheck      # macOS
```

**Usage:**
```bash
# Basic check
cppcheck file.cpp

# Enable all checks
cppcheck --enable=all file.cpp

# Specific checks
cppcheck --enable=warning,style,performance file.cpp

# XML output for CI
cppcheck --xml --xml-version=2 src/ 2> cppcheck.xml

# Suppress specific warnings
cppcheck --suppress=memleak file.cpp
```

**Example Issues Detected:**

```cpp
// Memory leak
void leak() {
    int* ptr = new int(42);
    // Missing delete - cppcheck detects this!
}

// Null pointer dereference
void crash(int* ptr) {
    *ptr = 42;  // No null check - cppcheck warns!
}

// Array out of bounds
void bounds() {
    int arr[5];
    arr[10] = 42;  // Out of bounds - detected!
}

// Division by zero
void divide(int x) {
    int result = 10 / x;  // If x=0, crash! - warned!
}
```

### 3.2 Clang Static Analyzer

**What it does:**
- Path-sensitive analysis
- Deep symbolic execution
- API usage errors
- Memory management issues

**Usage:**
```bash
# Analyze a file
clang --analyze file.cpp

# With specific checkers
clang --analyze -Xanalyzer -analyzer-checker=core,cplusplus file.cpp

# HTML report
scan-build make

# Custom output directory
scan-build -o /tmp/analysis make
```

### 3.3 PVS-Studio

**What it does:**
- Commercial analyzer (free for open source)
- Very comprehensive checking
- Copy-paste error detection
- Security vulnerability detection
- MISRA/AUTOSAR compliance

**Usage:**
```bash
# Analyze with compilation database
pvs-studio-analyzer analyze -o report.log

# Convert to readable format
plog-converter -t fullhtml report.log -o report/

# Suppress warnings
pvs-studio-analyzer suppress -o suppress.txt
```

### 3.4 Coverity

**What it does:**
- Enterprise-grade analyzer
- Security vulnerabilities
- Quality defects
- Compliance checking

**Usage:**
```bash
# Build with Coverity
cov-build --dir cov-int make

# Analyze
cov-analyze --dir cov-int

# Generate report
cov-format-errors --dir cov-int --html-output html-report
```

### 3.5 SonarQube

**What it does:**
- Continuous inspection platform
- Code quality metrics
- Technical debt tracking
- Security hotspots
- CI/CD integration

---

## 🧪 4. Sanitizers

### 4.1 AddressSanitizer (ASan)

**What it detects:**
- Heap buffer overflow
- Stack buffer overflow
- Use after free
- Use after return
- Use after scope
- Double free
- Memory leaks
- Invalid pointer arithmetic

**Compilation:**
```bash
# Clang
clang++ -fsanitize=address -g file.cpp -o program

# GCC
g++ -fsanitize=address -g file.cpp -o program

# With optimization (recommended for testing)
g++ -fsanitize=address -O1 -g file.cpp -o program
```

**Environment Variables:**
```bash
# Log to file
export ASAN_OPTIONS=log_path=/tmp/asan.log

# Halt on error
export ASAN_OPTIONS=halt_on_error=1

# Continue after error
export ASAN_OPTIONS=halt_on_error=0

# Print statistics
export ASAN_OPTIONS=print_stats=1

# Customize malloc behavior
export ASAN_OPTIONS=malloc_context_size=15
```

**Example Output:**

```
=================================================================
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60300000eff4
WRITE of size 4 at 0x60300000eff4 thread T0
    #0 0x4a1234 in main file.cpp:10:5
    #1 0x7f123456789a in __libc_start_main
    
0x60300000eff4 is located 4 bytes to the right of 20-byte region
allocated by thread T0 here:
    #0 0x4b1234 in operator new[](unsigned long)
    #1 0x4a1000 in main file.cpp:9:15

SUMMARY: AddressSanitizer: heap-buffer-overflow file.cpp:10:5 in main
=================================================================
```

**Common Issues Detected:**

```cpp
// Heap buffer overflow
void heapOverflow() {
    int* arr = new int[5];
    arr[10] = 42;  // ASan catches this!
    delete[] arr;
}

// Stack buffer overflow
void stackOverflow() {
    int arr[5];
    arr[10] = 42;  // ASan catches this!
}

// Use after free
void useAfterFree() {
    int* ptr = new int(42);
    delete ptr;
    *ptr = 100;  // ASan catches this!
}

// Memory leak
void leak() {
    int* ptr = new int(42);
    // Not deleted - ASan reports leak!
}
```

### 4.2 UndefinedBehaviorSanitizer (UBSan)

**What it detects:**
- Signed integer overflow
- Division by zero
- Null pointer dereference
- Misaligned pointer access
- Out of bounds array access
- Invalid shift operations
- Invalid enum/bool values
- Invalid pointer arithmetic

**Compilation:**
```bash
# All UB checks
clang++ -fsanitize=undefined -g file.cpp

# Specific checks
clang++ -fsanitize=integer,null -g file.cpp

# Combined with ASan
clang++ -fsanitize=address,undefined -g file.cpp
```

**Individual Checks:**
```bash
-fsanitize=alignment          # Misaligned pointer
-fsanitize=bool               # Invalid bool value
-fsanitize=bounds             # Array bounds
-fsanitize=enum               # Invalid enum value
-fsanitize=float-divide-by-zero
-fsanitize=integer-divide-by-zero
-fsanitize=null               # Null pointer dereference
-fsanitize=return             # Missing return value
-fsanitize=shift              # Invalid shift
-fsanitize=signed-integer-overflow
-fsanitize=unsigned-integer-overflow
-fsanitize=vptr               # Invalid virtual pointer
```

**Example Issues:**

```cpp
// Signed integer overflow
void overflow() {
    int max = INT_MAX;
    int overflow = max + 1;  // UBSan catches!
}

// Division by zero
void divZero(int x) {
    int result = 10 / x;  // If x=0, UBSan catches!
}

// Invalid shift
void invalidShift() {
    int x = 1;
    int y = x << 35;  // Shift > 32 bits, UBSan catches!
}

// Null pointer dereference
void nullDeref(int* ptr) {
    *ptr = 42;  // If ptr==nullptr, UBSan catches!
}
```

### 4.3 ThreadSanitizer (TSan)

**What it detects:**
- Data races
- Deadlocks
- Thread leaks
- Improper synchronization
- Lock order violations

**Compilation:**
```bash
# Enable TSan
clang++ -fsanitize=thread -g file.cpp

# Note: Cannot combine with ASan!
```

**Example - Data Race:**

```cpp
// ❌ Data race
int counter = 0;

void thread1() {
    counter++;  // Race condition!
}

void thread2() {
    counter++;  // Race condition!
}

// ✅ Fixed with mutex
int counter = 0;
std::mutex mtx;

void thread1() {
    std::lock_guard<std::mutex> lock(mtx);
    counter++;  // Safe!
}
```

**TSan Output:**

```
WARNING: ThreadSanitizer: data race (pid=12345)
  Write of size 4 at 0x7fff12345678 by thread T1:
    #0 thread1() file.cpp:10

  Previous write of size 4 at 0x7fff12345678 by thread T2:
    #0 thread2() file.cpp:15

SUMMARY: ThreadSanitizer: data race file.cpp:10 in thread1()
```

### 4.4 MemorySanitizer (MSan)

**What it detects:**
- Uninitialized memory reads
- Use of uninitialized values

**Compilation:**
```bash
# Clang only (not in GCC)
clang++ -fsanitize=memory -g file.cpp

# Track origins for better debugging
clang++ -fsanitize=memory -fsanitize-memory-track-origins -g file.cpp
```

**Example:**

```cpp
// Uninitialized memory
int getValue() {
    int x;  // Not initialized!
    return x;  // MSan catches this!
}

// Fixed
int getValue() {
    int x = 0;  // Initialized
    return x;
}
```

### 4.5 LeakSanitizer (LSan)

**What it detects:**
- Memory leaks

**Usage:**
```bash
# Standalone
clang++ -fsanitize=leak file.cpp

# Included in ASan by default
clang++ -fsanitize=address file.cpp
```

### Sanitizer Comparison

| Sanitizer | Overhead | Platform | Combines with | Best For |
|-----------|----------|----------|---------------|----------|
| ASan | ~2x | All | UBSan, LSan | Memory errors |
| UBSan | ~20% | All | ASan, MSan | Undefined behavior |
| TSan | 5-15x | All | None | Threading bugs |
| MSan | ~3x | Linux | UBSan | Uninitialized memory |
| LSan | Minimal | All | ASan | Memory leaks |

---

## 🔍 5. Valgrind

### What It Is

Valgrind is a dynamic analysis framework with multiple tools:

### 5.1 Memcheck (Memory Errors)

**What it detects:**
- Memory leaks
- Use of uninitialized memory
- Invalid memory access
- Double frees
- Mismatched new/delete

**Usage:**
```bash
# Basic memcheck
valgrind ./program

# Detailed leak check
valgrind --leak-check=full ./program

# Show leak origins
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program

# Suppress known issues
valgrind --suppressions=my.supp ./program
```

**Example Output:**
```
==12345== Invalid write of size 4
==12345==    at 0x4A1234: main (file.cpp:10)
==12345==  Address 0x4c3f040 is 0 bytes after a block of size 20 alloc'd
==12345==    at 0x4B2345: operator new[](unsigned long)
==12345==    by 0x4A1000: main (file.cpp:9)

==12345== LEAK SUMMARY:
==12345==    definitely lost: 40 bytes in 1 blocks
==12345==    indirectly lost: 0 bytes in 0 blocks
==12345==      possibly lost: 0 bytes in 0 blocks
==12345==    still reachable: 0 bytes in 0 blocks
```

### 5.2 Cachegrind (Cache Profiler)

**What it does:**
- Cache miss analysis
- Branch prediction analysis
- CPU cycle simulation

**Usage:**
```bash
# Run cachegrind
valgrind --tool=cachegrind ./program

# View results
cg_annotate cachegrind.out.12345

# Compare two runs
cg_diff cachegrind.out.old cachegrind.out.new
```

### 5.3 Callgrind (Call Graph Profiler)

**What it does:**
- Function call analysis
- Performance profiling
- Call graph visualization

**Usage:**
```bash
# Run callgrind
valgrind --tool=callgrind ./program

# Visualize with KCachegrind
kcachegrind callgrind.out.12345
```

### 5.4 Helgrind (Thread Errors)

**What it detects:**
- Data races
- Lock ordering problems
- Misuse of POSIX pthreads API

**Usage:**
```bash
valgrind --tool=helgrind ./program
```

### 5.5 Massif (Heap Profiler)

**What it does:**
- Heap memory profiling
- Memory usage over time
- Stack profiling

**Usage:**
```bash
# Run massif
valgrind --tool=massif ./program

# View results
ms_print massif.out.12345

# Visualize
massif-visualizer massif.out.12345
```

---

## 📊 6. Code Coverage

### 6.1 gcov

**What it does:**
- Line coverage
- Branch coverage
- Function coverage

**Compilation:**
```bash
# Compile with coverage
g++ -fprofile-arcs -ftest-coverage file.cpp -o program

# Or use --coverage flag
g++ --coverage file.cpp -o program
```

**Usage:**
```bash
# Run the program (generates .gcda files)
./program

# Generate coverage report
gcov file.cpp

# View results
cat file.cpp.gcov
```

**Example output (file.cpp.gcov):**
```
        -:    0:Source:file.cpp
        -:    1:#include <iostream>
        5:    2:int add(int a, int b) {
        5:    3:    return a + b;
        -:    4:}
        -:    5:
    #####:    6:int unused() {
    #####:    7:    return 42;  // Never executed!
        -:    8:}
```

Legend:
- `-`: Non-executable line
- `5`: Executed 5 times
- `#####`: Never executed

### 6.2 lcov (HTML Reports)

**What it does:**
- Generates HTML coverage reports
- Branch coverage visualization
- Summary statistics

**Installation:**
```bash
sudo apt install lcov  # Ubuntu/Debian
brew install lcov      # macOS
```

**Usage:**
```bash
# Capture coverage data
lcov --capture --directory . --output-file coverage.info

# Remove system headers
lcov --remove coverage.info '/usr/*' --output-file coverage.info

# Generate HTML report
genhtml coverage.info --output-directory coverage-report

# View report
open coverage-report/index.html
```

### 6.3 Coverage Goals

```
Excellent:  80-100% coverage ✅
Good:       60-79%  coverage ⚠️
Fair:       40-59%  coverage 🔶
Poor:       <40%    coverage 🚨
```

**What to aim for:**
- **Critical code:** 100% coverage
- **Business logic:** 90-100% coverage
- **Utility code:** 80-90% coverage
- **UI code:** 60-80% coverage
- **Generated code:** Can be excluded

---

## ⚡ 7. Profilers

### 7.1 gprof

**What it does:**
- Function-level profiling
- Call graph generation
- Time spent per function

**Usage:**
```bash
# Compile with profiling
g++ -pg file.cpp -o program

# Run the program (generates gmon.out)
./program

# Generate report
gprof program gmon.out > analysis.txt

# Flat profile
gprof --flat-profile program gmon.out

# Call graph
gprof --graph program gmon.out
```

### 7.2 perf (Linux Performance Tools)

**What it does:**
- CPU profiling
- Hardware counter access
- System-wide profiling
- Flame graph generation

**Usage:**
```bash
# Record CPU profile
perf record ./program

# View report
perf report

# System-wide profiling
sudo perf record -a -g sleep 10

# Generate flame graph
perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg
```

### 7.3 Valgrind Callgrind (see Valgrind section)

---

## 📚 8. Documentation Tools

### 8.1 Doxygen

**What it does:**
- Generates API documentation
- Multiple output formats (HTML, PDF, LaTeX)
- Call graphs and diagrams
- Cross-references

**Installation:**
```bash
sudo apt install doxygen graphviz
brew install doxygen graphviz
```

**Configuration (Doxyfile):**
```bash
# Generate default config
doxygen -g

# Edit Doxyfile
PROJECT_NAME           = "My Project"
OUTPUT_DIRECTORY       = docs
INPUT                  = src include
RECURSIVE              = YES
EXTRACT_ALL            = YES
GENERATE_HTML          = YES
GENERATE_LATEX         = NO
HAVE_DOT               = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
```

**Documentation Comments:**
```cpp
/**
 * @brief Calculates the sum of two integers
 * 
 * This function takes two integer parameters and returns their sum.
 * It's a simple example of Doxygen documentation.
 * 
 * @param a First integer
 * @param b Second integer
 * @return Sum of a and b
 * 
 * @warning This function does not check for integer overflow
 * 
 * @code
 * int result = add(5, 3);  // result = 8
 * @endcode
 * 
 * @see subtract()
 * @author John Doe
 * @date 2024-01-01
 */
int add(int a, int b) {
    return a + b;
}

/**
 * @class Calculator
 * @brief A simple calculator class
 * 
 * This class provides basic arithmetic operations.
 */
class Calculator {
public:
    /**
     * @brief Default constructor
     */
    Calculator();
    
    /**
     * @brief Adds two numbers
     * @param[in] a First operand
     * @param[in] b Second operand
     * @param[out] result Sum of a and b
     * @return true if successful, false on overflow
     */
    bool add(int a, int b, int& result);
};
```

**Generate Documentation:**
```bash
# Generate docs
doxygen Doxyfile

# View HTML
open docs/html/index.html
```

---

## 🔄 9. Integration Strategies

### 9.1 CMake Integration

```cmake
# CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(MyProject)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options for tools
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Clang-Tidy
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY 
            ${CLANG_TIDY_EXE};
            -checks=*;
        )
    endif()
endif()

# Clang-Format
if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_EXE NAMES "clang-format")
    if(CLANG_FORMAT_EXE)
        file(GLOB_RECURSE ALL_SOURCE_FILES 
            ${PROJECT_SOURCE_DIR}/src/*.cpp
            ${PROJECT_SOURCE_DIR}/include/*.h
        )
        add_custom_target(
            format
            COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_SOURCE_FILES}
        )
    endif()
endif()

# AddressSanitizer
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# UndefinedBehaviorSanitizer
if(ENABLE_UBSAN)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

# ThreadSanitizer
if(ENABLE_TSAN)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

# Code Coverage
if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif()

# Your executable
add_executable(myapp src/main.cpp)

# Build with sanitizers
# mkdir build-asan && cd build-asan
# cmake -DENABLE_ASAN=ON ..
# make
```

### 9.2 CI/CD Pipeline (GitHub Actions)

```yaml
# .github/workflows/ci.yml
name: CI

on: [push, pull_request]

jobs:
  format-check:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install clang-format
        run: sudo apt-get install clang-format
      
      - name: Check formatting
        run: |
          clang-format --dry-run --Werror $(find src include -name '*.cpp' -o -name '*.h')
  
  lint:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install clang-tidy
        run: sudo apt-get install clang-tidy
      
      - name: Run clang-tidy
        run: |
          clang-tidy src/*.cpp -- -std=c++17
  
  static-analysis:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install cppcheck
        run: sudo apt-get install cppcheck
      
      - name: Run cppcheck
        run: |
          cppcheck --enable=all --error-exitcode=1 src/
  
  build-sanitizers:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        sanitizer: [address, undefined, thread]
    steps:
      - uses: actions/checkout@v3
      
      - name: Build with ${{ matrix.sanitizer }} sanitizer
        run: |
          mkdir build
          cd build
          cmake -DENABLE_$(echo ${{ matrix.sanitizer }} | tr '[:lower:]' '[:upper:]')SAN=ON ..
          make
      
      - name: Run tests
        run: |
          cd build
          ./run_tests
  
  coverage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install lcov
        run: sudo apt-get install lcov
      
      - name: Build with coverage
        run: |
          mkdir build
          cd build
          cmake -DENABLE_COVERAGE=ON ..
          make
      
      - name: Run tests
        run: |
          cd build
          ./run_tests
      
      - name: Generate coverage report
        run: |
          lcov --capture --directory build --output-file coverage.info
          lcov --remove coverage.info '/usr/*' --output-file coverage.info
          genhtml coverage.info --output-directory coverage
      
      - name: Upload coverage
        uses: codecov/codecov-action@v3
        with:
          files: ./coverage.info
```

### 9.3 Pre-commit Hooks

```bash
#!/bin/bash
# .git/hooks/pre-commit

echo "Running pre-commit checks..."

# Get list of staged C++ files
STAGED_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|h)$')

if [ -z "$STAGED_FILES" ]; then
    echo "No C++ files to check"
    exit 0
fi

# Format check
echo "Checking code formatting..."
clang-format -i $STAGED_FILES
git add $STAGED_FILES

# Lint check
echo "Running clang-tidy..."
for file in $STAGED_FILES; do
    clang-tidy "$file" -- -std=c++17
    if [ $? -ne 0 ]; then
        echo "clang-tidy found issues in $file"
        exit 1
    fi
done

# Static analysis
echo "Running cppcheck..."
cppcheck --enable=warning,style --error-exitcode=1 $STAGED_FILES
if [ $? -ne 0 ]; then
    echo "cppcheck found issues"
    exit 1
fi

echo "All checks passed!"
exit 0
```

Make it executable:
```bash
chmod +x .git/hooks/pre-commit
```

---

## 🎯 Recommended Workflow

### Development Phase

```
Local Development:
1. IDE with clang-format on save
2. clang-tidy enabled in IDE
3. Compile with -Wall -Wextra -Werror
4. Run with ASan during development
```

### Before Commit

```
Pre-commit Checks:
1. Auto-format with clang-format
2. Run clang-tidy
3. Quick cppcheck scan
4. Unit tests with sanitizers
```

### CI/CD Pipeline

```
Continuous Integration:
1. Format checking (fail on violations)
2. Full clang-tidy analysis
3. cppcheck + other static analyzers
4. Build with all sanitizers separately
5. Run full test suite
6. Generate coverage reports
7. Performance benchmarks
```

### Weekly/Regular

```
Deep Analysis:
1. Valgrind memcheck on full suite
2. Profile with callgrind/perf
3. Review coverage trends
4. Update tools to latest versions
5. Technical debt review
```

---

## 📋 Tool Combination Matrix

| Use Case | Tools | Rationale |
|----------|-------|-----------|
| **Daily Development** | clang-format + clang-tidy + ASan | Fast feedback, catch issues early |
| **Testing** | ASan + UBSan + Coverage | Find runtime errors, measure tests |
| **CI/CD** | All static analyzers + All sanitizers | Comprehensive checking |
| **Performance** | Valgrind Callgrind + perf + Massif | Profile and optimize |
| **Thread Safety** | TSan + Helgrind | Find concurrency issues |
| **Release Candidate** | All tools + Manual review | Maximum quality assurance |

---

## ✅ Best Practices

### Do's ✅

1. **Integrate early** - Add tools from project start
2. **Automate everything** - CI/CD, pre-commit hooks
3. **Fix issues immediately** - Don't accumulate technical debt
4. **Use multiple tools** - Each catches different issues
5. **Test with sanitizers** - Always run tests with ASan/UBSan
6. **Track coverage** - Aim for 80%+ on critical code
7. **Profile before optimizing** - Measure, don't guess
8. **Document consistently** - Use Doxygen from the start
9. **Update regularly** - Keep tools up to date
10. **Train the team** - Ensure everyone knows the tools

### Don'ts ❌

1. **Don't skip sanitizers** - They catch real bugs
2. **Don't ignore warnings** - Fix or suppress with reason
3. **Don't commit unformatted code** - Use pre-commit hooks
4. **Don't disable checks without reason** - Document why
5. **Don't test only in Debug** - Test with optimizations too
6. **Don't forget thread safety** - Use TSan for concurrent code
7. **Don't rely on one tool** - Combine multiple analyzers
8. **Don't premature optimize** - Profile first
9. **Don't neglect documentation** - Document as you code
10. **Don't make CI optional** - Enforce quality gates

---

## 🚀 Quick Start Guide

### Minimal Setup (15 minutes)

```bash
# 1. Install tools
sudo apt install clang-tidy clang-format cppcheck

# 2. Create .clang-format
clang-format -style=llvm -dump-config > .clang-format

# 3. Create .clang-tidy
echo "Checks: 'modernize-*,readability-*,performance-*'" > .clang-tidy

# 4. Format all code
find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i

# 5. Run checks
clang-tidy src/*.cpp -- -std=c++17
cppcheck --enable=all src/

# 6. Build with sanitizers
g++ -fsanitize=address,undefined -g src/*.cpp -o program

# 7. Run tests
./program
```

### Full Setup (1 hour)

Follow the CMake integration + CI/CD pipeline setup from sections 9.1 and 9.2.

---

## 📊 Tool Comparison

| Tool | Type | Speed | Accuracy | Ease of Use | Cost |
|------|------|-------|----------|-------------|------|
| clang-tidy | Linter | Fast | High | Easy | Free |
| clang-format | Formatter | Very Fast | Perfect | Very Easy | Free |
| cppcheck | Static | Fast | Good | Easy | Free |
| PVS-Studio | Static | Medium | Very High | Medium | $$$ |
| Coverity | Static | Slow | Very High | Medium | $$$ |
| ASan | Runtime | Fast (2x) | Very High | Easy | Free |
| UBSan | Runtime | Fast (1.2x) | Very High | Easy | Free |
| TSan | Runtime | Slow (10x) | Very High | Easy | Free |
| MSan | Runtime | Medium (3x) | Very High | Medium | Free |
| Valgrind | Runtime | Very Slow (20x) | Very High | Medium | Free |

---

## 🎓 Learning Resources

### Official Documentation

- **Clang Tools**: https://clang.llvm.org/docs/
- **Sanitizers**: https://github.com/google/sanitizers/wiki
- **Valgrind**: https://valgrind.org/docs/manual/
- **Doxygen**: https://www.doxygen.nl/manual/

### Books

- **"Effective Modern C++"** by Scott Meyers
- **"C++ Core Guidelines"** by Bjarne Stroustrup & Herb Sutter
- **"Debugging with GDB"** by Richard Stallman

### Online Resources

- **CppCoreGuidelines**: https://isocpp.github.io/CppCoreGuidelines/
- **Compiler Explorer**: https://godbolt.org/
- **Quick Bench**: https://quick-bench.com/

---

## 💡 Key Takeaways

1. **Tools are essential** - Modern C++ development requires good tools
2. **Automate everything** - Let tools do the tedious work
3. **Catch bugs early** - Static analysis + sanitizers save time
4. **Consistent style** - Use formatters to eliminate style debates
5. **Test thoroughly** - Coverage + sanitizers ensure quality
6. **Profile before optimizing** - Measure, don't guess
7. **Integrate into workflow** - Make tools part of daily development
8. **Keep learning** - Tools evolve, stay updated

---

## 🎯 Final Checklist

```
□ clang-format configured and integrated
□ clang-tidy enabled in build system
□ Static analyzer (cppcheck minimum) in CI
□ ASan enabled for all tests
□ UBSan enabled for all tests
□ TSan for concurrent code
□ Code coverage tracking (80%+ goal)
□ Pre-commit hooks configured
□ CI/CD pipeline with all checks
□ Doxygen for API documentation
□ Team trained on tools
□ Regular tool updates scheduled
```

---

**Remember: The best time to set up tools was at project start. The second best time is now!**