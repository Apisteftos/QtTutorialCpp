# Realization — C++

## What is Realization?

A class **realizes** (implements) an abstract interface — it fulfills the
contract defined by the interface. Every pure virtual method must be
overridden in the concrete class.

---

## Class diagram — Printable interface

```mermaid
classDiagram
    class Printable {
        <<interface>>
        +print()* void
        +toText()* string
    }

    class Document {
        -string m_title
        -string m_content
        +print() void
        +toText() string
    }

    class Image {
        -string m_filename
        -int m_width
        -int m_height
        +print() void
        +toText() string
    }

    class Spreadsheet {
        -string m_name
        -int m_rows
        -int m_cols
        +print() void
        +toText() string
    }

    Printable <|.. Document : realizes
    Printable <|.. Image : realizes
    Printable <|.. Spreadsheet : realizes
    note for Printable "Pure abstract interface\nAll methods = 0\nNo data members"
```

---

## Compressor interface

```mermaid
classDiagram
    class Compressor {
        <<interface>>
        +getName()* string
        +getLevel()* int
        +compress(data)* string
        +decompress(data)* string
    }

    class ZipCompressor {
        +getName() string
        +getLevel() int
        +compress(data) string
        +decompress(data) string
    }

    class GzipCompressor {
        +getName() string
        +getLevel() int
        +compress(data) string
        +decompress(data) string
    }

    class ZstdCompressor {
        +getName() string
        +getLevel() int
        +compress(data) string
        +decompress(data) string
    }

    Compressor <|.. ZipCompressor : realizes
    Compressor <|.. GzipCompressor : realizes
    Compressor <|.. ZstdCompressor : realizes

    note for Compressor "processData(Compressor&)\nworks with ANY compressor\nno code changes needed"
```

---

## Multiple interfaces — Button

```mermaid
classDiagram
    class Drawable {
        <<interface>>
        +draw()* void
        +resize(w, h)* void
    }

    class Clickable {
        <<interface>>
        +onClick()* void
        +contains(x, y)* bool
    }

    class Button {
        -string m_label
        -int m_x
        -int m_y
        -int m_w
        -int m_h
        +draw() void
        +resize(w, h) void
        +onClick() void
        +contains(x, y) bool
    }

    Drawable <|.. Button : realizes
    Clickable <|.. Button : realizes
    note for Button "One class\ntwo interfaces\nfully implemented"
```

---

## Sequence — polymorphic dispatch through interface

```mermaid
sequenceDiagram
    participant Main
    participant items as vector~Printable*~
    participant Doc as Document
    participant Img as Image
    participant Sheet as Spreadsheet

    Main->>Doc: make_unique<Document>(...)
    Main->>Img: make_unique<Image>(...)
    Main->>Sheet: make_unique<Spreadsheet>(...)

    Main->>items: push all three

    loop for each item in items
        Main->>items: item->print()
        alt item is Document
            items->>Doc: print()
            Doc-->>Main: "[Document] C++23 Guide..."
        else item is Image
            items->>Img: print()
            Img-->>Main: "[Image] diagram.png (1920x1080)"
        else item is Spreadsheet
            items->>Sheet: print()
            Sheet-->>Main: "[Spreadsheet] Results.xlsx..."
        end
    end

    Note over Main: Same call — different behaviour
    Note over Main: Runtime polymorphism via interface
```

---

## Realization vs Inheritance

```mermaid
classDiagram
    class Printable {
        <<interface>>
        +print()* void
        +toText()* string
    }

    class Animal {
        #string m_name
        +breathe() void
        +makeSound()* void
    }

    class Document {
        +print() void
        +toText() string
    }

    class Dog {
        +makeSound() void
    }

    Printable <|.. Document : realization\n(pure interface)
    Animal <|-- Dog : inheritance\n(extends concrete class)

    note for Printable "No data\nNo implementation\nOnly contract"
    note for Animal "Has data m_name\nHas implementation breathe()\nCan have some pure virtuals"
```

---

## Examples in this file

| # | Interface | Realized by | Shows |
|---|-----------|-------------|-------|
| 1 | `Printable` | Document, Image, Spreadsheet | `vector<unique_ptr<Printable>>`, calls via the interface only |
| 2 | `Serializable` | UserProfile, AppSettings | `vector<Serializable*>` of different types; `UserProfile::print()` is *not* part of the interface |
| 3 | `Drawable` + `Clickable` | Button | one class realizing two interfaces, viewed through `Drawable&` and `Clickable&` |
| 4 | `Compressor` | Zip-, Gzip-, ZstdCompressor | `processData()` works with any compressor, no changes needed |

## 🧾 Notes moved from `main.cpp`

The explanatory printouts were removed from `main.cpp` (the demos now only print what they do). Their content, so nothing is lost:

- **Examples 1 and 4:** all classes realize the same interface; client code works through the interface and does not know the concrete type (`processData()` works with any compressor — no changes needed).
- **Example 3:** the same `Button` object viewed through two different interfaces.
- **Summary block:** is-a (implements a contract); base class pure abstract — no data, all `= 0`; derived class must implement ALL pure virtuals; benefit: interchangeable implementations; one class can realize many interfaces; UML `A <— — — — —B` (dashed, hollow triangle).
- **Realization vs Inheritance:** *Realization* implements a pure interface (no data, no implementation). *Inheritance* extends a concrete class (inherits data + implementation). See the diagram above.

---

## Key characteristics

- The base class is **purely abstract** — no data members, all methods `= 0`
- The concrete class must implement **all** pure virtual methods
- A class can realize multiple interfaces simultaneously
- Client code works through the interface — independent of concrete type
- Makes implementations interchangeable

---

## Code pattern

```cpp
// Interface — pure abstract, defines the CONTRACT
class Compressor {
public:
    virtual std::string compress(const std::string& data)   const = 0;
    virtual std::string decompress(const std::string& data) const = 0;
    virtual ~Compressor() = default;
};

// Realization — concrete class fulfills the contract
class ZipCompressor : public Compressor {
public:
    std::string compress(const std::string& data)   const override { /* ... */ }
    std::string decompress(const std::string& data) const override { /* ... */ }
};

// Client uses the interface — works with ANY realization
void processData(const Compressor& compressor, const std::string& data) {
    auto packed = compressor.compress(data);
}
```

> **C++ note:** "realization" is a UML term, there is no keyword for it. In C++ it is simply `public` inheritance from a class whose members are all pure virtual (plus a virtual destructor).

---

## When to use Realization

✅ Define a contract that multiple classes must fulfill
✅ Interchangeable implementations (compressors, serializers, printers)
✅ Depend on abstractions not concretions (Dependency Inversion)
✅ Safe multiple inheritance: interfaces have no data, so nothing is duplicated (note: a diamond is still possible if two interfaces share a common base)

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

---

## Build

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o realization
./realization
```

`main.cpp` uses `std::println` (`<print>`), which needs **GCC 14+**.
