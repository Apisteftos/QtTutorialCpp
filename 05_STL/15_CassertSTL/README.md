# CstdintSTL — Fixed-Width Integer Types in C++23

`<cstdint>` provides integer types with guaranteed sizes across all platforms.
Essential for binary protocols, hardware interfaces, and embedded systems.

---

## Header

```cpp
#include <cstdint>    // fixed-width types
#include <cstddef>    // size_t, ptrdiff_t
#include <climits>    // INT8_MAX etc. (macros)
#include <limits>     // std::numeric_limits<T>
```

---

## Why use them?

```cpp
int x;    // could be 2, 4, or 8 bytes depending on platform!

int32_t x;  // always exactly 4 bytes — guaranteed
```

When writing binary protocols, network packets, or hardware register maps,
the exact byte size is critical. `int` is unreliable across platforms.

---

## Signed types

| Type | Size | Range |
|------|------|-------|
| `int8_t` | 1 byte | -128 to 127 |
| `int16_t` | 2 bytes | -32,768 to 32,767 |
| `int32_t` | 4 bytes | -2,147,483,648 to 2,147,483,647 |
| `int64_t` | 8 bytes | -9.2e18 to 9.2e18 |

---

## Unsigned types

| Type | Size | Range |
|------|------|-------|
| `uint8_t` | 1 byte | 0 to 255 |
| `uint16_t` | 2 bytes | 0 to 65,535 |
| `uint32_t` | 4 bytes | 0 to 4,294,967,295 |
| `uint64_t` | 8 bytes | 0 to 18,446,744,073,709,551,615 |

---

## Minimum-width types

```cpp
int_least8_t    // at least 8 bits, smallest available
int_least16_t   // at least 16 bits
int_least32_t   // at least 32 bits
int_least64_t   // at least 64 bits
```

May be larger than minimum on some platforms.

---

## Fastest types

```cpp
int_fast8_t    // at least 8 bits, fastest on this CPU
int_fast32_t   // often 64-bit on x86_64 for performance
```

---

## Pointer-sized types

```cpp
intptr_t   // signed — same size as a pointer
uintptr_t  // unsigned — same size as a pointer
ptrdiff_t  // difference between two pointers
std::size_t // result of sizeof(), array index type
```

---

## Limits — macros vs numeric_limits

```cpp
// C-style macros (from <climits>)
INT8_MIN   INT8_MAX   UINT8_MAX
INT16_MIN  INT16_MAX  UINT16_MAX
INT32_MIN  INT32_MAX  UINT32_MAX
INT64_MIN  INT64_MAX  UINT64_MAX

// C++ template (preferred, from <limits>)
std::numeric_limits<int32_t>::min()     // -2147483648
std::numeric_limits<int32_t>::max()     // 2147483647
std::numeric_limits<uint64_t>::max()    // 18446744073709551615
std::numeric_limits<double>::epsilon()  // ~2.22e-16
```

---

## uint8_t printing pitfall

```cpp
uint8_t b = 65;
std::cout << b;              // prints 'A' — treated as char!
std::cout << (unsigned)b;    // prints 65 — correct
std::println("{}", (unsigned)b);  // C++23 — also correct
```

`uint8_t` and `int8_t` are typedefs for `unsigned char` / `signed char` —
streaming them outputs a character, not a number.

---

## Binary protocol frame

```cpp
struct FrameHeader {
    uint8_t  version;       // 1 byte
    uint8_t  type;          // 1 byte
    uint16_t length;        // 2 bytes
    uint32_t callId;        // 4 bytes
    uint64_t timestamp;     // 8 bytes
} __attribute__((packed));  // no padding
// Total: exactly 16 bytes
```

---

## Bit fields with fixed-width types

```cpp
struct StatusRegister {
    uint8_t connected : 1;   // bit 0
    uint8_t active    : 1;   // bit 1
    uint8_t encrypted : 1;   // bit 2
    uint8_t reserved  : 5;   // bits 3-7
};  // exactly 1 byte
```

---

## When to use fixed-width types

✅ Binary network protocols (MCX, SIP, RTP)
✅ File formats with defined byte layout
✅ Hardware register access
✅ Embedded systems
✅ Cross-platform serialization
✅ When you need to know the exact byte count

❌ General computation where size doesn't matter → use `int`, `long`
❌ Array index/size → use `std::size_t`
❌ Pointer arithmetic → use `ptrdiff_t`
