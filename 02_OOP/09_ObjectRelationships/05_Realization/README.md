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

## MCX Codec interface

```mermaid
classDiagram
    class McxCodec {
        <<interface>>
        +getName()* string
        +getBitrate()* int
        +encode(audio)* string
        +decode(data)* string
    }

    class AmrNbCodec {
        +getName() string
        +getBitrate() int
        +encode(audio) string
        +decode(data) string
    }

    class AmrWbCodec {
        +getName() string
        +getBitrate() int
        +encode(audio) string
        +decode(data) string
    }

    class OpusCodec {
        +getName() string
        +getBitrate() int
        +encode(audio) string
        +decode(data) string
    }

    McxCodec <|.. AmrNbCodec : realizes
    McxCodec <|.. AmrWbCodec : realizes
    McxCodec <|.. OpusCodec : realizes

    note for McxCodec "processAudio(McxCodec&)\nworks with ANY codec\nno code changes needed"
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
class McxCodec {
public:
    virtual std::string encode(const std::string& audio) const = 0;
    virtual std::string decode(const std::string& data)  const = 0;
    virtual ~McxCodec() = default;
};

// Realization — concrete class fulfills the contract
class AmrWbCodec : public McxCodec {
public:
    std::string encode(const std::string& audio) const override { ... }
    std::string decode(const std::string& data)  const override { ... }
};

// Client uses the interface — works with ANY realization
void processAudio(const McxCodec& codec, const std::string& audio) {
    auto encoded = codec.encode(audio);
}
```

---

## When to use Realization

✅ Define a contract that multiple classes must fulfill
✅ Interchangeable implementations (codecs, serializers, printers)
✅ Depend on abstractions not concretions (Dependency Inversion)
✅ Multiple inheritance without the diamond problem

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
