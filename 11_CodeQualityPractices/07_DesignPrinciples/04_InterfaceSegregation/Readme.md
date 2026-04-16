# Interface Segregation Principle (ISP)

## 📚 Overview

The **Interface Segregation Principle** states:

> **"No client should be forced to depend on methods it does not use."**  
> — Robert C. Martin

### What It Means

- **Many small interfaces** are better than one large interface
- **Clients shouldn't depend** on methods they don't use
- **Split fat interfaces** into smaller, focused ones
- **Classes implement only** what they need

### Why It Matters

✅ **No forced implementations** - No empty or throwing methods  
✅ **Better flexibility** - Classes use only what they need  
✅ **Reduced coupling** - Changes affect fewer clients  
✅ **Cleaner code** - Clear intent from interface names  
✅ **Compile-time safety** - Type system prevents misuse  

---

## 🚨 Violation Signs

How to spot ISP violations:

- 🚨 Empty method implementations
- 🚨 Methods throwing `NotImplementedException`
- 🚨 Methods returning `null` or default values
- 🚨 Comments like "not supported" or "not applicable"
- 🚨 Classes implementing large interfaces but using few methods
- 🚨 Interfaces with 10+ methods

---

## 📖 Classic Example 1: Multifunction Printer

### ❌ **Bad: Fat Interface Forces All Implementations**

```cpp
// Fat interface - one size fits all (poorly!)
class IMachine {
public:
    virtual void print(const string& doc) = 0;
    virtual void scan(const string& doc) = 0;
    virtual void fax(const string& doc) = 0;
    virtual void staple() = 0;
};

// ✅ This works fine
class MultiFunctionPrinter : public IMachine {
    void print(const string& doc) override { /* works */ }
    void scan(const string& doc) override { /* works */ }
    void fax(const string& doc) override { /* works */ }
    void staple() override { /* works */ }
};

// 🚨 VIOLATION: Simple printer forced to implement everything!
class SimplePrinter : public IMachine {
    void print(const string& doc) override {
        cout << "Printing..." << endl;
    }
    
    // Forced to implement these!
    void scan(const string& doc) override {
        throw runtime_error("Can't scan!");  // 🚨
    }
    
    void fax(const string& doc) override {
        throw runtime_error("Can't fax!");  // 🚨
    }
    
    void staple() override {
        throw runtime_error("Can't staple!");  // 🚨
    }
};
```

**Problems:**
- SimplePrinter depends on methods it doesn't use
- Runtime exceptions instead of compile-time safety
- Violates ISP - forced unnecessary dependencies
- Client code might call unsupported methods

### ✅ **Good: Segregated Interfaces**

```cpp
// Separate interfaces for separate capabilities
class IPrinter {
public:
    virtual void print(const string& doc) = 0;
};

class IScanner {
public:
    virtual void scan(const string& doc) = 0;
};

class IFax {
public:
    virtual void fax(const string& doc) = 0;
};

class IStapler {
public:
    virtual void staple() = 0;
};

// Simple printer only implements what it needs
class SimplePrinter : public IPrinter {
public:
    void print(const string& doc) override {
        cout << "Printing..." << endl;
    }
    // No scan, fax, or staple - type system prevents calling them!
};

// Multifunction printer implements multiple interfaces
class MultiFunctionPrinter : public IPrinter, 
                             public IScanner, 
                             public IFax, 
                             public IStapler {
public:
    void print(const string& doc) override { /* ... */ }
    void scan(const string& doc) override { /* ... */ }
    void fax(const string& doc) override { /* ... */ }
    void staple() override { /* ... */ }
};

// Scanner-only device
class Scanner : public IScanner {
public:
    void scan(const string& doc) override {
        cout << "Scanning..." << endl;
    }
};
```

**Solution:**
- Each interface has single, focused responsibility
- SimplePrinter implements only `IPrinter`
- MultiFunctionPrinter implements multiple interfaces
- No forced implementation of unused methods
- Type system enforces correct usage

---

## 🤖 Classic Example 2: Worker Interface (Robot vs Human)

### ❌ **Bad: Fat Interface with Methods Not Applicable to All**

