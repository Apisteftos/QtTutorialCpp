# 01 · Cohesion

> A class should have **one reason to change**.  
> If you can describe what a class does and the sentence needs the word "and" — it has low cohesion.

---

## High vs Low Cohesion

```mermaid
flowchart TD
    subgraph LOW["❌ Low Cohesion — one class does everything"]
        LC["UserManager\n─────────────\n+login()\n+logout()\n+sendEmail()\n+hashPassword()\n+logToFile()\n+validateInput()\n+connectDB()"]
    end

    subgraph HIGH["✅ High Cohesion — each class does one thing"]
        A["Authenticator\n─────────────\n+login()\n+logout()\n+hashPassword()"]
        B["EmailSender\n─────────────\n+sendEmail()\n+formatMessage()"]
        C["Logger\n─────────────\n+logToFile()\n+logToConsole()"]
        D["Validator\n─────────────\n+validateInput()\n+sanitise()"]
    end

    style LOW fill:#FF5722,color:#fff
    style HIGH fill:#4CAF50,color:#fff
```

---

## Types of Cohesion (worst → best)

```mermaid
flowchart LR
    A[Coincidental\nrandom functions\ngrouped together] -->
    B[Logical\nfunctions of same\ncategory grouped] -->
    C[Temporal\nfunctions called\nat same time] -->
    D[Procedural\nfunctions in same\nsequence] -->
    E[Communicational\nfunctions work on\nsame data] -->
    F[Sequential\noutput of one\nis input of next] -->
    G[Functional\none well-defined\ntask only ✅]

    style A fill:#F44336,color:#fff
    style B fill:#FF5722,color:#fff
    style C fill:#FF9800,color:#fff
    style D fill:#FFC107,color:#000
    style E fill:#8BC34A,color:#fff
    style F fill:#4CAF50,color:#fff
    style G fill:#2E7D32,color:#fff
```

---

## The Single Responsibility Principle

Cohesion is the *metric*. SRP is the *rule*:

```mermaid
mindmap
  root((SRP))
    One reason to change
      Authentication changes\nonly if auth rules change
      Logging changes\nonly if log format changes
      Email changes\nonly if email provider changes
    Not one method
      A class can have many methods
      They must all serve\none single purpose
    How to find violations
      Count reasons to change
      If more than one → split
```

---

## When to Split a Class

| Signal | Action |
|---|---|
| Method uses only 2 of 10 fields | Extract those 2 fields + method into new class |
| You test half the class in one test file and half in another | Split the class |
| The class name contains "And", "Manager", "Handler", "Util" | Rename and split |
| Changing email logic risks breaking auth logic | Separate immediately |

---

## C++ Specific

In C++, low cohesion often shows up as:
- Massive header files with 30+ methods
- `#include` chains pulling in unrelated dependencies
- `namespace utils { ... }` with hundreds of unrelated free functions

The fix: small focused classes, each in its own header.
