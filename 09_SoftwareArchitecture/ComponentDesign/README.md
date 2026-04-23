# Component Design

> Architectural patterns decide how systems are shaped.  
> Component design decides how each **piece** is built internally.  
> These are the rules that make individual components clean, testable, and replaceable.

---

## The Four Principles

```mermaid
flowchart TD
    A[Component Design] --> B[01 Cohesion\nEverything in a class\nbelongs together]
    A --> C[02 Coupling\nComponents depend on\nas little as possible]
    A --> D[03 Interfaces\nDepend on contracts\nnot implementations]
    A --> E[04 Dependency Inversion\nHigh-level modules own\nthe abstractions]

    style B fill:#4CAF50,color:#fff
    style C fill:#2196F3,color:#fff
    style D fill:#FF9800,color:#fff
    style E fill:#9C27B0,color:#fff
```

---

## How They Relate

```mermaid
flowchart LR
    COH[High Cohesion\none class, one job] -->|enables| LCO[Low Coupling\nclasses depend less]
    LCO -->|achieved via| INT[Interfaces\nprogram to abstraction]
    INT -->|enforced by| DIP[Dependency Inversion\nhigh-level owns the contract]

    style COH fill:#4CAF50,color:#fff
    style LCO fill:#2196F3,color:#fff
    style INT fill:#FF9800,color:#fff
    style DIP fill:#9C27B0,color:#fff
```

---

## Quick Reference

| Principle | Bad smell | Good sign |
|---|---|---|
| Cohesion | Class does 5 unrelated things | Class has one clear reason to change |
| Coupling | Changing A breaks B, C, D | Changing A has zero effect on B |
| Interfaces | `new ConcreteClass` everywhere | `IService* svc` everywhere |
| Dep. Inversion | Business layer imports DB layer | Both import a shared abstract layer |

---

## Learning Order

Start with **Cohesion** — it forces you to split large classes.  
Then **Coupling** — it forces you to reduce dependencies.  
Then **Interfaces** — the tool that makes low coupling possible.  
Then **Dependency Inversion** — the architectural rule that locks it all in.
