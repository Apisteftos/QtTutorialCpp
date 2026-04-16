# Code Style & Formatting - Complete Guide

## 📚 Overview

**Code style and formatting** refers to the consistent, readable way of writing code that makes it easy for humans to read and maintain. Good formatting is invisible - it just makes code effortless to understand.

### Why It Matters

✅ **Readability** - Code is read 10x more than written  
✅ **Maintainability** - Easier to spot errors and modify  
✅ **Collaboration** - Team works together smoothly  
✅ **Professionalism** - Shows attention to detail  
✅ **Consistency** - Reduces cognitive load  

---

## 🎯 Core Principles

### The Golden Rule

> **"Be consistent! Pick a style and use it everywhere!"**

Consistency is more important than any specific style choice. A codebase with consistent but "wrong" style is better than one with inconsistent "perfect" style.

---

## 📝 Naming Conventions

### Standard Conventions

#### C++ Standard Library Style (Recommended)

```cpp
// Variables and functions: snake_case
int user_age;
string customer_name;
double total_price;

void calculate_total() { }
void process_order() { }

// Classes and structs: PascalCase
class UserAccount { };
class OrderManager { };
struct CustomerData { };

// Constants: UPPER_SNAKE_CASE
const double TAX_RATE = 0.19;
const int MAX_USERS = 100;
constexpr double PI = 3.14159;

// Private members: trailing underscore
class MyClass {
private:
    int value_;
    string name_;
public:
    int getValue() const { return value_; }
};
```

#### Google C++ Style (Alternative)

```cpp
// Variables and functions: snake_case
int user_age;
void calculate_total() { }

// Classes: PascalCase
class UserAccount { };

// Constants: kConstantName (k prefix)
const double kTaxRate = 0.19;
const int kMaxUsers = 100;

// Private members: trailing underscore
class MyClass {
private:
    int value_;
};
```

### Naming Guidelines

**Variables**
```cpp
// ❌ BAD
int d;          // What is d?
int data1;      // Generic
int x, y, z;    // Meaningless

// ✅ GOOD
int days_until_expiry;
int customer_count;
int width, height, depth;  // Clear meaning
```

**Functions**
```cpp
// ❌ BAD
void proc();        // Vague
void do_it();       // What does it do?
void data();        // Not a verb

// ✅ GOOD
void processPayment();
void calculateTotal();
void validateUserInput();
```

**Classes**
```cpp
// ❌ BAD
class data { };     // Too generic
class mgr { };      // Abbreviation
class user_data { };  // Wrong case

// ✅ GOOD
class UserAccount { };
class PaymentProcessor { };
class DatabaseConnection { };
```

**Constants**
```cpp
// ❌ BAD
const int max = 100;       // Not clear it's constant
const double TaxRate = 0.19;  // Mixed case

// ✅ GOOD
const int MAX_USERS = 100;
const double TAX_RATE = 0.19;
constexpr double PI = 3.14159;
```

---

## 🎨 Brace Styles

### K&R Style (Most Popular in C++)

```cpp
void function() {
    if (condition) {
        doSomething();
    } else {
        doSomethingElse();
    }
}

class MyClass {
public:
    void method() {
        // Implementation
    }
};
```

**Pros:**
- More compact
- Most common in C++
- Used by Google, LLVM
- Matches standard library style

### Allman Style

```cpp
void function()
{
    if (condition)
    {
        doSomething();
    }
    else
    {
        doSomethingElse();
    }
}

class MyClass
{
public:
    void method()
    {
        // Implementation
    }
};
```

**Pros:**
- Clearer visual structure
- Easier to match braces
- Popular in C#

### Comparison

| Style | Opening Brace | Popularity | Space Usage |
|-------|--------------|------------|-------------|
| **K&R** | Same line | ⭐⭐⭐⭐⭐ C++ | Compact |
| **Allman** | New line | ⭐⭐⭐ C# | More vertical |

**Recommendation:** Use K&R for C++ projects

---

## 📏 Indentation

### Spaces vs Tabs

**Use Spaces (Strongly Recommended)**

```cpp
// 4 spaces (most common)
void function() {
    if (condition) {
        doSomething();
        if (nested) {
            doMore();
        }
    }
}

// 2 spaces (Google style)
void function() {
  if (condition) {
    doSomething();
  }
}
```

**Why Spaces?**
- ✅ Looks same in all editors
- ✅ No confusion about tab width
- ✅ Consistent in diffs and patches
- ✅ Works with all tools

