# Association — C++

## What is Association?

A **uses-a** relationship where two classes know about each other but
**neither owns the other**. Both objects exist independently and have
their own lifetimes.

---

## Class diagram

```mermaid
classDiagram
    class Teacher {
        -string m_name
        -string m_subject
        +teach(Student& student)
        +gradeStudent(const Student& student)
    }

    class Student {
        -string m_name
        -double m_grade
        +learn(string topic)
        +getName() string
        +getGrade() double
    }

    Teacher --> Student : teaches / grades
```

---

## Bidirectional association

```mermaid
classDiagram
    class Doctor {
        -string m_name
        -vector~Patient*~ m_patients
        +addPatient(Patient* p)
        +treatPatient(Patient& p)
    }

    class Patient {
        -string m_name
        -Doctor* m_doctor
        +setDoctor(Doctor* d)
        +describeCondition()
    }

    Doctor "1" --> "*" Patient : treats
    Patient "*" --> "1" Doctor : assigned to
```

---

## Many-to-many association

```mermaid
classDiagram
    class Driver {
        -string m_name
        -vector~Vehicle*~ m_vehicles
        +assignVehicle(Vehicle* v)
        +listVehicles()
    }

    class Vehicle {
        -string m_plate
        -string m_model
        +getPlate() string
        +getModel() string
    }

    Driver "*" --> "*" Vehicle : can drive
```

---

## Lifetime diagram

```mermaid
sequenceDiagram
    participant Main
    participant Teacher
    participant Alice
    participant Bob

    Main->>Teacher: create Teacher("Schmidt", "C++23")
    Main->>Alice: create Student("Alice", 9.5)
    Main->>Bob: create Student("Bob", 8.0)

    Main->>Teacher: teach(alice)
    Teacher->>Alice: learn("C++23")

    Main->>Teacher: teach(bob)
    Teacher->>Bob: learn("C++23")

    Note over Teacher,Bob: Teacher uses Students temporarily
    Note over Teacher,Bob: Neither owns the other

    Main->>Teacher: destroy Teacher
    Note over Alice,Bob: Alice and Bob still alive!
```

---

## Key characteristics

- No ownership — neither object creates or destroys the other
- Both can exist without the other
- One object is passed in from outside (parameter or pointer)
- Can be unidirectional or bidirectional
- Can be one-to-one, one-to-many, or many-to-many

---

## Code pattern

```cpp
class Teacher {
public:
    // Association — Student passed from outside, Teacher does NOT own it
    void teach(Student& student) {
        student.learn(m_subject);
    }
};

// Both exist independently
Teacher teacher("Mr. Schmidt", "C++23");
Student alice("Alice", 9.5);

teacher.teach(alice);   // Teacher uses Student temporarily
// Both still alive after this call
```

---

## Examples in this file

| # | Classes | Direction |
|---|---------|-----------|
| 1 | Teacher ↔ Student | unidirectional |
| 2 | Doctor ↔ Patient | bidirectional |
| 3 | Driver ↔ Vehicle | many-to-many |
| 4 | McxSession ↔ McxGroup | domain example |

---

## Association vs Dependency

Both are "uses-a" but differ in duration:

```mermaid
flowchart LR
    subgraph Dependency
        A1[ReportGenerator] -. "parameter only\ntemporary" .-> B1[Printer]
    end
    subgraph Association
        A2[McxSession] -- "stored pointer\nlonger lasting" --> B2[McxGroup]
    end
```

---

## When to use Association

✅ Objects need to collaborate but have independent lifetimes
✅ One object needs to call methods on another
✅ Many-to-many relationships (drivers and vehicles)
✅ Bidirectional knowledge (doctor knows patients, patients know doctor)

❌ If one object creates and owns the other → use **Composition**
❌ If one object contains the other but doesn't own it → use **Aggregation**
❌ If the relationship is only temporary (method parameter) → use **Dependency**

---

## Summary

| Property | Value |
|----------|-------|
| Type | uses-a |
| Ownership | None |
| Lifetime | Independent |
| Storage | Pointer or reference passed from outside |
| UML | `A ————————> B` solid line, open arrow |
