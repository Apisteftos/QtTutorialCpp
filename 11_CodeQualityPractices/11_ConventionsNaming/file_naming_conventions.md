# C++ File Naming Conventions

## The Core Question

Should your headers be `Socket.h`, `socket.h`, or `socket_handler.h`?  
There is no single enforced standard — but the industry has clear camps,
and each major style guide is consistent within itself.

---

## The Four Casing Styles

| Style | Example | Also called |
|---|---|---|
| `snake_case` | `socket_handler.h` | lowercase with underscores |
| `PascalCase` | `SocketHandler.h` | UpperCamelCase |
| `camelCase` | `socketHandler.h` | lowerCamelCase |
| `lowercase` | `sockethandler.h` | flat lowercase, no separator |

---

## What the Major Style Guides Say

### Google C++ Style Guide
- **Files:** `snake_case` — `my_class.h`, `my_class.cc`
- **Extension:** `.h` for headers, `.cc` for source (not `.cpp`)
- File name matches the primary class: class `MyClass` → file `my_class.h`
- Used by: abseil, protobuf, gRPC, Chromium, GoogleTest

### LLVM / Clang Style Guide
- **Files:** `PascalCase` — `SocketHandler.h`, `SocketHandler.cpp`
- File name matches the class name exactly: class `SocketHandler` → `SocketHandler.h`
- Used by: LLVM, Clang, clang-tidy

### Qt Style Guide
- **Files:** `lowercase` — `qsocket.h`, `qobject.h`
- All lowercase, no underscores, `q` prefix for Qt classes
- Extension: `.h` for everything (even C++ class headers)
- Used by: Qt, KDE

### Boost Style Guide
- **Files:** `snake_case` — `shared_ptr.hpp`, `static_assert.hpp`
- Extension: `.hpp` for C++ headers, `.h` only for C-compatible headers
- Used by: Boost, range-v3, many open-source C++ libraries

### STL (libstdc++ / libc++)
- **Files:** `snake_case`, no extension — `<vector>`, `<type_traits>`
- Internal implementation files use `snake_case` with `.h`

---

## Summary by Style Guide

| Style guide | File casing | Extension | Example |
|---|---|---|---|
| Google | `snake_case` | `.h` / `.cc` | `socket_handler.h` |
| LLVM | `PascalCase` | `.h` / `.cpp` | `SocketHandler.h` |
| Qt | `lowercase` | `.h` | `qsockethandler.h` |
| Boost | `snake_case` | `.hpp` / `.h` | `socket_handler.hpp` |
| STL | `snake_case` | none / `.h` | `<socket_handler>` |
| Microsoft | `PascalCase` | `.h` / `.cpp` | `SocketHandler.h` |

---

## The File-Matches-Class Rule

The most important rule regardless of which casing style you choose:

> **The file name should match the primary class it contains.**

```
class SocketHandler  →  SocketHandler.h   (LLVM style)
class SocketHandler  →  socket_handler.h  (Google style)
class QSocket        →  qsocket.h         (Qt style)
```

One class per file is the standard in C++. Multiple small related classes
can share a file, but the file is named after the primary one.

---

## What Goes in Capital Letters vs Lowercase

This question has two dimensions: **file names** and **identifiers inside the file**.
They follow different rules.

### File names

| Situation | Convention | Reason |
|---|---|---|
| Public API header | Match your chosen style consistently | Predictable for users |
| Internal / detail header | `snake_case` preferred | Most style guides agree here |
| Template implementation | Same as its paired header | `container.h` → `container_impl.h` |
| Forward declarations | `fwd.h` or `feature_fwd.h` | Universally lowercase |
| Config / platform | `config.h`, `export.h`, `platform.h` | Always lowercase |
| Test files | `socket_handler_test.cpp` | Google style; or `SocketHandlerTest.cpp` LLVM |

### Identifiers inside the file

| Identifier | Convention | Example |
|---|---|---|
| Class / struct / enum | `PascalCase` | `class SocketHandler` |
| Function / method | `camelCase` or `snake_case` | `connect()` / `do_connect()` |
| Variable | `camelCase` or `snake_case` | `portNumber` / `port_number` |
| Member variable | trailing `_` or `m_` prefix | `port_` or `m_port` |
| Constant / `constexpr` | `UPPER_CASE` or `kPascalCase` | `MAX_SIZE` / `kMaxSize` |
| Namespace | `snake_case` | `namespace socket_utils` |
| Macro | `UPPER_CASE` | `#define MYLIB_EXPORT` |
| Template parameter | `PascalCase` or single capital | `typename T`, `typename InputIter` |

---

## Case Sensitivity Warning

Linux and macOS (default HFS+) filesystems are **case-sensitive**.
Windows NTFS is **case-insensitive**.

This means:
```cpp
#include "SocketHandler.h"   // works on Windows
#include "sockethandler.h"   // same file on Windows, different file on Linux
```

**The rule:** always write `#include` paths exactly as the file is named on disk,
and always use consistent casing. Code that compiles on Windows may fail on Linux
if casing is inconsistent.

The safest choice for cross-platform projects: **`snake_case`** — it avoids
any ambiguity because there are no capital letters to get wrong.

---

## Structure of a Real Project — File Naming in Context

### Google style (snake_case)
```
include/
    mylib/
        mylib.h              ← umbrella public header
        socket_handler.h     ← public class SocketHandler
        buffer.h             ← public class Buffer
        detail/
            platform_impl.h  ← internal
            traits.h         ← internal
src/
    socket_handler.cc        ← implementation of SocketHandler
    buffer.cc                ← implementation of Buffer
    internal/
        helpers.cc           ← internal
tests/
    socket_handler_test.cc
    buffer_test.cc
```

### LLVM style (PascalCase)
```
include/
    mylib/
        MyLib.h
        SocketHandler.h
        Buffer.h
        detail/
            PlatformImpl.h
            Traits.h
src/
    SocketHandler.cpp
    Buffer.cpp
    Internal/
        Helpers.cpp
tests/
    SocketHandlerTest.cpp
    BufferTest.cpp
```

---

## Decision Guide

```
Are you contributing to an existing project?
  YES → follow whatever convention the project already uses. Full stop.
  NO  →
    Do you want maximum open-source compatibility?
      YES → snake_case + .hpp (Boost style)
    Do you follow Google / abseil coding standards?
      YES → snake_case + .h / .cc
    Do you come from a Windows / MSVC / Qt background?
      YES → PascalCase + .h / .cpp
    Are you writing a cross-platform library?
      RECOMMENDED → snake_case (safest on case-sensitive filesystems)
```

---

## Summary

| Decision | Recommendation |
|---|---|
| Which casing for files? | Pick one style, apply it everywhere |
| Cross-platform safe choice | `snake_case` — no ambiguity on Linux/macOS/Windows |
| File name vs class name | File name should match the primary class |
| Internal / detail headers | `snake_case` regardless of project style |
| Identifiers (classes) | Always `PascalCase` — universal across all guides |
| Identifiers (functions/vars) | `camelCase` or `snake_case` — depends on guide |
| Constants / macros | `UPPER_CASE` — universal |

**The one rule that has no exceptions:**

> Be consistent. Mixed casing within one project (`Socket.h` next to `buffer_utils.h`)
> is worse than any single style applied uniformly.