```cpp
class IWorker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;    // Not all workers eat!
    virtual void sleep() = 0;  // Not all workers sleep!
};

class HumanWorker : public IWorker {
    void work() override { cout << "Working..." << endl; }
    void eat() override { cout << "Eating..." << endl; }
    void sleep() override { cout << "Sleeping..." << endl; }
};

// 🚨 VIOLATION: Robot forced to implement eat and sleep!
class RobotWorker : public IWorker {
    void work() override { 
        cout << "Robot working 24/7" << endl; 
    }
    
    void eat() override {
        throw runtime_error("Robots don't eat!");  // 🚨
    }
    
    void sleep() override {
        throw runtime_error("Robots don't sleep!");  // 🚨
    }
};
```

**Problem:**
- Robots don't eat or sleep
- Forced to implement methods that make no sense
- Runtime errors instead of compile-time safety

### ✅ **Good: Segregated Interfaces**

```cpp
class IWorkable {
public:
    virtual void work() = 0;
};

class IFeedable {
public:
    virtual void eat() = 0;
};

class ISleepable {
public:
    virtual void sleep() = 0;
};

// Human implements all three
class HumanWorker : public IWorkable, 
                    public IFeedable, 
                    public ISleepable {
public:
    void work() override { cout << "Working..." << endl; }
    void eat() override { cout << "Eating..." << endl; }
    void sleep() override { cout << "Sleeping..." << endl; }
};

// Robot implements only work
class RobotWorker : public IWorkable {
public:
    void work() override { 
        cout << "Robot working 24/7" << endl; 
    }
    // No eat() or sleep() methods!
};
```

**Solution:**
- Three focused interfaces instead of one fat interface
- RobotWorker implements only `IWorkable`
- HumanWorker implements all three interfaces
- Type system prevents calling non-existent methods

---

## 📄 Classic Example 3: Document Interface (Read/Write)

### ❌ **Bad: All Documents Forced to be Read-Write**

```cpp
class IDocument {
public:
    virtual string read() = 0;
    virtual void write(const string& content) = 0;
    virtual void save() = 0;
    virtual void delete_doc() = 0;
};

// ✅ Works for editable documents
class EditableDocument : public IDocument {
    string read() override { return content; }
    void write(const string& c) override { content = c; }
    void save() override { /* save to disk */ }
    void delete_doc() override { /* delete */ }
};

// 🚨 VIOLATION: Read-only forced to implement write operations
class ReadOnlyDocument : public IDocument {
    string read() override { return content; }
    
    void write(const string& c) override {
        throw runtime_error("Document is read-only!");  // 🚨
    }
    
    void save() override {
        throw runtime_error("Cannot save!");  // 🚨
    }
    
    void delete_doc() override {
        throw runtime_error("Cannot delete!");  // 🚨
    }
};
```

### ✅ **Good: Segregated Read and Write Interfaces**

```cpp
class IReadable {
public:
    virtual string read() = 0;
};

class IWritable {
public:
    virtual void write(const string& content) = 0;
};

class ISaveable {
public:
    virtual void save() = 0;
};

class IDeletable {
public:
    virtual void delete_doc() = 0;
};

// Read-only document
class ReadOnlyDocument : public IReadable {
public:
    string read() override { return content; }
    // Only read - no write, save, or delete!
};

// Editable document
class EditableDocument : public IReadable, 
                         public IWritable, 
                         public ISaveable, 
                         public IDeletable {
public:
    string read() override { return content; }
    void write(const string& c) override { content = c; }
    void save() override { /* save */ }
    void delete_doc() override { /* delete */ }
};
```

**Solution:**
- Separate interfaces for read, write, save, delete
- ReadOnlyDocument implements only `IReadable`
- EditableDocument implements all four interfaces
- Type system enforces read-only nature

---

## 🔺 Classic Example 4: Shape Interface (2D vs 3D)

### ❌ **Bad: All Shapes Forced to Have Volume**

