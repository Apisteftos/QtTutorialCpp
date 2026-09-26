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
| Member variable | see **Member Variable Naming Conventions** below | `m_port`, `mPort`, `port_`, `port` |
| Constant / `constexpr` | `UPPER_CASE` or `kPascalCase` | `MAX_SIZE` / `kMaxSize` |
| Namespace | `snake_case` | `namespace socket_utils` |
| Macro | `UPPER_CASE` | `#define MYLIB_EXPORT` |
| Template parameter | `PascalCase` or single capital | `typename T`, `typename InputIter` |

---

## Member Variable Naming Conventions

Unlike file naming, there is no single dominant convention for member variables —
five distinct styles are all in active, widespread use. The right choice is
"whatever the project already does," but it helps to know what each style
signals and where it comes from.

| Style | Example | Also written | Used by |
|---|---|---|---|
| `m_` prefix (underscore) | `m_workingHours` | `m_port`, `m_count` | Qt (older APIs), many enterprise / MFC-influenced codebases |
| `m` prefix (no underscore) | `mWorkingHours` | `mPort`, `mCount` | Common in Java-influenced C++ shops, some Qt/QML-adjacent projects, Android NDK code |
| Trailing underscore | `workingHours_` | `port_`, `count_` | Google C++ Style Guide, Chromium, protobuf, abseil |
| No prefix / plain | `workingHours` | `port`, `count` | STL-style / "modern C++" codebases that rely on `this->` or scope to disambiguate |
| Leading underscore | `_workingHours` | `_port` | Rare — technically legal at class scope, but risky (see caveat below) |

### Why so many styles exist

All of them solve the same real problem — telling a member variable apart
from a local variable, a parameter, or a getter of the same conceptual name
(`int port() const { return m_port; }` vs. the ambiguity of `int port() const { return port; }`).
Which one "wins" is a matter of which style guide a codebase's founders followed:

- **`m_` prefix** is the classic C++/MFC/Qt convention — the underscore makes
  the prefix visually distinct from the rest of the name at a glance.
- **`m` prefix, no underscore** produces cleaner camelCase (`mWorkingHours`
  reads as one camelCase word) and is common where the team's naming style
  leans toward Java/Android conventions, or simply as a shorter variant of
  `m_`.
- **Trailing underscore** (`workingHours_`) is Google's and Chromium's choice
  specifically because it doesn't disturb the camelCase/snake_case of the
  base name — `_` is just appended, not inserted as a prefix.
- **No prefix** relies entirely on `this->member` or on the member simply
  not colliding with any local/parameter name in scope; more common in
  small classes or in codebases that prioritize minimal visual noise.

### The leading-underscore caveat

A single leading underscore on an ordinary member name (`_workingHours`) is
**legal** at class scope in C++. It becomes a problem only under two
specific patterns, both reserved for the implementation by the standard:

- A leading underscore **followed by a capital letter**, anywhere: `_Port` — reserved.
- **Two consecutive underscores**, anywhere in the identifier: `__port` — reserved.
- A leading underscore **at global/namespace scope** (not inside a class): reserved.

`_workingHours` itself (lowercase after the underscore, inside a class) does
not violate either rule, but it's easy to typo into a reserved form, and
many style guides ban leading underscores outright to avoid the ambiguity.
For that reason it's the least-used of the five styles in production code.

### Picking one

```
Contributing to an existing project?
  → Match whatever it already uses. This overrides every preference below.

Starting fresh, following Qt conventions?
  → m_ prefix (or mPrefix if you prefer the shorter camelCase look)

Starting fresh, following Google/Chromium/abseil conventions?
  → trailing underscore (workingHours_)

Small class, low collision risk, prioritizing minimal visual noise?
  → no prefix, disambiguate with this-> where needed
```

**The one rule that matters more than which style you pick:** be
consistent within a single class, and ideally within the whole project.
Mixing `m_port` and `mCount` and `label_` in the same class is worse than
any single style applied uniformly.

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
| Member variables | `m_` prefix, `m` prefix, trailing `_`, or no prefix — pick one, be consistent |
| Constants / macros | `UPPER_CASE` — universal |

**The one rule that has no exceptions:**

> Be consistent. Mixed casing within one project (`Socket.h` next to `buffer_utils.h`)
> is worse than any single style applied uniformly.
