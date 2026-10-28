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

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### 🎉 HIGH COHESION PRINCIPLE COMPLETE! 🎉 (closing summary)

Key Takeaway: Keep related functionality together!  
Each class should have a single, well-defined  
purpose with all methods working toward that goal.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `example1_god_class()`

- ❌ BAD CODE (Low Cohesion - God Class):

```cpp

```cpp
class UserManager {
    // User data
    // Database operations 🚨
    // Email operations 🚨
    // Logging operations 🚨
    // Validation operations 🚨
    // Order operations 🚨
};
// One class doing everything!
```

```

**🚨 PROBLEMS:**

  - Very low cohesion - unrelated responsibilities
  - Changes to email affect user management
  - Hard to test individual pieces
  - Cannot reuse email logic elsewhere
  - Difficult to maintain

- ✅ GOOD CODE (High Cohesion - Focused Classes):

```cpp

```cpp
class User { /* user data only */ };
class UserRepository { /* database only */ };
class EmailService { /* email only */ };
class Validator { /* validation only */ };
class Logger { /* logging only */ };
// Each class has single, clear purpose!
```

```

**✨ DEMONSTRATION:**

### From `example1_god_class()`

**✨ BENEFITS:**

  - Each class has clear, single purpose
  - EmailService reusable anywhere
  - Easy to test each component
  - Changes localized to specific class
  - Clear organization

### From `example2_utility_class()`

- ❌ BAD CODE:

```cpp
class Utils {
    toUpperCase()    // String
    factorial()      // Math
    getCurrentDate() // Date
    fileExists()     // File
    isValidIP()      // Network
};
```

🚨 Completely unrelated operations!

- ✅ GOOD CODE:

```cpp
class StringUtils { /* string operations */ };
class MathUtils { /* math operations */ };
class FileUtils { /* file operations */ };
class NetworkUtils { /* network operations */ };
```

- ✅ Each class groups related operations!

**✨ DEMONSTRATION:**

String operations:

### From `example2_utility_class()`

**✨ BENEFITS:**

  - Clear organization by purpose
  - Easy to find related operations
  - Can use StringUtils without pulling in network code
  - Each class has high cohesion

### From `example3_data_operations()`

- ❌ BAD CODE (Low Cohesion):

```cpp
struct OrderData { ... };  // Just data
class OrderOperations {    // Separate operations
```

calculateTotal(OrderData&)  
updateStatus(OrderData&)

```cpp
};
```

🚨 Data and operations separated!

- ✅ GOOD CODE (High Cohesion):

```cpp
class Order {
    int orderId;           // Data
    double amount;
    calculateTotal()       // Operations
    updateStatus()
};
```

- ✅ Data and operations together!

**✨ DEMONSTRATION:**

### From `example3_data_operations()`

**✨ BENEFITS:**

  - Data and operations naturally together
  - Methods use the data they need
  - High cohesion within Order class
  - Object-oriented design

### From `example4_cohesion_levels()`

**📊 COHESION LEVELS (Best to Worst):**

1. 🏆 FUNCTIONAL COHESION (Best):

```cpp
class PasswordHasher {
    hash()      // Single purpose
    addSalt()   // Support hashing
    computeHash() // Support hashing
};
```

- ✅ All methods work toward single goal

2. ⚠️  SEQUENTIAL COHESION:

```cpp
class DataProcessor {
    validate() → clean() → format()
};
```

- ⚠️  Output of one is input of next

7. 🚨 COINCIDENTAL COHESION (Worst):

```cpp
class MiscHelper {
    add()            // Math
    getCurrentTime() // Time
    isEven()         // Math
    printMessage()   // I/O
};
```

🚨 No relationship between methods!

**✨ DEMONSTRATION:**

### From `example4_cohesion_levels()`

- 💡 GOAL:

Always aim for FUNCTIONAL COHESION!

```cpp
Each class = one clear purpose
```

### From `example5_measuring_cohesion()`

**📏 HOW TO MEASURE COHESION:**

- ❌ LOW COHESION:

```cpp
class LowCohesion {
    name, age, salary, address  // 4 fields
    printName()     → uses only 'name'
    printAge()      → uses only 'age'
    calculateTax()  → uses only 'salary'
    printAddress()  → uses only 'address'
};
```

🚨 Each method uses 25% of data (low cohesion)

- ✅ HIGH COHESION:

```cpp
class HighCohesion {
    balance, interestRate  // 2 fields
    calculateInterest() → uses both fields
    addInterest()       → uses both fields
    getBalance()        → uses balance
    deposit()           → uses balance
};
```

- ✅ Methods use 50-100% of data (high cohesion)

**📊 COHESION METRIC:**

