# 01_BasicClass - Introduction to Classes in C++

## 📚 Overview

This folder introduces the fundamental concept of **Classes** in C++ - the building blocks of Object-Oriented Programming (OOP).

A **class** is like a blueprint or template that defines the structure and behavior of objects. An **object** is an instance (actual realization) of that class.

---

## 🎯 What You'll Learn

- What a class is and why we use it
- How to define a basic class
- Creating objects from classes
- Member variables (data/attributes)
- Member functions (methods/behavior)
- Accessing class members with the dot operator
- Creating multiple objects from one class
- Real-world examples of classes

---

## 📖 Concept Explanation

### What is a Class?

Think of a **class** as a **blueprint** for creating objects:
```
Class = Blueprint for a house
Object = Actual house built from that blueprint
```

A class defines:
- **Data** (member variables/attributes)
- **Functions** (member functions/methods)

### Real-World Analogy
```
Class: Student
├── Data: name, age, ID, GPA
└── Functions: study(), takeExam(), displayInfo()

From this blueprint, you can create many students:
- Student alice;
- Student bob;
- Student charlie;
```

Each student has their own name, age, and GPA, but they all share the same functions (behaviors).

---

## 💻 Code Structure

### Basic Syntax
```cpp
class ClassName {
public:
    // Member variables (data)
    int number;
    string name;
    
    // Member functions (behavior)
    void display() {
        cout << name << ": " << number << "\n";
    }
};
```

### Creating and Using Objects
```cpp
// 1. Create an object (instantiation)
ClassName obj;

// 2. Set member variables
obj.number = 42;
obj.name = "Example";

// 3. Call member functions
obj.display();
```

---

## 🔍 Examples in This Folder

### Example 1: Simplest Class
The most basic class possible - just a number and a display function.
```cpp
SimpleClass obj;
obj.number = 42;
obj.display();
```

### Example 2: Student Class
Real-world representation of a student with multiple attributes and behaviors.
```cpp
Student student;
student.name = "Alice";
student.age = 20;
student.displayInfo();
student.study("Mathematics");
```

### Example 3: Multiple Objects (Cars)
Demonstrates creating multiple objects from the same class.
```cpp
Car car1, car2, car3;
car1.brand = "Toyota";
car2.brand = "Tesla";
car3.brand = "BMW";
```

### Example 4: Calculator Class
Shows how functions can operate on class data.
```cpp
Calculator calc;
calc.num1 = 15.5;
calc.num2 = 3.5;
calc.displayOperations();  // Shows all operations
```

### Example 5: Bank Account
Practical example with deposit, withdraw, and balance operations.
```cpp
BankAccount account;
account.deposit(500);
account.withdraw(200);
account.displayBalance();
```

### Example 6: Game Character
Interactive example showing how objects can interact with each other.
```cpp
GameCharacter hero, monster;
hero.attack(monster);
monster.attack(hero);
hero.levelUp();
```

---

## 🎓 Key Concepts

### Class vs Object

| Class | Object |
|-------|--------|
| Blueprint/Template | Instance/Entity |
| Defined once | Can create many |
| Describes structure | Has actual data |
| `class Student { ... };` | `Student alice;` |

### Member Variables

**Also called:** attributes, properties, fields, data members
```cpp
class Student {
public:
    string name;      // Member variable
    int age;          // Member variable
    double gpa;       // Member variable
};
```

Each object has its own copy of these variables.

### Member Functions

**Also called:** methods, behaviors, operations
```cpp
class Student {
public:
    void study() {           // Member function
        cout << "Studying...\n";
    }
    
    void displayInfo() {     // Member function
        cout << name << ", Age: " << age << "\n";
    }
};
```

All objects share the same function code, but operate on their own data.

### The Dot Operator (.)

Used to access members of an object:
```cpp
Student s;
s.name = "Alice";     // Access variable
s.displayInfo();      // Call function
```

---

## 🔑 Important Points

### 1. Don't Forget the Semicolon!
```cpp
class MyClass {
    // ...
};  // ← Semicolon is REQUIRED!
```

### 2. Each Object is Independent
```cpp
Student alice, bob;
alice.name = "Alice";
bob.name = "Bob";
// alice and bob are completely separate
```

### 3. Public vs Private (Preview)

In this lesson, everything is `public` (accessible from outside).
```cpp
class MyClass {
public:     // Accessible from outside
    int x;
    void display() { }
    
private:    // Only accessible inside class
    int y;  // (We'll cover this in 05_Encapsulation)
};
```

---

