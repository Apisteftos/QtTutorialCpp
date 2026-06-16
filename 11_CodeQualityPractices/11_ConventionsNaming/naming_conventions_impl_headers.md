# Industry-Standard Naming Conventions for C++ Implementation Headers

## The Problem

When something is needed internally — shared between multiple classes or translation
units — but is **not** part of the public API, where does it live and what is it called?

This is one of those things nobody teaches explicitly, but every production codebase
solves the same way.

---

## The `detail` Namespace — The Universal Convention

The established C++ convention for "internal, not for users" is the **`detail` namespace**,
combined with a matching filename. This is not a language feature — it is a social contract:

> **"If you use anything from `detail::`, you are on your own."**

---

## What Actually Lives Inside `namespace detail`

This is the part nobody explains. The contents are **overwhelmingly templates** — not plain
functions. Here is the breakdown from real codebases (Boost, fmtlib, libstdc++):

| Category | Example | Why it's here |
|---|---|---|
| **Type traits / metafunctions** | `is_string<T>`, `has_trivial_assign<T>` | Answer questions about types at compile time |
| **Policy structs with static methods** | `struct copier<bool>`, `struct filler<bool>` | Selected by compiler, not at runtime |
| **Template specializations** | `struct copier<true>` (memcpy fast path) | Can't partially specialize functions — use structs |
| **Tag types** | `struct output_iterator_tag {}` | Carry information with zero runtime cost |
| **Internal implementation classes** | `class buffer<T>`, `class error_handler` | Full classes not meant for users |
| **Plain inline helpers** | `throw_format_error(const char*)` | Rare — only when no type variation needed |

### Why structs instead of plain functions?

Plain template functions **cannot be partially specialized** in C++. So `detail` uses the
classic workaround: wrap the logic in a **struct template**, then specialize the struct.
This is the dominant pattern in Boost, fmtlib, and the STL:

```cpp
namespace mylib {
namespace detail {

// Primary template — general case (slow path)
template <bool canOptimize>
struct copier {
    template<typename I1, typename I2>
    static I2 do_copy(I1 first, I1 last, I2 out) {
        while (first != last) { *out++ = *first++; }
        return out;
    }
};

// Full specialization — fast path using memcpy
template <>
struct copier<true> {
    template<typename I1, typename I2>
    static I2* do_copy(I1* first, I1* last, I2* out) {
        std::memcpy(out, first, (last - first) * sizeof(I2));
        return out + (last - first);
    }
};

} // namespace detail

// Public API — compiler selects the right copier at compile time
template<typename I1, typename I2>
inline I2 copy(I1 first, I1 last, I2 out) {
    constexpr bool canOpt = std::is_pointer_v<I1> &&
                            std::is_pointer_v<I2> &&
                            std::is_trivially_copyable_v<
                                std::remove_pointer_t<I1>>;
    return detail::copier<canOpt>::do_copy(first, last, out);
}
} // namespace mylib
```

### Typical `detail` structure in a real library

```cpp
namespace mylib {
namespace detail {

    // 1. Type traits — answer questions about types at compile time
    template<typename T> struct is_string             : std::false_type {};
    template<>           struct is_string<std::string>: std::true_type  {};

    // 2. Policy structs — compiler picks the right one
    template<bool Fast> struct copier { ... };
    template<>          struct copier<true> { ... };  // fast-path specialization

    // 3. Tag types — zero-cost compile-time markers
    struct output_iterator_tag {};
    struct contiguous_iterator_tag {};

    // 4. Internal implementation classes
    template<typename Char> class buffer { ... };
    class error_handler { ... };

    // 5. Plain helper functions — the minority
    inline void throw_format_error(const char* msg) {
        throw std::format_error(msg);
    }

} // namespace detail
} // namespace mylib
```

**The rule:** if logic needs to vary by type → struct template + specialization inside
`detail`. If it's a simple utility with no type variation → plain `inline` function inside
`detail`.