```cpp
class IShape {
public:
    virtual double getArea() = 0;
    virtual double getVolume() = 0;  // Not all shapes have volume!
};

// ✅ Works for 3D shapes
class Sphere : public IShape {
    double getArea() override {
        return 4 * PI * radius * radius;
    }
    
    double getVolume() override {
        return (4.0/3.0) * PI * radius * radius * radius;
    }
};

// 🚨 VIOLATION: 2D shape doesn't have volume
class Circle : public IShape {
    double getArea() override {
        return PI * radius * radius;
    }
    
    double getVolume() override {
        throw runtime_error("2D shape has no volume!");  // 🚨
    }
};
```

### ✅ **Good: Separate 2D and 3D Interfaces**

```cpp
class IShape {
public:
    virtual ~IShape() = default;
};

class IShape2D : public IShape {
public:
    virtual double getArea() = 0;
};

class IShape3D : public IShape {
public:
    virtual double getSurfaceArea() = 0;
    virtual double getVolume() = 0;
};

// 2D shapes
class Circle : public IShape2D {
    double getArea() override {
        return PI * radius * radius;
    }
    // No getVolume() method!
};

class Rectangle : public IShape2D {
    double getArea() override {
        return width * height;
    }
};

// 3D shapes
class Sphere : public IShape3D {
    double getSurfaceArea() override {
        return 4 * PI * radius * radius;
    }
    
    double getVolume() override {
        return (4.0/3.0) * PI * radius * radius * radius;
    }
};
```

**Solution:**
- Separate hierarchies for 2D and 3D shapes
- 2D shapes implement `IShape2D` (area only)
- 3D shapes implement `IShape3D` (surface area and volume)
- Type system enforces dimensionality

---

## ✅ How to Follow ISP

### 1. **Identify Client Needs**

```cpp
// What does each client actually need?

// ❌ BAD: All clients get everything
class IDatabase {
    virtual void read() = 0;
    virtual void write() = 0;
    virtual void backup() = 0;
    virtual void restore() = 0;
    virtual void migrate() = 0;
};

// ✅ GOOD: Clients get only what they need
class IReadable { virtual void read() = 0; };
class IWritable { virtual void write() = 0; };
class IBackupable { virtual void backup() = 0; };
class IRestorable { virtual void restore() = 0; };
class IMigratable { virtual void migrate() = 0; };
```

### 2. **Split Fat Interfaces**

```cpp
// If an interface has many methods, split it!

// ❌ BAD: Fat interface
class IUserService {
    virtual void login() = 0;
    virtual void logout() = 0;
    virtual void updateProfile() = 0;
    virtual void changePassword() = 0;
    virtual void deleteAccount() = 0;
    virtual void exportData() = 0;
    virtual void importData() = 0;
    // ... 10 more methods
};

// ✅ GOOD: Segregated interfaces
class IAuthentication {
    virtual void login() = 0;
    virtual void logout() = 0;
};

class IProfileManagement {
    virtual void updateProfile() = 0;
    virtual void changePassword() = 0;
};

class IAccountManagement {
    virtual void deleteAccount() = 0;
};

class IDataManagement {
    virtual void exportData() = 0;
    virtual void importData() = 0;
};
```

### 3. **Use Multiple Small Interfaces**

```cpp
// Classes can implement multiple interfaces

class UserService : public IAuthentication,
                    public IProfileManagement,
                    public IAccountManagement,
                    public IDataManagement {
    // Implements all methods from all interfaces
};

// Or implement just what's needed
class ReadOnlyUserService : public IAuthentication,
                            public IProfileManagement {
    // Only implements authentication and profile reading
    // No account deletion or data management
};
```

### 4. **Avoid Anti-Patterns**

```cpp
// ❌ DON'T DO THIS:

// Empty implementation
void method() override { }

// Throwing exception
void method() override {
    throw runtime_error("Not supported!");
}

// Returning null/default
string method() override { return ""; }
int method() override { return 0; }

// Comments indicating problems
void method() override {
    // Not implemented
    // Not supported
    // Not applicable
}

// ✅ DO THIS INSTEAD:
// Don't inherit that interface!
```

### 5. **Design from Client Perspective**

```cpp
// Think: What does the client need?

// Client 1: Only needs to read
void displayUser(IReadable& user) {
    cout << user.read();
}

// Client 2: Only needs to write
void updateUser(IWritable& user) {
    user.write("new data");
}

// Client 3: Needs both
void syncUser(IReadable& readable, IWritable& writable) {
    string data = readable.read();
    writable.write(data);
}
```

