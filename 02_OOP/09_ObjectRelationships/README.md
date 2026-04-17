# Object Relationships — C++

In OOP, classes and objects don't exist in isolation — they relate to each
other in well-defined ways. Understanding these relationships is essential
for designing clean, maintainable architectures.

There are **6 object relationships** in OOP, split into two groups:

---

## Overview

| # | Relationship | Type | Keyword | Strength |
|---|-------------|------|---------|----------|
| 1 | Association | uses-a | none | weakest |
| 2 | Dependency | uses-a | none | weak |
| 3 | Aggregation | has-a | member pointer/ref | medium |
| 4 | Composition | has-a | member object | strong |
| 5 | Realization | is-a | virtual / interface | strong |
| 6 | Inheritance | is-a | `: public` | strongest |

---

## 1. Association — uses-a (loosest)

Two classes know about each other but neither owns the other.
Both can exist independently and have their own lifetimes.

```cpp
class Teacher {
public:
    void teach(Student& student) {   // knows about Student
        student.learn();
    }
};

class Student {
public:
    void learn() { }
};

// Neither owns the other — both exist independently
Teacher teacher;
Student student;
teacher.teach(student);
```

**Real world:** A teacher teaches students. When the teacher leaves, students
still exist. When a student graduates, the teacher still exists.

---

## 2. Dependency — uses-a (temporary)

One class uses another temporarily — typically as a function parameter,
local variable, or return type. The weakest form of relationship.

```cpp
class ReportGenerator {
public:
    // Depends on Printer temporarily — only during this call
    void generate(Printer& printer) {
        printer.print("Report content");
    }
};

class Printer {
public:
    void print(const std::string& text) { }
};
```

**Difference from Association:** Dependency is temporary (method parameter),
Association is more permanent (stored reference or pointer).

---

## 3. Aggregation — has-a (part exists independently)

A "whole" contains "parts", but the parts can exist without the whole.
The whole does **not** own the lifetime of the parts.

```cpp
class Department {
private:
    std::vector<Employee*> m_employees;   // pointer — does not own

public:
    void addEmployee(Employee* e) {
        m_employees.push_back(e);
    }
};

// Employee exists independently of Department
Employee* emp = new Employee("Kostas");
Department dept;
dept.addEmployee(emp);

// Department destroyed — Employee still exists
// Employee can belong to multiple departments
```

**Real world:** A department has employees. If the department is dissolved,
the employees still exist and can join other departments.

**Key signal:** The "part" is stored as a **pointer or reference** — it was
created outside and passed in.

---

## 4. Composition — has-a (part cannot exist without whole)

The strongest form of has-a. The whole **owns** the parts — it creates them
and destroys them. The part's lifetime is tied to the whole.

```cpp
class Engine {
public:
    void start() { std::cout << "Engine started\n"; }
};

class Car {
private:
    Engine m_engine;   // member object — owned, created with Car

public:
    Car() { }          // Engine created automatically
    ~Car() { }         // Engine destroyed automatically with Car

    void start() { m_engine.start(); }
};

// Engine cannot exist outside a Car in this design
Car car;
car.start();
// When car is destroyed, engine is destroyed too
```

**Real world:** A car has an engine. If the car is destroyed (scrapped),
the engine is destroyed with it. The engine has no meaning without the car.

**Key signal:** The "part" is stored as a **member object** (not pointer) —
created in the constructor, destroyed in the destructor.

---

## 5. Realization — is-a (implements interface)

A class implements an abstract interface or pure virtual class.
The class "realizes" the contract defined by the interface.

```cpp
class Printable {                        // interface (abstract class)
public:
    virtual void print() const = 0;     // pure virtual — must implement
    virtual ~Printable() = default;
};

class Document : public Printable {     // realizes Printable
public:
    void print() const override {
        std::cout << "Printing document\n";
    }
};

class Image : public Printable {        // also realizes Printable
public:
    void print() const override {
        std::cout << "Printing image\n";
    }
};

// Use through the interface
Printable* p = new Document();
p->print();
```

**Real world:** A Document and an Image both implement the Printable
interface — they "realize" the printing contract in their own way.

**Difference from Inheritance:** Realization implements a pure interface
(no data, no implementation). Inheritance extends a concrete class.

---

## 6. Inheritance — is-a (extends base class)

A class inherits the properties and behaviour of another class,
extending or overriding them. The strongest and most well-known relationship.

```cpp
class Animal {
public:
    virtual void makeSound() const = 0;
    void breathe() { std::cout << "Breathing\n"; }
};

class Dog : public Animal {        // Dog IS-A Animal
public:
    void makeSound() const override {
        std::cout << "Woof!\n";
    }
};

class Cat : public Animal {        // Cat IS-A Animal
public:
    void makeSound() const override {
        std::cout << "Meow!\n";
    }
};

Animal* a = new Dog();
a->makeSound();   // Woof! — runtime polymorphism
```

> **Full coverage in `../04_Inheritance/`** which includes:
> - `01_Inheritance` — single inheritance, access specifiers, overriding
> - `02_MultiInheritance` — multiple base classes, diamond problem
> - `03_VirtualInheritance` — solving the diamond problem

---

## Comparison — when to use which

```
Does B use A temporarily (parameter/local)?
  YES → Dependency

Does B know about A but neither owns the other?
  YES → Association

Does B contain A, but A can exist without B?
  YES → Aggregation    (store as pointer/reference)

Does B contain A, and A cannot exist without B?
  YES → Composition    (store as member object)

Does B implement A's pure interface?
  YES → Realization    (abstract class / interface)

Does B extend A's behaviour and data?
  YES → Inheritance    (: public A)
```

---

## Relationship strength — memory ownership

```
Dependency   →  no ownership, temporary use
Association  →  no ownership, longer-term knowledge
Aggregation  →  no ownership, stores pointer/reference
Composition  →  full ownership, stores by value
Realization  →  no ownership, contract fulfillment
Inheritance  →  full ownership of base sub-object
```

---

## UML notation (for reference)

```
Association:   A ————————> B        solid line, open arrow
Dependency:    A - - - - -> B       dashed line, open arrow
Aggregation:   A <>————————B        solid line, hollow diamond at A
Composition:   A <♦>———————B        solid line, filled diamond at A
Realization:   A <— — — — —B        dashed line, hollow triangle at A
Inheritance:   A <—————————B        solid line, hollow triangle at A
```

---

## This folder

```
09_ObjectRelationships/
├── 01_Association
├── 02_Aggregation
├── 03_Composition
├── 04_Dependency
├── 05_Realization
└── README.md               ← this file

Note: Inheritance examples → ../04_Inheritance/
```
