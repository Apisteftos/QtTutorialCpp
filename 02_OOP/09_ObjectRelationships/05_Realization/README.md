# Realization — C++

## What is Realization?

A class **realizes** (implements) an abstract interface — it fulfills the
contract defined by the interface. Every pure virtual method must be
overridden in the concrete class.

---

## Key characteristics

- The base class is **purely abstract** — no data members, all methods `= 0`
- The concrete class must implement **all** pure virtual methods
- A class can realize multiple interfaces simultaneously
- Client code works through the interface — independent of concrete type
- Makes implementations interchangeable (Strategy, Dependency Injection)

---

## Code pattern

```cpp
// Interface — pure abstract class, defines the CONTRACT
class McxCodec {
public:
    virtual std::string encode(const std::string& audio) const = 0;
    virtual std::string decode(const std::string& data)  const = 0;
    virtual ~McxCodec() = default;
};

// Realization — concrete class fulfills the contract
class AmrWbCodec : public McxCodec {
public:
    std::string encode(const std::string& audio) const override {
        return "[AMR-WB] " + audio;
    }
    std::string decode(const std::string& data) const override {
        return "[decoded] " + data;
    }
};

class OpusCodec : public McxCodec {
public:
    std::string encode(const std::string& audio) const override { ... }
    std::string decode(const std::string& data)  const override { ... }
};

// Client uses the interface — works with ANY codec
void processAudio(const McxCodec& codec, const std::string& audio) {
    auto encoded = codec.encode(audio);   // which codec? doesn't matter!
}

AmrWbCodec amrWb;
OpusCodec  opus;
processAudio(amrWb, "audio");   // works
processAudio(opus,  "audio");   // works — same call
```

---

## Examples in this file

| # | Interface | Realizations | Key point |
|---|-----------|-------------|-----------|
| 1 | `Printable` | Document, Image, Spreadsheet | All print through same interface |
| 2 | `Serializable` | UserProfile, SessionData | Any object can be serialized |
| 3 | `Drawable` + `Clickable` | Button | One class, two interfaces |
| 4 | `McxCodec` | AMR-NB, AMR-WB, OPUS | Codec is swappable |

---

## Realization vs Inheritance

```cpp
// REALIZATION — pure interface, no data, no implementation
class Printable {
public:
    virtual void print() const = 0;   // pure virtual — no body
    virtual ~Printable() = default;
    // no data members
};

// INHERITANCE — concrete base class with data and implementation
class Animal {
protected:
    std::string m_name;              // data member
public:
    void breathe() { ... }           // concrete method — has implementation
    virtual void makeSound() = 0;   // can still have some pure virtuals
};
```

| | Realization | Inheritance |
|--|-------------|------------|
| Base has data | ❌ No | ✅ Yes |
| Base has implementation | ❌ No | ✅ Yes |
| All methods pure virtual | ✅ Yes | Not necessarily |
| Multiple at once | ✅ Easy | Risky (diamond) |
| Purpose | Define contract | Reuse and extend |

---

## Realizing multiple interfaces

```cpp
class Button : public Drawable, public Clickable {
public:
    // Must implement all pure virtuals from BOTH interfaces
    void draw()    const override { ... }   // from Drawable
    void resize(int w, int h) override { ... }   // from Drawable
    void onClick() override { ... }         // from Clickable
    bool contains(int x, int y) const override { ... }  // from Clickable
};

// Button can be used as Drawable OR Clickable
Drawable&  d = button;   // use as Drawable
Clickable& c = button;   // use as Clickable
```

---

## When to use Realization

✅ You want to define a contract that multiple classes must fulfill
✅ You want interchangeable implementations (codecs, serializers, printers)
✅ You want to depend on abstractions not concretions (Dependency Inversion)
✅ You need multiple inheritance without the diamond problem

❌ If you want to share code between classes → use **Inheritance**
❌ If the relationship is not "is-a" → use Association/Aggregation/Composition

---

## Summary

| Property | Value |
|----------|-------|
| Type | is-a (contract fulfillment) |
| Base class | Pure abstract — all `= 0`, no data |
| Derived class | Must implement all pure virtuals |
| Multiple | One class can realize many interfaces |
| Benefit | Interchangeable, testable, loosely coupled |
| UML | `A <— — — — —B` dashed line, hollow triangle |