### Is `namespace detail` compile-time or runtime?

**Both — but it leans heavily toward compile time.** The distinction is important:

| Content | When | Example |
|---|---|---|
| Type traits, metafunctions | Compile time | `is_string<T>`, `has_trivial_assign<T>` |
| Policy structs + specializations | Compile time | `copier<true>` — compiler picks the right one |
| Tag types | Compile time | Zero-size structs for overload resolution only |
| `constexpr` / `consteval` helpers | Compile time | Evaluated before the binary exists |
| Internal implementation classes | Runtime | `class buffer`, `class error_handler` — real objects |
| Plain `inline` helper functions | Runtime | `throw_format_error()` — called at runtime |
| Classes with virtual functions | Runtime | Runtime dispatch, vtable |

`namespace detail` is **not about when** — it is purely about **visibility**:

```
namespace detail  =  "not for users"
                       ↳ happens to be mostly compile-time in template libraries
                       ↳ can be runtime too — no rule against it
```

The compile-time dominance comes from the fact that template-heavy libraries (Boost,
fmtlib, STL) put most of their metaprogramming machinery in `detail` because it is
internal. A `detail` namespace in a non-template library would be mostly runtime code.
The visibility contract and the compile/runtime distinction are completely orthogonal.

---

## What the Industry Actually Uses

### Naming conventions by pattern

| Pattern | Example | Used by | Meaning |
|---|---|---|---|
| `detail.h` | `detail.h` | Boost, fmtlib | All internals in one flat file |
| `detail.hpp` | `detail.hpp` | Boost.Hana, range-v3 | Same, `.hpp` variant |
| `*_impl.h` | `socket_impl.h` | protobuf, abseil, gRPC | Per-feature implementation |
| `internal.h` | `internal.h` | abseil, gRPC | Google-style catch-all internals |
| `*_internal.h` | `format_internal.h` | abseil | Per-feature Google-style |
| `detail/` subdir | `detail/traits.h` | range-v3, Boost.Hana | Many internal headers, kept separate |
| `internal/` subdir | `internal/helpers.h` | abseil, protobuf, gRPC | Google-style subdirectory |
| `__detail` namespace | `std::__detail` | libstdc++, libc++ | Standard library internals (double underscore = reserved) |
| `_Impl` suffix (type) | `class VectorImpl` | Qt (QStringPrivate, etc.) | PIMPL — hides class internals in a separate type |
| `fwd.h` | `fwd.h` | Boost, fmtlib | Forward declarations only, no definitions |
| `config.h` / `config_impl.h` | `config_impl.h` | many | Build-time config, not public API |

---

## Namespace Naming Conventions

Beyond `detail`, real-world libraries use several namespace names for "not public":

| Namespace | Used by | Notes |
|---|---|---|
| `detail` | Boost, fmtlib, range-v3, nlohmann/json | Most common in open-source C++ |
| `internal` | abseil, gRPC, protobuf | Google house style |
| `impl` | some embedded / RTOS codebases | Less common than `detail` |
| `__detail` | libstdc++, libc++ | Double underscore = reserved for the implementation; **never use in your own code** |
| `_detail` | some codebases | Single leading underscore at namespace scope is technically reserved too; avoid |
| `impl` | some embedded / RTOS codebases | Less common than `detail` |
| `impl_` | rare | Trailing underscore variant — 1 char shorter than `detail`, signals "must be declared in headers" |
| `privates` | rare/humorous | Seen in comments and small projects |
| `core` | occasional | Ambiguous — sometimes means "the real implementation", sometimes public |
| `guts` | informal | Used by some developers as a shorter synonym for `detail`/`internal` |
| `gubbins` | very rare/British | Informal, same intent as `guts` |
| `malarkey` | joke/example | Seen in SO discussions — not serious |
| `priv` | rare | Short for "private"; uncommon |
| `hidden` | rare | Seen occasionally in older codebases |

