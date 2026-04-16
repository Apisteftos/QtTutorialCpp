# DRY Principle (Don't Repeat Yourself)

## 📚 Overview

The **DRY Principle** states:

> **"Every piece of knowledge must have a single, unambiguous, authoritative representation within a system."**  
> — Andy Hunt & Dave Thomas (The Pragmatic Programmer)

### What It Means

- **Don't duplicate code**, logic, or knowledge
- **Each piece of information** should exist in one place
- **Avoid copy-paste** programming
- **Extract common functionality** into reusable components
- **Single source of truth** for each piece of knowledge

### Why It Matters

✅ **Easier maintenance** - Change in one place  
✅ **Fewer bugs** - Fix once, fixed everywhere  
✅ **Better readability** - Clear, self-documenting code  
✅ **Reduced code size** - 30-70% less code  
✅ **Consistent behavior** - Same logic everywhere  

---

## 🚨 Violation Signs

How to spot DRY violations:

- 🚨 Copy-pasted code blocks
- 🚨 Same logic in multiple places
- 🚨 Magic numbers/strings repeated
- 🚨 Similar functions with slight variations
- 🚨 Multiple places to update for one change
- 🚨 "Find and replace" needed for simple changes

---

## 🎭 Types of Duplication

### 1. **Literal Duplication** (Exact Copy-Paste)

```cpp
// ❌ Exact same code in multiple places
void processOrder1() {
    double tax = amount * 0.1;
    double total = amount + tax;
    cout << "Total: " << total << endl;
}

void processOrder2() {
    double tax = amount * 0.1;  // 🚨 Duplicated!
    double total = amount + tax;
    cout << "Total: " << total << endl;
}
```

### 2. **Structural Duplication** (Same Pattern, Different Data)

```cpp
// ❌ Same structure repeated
double tax = amount * 0.1;      // 0.1 repeated
double shipping = amount * 0.05; // 0.05 repeated
double discount = amount * 0.02; // 0.02 repeated
```

### 3. **Semantic Duplication** (Same Meaning, Different Code)

```cpp
// ❌ Same concept expressed differently
if (user.age >= 18) { /* ... */ }
// Later in code:
if (user.isAdult()) { /* ... */ }  // Same concept!
```

### 4. **Algorithmic Duplication** (Same Algorithm, Different Types)

```cpp
// ❌ Same algorithm for different types
int findMaxInt(vector<int> nums) { /* ... */ }
double findMaxDouble(vector<double> nums) { /* ... */ }
string findMaxString(vector<string> strs) { /* ... */ }
```

---

## 📖 Classic Example 1: Magic Numbers and Strings

### ❌ **Bad: Magic Numbers Everywhere**

```cpp
void processOrder(double amount) {
    double tax = amount * 0.1;      // What is 0.1?
    double shipping = amount > 100 ? 0 : 5.99;  // What is 100? 5.99?
    double discount = amount * 0.05;  // What is 0.05?
    
    double total = amount + tax + shipping - discount;
}

void calculateTax(double amount) {
    double tax = amount * 0.1;  // 🚨 Duplicated tax rate!
}

void validateEmail(const string& email) {
    if (email.find("@") == string::npos) {  // 🚨
        cout << "Invalid email" << endl;
    }
}

void sendEmail(const string& email) {
    if (email.find("@") == string::npos) {  // 🚨 Duplicated validation!
        return;
    }
}
```

**Problems:**
- Tax rate (0.1) appears in multiple functions
- If tax changes to 0.12, must find and update all occurrences
- Magic numbers lack clear meaning
- Email validation logic duplicated
- Easy to make mistakes (one place updated, another forgotten)

### ✅ **Good: Named Constants and Functions**

