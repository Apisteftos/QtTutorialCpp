# Composition — C++

## What is Composition?

The strongest **has-a** relationship. The whole **owns** its parts completely —
it creates them, manages them, and destroys them. Parts **cannot exist**
without the whole and have no independent meaning outside it.

---

## Key characteristics

- Whole creates the parts (in constructor or member initialization)
- Parts are destroyed when the whole is destroyed
- Parts cannot be shared with other wholes
- Part lifetime is completely controlled by the whole
- Usually stored as member objects (by value) or `unique_ptr`

---

## Code pattern

```cpp
class Car {
private:
    Engine  m_engine;    // composition — owned by Car
    Gearbox m_gearbox;   // composition — owned by Car

public:
    Car() : m_engine(190, 2.0), m_gearbox(8, true) {}
    // Engine and Gearbox created WITH the Car

    ~Car() { }
    // Engine and Gearbox destroyed AUTOMATICALLY with the Car
};

{
    Car bmw("BMW 320d", 190, 2.0, 8, true);
    bmw.drive();
}   // bmw destroyed → engine and gearbox destroyed too
```

---

## Examples in this file

| # | Whole | Parts | Key point |
|---|-------|-------|-----------|
| 1 | Car | Engine, Gearbox | Parts have no meaning without the car |
| 2 | Human | Heart, Brain | Organs can't exist without the human |
| 3 | House | Rooms | Rooms destroyed when house demolished |
| 4 | McxClient | SessionConfig, AudioCodec | Config/codec owned by client |

---

## Storage options

```cpp
// Option 1: member by value (most common)
class Car {
    Engine m_engine;     // created in member initializer list
};

// Option 2: unique_ptr (when parts are polymorphic or created dynamically)
class House {
    std::vector<std::unique_ptr<Room>> m_rooms;
    void addRoom(const std::string& name, double area) {
        m_rooms.push_back(std::make_unique<Room>(name, area));
    }
    // unique_ptrs auto-delete all rooms when House is destroyed
};
```

---

## Composition vs Aggregation

```cpp
// COMPOSITION — owns the part (member object or unique_ptr)
class Car {
    Engine m_engine;   // value — destroyed with Car
};

// AGGREGATION — does not own (pointer to externally created object)
class Department {
    std::vector<Employee*> m_employees;   // pointer — NOT destroyed with Dept
    ~Department() { /* do NOT delete employees */ }
};
```

| | Composition | Aggregation |
|--|-------------|------------|
| Storage | `Part` value or `unique_ptr` | `Part*` raw pointer |
| Ownership | Full | None |
| Part lifetime | Tied to whole | Independent |
| Part sharing | Impossible | Possible |
| Destructor | Auto-cleanup | Do NOT delete |

---

## When to use Composition

✅ The part has no meaning without the whole (engine without a car)
✅ The part should always exist as long as the whole exists
✅ You want RAII — automatic cleanup when the whole goes out of scope
✅ The part should not be shared with other objects

❌ If the part can exist independently → use **Aggregation**
❌ If the part is optional/nullable → use pointer with **Aggregation**
❌ If neither contains the other → use **Association**

---

## Summary

| Property | Value |
|----------|-------|
| Type | has-a (strong) |
| Ownership | Full — whole owns and manages parts |
| Lifetime | Parts tied to whole |
| Storage | Member object by value, or `unique_ptr` |
| Part sharing | Not possible |
| UML | `A <♦>———————B` filled diamond at A |
