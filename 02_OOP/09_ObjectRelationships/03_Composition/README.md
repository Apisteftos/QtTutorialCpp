# Composition — C++

## What is Composition?

The strongest **has-a** relationship. The whole **owns** its parts completely —
it creates them, manages them, and destroys them. Parts **cannot exist**
without the whole and have no independent meaning outside it.

---

## Class diagram

```mermaid
classDiagram
    class Car {
        -string m_model
        -Engine m_engine
        -Gearbox m_gearbox
        +drive()
        +Car(model, hp, disp, gears, auto)
        +~Car()
    }

    class Engine {
        -int m_horsepower
        -double m_displacement
        +start()
        +getHp() int
    }

    class Gearbox {
        -int m_gears
        -bool m_isAutomatic
        +shiftTo(int gear)
    }

    Car "1" *-- "1" Engine : owns
    Car "1" *-- "1" Gearbox : owns
    note for Car "Filled diamond = composition\nEngine and Gearbox created\nand destroyed WITH Car"
```

---

## Lifetime diagram — parts tied to whole

```mermaid
sequenceDiagram
    participant Main
    participant Car
    participant Engine
    participant Gearbox

    Main->>Car: create Car("BMW 320d", 190, 2.0, 8, true)
    Car->>Engine: create Engine(190, 2.0)
    Note over Engine: Engine created WITH Car
    Car->>Gearbox: create Gearbox(8, true)
    Note over Gearbox: Gearbox created WITH Car

    Main->>Car: drive()
    Car->>Engine: start()
    Car->>Gearbox: shiftTo(1)
    Car->>Gearbox: shiftTo(2)

    Note over Car,Gearbox: Leaving scope...
    Main->>Car: destroy Car
    Car->>Gearbox: ~Gearbox() auto
    Car->>Engine: ~Engine() auto
    Note over Engine,Gearbox: Both destroyed WITH Car
    Note over Engine,Gearbox: Cannot exist without Car
```

---

## House — Room composition with unique_ptr

```mermaid
classDiagram
    class House {
        -string m_address
        -vector~unique_ptr~Room~~ m_rooms
        +addRoom(name, area)
        +describe()
        +totalArea() double
        +~House()
    }

    class Room {
        -string m_name
        -double m_areaSqm
        +describe()
        +getArea() double
    }

    House "1" *-- "1..*" Room : owns via unique_ptr
```

---

## MCX client composition

```mermaid
classDiagram
    class McxClient {
        -string m_clientId
        -SessionConfig m_config
        -AudioCodec m_codec
        +connect()
        +~McxClient()
    }

    class SessionConfig {
        -string m_host
        -uint16_t m_port
        -bool m_encrypted
        +toString() string
    }

    class AudioCodec {
        -string m_name
        -int m_bitrate
        +encode()
    }

    McxClient "1" *-- "1" SessionConfig : owns
    McxClient "1" *-- "1" AudioCodec : owns
    note for McxClient "Config and Codec have no\nmeaning without the Client"
```

---

## Storage options

```mermaid
flowchart TD
    subgraph ByValue ["Option 1: Member by value (most common)"]
        Car1[Car] --> Eng1["Engine m_engine\n(on Car's stack frame)"]
    end

    subgraph UniquePtr ["Option 2: unique_ptr (dynamic, polymorphic)"]
        House1[House] --> Rooms["unique_ptr&lt;Room&gt;\n(heap, auto-deleted)"]
    end

    note["Both are composition —\nwhole owns the lifetime"]
    style note fill:#2a2a2a,color:#aaa
```

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

## Composition vs Aggregation

```mermaid
flowchart LR
    subgraph Composition ["Composition — strong has-a"]
        C[Car] -- "Engine m_engine\nmember by value" --> E[Engine]
    end

    subgraph Aggregation ["Aggregation — weak has-a"]
        D[Department] -. "Employee* pointer\nexternal, not owned" .-> Emp[Employee]
    end
```

---

## When to use Composition

✅ The part has no meaning without the whole (engine without a car)
✅ The part should always exist as long as the whole exists
✅ You want RAII — automatic cleanup when the whole goes out of scope
✅ The part should not be shared with other objects

❌ If the part can exist independently → use **Aggregation**
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