```cpp
// Constants (single source of truth)
const double TAX_RATE = 0.10;
const double SHIPPING_FEE = 5.99;
const double FREE_SHIPPING_THRESHOLD = 100.0;
const double DISCOUNT_RATE = 0.05;

// Extracted validation (reusable)
bool isValidEmail(const string& email) {
    return email.find("@") != string::npos;
}

double calculateTax(double amount) {
    return amount * TAX_RATE;  // ✅ Single source
}

double calculateShipping(double amount) {
    return amount > FREE_SHIPPING_THRESHOLD ? 0 : SHIPPING_FEE;
}

void processOrder(double amount) {
    double tax = calculateTax(amount);
    double shipping = calculateShipping(amount);
    double discount = amount * DISCOUNT_RATE;
    double total = amount + tax + shipping - discount;
}

void validateEmail(const string& email) {
    if (!isValidEmail(email)) {  // ✅ Reusing validation
        cout << "Invalid email" << endl;
    }
}

void sendEmail(const string& email) {
    if (!isValidEmail(email)) {  // ✅ Same validation
        return;
    }
    // Send email...
}
```

**Solution:**
- Tax rate defined once: `const double TAX_RATE = 0.10`
- Change tax? Update one constant
- Named constants self-document meaning
- Email validation in single function
- All code uses same validation logic

---

## 🔄 Classic Example 2: Duplicate Validation Logic

### ❌ **Bad: Validation Repeated Everywhere**

```cpp
class Rectangle {
    double width, height;
    
    Rectangle(double w, double h) : width(w), height(h) {
        // 🚨 Validation logic
        if (width <= 0) {
            cout << "Width must be positive" << endl;
            width = 1;
        }
        if (height <= 0) {
            cout << "Height must be positive" << endl;
            height = 1;
        }
    }
    
    void setWidth(double w) {
        // 🚨 DUPLICATION: Same validation!
        if (w <= 0) {
            cout << "Width must be positive" << endl;
            width = 1;
        } else {
            width = w;
        }
    }
    
    void setHeight(double h) {
        // 🚨 DUPLICATION: Same validation!
        if (h <= 0) {
            cout << "Height must be positive" << endl;
            height = 1;
        } else {
            height = h;
        }
    }
};

class Circle {
    double radius;
    
    Circle(double r) : radius(r) {
        // 🚨 Similar validation logic duplicated!
        if (radius <= 0) {
            cout << "Radius must be positive" << endl;
            radius = 1;
        }
    }
    
    void setRadius(double r) {
        // 🚨 DUPLICATION!
        if (r <= 0) {
            cout << "Radius must be positive" << endl;
            radius = 1;
        } else {
            radius = r;
        }
    }
};
```

**Problem:** Validation logic appears 5+ times!

### ✅ **Good: Extracted Validation Function**

```cpp
// Single source of truth for validation
double validatePositive(double value, const string& name) {
    if (value <= 0) {
        cout << name << " must be positive, using 1" << endl;
        return 1.0;
    }
    return value;
}

class Rectangle {
    double width, height;
    
public:
    Rectangle(double w, double h)
        : width(validatePositive(w, "Width"))
        , height(validatePositive(h, "Height")) {}
    
    void setWidth(double w) {
        width = validatePositive(w, "Width");  // ✅ Reuse
    }
    
    void setHeight(double h) {
        height = validatePositive(h, "Height");  // ✅ Reuse
    }
};

class Circle {
    double radius;
    
public:
    Circle(double r)
        : radius(validatePositive(r, "Radius")) {}
    
    void setRadius(double r) {
        radius = validatePositive(r, "Radius");  // ✅ Reuse
    }
};
```

**Solution:**
- Validation in ONE function
- All classes use same validation
- Change validation logic? One place
- Guaranteed consistency

---

## 📋 Classic Example 3: Copy-Paste Report Generation

### ❌ **Bad: Nearly Identical Report Functions**

