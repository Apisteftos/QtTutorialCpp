# Liskov Substitution Principle (LSP)

## 📚 Overview

The **Liskov Substitution Principle** states:

> **"Objects of a superclass should be replaceable with objects of a subclass without breaking the application."**  
> — Barbara Liskov

### What It Means

- **Subclass must be substitutable** for the base class
- **Child must honor parent's contract** - no surprises
- **Polymorphism must work correctly** - no special cases
- **Client code shouldn't need to know** about specific subclasses

### Why It Matters

✅ **Reliable polymorphism** - Code works with entire hierarchy  
✅ **No runtime surprises** - Predictable behavior  
✅ **Type safety** - Compiler helps catch errors  
✅ **Code reusability** - Generic algorithms work  
✅ **Maintainability** - Add subclasses without breaking clients  

---

## 🚨 Violation Signs

How to spot LSP violations:

- 🚨 `instanceof` or `dynamic_cast` type checks
- 🚨 Overridden method throws new exceptions
- 🚨 Subclass changes expected behavior
- 🚨 Empty or throwing overridden methods
- 🚨 Strengthening preconditions
- 🚨 Weakening postconditions

---

## 📐 Contract Rules

A subclass must follow these rules:

### 1. **Preconditions Cannot Be Strengthened**

```cpp
// ❌ BAD
class FileReader {
    // Requires: filename not empty
    virtual string read(const string& filename);
};

class TextFileReader : public FileReader {
    // 🚨 VIOLATION: Now also requires .txt extension
    string read(const string& filename) override;
};
```

### 2. **Postconditions Cannot Be Weakened**

```cpp
// ❌ BAD
class Calculator {
    // Ensures: returns positive value
    virtual int calculate();
};

class BrokenCalculator : public Calculator {
    // 🚨 VIOLATION: Can return negative
    int calculate() override { return -1; }
};
```

### 3. **Invariants Must Be Preserved**

```cpp
// ❌ BAD
class Rectangle {
    // Invariant: width and height are independent
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
};

class Square : public Rectangle {
    // 🚨 VIOLATION: Changes both dimensions
    void setWidth(int w) override {
        width = w;
        height = w;  // Breaks invariant!
    }
};
```

### 4. **No New Exceptions** (unless subclass of expected)

```cpp
// ❌ BAD
class Bird {
    virtual void fly() { /* flies */ }
};

class Penguin : public Bird {
    // 🚨 VIOLATION: Throws unexpected exception
    void fly() override {
        throw runtime_error("Can't fly!");
    }
};
```

---

## 📖 Classic Example 1: Square-Rectangle

### ❌ **Bad: Square IS-A Rectangle (Violates LSP)**

```cpp
class Rectangle {
protected:
    int width, height;
public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getArea() const { return width * height; }
};

class Square : public Rectangle {
public:
    void setWidth(int w) override {
        width = w;
        height = w;  // Changes both!
    }
    
    void setHeight(int h) override {
        width = h;
        height = h;  // Changes both!
    }
};

void test(Rectangle& rect) {
    rect.setWidth(5);
    rect.setHeight(4);
    assert(rect.getArea() == 20);  // ✅ Works for Rectangle
                                    // ❌ Fails for Square (16)!
}
```

**Problem:**
- Client expects width and height to be independent
- Square violates this expectation
- Not truly substitutable

### ✅ **Good: Separate Hierarchies**

```cpp
class Shape {
public:
    virtual int getArea() const = 0;
};

class Rectangle : public Shape {
private:
    int width, height;
public:
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    int getArea() const override { return width * height; }
};

class Square : public Shape {
private:
    int side;
public:
    void setSide(int s) { side = s; }
    int getArea() const override { return side * side; }
};
```

**Solution:**
- Square and Rectangle are siblings, not parent-child
- Both implement Shape
- Each has its own appropriate interface

---

## 🐧 Classic Example 2: Bird-Penguin

### ❌ **Bad: Penguin IS-A Bird (Violates LSP)**

```cpp
class Bird {
public:
    virtual void fly() {
        cout << "Flying!" << endl;
    }
};

class Sparrow : public Bird {
    // Works fine
};

class Penguin : public Bird {
public:
    void fly() override {
        throw runtime_error("Penguins can't fly!");
    }
};

void makeBirdFly(Bird& bird) {
    bird.fly();  // ✅ Works for Sparrow
                 // ❌ Crashes for Penguin!
}
```

**Problem:**
- Client expects all birds to fly
- Penguin throws exception
- Not substitutable

### ✅ **Good: Separate Flying and Flightless**

