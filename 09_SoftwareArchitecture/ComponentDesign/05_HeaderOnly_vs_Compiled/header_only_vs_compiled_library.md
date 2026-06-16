# Header-Only vs Compiled Library — Structuring a C++ API

## The Question

When building a library or API in C++, you have a fundamental choice:

- Put everything in headers (`.h` / `.hpp`) — **header-only**
- Split into headers + source files (`.h` + `.cpp`) — **compiled library**

Neither is universally correct. The right answer depends on what you are building.

---

## Option 1 — Header-Only Library

Everything lives in headers. The user includes the header and gets the full
implementation. No `.cpp` files, no separate compilation step.

```
mylib/
├── include/
│   └── mylib/
│       ├── mylib.h        ← public API (what the user sees)
│       ├── container.h    ← feature header
│       ├── algorithm.h    ← feature header
│       └── detail/
│           ├── traits.h   ← internal, not for users
│           └── impl.h     ← internal, not for users
└── CMakeLists.txt
```

```cpp
// container.h — everything defined here, nothing in a .cpp
#pragma once
#include <vector>
#include <stdexcept>

namespace mylib {

template<typename T>
class Container {
public:
    void push(const T& value) { data_.push_back(value); }
    T pop() {
        if (data_.empty()) throw std::underflow_error("empty");
        T val = data_.back();
        data_.pop_back();
        return val;
    }
    std::size_t size() const { return data_.size(); }

private:
    std::vector<T> data_;
};

} // namespace mylib
```

### When header-only is the right choice

| Situation | Reason |
|---|---|
| Template-heavy library | Templates **must** be defined in headers — no choice |
| Header-only is the goal | Single `#include`, no build step for the user |
| Small utility library | Not worth the overhead of a compiled library |
| Cross-platform distribution | No binary compatibility concerns |

### Real-world header-only libraries

| Library | What it does |
|---|---|
| Eigen | Linear algebra, matrix math |
| nlohmann/json | JSON parsing |
| stb_image | Image loading |
| Catch2 (v2) | Unit testing |
| {fmt} (core) | String formatting |
| range-v3 | Ranges and views |

### Tradeoffs

| Pro | Con |
|---|---|
| Easy to distribute — just copy headers | Longer compile times (recompiled by every TU) |
| No ABI concerns | Implementation is visible to users |
| No link step for users | Template errors are verbose |
| Works naturally with templates | Cannot hide proprietary implementation |

---

## Option 2 — Compiled Library (Header + Source Separation)

The public header declares the interface. The `.cpp` file contains the
implementation. Users compile once and link against the result.

```
mylib/
├── include/
│   └── mylib/
│       ├── mylib.h        ← public API (what the user sees)
│       ├── socket.h       ← public class declaration
│       └── buffer.h       ← public class declaration
├── src/
│   ├── socket.cpp         ← implementation, hidden from users
│   ├── buffer.cpp         ← implementation, hidden from users
│   └── internal/
│       ├── platform.h     ← internal helpers, not installed
│       └── helpers.cpp    ← internal implementation
└── CMakeLists.txt
```

```cpp
// include/mylib/socket.h — declaration only, no implementation
#pragma once
#include <string>
#include <cstdint>

namespace mylib {

class Socket {
public:
    Socket();
    ~Socket();

    bool connect(const std::string& host, uint16_t port);
    void disconnect();
    bool send(const void* data, std::size_t size);
    bool receive(void* buffer, std::size_t size);

private:
    struct Impl;                    // PIMPL — implementation hidden
    std::unique_ptr<Impl> impl_;
};

} // namespace mylib
```

```cpp
// src/socket.cpp — full implementation, users never see this
#include "mylib/socket.h"
#include "internal/platform.h"
#include <sys/socket.h>

namespace mylib {

struct Socket::Impl {
    int fd = -1;
    // OS-specific members here — completely hidden
};

Socket::Socket() : impl_(std::make_unique<Impl>()) {}
Socket::~Socket() { disconnect(); }

bool Socket::connect(const std::string& host, uint16_t port) {
    // ... actual implementation
    return true;
}

} // namespace mylib
```

### When compiled is the right choice

| Situation | Reason |
|---|---|
| Proprietary/commercial library | Implementation is not visible in the binary |
| Large codebase | Compile once, link many times — faster rebuilds |
| Stable ABI required | Changing internals does not force user recompilation |
| OS-specific code | Platform details stay in `.cpp`, not exposed |
| Heavy dependencies | Third-party includes stay out of public headers |

### Real-world compiled libraries

| Library | What it does |
|---|---|
| Qt | GUI framework |
| Boost.Asio (non-header mode) | Async networking |
| OpenSSL | Cryptography |
| gRPC | RPC framework |
| protobuf | Serialization |
| SQLite | Embedded database |