```cpp
void generateUserReport(const vector<string>& users) {
    cout << "╔═══════════════════╗" << endl;
    cout << "║   USER REPORT     ║" << endl;
    cout << "╚═══════════════════╝" << endl;
    
    for (const auto& user : users) {
        cout << "• " << user << endl;
    }
    
    cout << "Total: " << users.size() << " users" << endl;
}

void generateProductReport(const vector<string>& products) {
    // 🚨 DUPLICATION: Almost identical!
    cout << "╔═══════════════════╗" << endl;
    cout << "║  PRODUCT REPORT   ║" << endl;
    cout << "╚═══════════════════╝" << endl;
    
    for (const auto& product : products) {
        cout << "• " << product << endl;
    }
    
    cout << "Total: " << products.size() << " products" << endl;
}

void generateOrderReport(const vector<string>& orders) {
    // 🚨 DUPLICATION: Same structure again!
    cout << "╔═══════════════════╗" << endl;
    cout << "║   ORDER REPORT    ║" << endl;
    cout << "╚═══════════════════╝" << endl;
    
    for (const auto& order : orders) {
        cout << "• " << order << endl;
    }
    
    cout << "Total: " << orders.size() << " orders" << endl;
}

// To change report format: Update 3 functions!
```

### ✅ **Good: Generic Report Generator**

```cpp
void generateReport(const string& title,
                   const vector<string>& items,
                   const string& itemType) {
    cout << "╔═══════════════════╗" << endl;
    cout << "║   " << title << "   ║" << endl;
    cout << "╚═══════════════════╝" << endl;
    
    for (const auto& item : items) {
        cout << "• " << item << endl;
    }
    
    cout << "Total: " << items.size() << " " << itemType << endl;
}

void generateUserReport(const vector<string>& users) {
    generateReport("USER REPORT", users, "users");
}

void generateProductReport(const vector<string>& products) {
    generateReport("PRODUCT REPORT", products, "products");
}

void generateOrderReport(const vector<string>& orders) {
    generateReport("ORDER REPORT", orders, "orders");
}

// To change report format: Update 1 function!
```

**Solution:**
- One generic `generateReport()` function
- Specific functions just call generic version
- 70% less code
- Change format in ONE place

---

## 🔢 Classic Example 4: Algorithm Duplication

### ❌ **Bad: Same Algorithm, Different Types**

```cpp
int findMaxInt(const vector<int>& numbers) {
    int max = numbers[0];
    for (int num : numbers) {
        if (num > max) max = num;
    }
    return max;
}

double findMaxDouble(const vector<double>& numbers) {
    // 🚨 DUPLICATION: Same algorithm!
    double max = numbers[0];
    for (double num : numbers) {
        if (num > max) max = num;
    }
    return max;
}

string findMaxString(const vector<string>& strings) {
    // 🚨 DUPLICATION: Same algorithm!
    string max = strings[0];
    for (const string& str : strings) {
        if (str > max) max = str;
    }
    return max;
}
```

### ✅ **Good: Generic Template**

```cpp
template<typename T>
T findMax(const vector<T>& items) {
    if (items.empty()) {
        throw runtime_error("Empty vector");
    }
    
    T max = items[0];
    for (const T& item : items) {
        if (item > max) {
            max = item;
        }
    }
    return max;
}

// Usage:
vector<int> numbers = {5, 2, 9, 1};
vector<double> decimals = {3.14, 2.71, 9.81};
vector<string> words = {"apple", "zebra", "banana"};

int maxNum = findMax(numbers);       // Works!
double maxDec = findMax(decimals);   // Works!
string maxWord = findMax(words);     // Works!
```

**Solution:**
- One template function for all types
- Fix bug once, fixed for all types
- Significantly less code

---

## ⚙️ Classic Example 5: Configuration Duplication

### ❌ **Bad: Connection Strings Everywhere**