```cpp
class Bird {
public:
    virtual void eat() = 0;
};

class FlyingBird : public Bird {
public:
    virtual void fly() = 0;
};

class FlightlessBird : public Bird {
public:
    virtual void walk() = 0;
};

class Sparrow : public FlyingBird {
    void fly() override { cout << "Flying!" << endl; }
    void eat() override { cout << "Eating seeds" << endl; }
};

class Penguin : public FlightlessBird {
    void walk() override { cout << "Waddling!" << endl; }
    void eat() override { cout << "Eating fish" << endl; }
    void swim() { cout << "Swimming!" << endl; }
};
```

**Solution:**
- Separate hierarchies for flying and flightless birds
- Each class can do what it promises
- No unexpected exceptions

---

## 🏦 Example 3: Account Withdrawal

### ❌ **Bad: Empty Override**

```cpp
class Account {
public:
    virtual void withdraw(double amount) {
        cout << "Withdrawing $" << amount << endl;
    }
    
    virtual void deposit(double amount) {
        cout << "Depositing $" << amount << endl;
    }
};

class FixedDepositAccount : public Account {
public:
    void withdraw(double amount) override {
        // Empty! Does nothing!
        cout << "Cannot withdraw" << endl;
    }
};

void processAccount(Account& account) {
    account.withdraw(100);  // ✅ Works for normal Account
                            // ❌ Does nothing for FixedDeposit!
}
```

### ✅ **Good: Proper Hierarchy**

```cpp
class Account {
public:
    virtual void deposit(double amount) = 0;
};

class WithdrawableAccount : public Account {
public:
    virtual void withdraw(double amount) = 0;
};

class SavingsAccount : public WithdrawableAccount {
    void deposit(double amount) override { /* ... */ }
    void withdraw(double amount) override { /* ... */ }
};

class FixedDepositAccount : public Account {
    void deposit(double amount) override { /* ... */ }
    // No withdraw method - type system prevents it!
};
```

---

## ✅ How to Follow LSP

### 1. **Design Based on Behavior, Not Concepts**

```cpp
// ❌ BAD: Conceptual relationship
class Rectangle { };
class Square : public Rectangle { };  // Mathematically true, but...

// ✅ GOOD: Behavioral relationship
class Shape { };
class Rectangle : public Shape { };
class Square : public Shape { };
```

### 2. **Use "IS-A" Test Properly**

Ask: **"Can I substitute without breaking anything?"**

```cpp
// ❌ Square IS-A Rectangle? (Conceptually yes, behaviorally NO)
// ✅ Sparrow IS-A FlyingBird? (Behaviorally YES)
// ✅ ArrayList IS-A List? (Behaviorally YES)
```

### 3. **Honor Contracts**

```cpp
class Base {
    // Contract: returns positive value
    virtual int getValue() {
        return 10;
    }
};

class Derived : public Base {
    // ✅ GOOD: Honors contract
    int getValue() override {
        return 20;  // Still positive
    }
    
    // ❌ BAD: Violates contract
    // int getValue() override {
    //     return -5;  // Negative!
    // }
};
```

### 4. **Don't Refuse Bequest**

```cpp
// ❌ BAD: Inherited method doesn't work
class List {
    virtual void add(int item);
};

class ImmutableList : public List {
    void add(int item) override {
        throw runtime_error("Immutable!");  // Bad!
    }
};

// ✅ GOOD: Don't inherit what you can't use
class List { };
class MutableList : public List {
    virtual void add(int item);
};
class ImmutableList : public List {
    // No add method
};
```

### 5. **Consider Composition Over Inheritance**

```cpp
// Instead of: Square IS-A Rectangle
// Consider: Square HAS-A dimension

class Square {
private:
    int side;  // Composition
public:
    int getArea() { return side * side; }
};
```

---

## 🎯 LSP Checklist

Before creating a subclass, ask:

- [ ] Can I use subclass everywhere parent is used?
- [ ] Does subclass accept all inputs parent accepts?
- [ ] Does subclass provide all outputs parent promises?
- [ ] Does subclass maintain all invariants?
- [ ] No need for type checks (`instanceof`, `dynamic_cast`)?
- [ ] No empty or throwing overridden methods?
- [ ] Preconditions not strengthened?
- [ ] Postconditions not weakened?

---

## ⚠️ Common Violations

### 1. Type Checking

```cpp
// ❌ BAD: Type checking defeats polymorphism
void process(Shape* shape) {
    if (auto* circle = dynamic_cast<Circle*>(shape)) {
        // Special handling for Circle
    }
    else if (auto* rectangle = dynamic_cast<Rectangle*>(shape)) {
        // Special handling for Rectangle
    }
}

// ✅ GOOD: Use polymorphism
void process(Shape* shape) {
    shape->draw();  // Polymorphic call
}
```

