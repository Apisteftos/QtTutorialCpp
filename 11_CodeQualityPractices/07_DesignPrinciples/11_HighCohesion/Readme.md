# High Cohesion Principle

## 📚 Overview

The **High Cohesion Principle** states:

> **"A class should have high cohesion - all its methods and data should be closely related and work toward a single purpose."**  
> — Software Engineering Principle

### What It Means

- **Group related functionality** together in one place
- **Each class/module** has a single, well-defined purpose
- **Methods in a class** work with the same data
- **Strong relationship** between class members
- **Changes to functionality** affect fewer classes

### Why It Matters

✅ **Easier to understand** - Clear, single purpose  
✅ **Easier to maintain** - Changes localized  
✅ **More reusable** - Focused functionality  
✅ **Easier to test** - Single responsibility  
✅ **Better organization** - Logical grouping  

---

## 🚨 Violation Signs

How to spot low cohesion:

- 🚨 Class does many unrelated things
- 🚨 Methods don't use class data
- 🚨 Half the class unused in each operation
- 🚨 Hard to name the class
- 🚨 "UtilityClass", "Helper", or "Manager" in name
- 🚨 Changes affect multiple areas

---

## 📊 Cohesion Levels (Best to Worst)

### 1. 🏆 Functional Cohesion (BEST)

**All parts work toward a single, well-defined purpose**

```cpp
class PasswordHasher {
private:
    string salt;
    
public:
    string hash(const string& password) {
        string salted = addSalt(password);
        return computeHash(salted);
    }
    
private:
    string addSalt(const string& password) {
        return password + salt;
    }
    
    string computeHash(const string& input) {
        // Hash computation
        return "hash_" + input;
    }
};

// ✅ All methods work toward single goal: hashing passwords
// ✅ This is IDEAL cohesion!
```

### 2. Sequential Cohesion

**Output of one part is input to the next**

```cpp
class DataProcessor {
public:
    string process(const string& data) {
        string validated = validate(data);
        string cleaned = clean(validated);
        string formatted = format(cleaned);
        return formatted;
    }
};

// ⚠️ Better than random, but not ideal
```

### 3-6. Other Cohesion Types

- **Communicational**: Operate on same data
- **Procedural**: Sequential execution
- **Temporal**: Executed at same time
- **Logical**: Similar but different operations

### 7. 🚨 Coincidental Cohesion (WORST)

**No meaningful relationship between parts**

```cpp
class Utils {
public:
    int add(int a, int b);           // Math
    string getCurrentTime();          // Time
    bool fileExists(string path);     // File I/O
    void printMessage(string msg);    // Console
};

// 🚨 Completely unrelated operations!
// 🚨 This is BAD cohesion!
```

---

## 📖 Classic Example 1: God Class (Low Cohesion)

### ❌ **Bad: One Class Does Everything**

```cpp
class UserManager {
private:
    string username;
    string email;
    
public:
    // User operations
    void setUserName(const string& name);
    void setEmail(const string& email);
    
    // 🚨 Database operations (unrelated!)
    void connectToDatabase();
    void saveToDatabase();
    
    // 🚨 Email operations (unrelated!)
    void sendWelcomeEmail();
    void sendPasswordResetEmail();
    
    // 🚨 Logging operations (unrelated!)
    void logActivity(const string& activity);
    void logError(const string& error);
    
    // 🚨 Validation operations (unrelated!)
    bool validateEmail(const string& email);
    bool validatePassword(const string& password);
};

// PROBLEMS:
// • Very low cohesion - 6 different responsibilities!
// • User data, database, email, logging, validation all mixed
// • Changes to email affect user management
// • Cannot reuse email logic elsewhere
// • Difficult to test individual pieces
```

### ✅ **Good: Separate Focused Classes (High Cohesion)**

```cpp
// User data only (high cohesion)
class User {
private:
    string username;
    string email;
    
public:
    User(const string& name, const string& email);
    string getUserName() const;
    string getEmail() const;
    void setUserName(const string& name);
    void setEmail(const string& email);
};

// Database operations only (high cohesion)
class UserRepository {
public:
    void save(const User& user);
    User* load(const string& username);
};

// Email operations only (high cohesion)
class EmailService {
public:
    void sendWelcomeEmail(const User& user);
    void sendPasswordResetEmail(const User& user);
};

// Validation only (high cohesion)
class Validator {
public:
    bool isValidEmail(const string& email);
    bool isValidPassword(const string& password);
};

// Logging only (high cohesion)
class Logger {
public:
    void logActivity(const string& activity);
    void logError(const string& error);
};

// SOLUTION:
// • Each class has single, clear purpose
// • High cohesion within each class
// • EmailService reusable anywhere
// • Easy to test each component independently
// • Changes localized to specific class
```

---

