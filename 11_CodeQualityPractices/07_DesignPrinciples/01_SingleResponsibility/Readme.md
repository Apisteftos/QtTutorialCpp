# Single Responsibility Principle (SRP)

## 📚 Overview

The **Single Responsibility Principle** states:

> **"A class should have one, and only one, reason to change."**  
> — Robert C. Martin (Uncle Bob)

### What It Means

- Each class should have **one responsibility**
- Each class should have **one reason to change**
- A responsibility is a **reason to change**

### Why It Matters

✅ **Easier to understand** - Small, focused classes  
✅ **Easier to test** - Test one thing at a time  
✅ **Easier to maintain** - Changes are isolated  
✅ **Easier to reuse** - No unnecessary baggage  
✅ **Lower coupling** - Classes are independent  

---

## 🚨 Violation Signs

How to spot SRP violations:

- 🚨 Class name contains "And", "Manager", "Handler", "Controller"
- 🚨 Methods that don't relate to each other
- 🚨 Multiple reasons to change the class
- 🚨 Large classes (> 500 lines)
- 🚨 Hard to describe the class in one sentence
- 🚨 Class name is too generic (Data, Manager, Processor)

---

## 💡 The Core Idea

### Gather Together / Separate Apart

```
Gather together:
- Things that change for the SAME reason
- Things that are used together
- Things that share the same data

Separate apart:
- Things that change for DIFFERENT reasons
- Things that are used independently
- Things that serve different actors
```

---

## 📖 Examples

### Example 1: Employee Class (Classic)

#### ❌ **Bad: Multiple Responsibilities**

```cpp
class Employee {
private:
    string name;
    double salary;
    
public:
    // Responsibility 1: Data management
    void setName(string n) { name = n; }
    string getName() const { return name; }
    
    // Responsibility 2: Business logic
    double calculatePay() {
        return salary * 1.1;  // Complex calculation
    }
    
    // Responsibility 3: Database
    void save() {
        // Save to database
    }
    
    // Responsibility 4: Reporting
    string generateReport() {
        return "Report: " + name;
    }
};
```

**Problems:**
- Change in database → modify Employee
- Change in pay calculation → modify Employee
- Change in report format → modify Employee
- 4 different reasons to change!

#### ✅ **Good: Separated Responsibilities**

```cpp
// Responsibility 1: Employee data
class Employee {
private:
    string name;
    double salary;
    
public:
    void setName(string n) { name = n; }
    string getName() const { return name; }
    double getSalary() const { return salary; }
};

// Responsibility 2: Pay calculation
class PayrollCalculator {
public:
    double calculatePay(const Employee& emp) {
        return emp.getSalary() * 1.1;
    }
};

// Responsibility 3: Database operations
class EmployeeRepository {
public:
    void save(const Employee& emp) {
        // Database logic
    }
};

// Responsibility 4: Reporting
class EmployeeReportGenerator {
public:
    string generate(const Employee& emp) {
        return "Report: " + emp.getName();
    }
};
```

**Benefits:**
- ✅ Each class has one clear job
- ✅ Changes are isolated
- ✅ Easy to test each separately
- ✅ Can reuse components

---

### Example 2: User Registration

#### ❌ **Bad: God Class**

```cpp
class UserRegistration {
public:
    bool registerUser(string user, string email, string pass) {
        // Validation
        if (user.empty() || email.empty() || pass.empty()) {
            return false;
        }
        
        // Password hashing
        string hashed = hashPassword(pass);
        
        // Database
        saveToDatabase(user, email, hashed);
        
        // Email
        sendWelcomeEmail(email);
        
        return true;
    }
    
private:
    string hashPassword(string pass) { /* ... */ }
    void saveToDatabase(string u, string e, string p) { /* ... */ }
    void sendWelcomeEmail(string email) { /* ... */ }
};
```

#### ✅ **Good: Separated Concerns**

```cpp
// 1. Validation
class UserValidator {
public:
    bool validate(const string& username, const string& email, const string& password);
};

// 2. Security
class PasswordHasher {
public:
    string hash(const string& password);
};

// 3. Database
class UserRepository {
public:
    void save(const string& username, const string& email, const string& hashedPassword);
};

// 4. Email
class EmailService {
public:
    void sendWelcomeEmail(const string& email);
};

// 5. Coordinator
class UserRegistrationService {
private:
    UserValidator validator;
    PasswordHasher hasher;
    UserRepository repository;
    EmailService emailService;
    
public:
    bool registerUser(const string& username, const string& email, const string& password) {
        if (!validator.validate(username, email, password)) return false;
        
        string hashed = hasher.hash(password);
        repository.save(username, email, hashed);
        emailService.sendWelcomeEmail(email);
        
        return true;
    }
};
```

---

## 🎯 How to Apply SRP

### Step 1: Identify Responsibilities

Ask yourself:
- What does this class do?
- Why would this class change?
- Can I describe it without using "AND"?

### Step 2: List Reasons to Change

For each method, ask:
- Why would this method change?
- Who would request this change?

### Step 3: Extract Responsibilities

Create a new class for each responsibility:
```cpp
// Before: One class, many responsibilities
class OrderProcessor {
    void validateOrder();    // Validation
    void calculateTotal();   // Business logic
    void saveOrder();        // Database
    void sendConfirmation(); // Email
};

// After: Four classes, one responsibility each
class OrderValidator { void validate(Order); };
class PriceCalculator { double calculate(Order); };
class OrderRepository { void save(Order); };
class EmailService { void sendConfirmation(Order); };
```

