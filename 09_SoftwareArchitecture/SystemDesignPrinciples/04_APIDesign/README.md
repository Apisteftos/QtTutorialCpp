# 04 · API Design

> An API is a **promise** to your callers.  
> A bad API is a promise you'll regret keeping.  
> Good API design makes correct usage obvious and incorrect usage hard to compile.

---

## The Three Layers of a Good API

```mermaid
flowchart TD
    L1["🎯 Correct by Construction\nMake wrong usage a compile error\nnot a runtime crash"]
    L2["📖 Self-Documenting\nNames reveal intent\nNo comments needed for basic usage"]
    L3["🔒 Encapsulation\nCaller cannot break invariants\nInternal state is private"]

    L1 --> L2 --> L3
```

---

## Fluent Interface — Builder Pattern

```mermaid
flowchart LR
    A["QueryBuilder\n.select('id','name')\n.from('users')\n.where('age > 18')\n.orderBy('name')\n.limit(10)\n.build()"] -->|"returns"| B["SELECT id, name\nFROM users\nWHERE age > 18\nORDER BY name\nLIMIT 10"]

    NOTE["Each method returns *this\nenabling method chaining\nValidation happens in build()"]

    style NOTE fill:#FFF3E0
```

---

## Versioning Strategy

```mermaid
flowchart TD
    V1["v1: GET /users/{id}\nreturns {id, name}"] -->|"breaking change"| V2["v2: GET /v2/users/{id}\nreturns {id, name, email, role}"]

    subgraph GOOD["✅ Non-breaking changes"]
        G1[Add optional field]
        G2[Add new endpoint]
        G3[Add optional parameter]
    end

    subgraph BAD["❌ Breaking changes"]
        B1[Remove field]
        B2[Rename field]
        B3[Change field type]
        B4[Remove endpoint]
    end
```

---

## Error Design

```mermaid
flowchart LR
    subgraph OPAQUE["❌ Opaque errors"]
        E1["throw std::runtime_error('error')"]
        E2["return -1"]
        E3["return nullptr"]
    end

    subgraph RICH["✅ Rich errors"]
        R1["throw ValidationError('age must be > 0', field='age')"]
        R2["return Result<T, Error>"]
        R3["return std::expected<T, ApiError>"]
    end
```

---

## API Design Rules

| Rule | Bad | Good |
|---|---|---|
| Name reveals intent | `process()` | `validateAndSave()` |
| Fail loudly | Return -1 on error | Throw typed exception |
| Immutable returns | Return raw pointer | Return `const&` or value |
| Narrow inputs | Take `User` struct | Take only needed fields |
| No boolean params | `sort(true)` | `sort(Order::Ascending)` |
| Const-correct | `getName()` modifies state | `getName() const` |