### Tradeoffs

| Pro | Con |
|---|---|
| Fast incremental builds | More complex to distribute |
| Implementation is hidden | ABI must be stable across versions |
| Stable ABI possible | Harder to use with templates |
| Heavy includes stay private | Users must link (`target_link_libraries`) |

---

## Option 3 — Mixed (The Most Common in Practice)

Most real libraries combine both approaches:
- Template code and small utilities → header-only
- Heavy implementation, OS-specific code, third-party deps → compiled

```
mylib/
├── include/
│   └── mylib/
│       ├── mylib.h            ← public umbrella header
│       ├── algorithm.h        ← header-only (templates)
│       ├── traits.h           ← header-only (type traits)
│       ├── socket.h           ← declaration only → has .cpp
│       ├── server.h           ← declaration only → has .cpp
│       └── detail/
│           └── impl.h         ← internal header-only helpers
├── src/
│   ├── socket.cpp
│   └── server.cpp
└── CMakeLists.txt
```

```cpp
// mylib.h — umbrella header, users include only this
#pragma once

#include "mylib/algorithm.h"   // header-only templates
#include "mylib/traits.h"      // header-only traits
#include "mylib/socket.h"      // compiled class
#include "mylib/server.h"      // compiled class
```

---

## The Public API Boundary — The Most Important Concept

Regardless of header-only or compiled, the critical discipline is:

**What goes in the public header is your contract with the user.**

```
include/mylib/       ← PUBLIC — installed, versioned, stable
    mylib.h
    socket.h
    algorithm.h

src/internal/        ← PRIVATE — never installed, never seen by users
    platform.h
    helpers.h
    helpers.cpp
```

Rules for the public header:
- Only declare what users need — nothing else
- Never include heavy or platform-specific headers (`<windows.h>`, `<sys/socket.h>`)
- Use forward declarations where possible to keep it lean
- Changing it is a breaking change — treat it seriously

Rules for internal headers:
- Anything that could ever change goes here
- Include whatever you need — users never see it
- Use `namespace detail` or `namespace internal`

---

## Directory Layout by Library Type

### Small header-only utility library
```
mylib/
└── include/
    └── mylib/
        ├── mylib.h        ← single include for users
        ├── feature_a.h
        ├── feature_b.h
        └── detail/
            └── impl.h
```

### Compiled library (static or shared)
```
mylib/
├── include/
│   └── mylib/             ← installed to system, users include from here
│       ├── mylib.h
│       └── socket.h
├── src/                   ← never installed, implementation only
│   ├── socket.cpp
│   └── internal/
│       └── platform.h
├── tests/
└── CMakeLists.txt
```

### Mixed library (most common)
```
mylib/
├── include/
│   └── mylib/
│       ├── mylib.h        ← umbrella
│       ├── algorithm.h    ← header-only (templates)
│       └── socket.h       ← compiled (declaration only)
├── src/
│   ├── socket.cpp
│   └── internal/
└── CMakeLists.txt
```

---

## CMakeLists.txt — How Each Type Is Expressed

### Header-only (interface library — no compilation)
```cmake
add_library(mylib INTERFACE)

target_include_directories(mylib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

### Compiled (static or shared)
```cmake
add_library(mylib STATIC     # or SHARED
    src/socket.cpp
    src/server.cpp
)

target_include_directories(mylib
    PUBLIC  include/         # users get this
    PRIVATE src/internal/    # only this library sees this
)
```

### Mixed
```cmake
add_library(mylib STATIC
    src/socket.cpp           # compiled part
    src/server.cpp
    # algorithm.h is header-only — not listed
)

target_include_directories(mylib
    PUBLIC  include/
    PRIVATE src/internal/
)
```

The `PUBLIC` / `PRIVATE` / `INTERFACE` distinction in CMake mirrors the
public/private API boundary exactly — this is not coincidence.

---

## Decision Guide

```
Are you writing template-heavy code?
  YES → header-only (templates must live in headers)
  NO  →
    Do you need to hide the implementation?
      YES → compiled (.h declaration + .cpp definition)
      NO  →
        Is the library large with heavy dependencies?
          YES → compiled (faster builds, private includes)
          NO  → header-only (simpler distribution)

In practice: use mixed — templates and traits header-only,
             everything else compiled.
```

---

## Summary

| Approach | Templates | Build speed | Hide impl | Distribution |
|---|---|---|---|---|
| Header-only | Natural | Slow (recompiled everywhere) | No | Just copy headers |
| Compiled | Awkward | Fast (compile once) | Yes | Build + install |
| Mixed | Natural | Good | Partially | Both |

**The rule that never changes:**

> `include/` contains your public contract — stable, minimal, no internals.  
> `src/` contains your implementation — anything goes, users never see it.  
> Templates live in headers no matter what approach you choose.