## 🔧 Classic Example 2: Utility Class (Low Cohesion)

### ❌ **Bad: Random Utility Grab Bag**

```cpp
class Utils {
public:
    // String operation
    static string toUpperCase(const string& str);
    
    // Math operation
    static int factorial(int n);
    
    // Date operation
    static string getCurrentDate();
    
    // File operation
    static bool fileExists(const string& filename);
    
    // Network operation
    static bool isValidIP(const string& ip);
};

// 🚨 PROBLEM: No cohesion!
// - String, math, date, file, network all mixed
// - Completely unrelated operations
// - "Utils" is a code smell
```

### ✅ **Good: Grouped by Purpose (High Cohesion)**

```cpp
// String operations (high cohesion)
class StringUtils {
public:
    static string toUpperCase(const string& str);
    static string toLowerCase(const string& str);
    static string trim(const string& str);
};

// Math operations (high cohesion)
class MathUtils {
public:
    static int factorial(int n);
    static int fibonacci(int n);
    static bool isPrime(int n);
};

// File operations (high cohesion)
class FileUtils {
public:
    static bool exists(const string& filename);
    static string readFile(const string& filename);
    static void writeFile(const string& filename, const string& content);
};

// Network operations (high cohesion)
class NetworkUtils {
public:
    static bool isValidIP(const string& ip);
    static bool isValidURL(const string& url);
};

// SOLUTION:
// • Each class groups related operations
// • Clear organization by purpose
// • Can use StringUtils without pulling in network code
// • High cohesion within each class
```

---

## 💾 Classic Example 3: Data and Operations Together

### ❌ **Bad: Data Separated from Operations**

```cpp
// Just data
struct OrderData {
    int orderId;
    double amount;
    string status;
};

// Operations separated (low cohesion)
class OrderOperations {
public:
    double calculateTotal(const OrderData& order, double taxRate) {
        return order.amount * (1 + taxRate);
    }
    
    void updateStatus(OrderData& order, const string& status) {
        order.status = status;
    }
    
    bool isValid(const OrderData& order) {
        return order.amount > 0;
    }
};

// 🚨 PROBLEM: Data and operations separated!
// - Low cohesion between data and behavior
// - Not object-oriented
```

### ✅ **Good: Data and Operations Together**

```cpp
class Order {
private:
    int orderId;
    double amount;
    string status;
    
public:
    Order(int id, double amt) 
        : orderId(id), amount(amt), status("pending") {}
    
    // Operations work with the data in this class
    double calculateTotal(double taxRate) const {
        return amount * (1 + taxRate);
    }
    
    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
    
    bool isValid() const {
        return amount > 0;
    }
    
    // Getters
    int getId() const { return orderId; }
    double getAmount() const { return amount; }
    string getStatus() const { return status; }
};

// ✅ SOLUTION:
// - Data and operations naturally together
// - Methods use the data they need
// - High cohesion within Order class
// - Object-oriented design
```

---

## 📏 Measuring Cohesion

### LCOM (Lack of Cohesion of Methods)

**Simple formula:**
```
For each field:
    Cohesion = (Methods using field) / (Total methods)

Average across all fields:
• 80-100% = High cohesion ✅
• 50-79%  = Medium cohesion ⚠️
• < 50%   = Low cohesion 🚨
```

### Example: Low Cohesion

```cpp
class LowCohesion {
private:
    string name;      // Used by 1 method
    int age;          // Used by 1 method
    double salary;    // Used by 1 method
    string address;   // Used by 1 method
    
public:
    void printName() { /* uses only name */ }
    void printAge() { /* uses only age */ }
    void calculateTax() { /* uses only salary */ }
    void printAddress() { /* uses only address */ }
};

// Each method uses 25% of data (low cohesion)
// Average: 25% → LOW COHESION 🚨
```

### Example: High Cohesion

```cpp
class HighCohesion {
private:
    double balance;       // Used by 4 methods
    double interestRate;  // Used by 3 methods
    
public:
    double calculateInterest() { 
        /* uses balance, interestRate */ 
    }
    
    void addInterest() { 
        /* uses balance, interestRate */ 
    }
    
    double getBalance() { 
        /* uses balance */ 
    }
    
    void deposit(double amount) { 
        /* uses balance */ 
    }
};

// Methods use 50-100% of data (high cohesion)
// Average: 75% → HIGH COHESION ✅
```

---

## 🔧 How to Improve Cohesion

### Step-by-Step Process

**1. Identify unrelated data/methods**

```cpp
// ❌ Employee class with unrelated report generation
class Employee {
    string name;           // Employee data
    double salary;
    string reportFormat;   // 🚨 Unrelated!
    string reportHeader;   // 🚨 Unrelated!
    
    void generateReport(); // 🚨 Unrelated!
};
```