### 2. Refused Bequest

```cpp
// ❌ BAD: Inheriting methods you can't support
class Stack : public Vector {
    // But Stack shouldn't allow random access
    // which Vector provides!
};

// ✅ GOOD: Composition
class Stack {
private:
    Vector data;  // Composition
public:
    void push(int x) { data.push_back(x); }
    int pop() { /* ... */ }
};
```

### 3. Empty Methods

```cpp
// ❌ BAD: Empty override
class Base {
    virtual void doSomething() { /* actual work */ }
};

class Derived : public Base {
    void doSomething() override { }  // Does nothing!
};

// ✅ GOOD: Don't inherit if you can't implement
```

---

## ✅ Benefits

### 1. Reliable Polymorphism

```cpp
// Can use any Shape without checking type
void drawAll(const vector<unique_ptr<Shape>>& shapes) {
    for (const auto& shape : shapes) {
        shape->draw();  // Works for all!
    }
}
```

### 2. Code Reusability

```cpp
// Generic algorithm works with entire hierarchy
template<typename Container>
void processAll(Container& items) {
    for (auto& item : items) {
        item.process();  // Works for all subclasses
    }
}
```

### 3. Maintainability

```cpp
// Add new subclass without breaking clients
class NewShape : public Shape {
    void draw() override { /* ... */ }
};
// Existing code works without changes!
```

---

## 💡 Key Insights

### Behavioral vs Conceptual

**Conceptual relationship:**
- "A square IS-A rectangle" (mathematically)
- "A penguin IS-A bird" (biologically)

**Behavioral relationship:**
- "Can I substitute without breaking code?"
- "Does it honor the contract?"

**Use behavioral for OOP design!**

### Contract Flexibility

```
Parent contract:
    Preconditions:  [Least restrictive]
    Postconditions: [Least guarantees]

Child contract:
    Preconditions:  [Can be less restrictive] ✅
    Postconditions: [Can guarantee more] ✅
```

### IS-A vs HAS-A

```cpp
// Sometimes HAS-A is better than IS-A
class Square {
    int side;  // HAS-A dimension
};

// Instead of:
class Square : public Rectangle { };  // IS-A?
```

---

## 🎯 Quick Reference

### Red Flags (LSP Violations)

- ❌ `dynamic_cast` or `instanceof` checks
- ❌ Empty overridden methods
- ❌ Methods that throw "Not implemented"
- ❌ Special cases for certain subclasses
- ❌ Changing expected behavior
- ❌ Breaking parent's assumptions

### Green Flags (LSP Compliance)

- ✅ Subclass works anywhere parent works
- ✅ No type checking needed
- ✅ Consistent behavior across hierarchy
- ✅ All inherited methods work
- ✅ Clear, consistent contracts
- ✅ Tests for parent apply to children

---

## 💭 Quotes

> "What is wanted here is something like the following substitution property:  
> If for each object o1 of type S there is an object o2 of type T such that  
> for all programs P defined in terms of T, the behavior of P is unchanged  
> when o1 is substituted for o2, then S is a subtype of T."  
> — Barbara Liskov

> "Subclasses should be substitutable for their base classes."  
> — Robert C. Martin

> "A violation of LSP is a latent violation of OCP."  
> — Robert C. Martin

---

## 🚀 Next Steps

After mastering LSP, explore:
- **Interface Segregation Principle** - Many small interfaces
- **Dependency Inversion Principle** - Depend on abstractions
- **Design by Contract** - Formal contracts
- **Composition vs Inheritance** - When to use each

---

## 📚 Resources

- **"Program Development in Java" by Barbara Liskov** - Original LSP
- **"Agile Software Development" by Robert C. Martin** - SOLID principles
- **"Design by Contract" by Bertrand Meyer** - Contract programming
- **C++ Core Guidelines** - Inheritance guidelines

---

## 🎓 Key Takeaways

1. **Substitutability is about behavior** - not concepts
2. **Honor the contract** - don't strengthen preconditions
3. **Design for behavior** - not just relationships
4. **Use composition** when inheritance doesn't fit
5. **Test substitutability** - can you swap without breaking?
6. **Type checks indicate violation** - rethink your hierarchy
7. **Empty methods are red flags** - don't inherit what you can't use

---

## 🔍 Final Wisdom

> "If it looks like a duck, quacks like a duck, but needs batteries -  
> you probably have the wrong abstraction."

The Liskov Substitution Principle ensures that your inheritance hierarchies make sense behaviorally, not just conceptually. When in doubt, ask: **"Can I truly substitute this without any surprises?"**