LCOM (Lack of Cohesion of Methods):

  - Count methods that don't share fields
  - Lower LCOM = Higher cohesion
  - LCOM = 0 is ideal (all methods share data)

- 💡 RULE OF THUMB:

If methods use < 50% of class data:

- → Consider splitting the class!

### From `example6_improving_cohesion()`

**🔧 HOW TO IMPROVE COHESION:**

- ❌ BEFORE (Low Cohesion):

```cpp
class Employee {
    // Employee data
```

name, salary, department

```cpp
// Unrelated: Report data 🚨
```

reportFormat, reportHeader

```cpp
    generateReport()
};
```

- ✅ AFTER (High Cohesion):

```cpp
class Employee {
    name, salary, department  // Related data
    setSalary(), setDepartment()
};
class EmployeeReportGenerator {
    format, header            // Related data
```

generateReport(Employee&)

```cpp
};
```

**✨ DEMONSTRATION:**

### From `example6_improving_cohesion()`

**📋 IMPROVEMENT STEPS:**

1. Identify unrelated data/methods
2. Group related items together
3. Create new class for unrelated group
4. Move unrelated items to new class
5. Verify each class has single purpose

### From `example7_benefits()`

- ✅ BENEFITS OF HIGH COHESION:

1. EASIER TO UNDERSTAND:
  - Class has clear, single purpose
  - Related functionality in one place
  - Easy to name the class

2. EASIER TO MAINTAIN:
  - Changes localized to one class
  - Less ripple effect
  - Fewer dependencies

3. MORE REUSABLE:
  - Focused functionality
  - Can use class independently
  - No unnecessary baggage

4. EASIER TO TEST:
  - Test single responsibility
  - Fewer test cases
  - Clear test scenarios

5. BETTER ORGANIZATION:
  - Logical grouping
  - Clear structure
  - Easy to navigate codebase

- ⚠️  LOW COHESION PROBLEMS:
  - Hard to understand purpose
  - Changes affect multiple areas
  - Hard to reuse
  - Difficult to test
  - Poor organization
  - God classes

- 💡 KEY INSIGHT:

High cohesion means:  
"Everything in this class belongs together  
and nothing else should be here."

**🎯 RELATIONSHIP TO OTHER PRINCIPLES:**

```cpp
High Cohesion + Low Coupling = Good Design
```

High Cohesion ≈ Single Responsibility Principle  
High Cohesion → Easier to apply SOLID

### From `example8_how_to_achieve()`

- ✅ STRATEGIES FOR HIGH COHESION:

1. SINGLE RESPONSIBILITY:
  - Each class does ONE thing
  - One reason to change
  - Clear, focused purpose

2. GROUP RELATED DATA AND OPERATIONS:
  - Data and methods that work with it together
  - Don't separate data from operations
  - Object-oriented design

3. AVOID UTILITY/HELPER CLASSES:
- ❌ class Utils { /* random methods */ }
- ✅ class StringUtils { /* string methods */ }
- ✅ class MathUtils { /* math methods */ }

4. CHECK METHOD-DATA USAGE:
  - Do methods use most of the class data?
  - If not, consider splitting
  - Aim for 50%+ data usage per method

5. ONE CONCEPT PER CLASS:
  - User management → UserManager
  - Email sending → EmailService
  - Database access → Repository
  - Don't mix concepts

6. EXTRACT UNRELATED FUNCTIONALITY:
  - If methods don't use class data → extract
  - If functionality unrelated → new class
  - Split large classes

**🔍 COHESION CHECKLIST:**

□ Does class have single, clear purpose?  
□ Can you name it without 'and' or 'or'?  
□ Do methods use most of the data?  
□ Are all methods related to the purpose?  
□ Would you reuse this class as-is?  
□ Is it easy to explain what class does?

- ⚠️  WARNING SIGNS (Low Cohesion):

🚨 Class name has 'Manager', 'Handler', 'Helper', 'Utils'  
🚨 Methods use different subsets of data  
🚨 Hard to name the class  
🚨 Class doing multiple unrelated things  
🚨 Changes affect multiple areas

**📏 MEASURING COHESION:**

Simple formula:

```cpp
Cohesion = (Methods using field) / (Total methods)
```

Average across all fields:

  - 80-100% = High cohesion ✅
  - 50-79%  = Medium cohesion ⚠️
  - < 50%   = Low cohesion 🚨

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run and compared with the original `std::cout` output line by line. Differences: the moved lecture text (see *Notes moved from main.cpp*), `bool` values print as `true`/`false` instead of `1`/`0`, floating-point numbers print in their shortest exact form instead of 6 significant digits, and values that change on every run (thread ids, timings, random data). I sampled the differing lines; I did not review every one.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