**2. Group related items together**

```
Employee data:    name, salary
Report data:      reportFormat, reportHeader
```

**3. Create new class for unrelated group**

```cpp
class EmployeeReportGenerator {
    string format;
    string header;
    void generateReport(const Employee& emp);
};
```

**4. Move unrelated items**

```cpp
// ✅ After: High cohesion
class Employee {
    string name;       // Related
    double salary;     // Related
    // Employee operations only
};

class EmployeeReportGenerator {
    string format;     // Related
    string header;     // Related
    // Report operations only
};
```

**5. Verify single purpose**

✅ Employee: Manages employee data  
✅ EmployeeReportGenerator: Generates reports

---

## ✅ Strategies for High Cohesion

### 1. **Single Responsibility**

```cpp
// Each class does ONE thing
class PasswordHasher { /* only hashes passwords */ };
class EmailSender { /* only sends emails */ };
class DataValidator { /* only validates data */ };
```

### 2. **Group Related Data and Operations**

```cpp
// ✅ DO THIS:
class BankAccount {
    double balance;              // Data
    void deposit(double amount); // Operation on data
    void withdraw(double amount);// Operation on data
};

// ❌ DON'T DO THIS:
struct AccountData { double balance; };
class AccountOperations { 
    void deposit(AccountData&, double); 
};
```

### 3. **Avoid Generic Names**

```cpp
// 🚨 BAD (low cohesion indicators):
class Utils { };
class Helper { };
class Manager { };
class Handler { };

// ✅ GOOD (specific purpose):
class StringUtils { };      // Only string operations
class EmailService { };     // Only email operations
class UserRepository { };   // Only user persistence
```

### 4. **Check Method-Data Usage**

```cpp
// Ask: Do methods use most of the class data?

// ❌ Low cohesion:
class Example {
    int a, b, c, d;  // 4 fields
    void method1() { /* uses only a */ }
    void method2() { /* uses only b */ }
    void method3() { /* uses only c */ }
    void method4() { /* uses only d */ }
};

// ✅ High cohesion:
class Example {
    int a, b;  // 2 fields
    void method1() { /* uses a and b */ }
    void method2() { /* uses a and b */ }
    void method3() { /* uses a and b */ }
};
```

### 5. **Extract Unrelated Functionality**

```cpp
// If methods don't use class data → extract them

// ❌ Before:
class Order {
    int orderId;
    double amount;
    
    void save();  // Uses orderId, amount
    void sendConfirmationEmail();  // 🚨 Doesn't use order data!
};

// ✅ After:
class Order {
    int orderId;
    double amount;
    void save();  // Uses order data
};

class OrderNotification {
    void sendConfirmationEmail(const Order& order);
};
```

---

## 🎯 Cohesion Checklist

Before finalizing a class, ask:

- [ ] Does class have single, clear purpose?
- [ ] Can I name it without using "and" or "or"?
- [ ] Do methods use most of the class data?
- [ ] Are all methods related to the class purpose?
- [ ] Would I reuse this class as-is elsewhere?
- [ ] Is it easy to explain what the class does?
- [ ] Does changing one feature affect only this class?
- [ ] Are fields and methods tightly related?

---

## ⚠️ Warning Signs (Low Cohesion)

### Red Flags

- 🚨 Class name contains "Manager", "Handler", "Helper", "Utils"
- 🚨 Methods use different subsets of data
- 🚨 Hard to name the class succinctly
- 🚨 Class does multiple unrelated things
- 🚨 "God class" with 20+ methods
- 🚨 Changes to one feature affect multiple areas
- 🚨 Half the methods don't use class data

### Example Warning Signs

```cpp
// 🚨 Name contains "Manager"
class UserAccountEmailDatabaseManager { };

// 🚨 Hard to name without "and"
class UserAndOrderManager { };

// 🚨 Methods don't use data
class Example {
    int data;
    void method1() { /* doesn't use 'data' */ }
    void method2() { /* doesn't use 'data' */ }
};
```

---

## ✅ Benefits

### 1. Easier to Understand

```
High Cohesion:   "This class manages bank accounts"
Low Cohesion:    "This class... uh... does many things?"

Clear purpose vs unclear purpose
```

### 2. Easier to Maintain

```
High Cohesion:   Change account logic → modify BankAccount
Low Cohesion:    Change account logic → modify 5 classes

Localized changes vs ripple effects
```

### 3. More Reusable

```
High Cohesion:   EmailService used in 10 places
Low Cohesion:    UserManager too specific, can't reuse

Focused functionality vs tangled dependencies
```

### 4. Easier to Test

```
High Cohesion:   Test PasswordHasher in isolation
Low Cohesion:    Test UserManager requires mocking database,
                 email, logging, validation...

Simple tests vs complex setup
```