**The safe choices for your own code: `detail` or `internal`.**

The community consensus: **pick one and be consistent**. `detail` wins in open-source C++;
`internal` wins in Google-style codebases. Everything else is personal preference.

---

## The PIMPL Idiom — A Different Kind of "Internal"

PIMPL (*Pointer to IMPLementation*, also called the *Cheshire Cat* or *Compiler Firewall*)
is a structural pattern, not just a naming convention. It hides a class's private data
behind a forward-declared pointer so that changing the internals does not recompile users.

```cpp
// Widget.h — public header, no private details exposed
class Widget {
public:
    Widget();
    ~Widget();
    void render();

private:
    struct Impl;              // forward declaration only
    std::unique_ptr<Impl> d_; // only a pointer — no definition needed here
};
```

```cpp
// Widget.cpp — implementation file, the real internals live here
struct Widget::Impl {
    int x, y;
    std::string label;
    void doRender() { ... }
};

Widget::Widget() : d_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;  // must be in .cpp where Impl is complete
void Widget::render() { d_->doRender(); }
```

Used heavily by: **Qt** (`QStringPrivate`, `QObjectPrivate`), **KDE**, many GUI frameworks.

The naming convention for the impl struct varies:

| Name | Example |
|---|---|
| `Impl` (nested struct) | `struct Widget::Impl` |
| `Private` (Qt convention) | `class QWidgetPrivate` |
| `d_` pointer name | `d_` or `d` (Qt uses `Q_D` macro) |
| Separate file | `widget_p.h` (`_p` = private, Qt convention) |

---

## Forward Declaration Headers (`fwd.h`)

A pattern you see in large codebases: a **`fwd.h`** (or `forward.h`) that contains
only forward declarations, no definitions. Users include this in their own headers
to avoid pulling in the full definition.

```cpp
// fwd.h
#pragma once

namespace mylib {
    class Socket;
    class Buffer;
    template<typename T> class Span;
} // namespace mylib
```

```cpp
// Socket.h — full definition, only included in .cpp files that need it
#pragma once
#include "fwd.h"

namespace mylib {
class Socket {
    // ...
};
}
```

Used by: **Boost**, **fmtlib**, **abseil**, **Qt** (`qnamespace.h`, `qglobal.h`).

This keeps compile times short in large projects — a common source of pain when
headers include headers include headers.

---

## Config / Platform Headers

Another category of "not really public API but must exist" headers:

| Name | Purpose | Used by |
|---|---|---|
| `config.h` | Build-time detected constants (`#define HAVE_EPOLL`) | autoconf, CMake |
| `export.h` | `__declspec(dllexport)` / visibility macros | CMake `GenerateExportHeader` |
| `platform.h` | OS/compiler detection and portability macros | many |
| `version.h` | Library version as `#define` constants | almost all libraries |
| `compiler.h` | Compiler-specific workarounds | abseil, Boost |

These are public in the sense that users include them, but they are not "API" in the
semantic sense — they are infrastructure.

---

## Summary: Which to Use When

```
You have...                              Use...
───────────────────────────────────────────────────────────────
A few shared internal helpers            detail.h  +  namespace detail
Per-feature internal helpers             feature_impl.h  +  namespace detail
Many internal headers                    detail/ subdirectory
Google-style project                     internal.h  +  namespace internal
Need to hide class private data          PIMPL  →  struct Impl in .cpp
Large project, slow compile times        fwd.h  (forward declarations only)
Build config / platform detection        config.h, export.h, platform.h
```

**The rules to remember:**

> Anything not meant for the user goes in `namespace detail` (or `namespace internal`).  
> The filename signals this with `_impl`, `detail`, or `internal`.  
> If it's a whole class's private data, use PIMPL.  
> Inside `detail`: logic that varies by type → struct template + specialization. Simple utility with no type variation → plain `inline` function.

This is what Boost, abseil, fmtlib, range-v3, Qt, protobuf, and gRPC all do.