```cpp
void connectToDatabase() {
    string connection = "localhost:5432/mydb";  // 🚨
    cout << "Connecting to: " << connection << endl;
}

void backupDatabase() {
    string connection = "localhost:5432/mydb";  // 🚨 Duplicated!
    cout << "Backing up: " << connection << endl;
}

void restoreDatabase() {
    string connection = "localhost:5432/mydb";  // 🚨 Duplicated!
    cout << "Restoring: " << connection << endl;
}

// Change database server? Update 3 places!
```

### ✅ **Good: Configuration Class**

```cpp
class Config {
public:
    static const string DB_HOST;
    static const int DB_PORT;
    static const string DB_NAME;
    
    static string getConnectionString() {
        return DB_HOST + ":" + to_string(DB_PORT) + "/" + DB_NAME;
    }
};

const string Config::DB_HOST = "localhost";
const int Config::DB_PORT = 5432;
const string Config::DB_NAME = "mydb";

void connectToDatabase() {
    cout << "Connecting to: " << Config::getConnectionString() << endl;
}

void backupDatabase() {
    cout << "Backing up: " << Config::getConnectionString() << endl;
}

void restoreDatabase() {
    cout << "Restoring: " << Config::getConnectionString() << endl;
}

// Change database server? Update 1 place!
```

---

## ⚠️ When NOT to Apply DRY (False Abstraction)

### Don't Force Unrelated Concepts Together!

```cpp
// ❌ BAD: Over-abstraction
void processData(int data, const string& type) {
    // Forcing user and product processing together
    // just because they LOOK similar
    if (type == "user") {
        // User logic
    } else if (type == "product") {
        // Product logic
    }
    // Problem: These are different concepts!
}

// ✅ GOOD: Keep separate if different concepts
void processUserData(int userId) {
    // User-specific business logic
    // This will evolve independently
}

void processProductData(int productId) {
    // Product-specific business logic
    // This will evolve independently
}
```

### The Three Strikes Rule

```
1st occurrence: Write it
2nd occurrence: Copy it (wince, but acceptable)
3rd occurrence: Abstract it (apply DRY)
```

**Why?**
- Premature abstraction is as bad as duplication
- Wait to see the real pattern
- Don't abstract on coincidental similarity

### When to Keep Code Separate

**❌ Don't DRY if:**
- Coincidental similarity (looks same NOW, but different purpose)
- Different concepts (will evolve independently)
- Only seen twice (wait for third occurrence)
- Forced abstraction makes code harder to understand

**✅ Do DRY if:**
- Same knowledge (business rule, algorithm)
- Same logic (validation, calculation)
- Same configuration (constants, settings)
- Three or more occurrences

---

## ✅ How to Apply DRY

### 1. **Extract Functions**

```cpp
// ❌ Duplicated code blocks
void functionA() {
    // ... 20 lines of code ...
}

void functionB() {
    // ... same 20 lines of code ...
}

// ✅ Extract to function
void commonLogic() {
    // ... 20 lines of code (once!) ...
}

void functionA() { commonLogic(); }
void functionB() { commonLogic(); }
```

### 2. **Use Named Constants**

```cpp
// ❌ Magic numbers
double tax = amount * 0.1;
double discount = amount * 0.05;

// ✅ Named constants
const double TAX_RATE = 0.10;
const double DISCOUNT_RATE = 0.05;

double tax = amount * TAX_RATE;
double discount = amount * DISCOUNT_RATE;
```

### 3. **Create Configuration Classes**

```cpp
// ❌ Hardcoded values
string url = "https://api.example.com";
int timeout = 30000;

// ✅ Configuration class
class Config {
    static const string API_URL;
    static const int TIMEOUT_MS;
};
```

### 4. **Use Templates/Generics**

```cpp
// ❌ Type-specific functions
int maxInt(vector<int> nums);
double maxDouble(vector<double> nums);

// ✅ Generic template
template<typename T>
T max(vector<T> items);
```

### 5. **Extract Base Classes**