### 5. Better Organization

```
High Cohesion:   Clear module boundaries
                 Easy to navigate codebase
                 Logical file structure

Low Cohesion:    Unclear boundaries
                 Hard to find functionality
                 Messy structure
```

---

## 💡 Key Insights

### High Cohesion + Low Coupling = Good Design

```
High Cohesion:  Things that belong together ARE together
Low Coupling:   Things that don't belong together AREN'T together

Together:       Well-organized, maintainable codebase
```

### Cohesion vs Single Responsibility

```
Single Responsibility:  One reason to change
High Cohesion:         Everything works toward same goal

They're closely related and often achieved together!
```

### The Test: Can You Name It?

```
✅ Easy to name:
   - BankAccount
   - EmailSender
   - PasswordHasher
   → High cohesion

🚨 Hard to name:
   - UserAccountEmailManager
   - SystemHelper
   - DataUtils
   → Low cohesion
```

---

## 🎯 Quick Reference

### Red Flags (Low Cohesion)

- 🚨 "Utils", "Helper", "Manager" in class name
- 🚨 Methods use < 50% of class data
- 🚨 God class (20+ methods)
- 🚨 Class does unrelated things
- 🚨 Hard to name class
- 🚨 Changes affect multiple areas
- 🚨 Cannot reuse class

### Green Flags (High Cohesion)

- ✅ Clear, specific class name
- ✅ Methods use 80%+ of class data
- ✅ Single, well-defined purpose
- ✅ Easy to explain what class does
- ✅ Can reuse class as-is
- ✅ Changes localized to class
- ✅ All methods related to purpose

---

## 💭 Quotes

> "High cohesion within modules is a key to maintainability."  
> — Code Complete

> "The primary value of software is not what it does, but how easy it is to change."  
> — Kent Beck

> "Good software design is all about managing dependencies."  
> — Robert C. Martin

---

## 🚀 Next Steps

After mastering High Cohesion, explore:
- **Low Coupling** - Reducing dependencies between modules
- **Single Responsibility Principle** - One reason to change
- **Interface Segregation** - Small, focused interfaces
- **Separation of Concerns** - Distinct responsibilities
- **Module Design** - Organizing larger systems

---

## 📚 Resources

- **"Code Complete" by Steve McConnell** - Cohesion principles
- **"Clean Code" by Robert C. Martin** - Class design
- **"Object-Oriented Software Engineering" by Bernd Bruegge** - Cohesion metrics
- **"Software Engineering" by Ian Sommerville** - Module cohesion

---

## 🎓 Key Takeaways

1. **Group related functionality** - Keep things that belong together
2. **Single purpose** - Each class does one thing well
3. **Data and operations together** - Object-oriented approach
4. **Avoid utility classes** - Group by specific purpose
5. **Methods use data** - High usage of class fields
6. **Easy to name** - Clear purpose = clear name
7. **Localized changes** - Modifications in one place
8. **Test independently** - Focused responsibility

---

## 📝 Final Wisdom

> "High cohesion means everything in a class belongs together.  
> If a method doesn't use the class data, it probably doesn't belong.  
> If a class does multiple unrelated things, split it up.  
> Good design is about putting things where they belong."

The High Cohesion principle ensures maintainable, understandable code. When in doubt, ask: **"Do all parts of this class work toward the same goal?"**

---

## 🔍 Real-World Analogy

Think of **Kitchen Organization**:

**Low Cohesion (Bad Kitchen):**
- Spoons in 3 different drawers
- Pots in kitchen, garage, and bedroom
- Cooking utensils scattered everywhere
- Coffee maker in bathroom
- Hard to find anything!

**High Cohesion (Good Kitchen):**
- All utensils in utensil drawer
- All pots and pans in pot drawer
- All baking supplies together
- Coffee station in one place
- Everything where it belongs!

**High Cohesion says: Keep related items together!**

---

## 📊 Cohesion Self-Assessment

Rate your class:

```
Class: __________________

□ Clear, specific name?                      /10
□ Single, well-defined purpose?              /10
□ Methods use 50%+ of data?                  /10
□ All methods related?                       /10
□ Easy to explain purpose?                   /10
□ Can reuse independently?                   /10
□ Changes localized?                         /10
□ No unrelated functionality?                /10
□ Data and operations together?              /10
□ Would split further hurt it?               /10

Total: ___/100

90-100: Excellent cohesion ✅
70-89:  Good cohesion
50-69:  Needs improvement ⚠️
<50:    Low cohesion - refactor! 🚨
```

---

## 🎯 Remember

```
High Cohesion = Strong Relationship

Everything in the class:
✅ Works toward same goal
✅ Uses same data
✅ Belongs together
✅ Changes together

If something doesn't fit:
→ Extract it to another class!
```