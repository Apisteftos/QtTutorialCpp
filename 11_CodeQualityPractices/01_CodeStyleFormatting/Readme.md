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

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### 🎉 CODE STYLE & FORMATTING COMPLETE! 🎉 (closing summary)

You've mastered professional code formatting!  
Your code will look clean and consistent!

Next: Apply these principles with clang-format!

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `example1_naming_conventions()`

- ❌ BAD CODE (Inconsistent):

```cpp

```cpp
// Mixing styles randomly
int UserAge;            // PascalCase for variable (wrong!)
string customer_name;   // snake_case
double TotalPrice;      // PascalCase again
bool isactive;          // no separators

void Calculate_Total() { }  // Mixed style
void processOrder() { }     // Different style

class user_account { };     // snake_case for class (wrong!)
class OrderManager { };     // PascalCase

const double tax_rate = 0.19;  // Should be UPPER
const int MAX_USERS = 100;     // Correct
```

```

**🚨 PROBLEMS:**

  - Mixing camelCase, PascalCase, snake_case randomly
  - No clear pattern
  - Confusing for readers
  - Hard to remember which style to use

- ✅ GOOD CODE (C++ Standard Style):

```cpp

```cpp
// Variables and functions: snake_case (like STL)
int user_age;
string customer_name;
double total_price;
bool is_active;

void calculate_total() { }
void process_order() { }

// Classes: PascalCase
class UserAccount { };
class OrderManager { };

// Constants: UPPER_SNAKE_CASE
const double TAX_RATE = 0.19;
const int MAX_USERS = 100;

// Private members: trailing underscore (optional)
class MyClass {
```

**private:**

```cpp
int value_;        // Private member
string name_;
```

**public:**

```cpp
    int getValue() const { return value_; }
};
```

```

- ✅ ALTERNATIVE (Google C++ Style):

```cpp

```cpp
// Variables and functions: snake_case
int user_age;
void calculate_total() { }

// Classes: PascalCase
class UserAccount { };

// Constants: kConstantName (Google style)
const double kTaxRate = 0.19;
const int kMaxUsers = 100;

// Private members: trailing underscore
class MyClass {
```

**private:**

```cpp
    int value_;
};
```

```

**✨ IMPROVEMENTS:**

- ✅ Consistent style throughout
- ✅ Easy to recognize types (class vs variable)
- ✅ Follows established conventions
- ✅ Clear distinction between public/private

- 💡 KEY PRINCIPLE:

"Pick ONE naming convention and use it consistently!  
Follow your team's or project's style guide."

### From `example2_brace_styles()`

- ❌ BAD CODE (Inconsistent Braces):

```cpp

```cpp
void function1() {  // K&R style
    if (condition) {
        doSomething();
    }
}

void function2()    // Allman style
{
```

if (condition)

```cpp
    {
        doSomething();
    }
}

void function3() {  // Mixed!
```

if (condition)

```cpp
    {
        doSomething(); }
}
```

```

**🚨 PROBLEMS:**

  - Three different brace styles in same file
  - Inconsistent and confusing
  - Makes code look messy
  - Harder to scan visually

- ✅ GOOD CODE (K&R Style - Most Popular):

```cpp

```cpp
void function1() {
    if (condition) {
        doSomething();
    } else {
        doSomethingElse();
    }
}

void function2() {
    for (int i = 0; i < n; i++) {
        process(i);
    }
}

class MyClass {
```

**public:**

```cpp
    void method() {
        // Implementation
    }
};
```

```

- ✅ ALTERNATIVE (Allman Style):

```cpp

```cpp
void function1()
{
```

if (condition)

```cpp
{
    doSomething();
}
```

else

```cpp
    {
        doSomethingElse();
    }
}

class MyClass
{
```

**public:**

```cpp
    void method()
    {
        // Implementation
    }
};
```

```

**✨ IMPROVEMENTS:**

- ✅ Consistent style throughout file
- ✅ Easy to scan and read
- ✅ Professional appearance
- ✅ Matches team/project style

**📋 BRACE STYLE COMPARISON:**

K&R (Kernighan & Ritchie):

  - Opening brace on same line
  - Most compact
  - Most popular in C++
  - Used by Google, LLVM

Allman:

  - Opening brace on new line
  - More vertical space
  - Popular in C#, some C++
  - Clearer structure visibility

- 💡 KEY PRINCIPLE:

"Pick K&R or Allman, use it everywhere!  
Most C++ projects use K&R."

### From `example3_indentation()`

- ❌ BAD CODE (Inconsistent Indentation):

```cpp

```cpp
void processData() {
  if (condition1) {    // 2 spaces
      doSomething();   // 6 spaces
    if (condition2) {  // 4 spaces
       doMore();       // 7 spaces
    }
  }
}

// Mixing tabs and spaces (disaster!)
void mixedIndentation() {
```

- →   if (x) {        // Tab + spaces
- →   doIt();    // Spaces + tab

```cpp
    }
}
```

```

**🚨 PROBLEMS:**

  - Random indentation levels
  - Mixing tabs and spaces (worst!)
  - Hard to see code structure
  - Looks unprofessional
  - Editor-dependent appearance

- ✅ GOOD CODE (4 Spaces - Most Common):

```cpp

```cpp
void processData() {
    if (condition1) {
        doSomething();
        if (condition2) {
            doMore();
        }
    }
}

class MyClass {
```

**public:**

```cpp
void method() {
    for (int i = 0; i < n; i++) {
        process(i);
    }
}
```

**private:**

```cpp
    int value_;
};
```

```

- ✅ ALTERNATIVE (2 Spaces - Google Style):

```cpp

```cpp
void processData() {
  if (condition1) {
    doSomething();
    if (condition2) {
      doMore();
    }
  }
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Consistent indentation level
- ✅ Clear visual hierarchy
- ✅ Easy to spot structure
- ✅ Same in all editors

**📊 INDENTATION GUIDELINES:**

  - Use spaces (not tabs) for consistency
  - 4 spaces: Most common (LLVM, Mozilla)
  - 2 spaces: More compact (Google)
  - Configure editor to insert spaces
  - Never mix tabs and spaces!

- 💡 KEY PRINCIPLE:

"Use 2 or 4 spaces consistently. Never tabs!  
Configure your editor properly."

### From `example4_spacing()`

- ❌ BAD CODE (Poor Spacing):

```cpp

```cpp
int calculate(int x,int y){  // No spaces!
    int result=x+y*2;        // Cramped
    if(result>100){          // Hard to read
        result=result-10;
    }
    return result;
}

void processArray(vector<int>&v){
    for(int i=0;i<v.size();i++){
        v[i]=v[i]*2;
    }
}
```

```

**🚨 PROBLEMS:**

  - No spaces around operators
  - No spaces after commas
  - No space before opening brace
  - Cramped and hard to read
  - Looks unprofessional

- ✅ GOOD CODE (Proper Spacing):

```cpp

```cpp
int calculate(int x, int y) {
    int result = x + y * 2;
    if (result > 100) {
        result = result - 10;
    }
    return result;
}

void processArray(vector<int>& v) {
    for (int i = 0; i < v.size(); i++) {
        v[i] = v[i] * 2;
    }
}

// Logical grouping with blank lines
void complexFunction() {
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

```

**✨ IMPROVEMENTS:**

- ✅ Space after comma
- ✅ Space around operators (=, +, -, *, <, >)
- ✅ Space before opening brace
- ✅ Space after keywords (if, for, while)
- ✅ Blank lines between logical sections

**📋 SPACING RULES:**

- ✅ Space after comma: foo(a, b, c)
- ✅ Space around binary operators: a + b
- ✅ Space after keywords: if (condition)
- ✅ Space before opening brace: void func() {
- ❌ No space after function name: foo()
- ❌ No space inside parentheses: if (x)
- ✅ Blank line between functions
- ✅ Blank line between logical sections

- 💡 KEY PRINCIPLE:

"Breathing room makes code readable!  
Space around operators and after commas."

### From `example5_line_length()`

- ❌ BAD CODE (Lines Too Long):

```cpp

```cpp
void processUserData(string firstName, string lastName, string email, string phoneNumber, string address, string city, string zipCode, string country, int age, bool isActive) { // 180+ characters!
    if (firstName.empty() || lastName.empty() || email.empty() || phoneNumber.empty() || address.empty() || city.empty() || zipCode.empty() || country.empty()) { // Impossible to read!
        throw invalid_argument("All fields are required: firstName, lastName, email, phoneNumber, address, city, zipCode, country");
    }
}
```

```

**🚨 PROBLEMS:**

  - Lines over 120 characters
  - Horizontal scrolling required
  - Hard to read on smaller screens
  - Can't see code in split view
  - Difficult in code reviews

- ✅ GOOD CODE (Proper Line Breaks):

```cpp

```cpp
// Use struct for many parameters
struct UserData {
    string firstName, lastName;
    string email, phoneNumber;
    string address, city, zipCode, country;
    int age;
    bool isActive;
};

void processUserData(const UserData& user) {
    if (user.firstName.empty() ||
        user.lastName.empty() ||
        user.email.empty()) {
```

throw invalid_argument(  
"First name, last name, and email are required"

```cpp
        );
    }
}

// Break long chains
auto result = data
    .filter([](int x) { return x > 0; })
    .map([](int x) { return x * 2; })
    .reduce(0, [](int a, int b) { return a + b; });

// Break long conditions
```

if (isValidUser(user) &&  
hasPermission(user) &&

```cpp
    isWithinBusinessHours() &&
    !isMaintenanceMode()) {
    processRequest();
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Lines under 80-100 characters
- ✅ No horizontal scrolling
- ✅ Readable on all screens
- ✅ Works in split view
- ✅ Better for code reviews

**📏 LINE LENGTH GUIDELINES:**

  - 80 characters: Traditional (still common)
  - 100 characters: Modern compromise
  - 120 characters: Maximum for most teams
  - Break at logical points (operators, commas)
  - Indent continued lines

- 💡 KEY PRINCIPLE:

"Keep lines under 100 characters.  
Break long lines at logical points."

### From `example6_organization()`

- ❌ BAD CODE (Poor Organization):

```cpp

```cpp
class UserManager {
    int count;  // Public data (bad!)
```

**public:**

```cpp
void save() { }
```

**private:**

```cpp
string name;
```

**public:**

```cpp
void load() { }
void delete() { }
```

**private:**

```cpp
void helper() { }
```

**public:**

```cpp
    int getValue() { return count; }
};
```

```

**🚨 PROBLEMS:**

  - public/private sections mixed randomly
  - Related methods separated
  - Hard to find methods
  - Confusing structure

- ✅ GOOD CODE (Well-Organized Class):

```cpp

```cpp
class UserManager {
```

**public:**

```cpp
// Constructors
UserManager();
~UserManager();
// Public interface (grouped logically)
void save();
void load();
void remove();
// Getters/Setters
int getCount() const { return count_; }
string getName() const { return name_; }
```

**private:**

```cpp
    // Helper methods
    void validateData();
    void notifyObservers();
    // Member variables (at end)
    int count_;
    string name_;
    vector<Observer*> observers_;
};

// Source file organization
// 1. Constructors/Destructors
UserManager::UserManager() : count_(0) { }
UserManager::~UserManager() { }

// 2. Public methods (in header order)
void UserManager::save() { }
void UserManager::load() { }
void UserManager::remove() { }

// 3. Private methods
void UserManager::validateData() { }
void UserManager::notifyObservers() { }
```

```

**✨ IMPROVEMENTS:**

- ✅ Clear section separation
- ✅ Public interface first
- ✅ Related methods grouped
- ✅ Easy to navigate
- ✅ Logical order

**📋 CLASS ORGANIZATION ORDER:**

1. public section:
    - Constructors/Destructors
    - Public methods (grouped logically)
    - Getters/Setters
2. protected section (if needed)
3. private section:
    - Helper methods
    - Member variables (at end)

- 💡 KEY PRINCIPLE:

"Organize logically: Public first, private last.  
Group related methods together."

### From `example7_file_organization()`

- ❌ BAD CODE (Random Header Order):

```cpp

```cpp
#include "MyClass.h"
#include <vector>
#include "Utility.h"
#include <iostream>
#include <string>
#include "../other/Helper.h"
#include <algorithm>
using namespace std;  // In header (bad!)

// No logical organization
// Hard to spot missing includes
```

```

**🚨 PROBLEMS:**

  - Random include order
  - Mixed system and local headers
  - Hard to find duplicates
  - Using namespace in header (bad!)

- ✅ GOOD CODE (Organized Headers):

```cpp

```cpp
// ============================================
// MyClass.cpp
// Description: User management class
// Author: Your Name
// Date: 2024-11-18
// ============================================

// Related header first
#include "MyClass.h"

// C++ standard library (alphabetical)
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Third-party libraries
#include <boost/filesystem.hpp>

// Project headers (alphabetical)
#include "Helper.h"
#include "Utility.h"

// Namespace usage (only in .cpp, never in .h)
using namespace std;

// Constants
namespace {
    const int MAX_RETRIES = 3;
    const double TIMEOUT = 30.0;
}

// Implementation follows...
```

```

**✨ IMPROVEMENTS:**

- ✅ Clear header comment
- ✅ Logical include order
- ✅ Grouped by category
- ✅ Alphabetical within groups
- ✅ Easy to spot missing includes

**📂 INCLUDE ORDER:**

1. Related header (MyClass.h for MyClass.cpp)
2. C standard library (<cstdio>, <cmath>)
3. C++ standard library (<iostream>, <vector>)
4. Third-party libraries (<boost/...>)
5. Project headers ("MyHeader.h")
  - Alphabetical within each group
  - Blank line between groups
  - System headers: <...>
  - Local headers: "..."

- 💡 KEY PRINCIPLE:

"Organize includes logically and alphabetically.  
Never 'using namespace' in headers!"

### From `example8_comment_style()`

- ❌ BAD CODE (Poor Comments):

```cpp

```cpp
// add two numbers
int add(int a, int b) {  // function
    return a + b;  // return sum
}

//calculate price
double calc ( int x , int y )  //x is quantity
{//y is price per unit
return x*y;//multiply
}
```

/* This is a comment  
that explains  
obvious code */

```cpp
int x = 5;  // Set x to 5
```

```

**🚨 PROBLEMS:**

  - Comments state the obvious
  - Inconsistent spacing
  - No capitalization or punctuation
  - Comments explain WHAT, not WHY

- ✅ GOOD CODE (Good Comments):

```cpp  
/**

- Calculate the total price including bulk discounts.

*

- Bulk discount rules:
- - 10% off for quantities >= 100
- - 20% off for quantities >= 500

*

- @param quantity Number of items
- @param pricePerUnit Price of single item
- @return Total price after discounts

*/

```cpp
double calculateBulkPrice(int quantity, double pricePerUnit) {
    double total = quantity * pricePerUnit;
    // Apply bulk discounts
    if (quantity >= 500) {
        total *= 0.8;  // 20% discount
    } else if (quantity >= 100) {
        total *= 0.9;  // 10% discount
    }
    return total;
}

// FIXME: This algorithm is O(n²), needs optimization
void slowSort(vector<int>& data) {
    // Bubble sort implementation
}

// TODO: Add input validation
void processInput(string input) {
    // Current implementation
}

// HACK: Workaround for bug in external library v1.2
// Remove this when library is updated to v1.3
void workaround() {
    // Temporary fix
}
```

```

📌 SECTION MARKERS — for large file organization  
Use when a file has multiple logical sections  
that benefit from clear visual separation.  
Common in:

  - Files with 200+ lines
  - Multiple class definitions
  - Mixed concerns (types, helpers, main logic)

- ❌ BAD - no visual separation in a large file:

```cpp

```cpp
void connectDatabase() { }
int calculatePrice() { }
void sendEmail() { }
void validateInput() { }
class UserManager { };
class OrderManager { };
```

```

**🚨 PROBLEMS:**

  - No visual separation between unrelated functions
  - Hard to navigate in large files
  - Mixed concerns with no grouping

- ✅ GOOD - clear sections make navigation easy:

```cpp

```cpp
// ============================================
// DATABASE
// ============================================
void connectDatabase() { }
void disconnectDatabase() { }

// ============================================
// BUSINESS LOGIC
// ============================================
int calculatePrice() { }
void validateInput() { }

// ============================================
// COMMUNICATION
// ============================================
void sendEmail() { }
void sendNotification() { }

// ============================================
// MANAGERS
// ============================================
class UserManager { };
class OrderManager { };
```

```

**✨ IMPROVEMENTS:**

- ✅ Clear visual separation between sections
- ✅ Related functions grouped together
- ✅ Easy to navigate in large files
- ✅ Immediately clear what each section contains

- 💡 RULE OF THUMB:

File under ~100 lines  → no section markers needed  
File over  ~200 lines  → section markers help navigation  
File with mixed topics → section markers are essential

- ⚠️  BAD USE (overkill for small files):

```cpp

```cpp
// ============================================
// ADD FUNCTION
// ============================================
int add(int a, int b) { return a + b; }  // no section needed!
```

```

**✨ IMPROVEMENTS:**

- ✅ Doxygen-style documentation
- ✅ Explains WHY, not WHAT
- ✅ Proper capitalization and punctuation
- ✅ Consistent spacing
- ✅ Useful markers (TODO, FIXME, HACK)

**📝 COMMENT GUIDELINES:**

- ✅ Document WHY, not WHAT
- ✅ Use Doxygen for public API
- ✅ Capitalize and punctuate
- ✅ Space after // or /*
- ✅ Use TODO, FIXME, HACK markers
- ❌ Don't comment obvious code
- ❌ Don't leave commented-out code
- ❌ Don't use comments to fix bad names

- 💡 KEY PRINCIPLE:

"Good code documents itself. Comments explain WHY,  
not WHAT. Use Doxygen for public APIs."

### From `example9_consistency()`

**🎯 WHY CONSISTENCY MATTERS:**

  - Reduces cognitive load
  - Easier to spot patterns and errors
  - Faster code reviews
  - Team collaboration
  - Professional appearance

**📋 CONSISTENCY CHECKLIST:**

NAMING:  
□ One naming convention (not mixed)  
□ Variables: snake_case or camelCase  
□ Classes: PascalCase  
□ Constants: UPPER_SNAKE_CASE  
□ Private members: trailing_ or leading m_

FORMATTING:  
□ One brace style (K&R or Allman)  
□ Consistent indentation (2 or 4 spaces)  
□ Space after comma, around operators  
□ Lines under 100-120 characters  
□ Blank lines between functions

ORGANIZATION:  
□ Consistent include order  
□ Public before private  
□ Logical grouping of methods  
□ Related functions together

COMMENTS:  
□ Doxygen for public API  
□ Consistent comment style  
□ Proper capitalization  
□ Explain WHY, not WHAT

**🔧 TOOLS TO ENFORCE CONSISTENCY:**

  - clang-format: Auto-format code

$ clang-format -i file.cpp

  - .clang-format file: Team-wide settings

  - EditorConfig: Editor settings

  - Pre-commit hooks: Enforce before commit

  - CI/CD checks: Verify in pipeline

- 💡 KEY PRINCIPLE:

"Pick a style and stick to it!  
Consistency > Personal preference."

### From `example10_modern_cpp_style()`

**✨ MODERN C++ STYLE:**

1. USE auto APPROPRIATELY:
- ✅ auto x = 5;                    // Clear type
- ✅ auto v = vector<int>{1, 2, 3}; // Obvious
- ✅ auto it = map.begin();         // Iterator
- ❌ auto x = calculatePrice();     // Not clear!
- ✅ double x = calculatePrice();   // Clear

2. USE nullptr (NOT NULL):
- ❌ int* ptr = NULL;   // Old C style
- ✅ int* ptr = nullptr; // Modern C++

3. USE enum class (NOT enum):
- ❌ enum Color { RED, GREEN };  // Pollutes namespace
- ✅ enum class Color { Red, Green };  // Type-safe

4. USE range-based for:
- ❌ for (int i = 0; i < v.size(); i++)  // Old
- ✅ for (const auto& item : v)          // Modern

5. USE uniform initialization:
- ❌ vector<int> v(10);      // May be confusing
- ✅ vector<int> v{10};      // Clear
- ✅ int x{5};               // Modern

6. USE smart pointers:
- ❌ Widget* w = new Widget();  // Manual management
- ✅ auto w = make_unique<Widget>();  // RAII

7. USE [[nodiscard]] for important returns:

```cpp
[[nodiscard]] bool save() { /* ... */ }
```

8. USE constexpr when possible:
- ❌ const double PI = 3.14159;
- ✅ constexpr double PI = 3.14159;

9. USE structured bindings (C++17):
- ❌ auto pair = getCoordinates();

```cpp
int x = pair.first;
int y = pair.second;
```

- ✅ auto [x, y] = getCoordinates();

10. USE trailing return type (when clearer):
- ❌ template<typename T, typename U>

```cpp
decltype(t + u) add(T t, U u);
```

- ✅ template<typename T, typename U>

```cpp
auto add(T t, U u) -> decltype(t + u);
```

- 💡 KEY PRINCIPLE:

"Use modern C++ features for clarity and safety!  
Embrace C++11/14/17/20 improvements."

### From `example11_clang_format()`

**🔧 USING CLANG-FORMAT:**

1. INSTALL:

$ sudo apt install clang-format  # Ubuntu/Debian  
$ brew install clang-format      # macOS

2. FORMAT A FILE:

$ clang-format -i file.cpp  # Format in-place  
$ clang-format file.cpp     # Show formatted output

3. USE PREDEFINED STYLE:

$ clang-format -style=Google -i file.cpp  
$ clang-format -style=LLVM -i file.cpp  
$ clang-format -style=Mozilla -i file.cpp

4. CREATE .clang-format FILE:

```yaml  
# .clang-format  
---  
BasedOnStyle: LLVM  
IndentWidth: 4  
ColumnLimit: 100  
PointerAlignment: Left  
BreakBeforeBraces: Attach  
AllowShortFunctionsOnASingleLine: Empty  
AllowShortIfStatementsOnASingleLine: Never  
SpaceAfterCStyleCast: true  
SpacesInParentheses: false  
```

5. INTEGRATE WITH EDITOR:
  - VS Code: C/C++ extension
  - CLion: Built-in support
  - Vim: vim-clang-format plugin
  - Format on save

6. PRE-COMMIT HOOK:

```bash  
#!/bin/bash  
# .git/hooks/pre-commit  
FILES=$(git diff --cached --name-only --diff-filter=ACM | grep '\.cpp$\|\.h$')

```cpp
for FILE in $FILES; do
```

clang-format -i $FILE  
git add $FILE  
done  
```

**📊 POPULAR STYLE GUIDES:**

  - Google C++ Style:
    - 2-space indentation
    - K&R bracing
    - 80 character limit

  - LLVM Style:
    - 2-space indentation
    - K&R bracing
    - 80 character limit

  - Mozilla Style:
    - 2-space indentation
    - Allman bracing
    - 80 character limit

- 💡 KEY PRINCIPLE:

"Automate formatting with clang-format!  
Spend time on logic, not formatting."

### From `example12_summary()`

- ✅ CODE STYLE CHECKLIST:

**📝 NAMING:**

- ✅ Consistent convention (snake_case or camelCase)
- ✅ Classes: PascalCase
- ✅ Constants: UPPER_SNAKE_CASE
- ✅ Private members: trailing underscore
- ✅ Descriptive names (no x, tmp, data)

**🎨 FORMATTING:**

- ✅ One brace style (K&R recommended)
- ✅ 4 spaces indentation (or 2)
- ✅ Space after comma, around operators
- ✅ Lines under 100 characters
- ✅ Blank lines between functions
- ✅ Blank lines between logical sections

**📂 ORGANIZATION:**

- ✅ Includes: Related → STL → Third-party → Project
- ✅ Alphabetical within groups
- ✅ Classes: Public → Private
- ✅ Member variables at end

**💬 COMMENTS:**

- ✅ Doxygen for public API
- ✅ Explain WHY, not WHAT
- ✅ Proper capitalization
- ✅ TODO/FIXME/HACK markers
- ❌ No commented-out code

**✨ MODERN C++:**

- ✅ Use auto (when clear)
- ✅ Use nullptr (not NULL)
- ✅ Use enum class
- ✅ Use range-based for
- ✅ Use smart pointers
- ✅ Use constexpr

**🔧 AUTOMATION:**

- ✅ Use clang-format
- ✅ .clang-format in repo
- ✅ Format on save
- ✅ Pre-commit hooks
- ✅ CI/CD checks

**🎯 GOLDEN RULES:**

1. Consistency > Personal preference
2. Follow team/project style guide
3. Use automation (clang-format)
4. Make code readable
5. Keep it simple
6. Modern C++ over legacy

**📚 RECOMMENDED STYLE GUIDES:**

  - Google C++ Style Guide
  - LLVM Coding Standards
  - Mozilla Coding Style
  - C++ Core Guidelines

- 💡 FINAL WISDOM:

"Code formatting is like typography.  
Done well, you don't notice it.  
Done poorly, you can't read anything."

**🎓 REMEMBER:**

  - Formatting shows professionalism
  - Consistency helps collaboration
  - Tools automate the boring parts
  - Good style makes code maintainable

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output matches the original `std::cout` version except that the lecture text moved into *Notes moved from main.cpp* is no longer printed (and the remaining example titles were renumbered where an example was removed).
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