**Configure Your Editor:**
```
// VS Code settings.json
{
    "editor.insertSpaces": true,
    "editor.tabSize": 4
}

// Vim
set expandtab
set tabstop=4
set shiftwidth=4
```

### Indentation Guidelines

- **One level:** One if/for/while/function
- **4 spaces:** Most common (LLVM, Mozilla)
- **2 spaces:** Google style, more compact
- **Never mix:** Tabs and spaces = disaster!

---

## 🌊 Spacing & Whitespace

### Operators

```cpp
// ❌ BAD: No spacing
int result=a+b*c;
if(x>10){doSomething();}

// ✅ GOOD: Proper spacing
int result = a + b * c;
if (x > 10) { doSomething(); }
```

### Commas

```cpp
// ❌ BAD
void function(int a,int b,int c){}
auto v = vector<int>{1,2,3,4,5};

// ✅ GOOD
void function(int a, int b, int c) { }
auto v = vector<int>{1, 2, 3, 4, 5};
```

### Keywords

```cpp
// ❌ BAD
if(condition){}
for(int i=0;i<n;i++){}
while(running){}

// ✅ GOOD
if (condition) { }
for (int i = 0; i < n; i++) { }
while (running) { }
```

### Logical Grouping

```cpp
void processData() {
    // Initialize variables
    int total = 0;
    int count = 0;
    
    // Process data
    for (auto value : data) {
        total += value;
        count++;
    }
    
    // Calculate result
    double average = static_cast<double>(total) / count;
    return average;
}
```

### Spacing Rules Summary

| Element | Spacing |
|---------|---------|
| After comma | `foo(a, b, c)` |
| Around binary operators | `a + b`, `x == y` |
| After keywords | `if (x)`, `for (...)` |
| Before opening brace | `void func() {` |
| Function call | `foo()` (no space) |
| Inside parentheses | `if (x)` (no space) |
| Between functions | Blank line |
| Between sections | Blank line |

---

## 📐 Line Length

### Guidelines

- **80 characters:** Traditional, still common
- **100 characters:** Modern compromise (recommended)
- **120 characters:** Maximum for most teams

### Breaking Long Lines

#### Function Parameters

```cpp
// ❌ BAD: Too long
void createUser(string firstName, string lastName, string email, string phone, string address, string city, string zip, string country) {
}

// ✅ GOOD: Use struct
struct UserData {
    string firstName, lastName;
    string email, phone;
    string address, city, zip, country;
};

void createUser(const UserData& user) {
}

// ✅ GOOD: Break at commas
void createUser(
    const string& firstName,
    const string& lastName,
    const string& email,
    const string& phone) {
}
```

#### Conditions

```cpp
// ❌ BAD: One long line
if (isValidUser(user) && hasPermission(user) && isWithinBusinessHours() && !isMaintenanceMode()) {
}

// ✅ GOOD: Break at operators
if (isValidUser(user) &&
    hasPermission(user) &&
    isWithinBusinessHours() &&
    !isMaintenanceMode()) {
    processRequest();
}
```

#### Method Chains

```cpp
// ❌ BAD: One long line
auto result = data.filter([](int x) { return x > 0; }).map([](int x) { return x * 2; }).reduce(0, [](int a, int b) { return a + b; });

// ✅ GOOD: One method per line
auto result = data
    .filter([](int x) { return x > 0; })
    .map([](int x) { return x * 2; })
    .reduce(0, [](int a, int b) { return a + b; });
```

---

## 🗂️ File Organization

### Include Order

```cpp
// ============================================
// MyClass.cpp
// ============================================

// 1. Related header first
#include "MyClass.h"

// 2. C standard library
#include <cstdio>
#include <cmath>

// 3. C++ standard library (alphabetical)
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// 4. Third-party libraries
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

// 5. Project headers (alphabetical)
#include "Helper.h"
#include "Utility.h"

// Only in .cpp files, NEVER in headers!
using namespace std;
```

### Class Organization

```cpp
class MyClass {
public:
    // 1. Constructors and destructor
    MyClass();
    MyClass(int value);
    ~MyClass();
    
    // 2. Public methods (grouped logically)
    void initialize();
    void process();
    void finalize();
    
    // 3. Getters and setters
    int getValue() const { return value_; }
    void setValue(int value) { value_ = value; }
    
protected:
    // Protected methods (if needed)
    void protectedHelper();
    
private:
    // 4. Private helper methods
    void validateData();
    void cleanup();
    
    // 5. Member variables (at end)
    int value_;
    string name_;
    vector<int> data_;
};
```