---

## 🎯 ISP Checklist

Before creating an interface, ask:

- [ ] Does every implementer need all methods?
- [ ] Will some implementations throw "not supported"?
- [ ] Can the interface be split into smaller parts?
- [ ] Is the interface client-specific or generic?
- [ ] Does the interface have more than 6 methods?
- [ ] Do different clients need different subsets of methods?
- [ ] Are there empty implementations?
- [ ] Can methods be grouped by responsibility?

---

## 📏 Interface Size Guidelines

### Ideal Size:
- **1-3 methods**: ✅ Perfect - highly cohesive
- **4-6 methods**: ⚠️ Acceptable, but consider splitting
- **7+ methods**: 🚨 Too large - definitely split

### Examples:

```cpp
// ✅ GOOD: 1-3 methods
class IReadable {
    virtual string read() = 0;
};

class IComparable {
    virtual int compareTo(const IComparable& other) = 0;
};

class ICloneable {
    virtual ICloneable* clone() const = 0;
};

// ⚠️ ACCEPTABLE: 4-6 methods (consider context)
class ICollection {
    virtual void add(int item) = 0;
    virtual void remove(int item) = 0;
    virtual bool contains(int item) const = 0;
    virtual int size() const = 0;
    virtual void clear() = 0;
};

// 🚨 BAD: 10+ methods
class IEverything {
    // Too many methods - split it!
};
```

---

## ⚠️ Common Violations

### 1. God Interface

```cpp
// ❌ BAD: Interface that does everything
class IService {
    virtual void methodA() = 0;
    virtual void methodB() = 0;
    virtual void methodC() = 0;
    virtual void methodD() = 0;
    virtual void methodE() = 0;
    // ... 20 more methods
};

// ✅ GOOD: Focused interfaces
class IServiceA { virtual void methodA() = 0; };
class IServiceB { virtual void methodB() = 0; };
class IServiceC { virtual void methodC() = 0; };
```

### 2. Header Interface (Interface Pollution)

```cpp
// ❌ BAD: Adding methods "just in case"
class IPrinter {
    virtual void print() = 0;
    virtual void printColor() = 0;      // Not all printers
    virtual void printDoubleSided() = 0; // Not all printers
    virtual void printLargePaper() = 0;  // Not all printers
};

// ✅ GOOD: Base interface + optional extensions
class IPrinter {
    virtual void print() = 0;
};

class IColorPrinter : public IPrinter {
    virtual void printColor() = 0;
};

class IDuplexPrinter : public IPrinter {
    virtual void printDoubleSided() = 0;
};
```

### 3. Abstract Class as Interface

```cpp
// ❌ BAD: Abstract class with many methods
class Animal {
public:
    virtual void walk() = 0;
    virtual void fly() = 0;   // Not all animals fly
    virtual void swim() = 0;  // Not all animals swim
};

// ✅ GOOD: Capability-based interfaces
class IWalkable { virtual void walk() = 0; };
class IFlyable { virtual void fly() = 0; };
class ISwimmable { virtual void swim() = 0; };

class Dog : public IWalkable, public ISwimmable { };
class Bird : public IWalkable, public IFlyable { };
class Fish : public ISwimmable { };
```

---

## ✅ Benefits

### 1. Cleaner Code

```cpp
// No empty implementations
// No throwing "not supported"
// Clear intent from interface names
```

### 2. Better Flexibility

```cpp
// Classes implement only what they need
// Easy to create new combinations
// More composable design
```

### 3. Reduced Coupling

```cpp
// Clients depend only on methods they use
// Changes to unused methods don't affect clients
// Less ripple effect
```

### 4. Compile-Time Safety

```cpp
// Type system prevents calling unsupported methods
// No runtime exceptions for "not implemented"
// Errors caught during compilation
```

### 5. Easier Testing

```cpp
// Mock only the methods you need
// Smaller interfaces = simpler mocks
// More focused unit tests
```

---

## 💡 Key Insights

### Fat Interfaces Are Problematic