### Step 4: Create Coordinator (Optional)

```cpp
class OrderProcessor {
private:
    OrderValidator validator;
    PriceCalculator calculator;
    OrderRepository repository;
    EmailService emailService;
    
public:
    void processOrder(Order order) {
        validator.validate(order);
        double total = calculator.calculate(order);
        repository.save(order);
        emailService.sendConfirmation(order);
    }
};
```

---

## ✅ Benefits

### 1. Easier to Understand

```cpp
// Clear, focused purpose
class EmailSender {
    void send(Email email);
};

// vs confused purpose
class UserManager {
    void createUser();
    void sendEmail();
    void logActivity();
    void calculateDiscount();
    // What is this class about??
};
```

### 2. Easier to Test

```cpp
// Test one thing
TEST(EmailSender, SendsEmail) {
    EmailSender sender;
    Email email = createTestEmail();
    sender.send(email);
    // Simple, focused test
}

// vs testing everything
TEST(UserManager, DoesEverything) {
    // Need to mock database, email, logger, calculator...
}
```

### 3. Easier to Maintain

```cpp
// Change email format
// → Only modify EmailFormatter

// vs
// Change email format
// → Modify UserManager (risk breaking user creation, logging, etc.)
```

### 4. Easier to Reuse

```cpp
// Reuse EmailSender anywhere
class OrderProcessor {
    EmailSender emailSender;  // Easy!
};

// vs can't reuse UserManager (brings everything)
```

---

## ⚠️ Common Mistakes

### 1. Over-Engineering

```cpp
// TOO MUCH (ridiculous)
class FirstNameGetter { string getFirstName(); };
class LastNameGetter { string getLastName(); };
class FullNameCombiner { string combine(string, string); };

// Just right
class Person {
    string getFirstName();
    string getLastName();
    string getFullName();
};
```

### 2. Artificial Separation

```cpp
// BAD: Pointless separation
class UserData { string name; };
class UserNameGetter { string getName(UserData); };

// GOOD: Natural cohesion
class User {
    string getName() const { return name; }
private:
    string name;
};
```

### 3. Wrong Boundaries

Think about **who requests changes**:
- CFO wants financial reports → FinancialReporter
- CTO wants technical metrics → TechnicalReporter
- CEO wants executive summary → ExecutiveSummaryGenerator

Don't split by **technical** concerns, split by **business** concerns!

---

## 🔍 Real-World Examples

### Logger (Common Pattern)

```cpp
// ✅ GOOD: Separated
class LogStorage { void add(string message); };
class LogFormatter { string format(string message); };
class LogFileWriter { void write(string filename, vector<string> logs); };
class LogNetworkSender { void send(string url, vector<string> logs); };
```

### File Operations

```cpp
// ✅ GOOD: Separated
class FileReader { string read(string path); };
class FileWriter { void write(string path, string content); };
class FileValidator { bool validate(string path); };
class FileCompressor { string compress(string content); };
```

### Order Processing

```cpp
// ✅ GOOD: Separated
class OrderValidator { bool validate(Order); };
class PriceCalculator { double calculate(Order); };
class InventoryManager { void reserve(Order); };
class PaymentProcessor { bool process(Payment); };
class ShippingService { void ship(Order); };
class NotificationService { void notify(Order); };
```

---

## 📊 Quick Reference

### SRP Checklist

Before writing a class, ask:
- [ ] Can I describe this class in one sentence?
- [ ] Does this class have only one reason to change?
- [ ] Are all methods related to the same responsibility?
- [ ] Would different people request changes to different methods?
- [ ] Is the class name clear and specific (not generic)?

### Red Flags

- ❌ Class name: UserManagerHandler
- ❌ Methods: createUser(), sendEmail(), generateReport()
- ❌ Size: > 500 lines
- ❌ Dependencies: > 5 classes
- ❌ Description: "This class manages users AND sends emails AND logs"

### Green Flags

- ✅ Class name: EmailSender
- ✅ Methods: send(), retry(), formatBody()
- ✅ Size: < 200 lines
- ✅ Dependencies: 1-3 classes
- ✅ Description: "This class sends emails"

---

## 💭 Quotes

> "The Single Responsibility Principle is about people."  
> — Robert C. Martin

> "Gather together the things that change for the same reasons.  
> Separate those things that change for different reasons."  
> — Robert C. Martin

> "A class should have only one reason to change."  
> — Robert C. Martin

---

## 🎯 Key Takeaways

1. **One class = One responsibility**
2. **One class = One reason to change**
3. **Think about WHO requests changes**
4. **Avoid "AND" in descriptions**
5. **Small, focused classes > Large, multi-purpose classes**
6. **SRP enables testability, maintainability, reusability**

---

## 🚀 Next Steps

After mastering SRP, explore:
- **Open/Closed Principle** - Open for extension, closed for modification
- **Liskov Substitution Principle** - Subclasses must be substitutable
- **Interface Segregation Principle** - Many small interfaces > One large
- **Dependency Inversion Principle** - Depend on abstractions

---

## 📚 Resources

- **"Clean Architecture" by Robert C. Martin** - Chapter on SRP
- **"Agile Software Development" by Robert C. Martin** - SOLID principles
- **C++ Core Guidelines** - Class design principles
- **Martin Fowler's Refactoring** - Extracting classes