### Source File Organization

```cpp
// 1. File header comment
// ============================================
// UserManager.cpp
// Description: Manages user accounts
// Author: Your Name
// Date: 2024-11-18
// ============================================

// 2. Includes
#include "UserManager.h"
#include <iostream>

// 3. Anonymous namespace for file-scope constants
namespace {
    const int MAX_RETRIES = 3;
    const double TIMEOUT = 30.0;
}

// 4. Constructors/Destructors
UserManager::UserManager() { }
UserManager::~UserManager() { }

// 5. Public methods (in header order)
void UserManager::createUser() { }
void UserManager::deleteUser() { }

// 6. Private methods
void UserManager::validateUser() { }
```

---

## 💬 Comment Style

### Doxygen Documentation

```cpp
/**
 * @brief Calculate the total price with discounts
 * 
 * Applies bulk discounts based on quantity:
 * - 10% off for quantities >= 100
 * - 20% off for quantities >= 500
 * 
 * @param quantity Number of items to purchase
 * @param pricePerUnit Price of a single item
 * @return Total price after discounts
 * @throws invalid_argument if quantity or price is negative
 */
double calculateBulkPrice(int quantity, double pricePerUnit);
```

### Inline Comments

```cpp
// Good: Explains WHY
void processPayment() {
    // Use exponential backoff to avoid overwhelming payment gateway
    for (int i = 0; i < MAX_RETRIES; i++) {
        if (attemptPayment()) {
            return;
        }
        sleep(pow(2, i));  // 1s, 2s, 4s...
    }
}
```

### Special Markers

```cpp
// TODO: Add input validation
void processInput(string input) { }

// FIXME: This is O(n²), needs optimization
void slowSort(vector<int>& data) { }

// HACK: Workaround for bug in library v1.2
// Remove when upgraded to v1.3
void temporaryFix() { }

// NOTE: Thread-safe as of version 2.0
void sharedResourceAccess() { }
```

### Comment Guidelines

**DO:**
- ✅ Explain WHY, not WHAT
- ✅ Document complex algorithms
- ✅ Use Doxygen for public APIs
- ✅ Capitalize and punctuate properly
- ✅ Use TODO/FIXME/HACK markers

**DON'T:**
- ❌ Comment obvious code
- ❌ Leave commented-out code
- ❌ Use comments to fix bad names
- ❌ Write novels - keep it concise

---

## ✨ Modern C++ Style

### Use auto (Appropriately)

```cpp
// ✅ GOOD: Type is obvious
auto x = 5;
auto name = string{"Alice"};
auto v = vector<int>{1, 2, 3};
auto it = map.begin();

// ❌ BAD: Type not obvious
auto result = calculate();  // What type?

// ✅ GOOD: Type is clear
double result = calculate();
```

### Use nullptr

```cpp
// ❌ BAD
int* ptr = NULL;
void* p = 0;

// ✅ GOOD
int* ptr = nullptr;
void* p = nullptr;
```

### Use enum class

```cpp
// ❌ BAD: Pollutes namespace
enum Color { RED, GREEN, BLUE };
int x = RED;  // Compiles but wrong!

// ✅ GOOD: Type-safe
enum class Color { Red, Green, Blue };
Color c = Color::Red;
```

### Use Range-Based For

```cpp
// ❌ OLD
for (int i = 0; i < v.size(); i++) {
    process(v[i]);
}

// ✅ MODERN
for (const auto& item : v) {
    process(item);
}
```

### Use Smart Pointers

```cpp
// ❌ OLD: Manual management
Widget* w = new Widget();
// ... use w
delete w;

// ✅ MODERN: RAII
auto w = make_unique<Widget>();
// Automatic cleanup
```

### Use Uniform Initialization

```cpp
// ❌ AMBIGUOUS
vector<int> v(10);     // 10 elements or {10}?

// ✅ CLEAR
vector<int> v{10};     // One element: 10
vector<int> v(10);     // 10 default elements
int x{5};              // Initialize x
```

---

## 🔧 Tools & Automation

### clang-format

**Installation:**
```bash
# Ubuntu/Debian
sudo apt install clang-format

# macOS
brew install clang-format

# Windows
# Download from LLVM website
```

**Usage:**
```bash
# Format file in-place
clang-format -i file.cpp

# Show formatted output
clang-format file.cpp

# Format all C++ files
find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i

# Use predefined style
clang-format -style=Google -i file.cpp
clang-format -style=LLVM -i file.cpp
```