## 📊 Visual Structure
```
┌─────────────────────────────┐
│         Class               │
│      (Blueprint)            │
│                             │
│  Member Variables:          │
│    - name                   │
│    - age                    │
│    - id                     │
│                             │
│  Member Functions:          │
│    - displayInfo()          │
│    - study()                │
│    - takeExam()             │
└─────────────────────────────┘
           │
           │ Create objects from class
           │
    ┌──────┴──────┬──────────┐
    │             │          │
┌─────────┐  ┌─────────┐  ┌─────────┐
│ Object1 │  │ Object2 │  │ Object3 │
│ (alice) │  │  (bob)  │  │(charlie)│
│         │  │         │  │         │
│ name:   │  │ name:   │  │ name:   │
│ "Alice" │  │  "Bob"  │  │"Charlie"│
│ age: 20 │  │ age: 22 │  │ age: 19 │
└─────────┘  └─────────┘  └─────────┘
```

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 basic_class.cpp -o basic_class
```

### Run:
```bash
./basic_class
```

### Expected Output:
The program will demonstrate:
1. Simple class creation and usage
2. Real-world student example
3. Multiple car objects
4. Calculator operations
5. Bank account transactions
6. Game character battles

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Forgetting Semicolon
```cpp
class MyClass {
    // ...
}  // ❌ Missing semicolon!
```
**Fix:**
```cpp
class MyClass {
    // ...
};  // ✅ Correct
```

### ❌ Mistake 2: Using Wrong Access Operator
```cpp
Student* ptr = new Student;
ptr.name = "Alice";  // ❌ Wrong! Use -> for pointers
```
**Fix:**
```cpp
Student* ptr = new Student;
ptr->name = "Alice";  // ✅ Correct for pointers

Student obj;
obj.name = "Alice";   // ✅ Correct for stack objects
```

### ❌ Mistake 3: Trying to Access Before Setting
```cpp
Student s;
s.displayInfo();  // ⚠️ Variables not initialized!
```
**Fix:**
```cpp
Student s;
s.name = "Alice";
s.age = 20;
s.displayInfo();  // ✅ Now it's safe
```

---

## 🎯 Practice Exercises

### Exercise 1: Create a Book Class
Create a class representing a book with:
- Variables: title, author, pages, price
- Functions: displayInfo(), read(), calculateReadingTime()

### Exercise 2: Create a Rectangle Class
Create a class for a rectangle with:
- Variables: width, height
- Functions: calculateArea(), calculatePerimeter(), display()

### Exercise 3: Create a Phone Class
Create a class for a phone with:
- Variables: brand, model, batteryLevel
- Functions: makeCall(), sendMessage(), charge(), displayInfo()

### Exercise 4: Extend Bank Account
Modify the BankAccount class to:
- Add a transaction history
- Add a function to display all transactions
- Add interest calculation

---

## 📚 Related Topics

### Coming Next:

**02_Constructor**
- How to initialize objects automatically
- Different types of constructors
- Default values

**03_Destructor**
- Automatic cleanup
- Resource management

**05_Encapsulation**
- Making members private
- Getters and setters
- Data hiding

---

## 🔗 Terminology

| Term | Meaning | Example |
|------|---------|---------|
| **Class** | Blueprint/template | `class Student { ... };` |
| **Object** | Instance of a class | `Student alice;` |
| **Member Variable** | Data in the class | `int age;` |
| **Member Function** | Function in the class | `void display();` |
| **Instantiation** | Creating an object | `Student s;` |
| **Dot Operator** | Access class members | `obj.name` |

---

## 🎓 Summary

### What We Learned:

✅ **Class** = Blueprint defining structure and behavior
✅ **Object** = Instance created from a class
✅ **Member Variables** = Data stored in objects
✅ **Member Functions** = Operations on object data
✅ **Dot Operator (.)** = Access members of an object
✅ **Multiple Objects** = Can create many from one class

### Key Syntax:
```cpp
// Define class
class ClassName {
public:
    int data;
    void function() { }
};

// Create object
ClassName obj;

// Use object
obj.data = 42;
obj.function();
```

### Why Use Classes?

1. **Organization** - Group related data and functions
2. **Reusability** - Write once, use many times
3. **Abstraction** - Model real-world entities
4. **Maintainability** - Changes in one place
5. **Clarity** - Clear structure and relationships

---

## 📖 Further Reading

- [cppreference.com - Classes](https://en.cppreference.com/w/cpp/language/class)
- C++ Core Guidelines on classes
- "Effective C++" by Scott Meyers

---

## ✨ Quick Reference Card
```cpp
// CLASS DEFINITION
class MyClass {
public:
    // Member variables
    int number;
    string name;
    
    // Member functions
    void display() {
        cout << name << ": " << number << "\n";
    }
};

// OBJECT CREATION
MyClass obj;                    // Stack
MyClass* ptr = new MyClass;     // Heap

// MEMBER ACCESS
obj.number = 42;                // Dot for stack objects
ptr->number = 42;               // Arrow for pointers

// MEMBER FUNCTION CALL
obj.display();
ptr->display();

// CLEANUP
delete ptr;                     // Only for heap objects
```

---

**Next Topic:** [02_Constructor](../02_Constructor/) - Learn how to initialize objects automatically!

---

*Part of the C++ Classes and Objects series*
*Difficulty: Beginner*
*Prerequisites: Basic C++ syntax, variables, functions*