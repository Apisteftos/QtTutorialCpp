# BitsetSTL — std::bitset in C++23

`std::bitset<N>` is a fixed-size sequence of N bits with a rich manipulation interface.
Size is a compile-time constant. More readable and safer than manual bitmask operations.

---

## Header

```cpp
#include <bitset>
```

---

## Declaration

```cpp
std::bitset<8> b;                    // all zeros
std::bitset<8> b(0b10110100);        // from integer literal
std::bitset<8> b(0xA5);             // from hex
std::bitset<8> b("11001010");       // from string (MSB = leftmost char)
```

Size `N` must be a compile-time constant.

---

## Setting, clearing, flipping

```cpp
b.set(3)     // set bit 3 to 1
b.set()      // set ALL bits to 1
b.reset(3)   // clear bit 3 to 0
b.reset()    // clear ALL bits to 0
b.flip(3)    // toggle bit 3
b.flip()     // toggle ALL bits
```

---

## Accessing bits

```cpp
b[3]         // no bounds check
b.test(3)    // bounds checked — throws std::out_of_range

b.count()    // number of set bits (1s)
b.size()     // total number of bits (always N)
b.any()      // true if any bit is 1
b.all()      // true if all bits are 1
b.none()     // true if no bits are 1
```

---

## Bitwise operators

```cpp
x & y    // AND
x | y    // OR
x ^ y    // XOR
~x       // NOT (flip all)
x << 2   // left shift
x >> 2   // right shift

x &= y; x |= y; x ^= y;  // in-place versions
```

---

## Conversion

```cpp
b.to_ulong()    // to unsigned long
b.to_ullong()   // to unsigned long long
b.to_string()   // to "10110100" string
b.to_string('O', 'I')  // custom chars for 0/1
```

---

## Permission flags

```cpp
enum Perm { READ=0, WRITE=1, EXEC=2 };
std::bitset<3> perms;

perms.set(READ);
perms.set(WRITE);

perms.test(READ)   // true
perms.test(EXEC)   // false
perms.to_ulong()   // 3 (binary 011)
```

---

## Feature flags

```cpp
enum Feature { DARK_MODE=0, NOTIFICATIONS=1, ANALYTICS=2 };
std::bitset<8> features;

features.set(DARK_MODE);
features.set(NOTIFICATIONS);

features.test(DARK_MODE)     // true
features.flip(DARK_MODE)     // toggle
features.reset(NOTIFICATIONS) // disable
```

---

## Sieve of Eratosthenes

```cpp
const int N = 1000;
std::bitset<N+1> isPrime;
isPrime.set();
isPrime.reset(0); isPrime.reset(1);

for (int i = 2; i*i <= N; ++i)
    if (isPrime.test(i))
        for (int j = i*i; j <= N; j += i)
            isPrime.reset(j);

isPrime.count()  // number of primes up to N
```

---

## bitset vs uint32_t bitmask

| | `uint32_t` | `std::bitset<N>` |
|--|-----------|-----------------|
| Max bits | 64 | Any N |
| `count()` set bits | Manual (`__builtin_popcount`) | ✅ built-in |
| Bounds check | ❌ | ✅ with `test()` |
| String output | ❌ | ✅ `to_string()` |
| Arithmetic ops | ✅ | ❌ |
| C interop | ✅ | via `to_ulong()` |
| Runtime size | ✅ | ❌ compile-time only |

---

## When to use bitset

✅ Fixed-size bit flags (permissions, features, status)
✅ Need `count()` of set bits
✅ Sieve of Eratosthenes
✅ Debugging — easy string representation
✅ More than 64 bits needed

❌ Size not known at compile time → use `std::vector<bool>` or `uint64_t`
❌ Need arithmetic on the value → use `uint32_t`/`uint64_t`
❌ C interoperability → use plain integer bitmask