**Symptoms:**
- Many methods
- Some implementations throw exceptions
- Empty implementations
- Comments like "not supported"

**Solution:**
- Split into multiple small interfaces
- Each interface has single, focused responsibility
- Classes implement only what they need

### Design from Client Perspective

**Ask:**
- What does the client actually need?
- Does every client need all methods?
- Can we group methods by client needs?

**Result:**
- Client-specific interfaces
- Role-based interfaces
- Minimal dependencies

### Composition Over Fat Interfaces

```cpp
// Instead of one fat interface
class IEverything { /* many methods */ };

// Use composition of small interfaces
class Implementation : public IA, public IB, public IC { };
```

---

## 🎯 Quick Reference

### Red Flags (ISP Violations)

- ❌ Empty method implementations
- ❌ Methods throwing `NotImplementedException`
- ❌ Returning `null` or default values
- ❌ Interfaces with 10+ methods
- ❌ Comments "not supported" or "not applicable"
- ❌ Clients depending on unused methods

### Green Flags (ISP Compliance)

- ✅ Small, focused interfaces (1-3 methods)
- ✅ All implementations use all methods
- ✅ Client-specific interfaces
- ✅ Multiple inheritance of small interfaces
- ✅ No forced empty implementations
- ✅ Clear, single-purpose interface names

---

## 🔄 ISP and Other Principles

### ISP + SRP (Single Responsibility)

```cpp
// Interface has single responsibility
class IPrinter {
    virtual void print() = 0;  // Only printing
};
```

### ISP + OCP (Open/Closed)

```cpp
// Easy to extend with new interfaces
class IColorPrinter : public IPrinter {
    virtual void printColor() = 0;
};
```

### ISP + LSP (Liskov Substitution)

```cpp
// Subclass can substitute parent
// No "not supported" violations
```

### ISP + DIP (Dependency Inversion)

```cpp
// Depend on focused abstractions
void process(IPrinter& printer) {
    printer.print();  // Only depends on printing
}
```

---

## 💭 Quotes

> "The interface-segregation principle (ISP) states that no client should be forced to depend on methods it does not use."  
> — Robert C. Martin

> "Make fine grained interfaces that are client specific."  
> — Robert C. Martin

> "Clients should not be forced to depend upon interfaces that they do not use."  
> — Agile Software Development: Principles, Patterns, and Practices

---

## 🚀 Next Steps

After mastering ISP, explore:
- **Dependency Inversion Principle** - Depend on abstractions
- **Single Responsibility Principle** - One reason to change
- **Adapter Pattern** - Adapt fat interfaces
- **Facade Pattern** - Simplify complex interfaces

---

## 📚 Resources

- **"Agile Software Development" by Robert C. Martin** - Original ISP definition
- **"Clean Architecture" by Robert C. Martin** - Interface design
- **"Design Patterns" by Gang of Four** - Interface patterns
- **C++ Core Guidelines** - Interface guidelines

---

## 🎓 Key Takeaways

1. **No forced dependencies** - Clients use only what they need
2. **Many small interfaces** - Better than one large interface
3. **Split fat interfaces** - Group related methods
4. **Design from client** - What does the client actually use?
5. **Avoid empty methods** - Sign of ISP violation
6. **Type system helps** - Compile-time safety
7. **Flexible composition** - Implement multiple small interfaces
8. **Reduced coupling** - Changes affect fewer clients

---

## 📝 Final Wisdom

> "When you see a fat interface, split it into smaller, more focused interfaces.  
> If a class is forced to implement methods it doesn't use,  
> you've violated the Interface Segregation Principle."

The Interface Segregation Principle ensures clients depend only on methods they actually use. When in doubt, ask: **"Does every implementer need this method?"**

---

## 🔍 Real-World Analogy

Think of a **Swiss Army Knife** vs **Specialized Tools**:

**Fat Interface** = Swiss Army Knife
- Has everything
- Most people only use 20% of features
- Heavy and complex
- "Just in case" features

**Segregated Interfaces** = Specialized Tools
- Each tool does one thing well
- Take only what you need
- Simple and focused
- No unused features

**ISP says: Give people specialized tools, not a Swiss Army Knife!**