```cpp
// ❌ Duplicate methods in multiple classes
class UserRepository { void save() { /* ... */ } };
class ProductRepository { void save() { /* ... */ } };

// ✅ Base class with common functionality
class Repository {
    virtual void save() { /* ... */ }
};

class UserRepository : public Repository { };
class ProductRepository : public Repository { };
```

### 6. **Extract Validation**

```cpp
// ❌ Validation duplicated
if (email.find("@") == string::npos) { /* error */ }
// ... later ...
if (email.find("@") == string::npos) { /* error */ }

// ✅ Validation function
bool isValidEmail(const string& email) {
    return email.find("@") != string::npos;
}
```

---

## 🎯 DRY Detection Checklist

Before writing code, ask:

- [ ] Is this code/logic already somewhere?
- [ ] Are there magic numbers that should be constants?
- [ ] Is this the 3rd time I'm writing similar code?
- [ ] Would changing this require updates in multiple places?
- [ ] Can this validation/calculation be extracted?
- [ ] Are there repeated strings that should be constants?
- [ ] Is this configuration that should be centralized?
- [ ] Can I use a template/generic instead of type-specific functions?

---

## 📏 DRY Guidelines

### Apply DRY When:

✅ **Same Knowledge**
- Business rules
- Algorithms
- Calculations
- Validation logic

✅ **Same Configuration**
- Constants
- Connection strings
- API endpoints
- Settings

✅ **Same Structure**
- Report generation
- Data processing
- Error handling

### Don't Apply DRY When:

❌ **Different Concepts**
- Coincidentally similar code
- Different business domains
- Independent evolution paths

❌ **Premature Abstraction**
- Only seen once or twice
- Unclear pattern
- Forced abstraction

❌ **Performance Critical**
- Hot path optimization
- Micro-optimizations
- When duplication is faster

---

## ⚖️ Balance: DRY vs Readability

```cpp
// Sometimes, a little duplication is okay for clarity

// ❌ TOO DRY (over-abstracted)
auto result = process(data, {
    .validator = [](auto x) { return x > 0; },
    .transformer = [](auto x) { return x * 2; },
    .formatter = [](auto x) { return to_string(x); }
});

// ✅ GOOD BALANCE (clear and DRY)
bool isPositive(int x) { return x > 0; }
int double(int x) { return x * 2; }
string format(int x) { return to_string(x); }

if (isPositive(data)) {
    int doubled = double(data);
    return format(doubled);
}
```

**Rule:** Optimize for understanding first, DRY second.

---

## ✅ Benefits

### 1. Easier Maintenance

```
Change tax rate:
❌ Without DRY: Find 10 places, update each
✅ With DRY: Change 1 constant
```

### 2. Fewer Bugs

```
Fix validation bug:
❌ Without DRY: Fix in 5 places, might miss one
✅ With DRY: Fix once, fixed everywhere
```

### 3. Better Readability

```cpp
// ❌ Without DRY
double result = amount * 0.1;  // What is 0.1?

// ✅ With DRY
double tax = amount * TAX_RATE;  // Clear!
```

### 4. Reduced Code Size

```
❌ Without DRY: 1000 lines with duplication
✅ With DRY: 400 lines (60% reduction)
```

### 5. Consistent Behavior

```
❌ Without DRY: Tax calculated differently in places
✅ With DRY: Same calculation everywhere
```

---

## 💡 Key Insights

### Single Source of Truth

**Every piece of knowledge should have ONE authoritative location:**

```
Tax Rate: const double TAX_RATE = 0.10;
Validation: bool isValidEmail(string email);
Connection: Config::getConnectionString();
```

### DRY Levels

```
Level 1: Don't duplicate literals (magic numbers/strings)
Level 2: Don't duplicate logic (functions/methods)
Level 3: Don't duplicate structure (templates/base classes)
Level 4: Don't duplicate knowledge (configuration/rules)
```

### The DRY Spectrum

```
WET ←------------------------→ DRY ←------------------------→ AHA
(Write Everything Twice)       (Don't Repeat Yourself)       (Avoid Hasty Abstractions)

Too much duplication ←--- Sweet spot --→ Over-abstraction
```

