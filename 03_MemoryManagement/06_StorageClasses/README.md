# Storage Classes — C++

## What is a Storage Class?

A storage class defines three things for a variable or function:
- **Lifetime** — how long it exists in memory
- **Scope** — where it is visible/accessible
- **Linkage** — whether it is accessible from other translation units

---

## Overview

```mermaid
flowchart TD
    subgraph StorageClasses ["Storage Classes in C++"]
        A["auto\nType deduction C++11\nLifetime: block scope\nScope: local"]
        S["static\nPersists across calls\nLifetime: program\nScope: local/file/class"]
        E["extern\nDefined elsewhere\nLifetime: program\nScope: global"]
        M["mutable\nModifiable in const\nLifetime: object lifetime\nScope: class member"]
        R["register\nDEPRECATED C++11\nREMOVED C++17"]
        T["thread_local C++11\nOne copy per thread\nLifetime: thread\nScope: any"]
    end
```

---

## 1. auto — type deduction (C++11)

```cpp
auto a = 32;             // int
auto b = 3.2;            // double
auto c = "Konstantinos"; // const char*
auto d = 'I';            // char
auto e = true;           // bool
```

The compiler deduces the type from the initializer — you never write the type.

```mermaid
flowchart LR
    init["auto a = 32"] --> deduce["Compiler sees: 32 is int"]
    deduce --> result["a is int\ntype locked at compile time"]
```

> **Historical note:** In C++98 `auto` meant "automatic storage duration"
> (the default for local variables). This was removed in C++11 when the
> keyword was repurposed for type deduction.

**Common uses:**
```cpp
auto it = myMap.begin();          // iterator — long type
for (const auto& item : vec) { } // range-based for
auto result = computeHeavy();     // return type may change
```

---

## 2. static — persists across calls

Three different contexts — same keyword, different meanings:

```mermaid
flowchart TD
    static["static keyword"] --> local["Local variable\nInitialized once\nPersists between calls"]
    static --> file["File scope\nInternal linkage\nNot visible from other .cpp files"]
    static --> member["Class member\nShared across ALL instances\nAccess via ClassName::member"]
```

### static local variable

```cpp
int generateId() {
    static int s_nextId = 1000;  // initialized ONCE on first call
    return s_nextId++;
}

generateId();  // 1000
generateId();  // 1001
generateId();  // 1002  — value persists!
```

### static class member

```cpp
class Counter {
public:
    static int s_count;  // ONE copy shared by ALL instances
};
Counter::s_count = 0;   // must define outside class

Counter a, b, c;
// Counter::s_count == 3 — all three instances share it
```

### static at file scope (internal linkage)

```cpp
static int fileOnlyVar = 42;  // only visible in this .cpp file
// Other .cpp files cannot access this — no name collision risk
```

---

## 3. extern — defined in another translation unit

```mermaid
sequenceDiagram
    participant header as config.h
    participant cpp1 as config.cpp
    participant cpp2 as main.cpp

    header->>cpp2: extern int g_port;\nextern string g_host;
    cpp1->>cpp1: int g_port = 5060;\nstring g_host = "localhost";
    cpp2->>cpp2: uses g_port, g_host
    Note over cpp1,cpp2: Linker connects declaration → definition
```

```cpp
// config.h — declaration (tell compiler it exists)
extern int         g_port;
extern std::string g_host;

// config.cpp — definition (actual memory allocated here)
int         g_port = 5060;
std::string g_host = "localhost";

// main.cpp — just include config.h and use
#include "config.h"
std::cout << g_port;   // works — linker resolves it
```

**Rule:** `extern` → declare in `.h`, define in exactly ONE `.cpp`.

---

## 4. mutable — modify in const context

```mermaid
flowchart LR
    subgraph ConstObject ["const Session session"]
        m_id["string m_id\nnot mutable\ncannot change"]
        m_count["int m_accessCount\nmutable\nCAN change even in const!"]
    end

    caller["session.getId()"] --> m_id
    caller --> m_count
    m_count --> increment["++m_accessCount\nallowed!"]
```

```cpp
class Session {
    std::string m_id;
    mutable int m_accessCount = 0;  // mutable!

public:
    std::string getId() const {
        ++m_accessCount;   // OK — mutable bypasses const
        return m_id;
    }
};

const Session s("SES-001");
s.getId();  // allowed — const method
s.getId();  // allowed
// s.m_id = "X"  // ERROR — not mutable
```

**When to use mutable:**
- Lazy-evaluated caches inside const getters
- Access counters / audit logging in const methods
- `std::mutex` inside a const thread-safe class

---

## 5. register — removed in C++17

```mermaid
flowchart LR
    cpp98["C++98\nregister int x;\nHint: store in CPU register"] --> cpp11["C++11\nDeprecated\nCompiler ignores hint"]
    cpp11 --> cpp17["C++17\nKeyword REMOVED\nregister int x; → ERROR"]
```

```cpp
// C++98 — valid but useless hint
register int x = 5;

// C++17+ — compile error!
// register int x = 5;  ← ERROR: 'register' storage class removed

// Modern: trust the compiler optimizer
// It does register allocation better than manual hints
int x = 5;   // compiler decides where to store it
```

---

## 6. thread_local — one copy per thread (C++11)

```mermaid
flowchart TD
    subgraph Memory ["thread_local int tl_id"]
        main["Main thread\ntl_id = 99"]
        t1["Thread 1\ntl_id = 1"]
        t2["Thread 2\ntl_id = 2"]
        t3["Thread 3\ntl_id = 3"]
    end

    note["Each thread has its OWN copy\nChanges in one thread\ndo NOT affect others\nNo mutex needed!"]
    style note fill:#2a2a2a,color:#aaa
```

```cpp
thread_local int tl_sessionId = 0;  // separate per thread

void worker(int id) {
    tl_sessionId = id;   // only affects THIS thread's copy
    std::cout << tl_sessionId;
}

std::thread t1(worker, 1);
std::thread t2(worker, 2);
// t1 and t2 each have their own tl_sessionId — no race condition
```

**Common uses:**
- Per-thread random number generators (`std::mt19937`)
- `errno` — is thread_local in POSIX
- Per-thread logging context or session ID
- Avoid mutex overhead for thread-specific data

---

## Lifetime comparison

```mermaid
flowchart LR
    subgraph Lifetimes
        block["Block/auto\nCreated at {}\nDestroyed at }"]
        static2["static local\nCreated on first call\nDestroyed at program end"]
        global["extern/global\nCreated at program start\nDestroyed at program end"]
        thread["thread_local\nCreated when thread starts\nDestroyed when thread ends"]
    end
```

---

## Summary table

| Keyword | Lifetime | Scope | Linkage | C++ status |
|---------|---------|-------|---------|------------|
| `auto` | Block | Local | None | ✅ Type deduction C++11 |
| `static` (local) | Program | Local | None | ✅ Persists between calls |
| `static` (file) | Program | File | Internal | ✅ Not visible outside `.cpp` |
| `static` (class) | Program | Class | External | ✅ Shared across instances |
| `extern` | Program | Global | External | ✅ Defined elsewhere |
| `mutable` | Object | Class member | N/A | ✅ Bypass const |
| `register` | Block | Local | None | ❌ Removed C++17 |
| `thread_local` | Thread | Any | Any | ✅ C++11 |