### .clang-format Configuration

```yaml
---
# .clang-format
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
PointerAlignment: Left
BreakBeforeBraces: Attach
AllowShortFunctionsOnASingleLine: Empty
AllowShortIfStatementsOnASingleLine: Never
SpaceAfterCStyleCast: true
SpacesInParentheses: false
Standard: c++17
```

### Editor Integration

**VS Code:**
```json
{
    "editor.formatOnSave": true,
    "C_Cpp.clang_format_style": "file"
}
```

**Vim:**
```vim
" .vimrc
autocmd FileType cpp setlocal formatprg=clang-format\ -style=file
```

**CLion:**
- Settings → Editor → Code Style → C/C++
- Enable ClangFormat

### Pre-commit Hook

```bash
#!/bin/bash
# .git/hooks/pre-commit

FILES=$(git diff --cached --name-only --diff-filter=ACM | \
        grep -E '\.(cpp|h|hpp)$')

for FILE in $FILES; do
    clang-format -i "$FILE"
    git add "$FILE"
done
```

---

## 📊 Popular Style Guides

### Google C++ Style Guide

**Key Points:**
- 2-space indentation
- K&R bracing
- 80 character line limit
- snake_case for variables/functions
- PascalCase for classes
- kConstantName for constants

### LLVM Coding Standards

**Key Points:**
- 2-space indentation
- K&R bracing
- 80 character line limit
- Similar to standard library style

### Mozilla Coding Style

**Key Points:**
- 2-space indentation
- Allman bracing
- 80 character line limit
- camelCase for variables

### C++ Core Guidelines

**Focus:**
- Modern C++ practices
- Safety and correctness
- Performance
- Not just formatting

---

## ✅ Complete Checklist

### Before Committing Code

**Naming:**
- [ ] Consistent convention throughout
- [ ] Classes in PascalCase
- [ ] Variables/functions in snake_case or camelCase
- [ ] Constants in UPPER_SNAKE_CASE
- [ ] Descriptive names (no x, tmp, data)

**Formatting:**
- [ ] Consistent brace style (K&R or Allman)
- [ ] Consistent indentation (2 or 4 spaces)
- [ ] Space after commas
- [ ] Space around binary operators
- [ ] Space after keywords (if, for, while)
- [ ] Lines under 100 characters
- [ ] Blank lines between functions

**Organization:**
- [ ] Includes in correct order
- [ ] Alphabetical within groups
- [ ] Public before private in classes
- [ ] Member variables at end of class
- [ ] Related methods grouped together

**Comments:**
- [ ] Doxygen for public API
- [ ] Explain WHY, not WHAT
- [ ] Proper capitalization
- [ ] No commented-out code
- [ ] TODO/FIXME where appropriate

**Modern C++:**
- [ ] Use auto where appropriate
- [ ] Use nullptr (not NULL)
- [ ] Use enum class
- [ ] Use range-based for
- [ ] Use smart pointers
- [ ] Use constexpr

---

## 🎯 Key Takeaways

1. **Consistency beats perfection** - Pick a style, stick to it
2. **Automate formatting** - Use clang-format
3. **Follow team standards** - Don't go rogue
4. **Make code readable** - Formatting serves readability
5. **Use modern C++** - Embrace new features
6. **Document thoughtfully** - Comments explain WHY
7. **Organize logically** - Structure helps understanding

---

## 💡 Final Wisdom

> "Programs must be written for people to read,  
> and only incidentally for machines to execute."  
> — Abelson & Sussman

> "Code formatting is like typography.  
> Done well, you don't notice it.  
> Done poorly, you can't read anything."

> "Any fool can write code that a computer can understand.  
> Good programmers write code that humans can understand."  
> — Martin Fowler

---

## 🚀 Next Steps

After mastering code style and formatting:
- **Design Principles** - SOLID, DRY, KISS
- **Best Practices** - C++ specific guidelines
- **Code Review** - Giving and receiving feedback
- **Refactoring** - Improving existing code

---

## 📚 Resources

- **Google C++ Style Guide:** https://google.github.io/styleguide/cppguide.html
- **LLVM Coding Standards:** https://llvm.org/docs/CodingStandards.html
- **C++ Core Guidelines:** https://isocpp.github.io/CppCoreGuidelines/
- **clang-format:** https://clang.llvm.org/docs/ClangFormat.html