**Aim for the sweet spot!**

---

## 🎯 Quick Reference

### Red Flags (DRY Violations)

- ❌ Copy-pasted code blocks
- ❌ Magic numbers: `0.1`, `100`, `3.14159`
- ❌ Magic strings: `"error"`, `"@gmail.com"`
- ❌ Same validation in multiple places
- ❌ Similar functions: `getUserById`, `getProductById`, `getOrderById`
- ❌ Need "find and replace" for simple changes

### Green Flags (DRY Compliance)

- ✅ Named constants: `TAX_RATE`, `MAX_SIZE`
- ✅ Extracted functions: `calculateTax()`, `validateEmail()`
- ✅ Configuration classes: `Config::DB_HOST`
- ✅ Templates/generics: `findMax<T>()`
- ✅ Base classes: `Repository`
- ✅ Single source of truth for each concept

---

## 💭 Quotes

> "Every piece of knowledge must have a single, unambiguous, authoritative representation within a system."  
> — The Pragmatic Programmer

> "Don't repeat yourself."  
> — Andy Hunt & Dave Thomas

> "Duplication is the root of all evil in software."  
> — Robert C. Martin

> "Once and only once."  
> — Extreme Programming principle

---

## 🚀 Next Steps

After mastering DRY, explore:
- **KISS Principle** - Keep It Simple, Stupid
- **YAGNI** - You Aren't Gonna Need It
- **Code Refactoring** - Improving existing code
- **Design Patterns** - Reusable solutions
- **Clean Code Practices** - Writing maintainable code

---

## 📚 Resources

- **"The Pragmatic Programmer" by Hunt & Thomas** - Original DRY
- **"Clean Code" by Robert C. Martin** - Code quality
- **"Refactoring" by Martin Fowler** - Improving code structure
- **"Code Complete" by Steve McConnell** - Best practices

---

## 🎓 Key Takeaways

1. **Don't repeat yourself** - Extract common patterns
2. **Single source of truth** - One place for each piece of knowledge
3. **Use constants** - No magic numbers or strings
4. **Extract functions** - Reusable logic
5. **Templates/generics** - One algorithm, all types
6. **Configuration** - Centralize settings
7. **Three strikes rule** - Wait for third occurrence
8. **Balance** - Don't over-abstract

---

## 📝 Final Wisdom

> "The DRY principle is about reducing repetition of knowledge, not code.  
> Two pieces of code can look identical but represent different knowledge.  
> Conversely, two pieces of different-looking code might represent the same knowledge."

The DRY principle ensures maintainable, consistent code. When in doubt, ask: **"If this changes, how many places need updating? Should it be just one?"**

---

## 🔍 Real-World Analogy

Think of a **Restaurant Recipe Book**:

**Without DRY (WET):**
- Each dish has its own "how to make tomato sauce" instructions
- Change tomato sauce recipe? Update 50 dishes!
- Easy to miss updates
- Inconsistent sauce across dishes

**With DRY:**
- One "Master Tomato Sauce" recipe
- All dishes reference: "Use Master Tomato Sauce"
- Change recipe once, all dishes updated
- Guaranteed consistency

**DRY says: Write the recipe once, reference it everywhere!**

---

## 📊 DRY Metrics

Track these in your codebase:

```
✅ Good:
- Code duplication: < 5%
- Magic numbers: 0
- Magic strings: 0
- Copy-paste commits: 0
- Configuration centralized: 100%

❌ Needs Work:
- Code duplication: > 10%
- Magic numbers: > 0
- Multiple validation implementations
- Scattered configuration
```

---

## 🎯 Remember

```
DRY = Single Source of Truth
WET = Write Everything Twice
AHA = Avoid Hasty Abstractions

Start WET, refactor to DRY when pattern emerges,
but avoid AHA (premature abstraction)!
```