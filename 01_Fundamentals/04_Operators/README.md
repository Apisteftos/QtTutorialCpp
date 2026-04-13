# 03 – Operators in C++23

This project demonstrates all fundamental C++ operators with practical examples.

---

## Example 1 — Arithmetic Operators

| Operator | Name | Example | Result |
|----------|------|---------|--------|
| `+` | Addition | `20 + 6` | `26` |
| `-` | Subtraction | `20 - 6` | `14` |
| `*` | Multiplication | `20 * 6` | `120` |
| `/` | Division | `20 / 6` | `3` (truncated!) |
| `%` | Modulo | `20 % 6` | `2` |

### Integer division pitfall
```cpp
7 / 2       // = 3  — truncates, does NOT round
7.0 / 2     // = 3.5 — correct, one operand is double
(double)7/2 // = 3.5 — cast forces floating point division
```
Always make at least one operand a `double` when you need decimal results.

### Modulo use cases
```cpp
n % 2 == 0   // check if n is even
n % 10       // last digit of n
index % size // wrap around array index (circular buffer)
```

---

## Example 2 — Increment & Decrement

| Operator | Name | Behavior |
|----------|------|----------|
| `++x` | Prefix increment | Increment first, then return new value |
| `x++` | Postfix increment | Return current value, then increment |
| `--x` | Prefix decrement | Decrement first, then return new value |
| `x--` | Postfix decrement | Return current value, then decrement |

```cpp
int x = 5;
std::cout << ++x;  // prints 6 — x is now 6
std::cout << x++;  // prints 6 — x becomes 7 after
std::cout << x;    // prints 7
```

### Prefix vs Postfix — which to prefer?
In modern C++ **prefer prefix** (`++x`) unless you specifically need the old value.
Postfix creates a temporary copy of the old value which is wasteful, especially for
iterators and complex objects. For plain `int` the compiler optimizes them to the same thing,
but the habit of using prefix is good practice.

---

## Example 3 — Assignment Operators

Compound assignment operators combine an arithmetic/bitwise operation with assignment:

| Operator | Equivalent | Example (`n=10`) | Result |
|----------|-----------|-----------------|--------|
| `+=` | `n = n + x` | `n += 5` | `15` |
| `-=` | `n = n - x` | `n -= 3` | `12` |
| `*=` | `n = n * x` | `n *= 2` | `24` |
| `/=` | `n = n / x` | `n /= 4` | `6` |
| `%=` | `n = n % x` | `n %= 4` | `2` |
| `&=` | `n = n & x` | `n &= 10` | bitwise AND |
| `\|=` | `n = n \| x` | `n \|= 3` | bitwise OR |
| `^=` | `n = n ^ x` | `n ^= 10` | bitwise XOR |
| `<<=` | `n = n << x` | `n <<= 2` | left shift |
| `>>=` | `n = n >> x` | `n >>= 2` | right shift |

---

## Example 4 — Comparison Operators

| Operator | Meaning | Example | Result |
|----------|---------|---------|--------|
| `==` | Equal | `10 == 20` | `0` (false) |
| `!=` | Not equal | `10 != 20` | `1` (true) |
| `<` | Less than | `10 < 20` | `1` (true) |
| `>` | Greater than | `10 > 20` | `0` (false) |
| `<=` | Less or equal | `10 <= 10` | `1` (true) |
| `>=` | Greater or equal | `10 >= 20` | `0` (false) |

All comparison operators return `bool` (`0` or `1`).

### C++20 Spaceship operator `<=>`
The three-way comparison operator returns an ordering value:
```cpp
auto result = a <=> b;
// result < 0  → a is less than b
// result > 0  → a is greater than b
// result == 0 → a equals b
```
Useful for implementing sorting and custom comparisons. When you add `operator<=>` to a class,
the compiler automatically generates all six comparison operators for you.

---

## Example 5 — Logical Operators

| Operator | Name | Result |
|----------|------|--------|
| `&&` | AND | `true` only if **both** operands are `true` |
| `\|\|` | OR | `true` if **at least one** operand is `true` |
| `!` | NOT | Inverts the value |

### Truth table
| A | B | A && B | A \|\| B | !A |
|---|---|--------|---------|-----|
| true | true | true | true | false |
| true | false | false | true | false |
| false | true | false | true | true |
| false | false | false | false | true |

### Short-circuit evaluation — important!
C++ stops evaluating as soon as the result is determined:

```cpp
// AND — if left side is false, right side is NEVER evaluated
false && (++counter > 0)   // counter is NOT incremented

// OR — if left side is true, right side is NEVER evaluated
true || (++counter > 0)    // counter is NOT incremented
```

This means side effects (like function calls or increments) in the right operand
may never execute. Use this deliberately:

```cpp
// Safe null check — ptr is only dereferenced if not null
if (ptr != nullptr && ptr->isValid()) { ... }
```

---

## Example 6 — Bitwise Operators

Operate on individual bits of integer values.

| Operator | Name | Description |
|----------|------|-------------|
| `&` | AND | Bit is `1` only if both bits are `1` |
| `\|` | OR | Bit is `1` if either bit is `1` |
| `^` | XOR | Bit is `1` if bits are different |
| `~` | NOT | Flips all bits |
| `<<` | Left shift | Shifts bits left — multiplies by 2 per shift |
| `>>` | Right shift | Shifts bits right — divides by 2 per shift |

```
u = 00001100  (12)
v = 00001010  (10)

u & v = 00001000  (8)   — only bits set in BOTH
u | v = 00001110  (14)  — bits set in EITHER
u ^ v = 00000110  (6)   — bits set in ONE but not both
~u    = 11110011  (243 as uint8)
u << 1= 00011000  (24)  — 12 * 2
u >> 1= 00000110  (6)   — 12 / 2
```

### Bit flags — practical pattern
Used extensively in Qt (`Qt::WindowFlags`, `Qt::AlignmentFlag`) and embedded/protocol work:

```cpp
constexpr uint8_t FLAG_READ    = 0b00000001;  // bit 0
constexpr uint8_t FLAG_WRITE   = 0b00000010;  // bit 1
constexpr uint8_t FLAG_EXECUTE = 0b00000100;  // bit 2

uint8_t permissions = 0;

permissions |= FLAG_READ;           // SET a flag
permissions |= FLAG_WRITE;          // SET another flag
bool canRead = permissions & FLAG_READ;     // CHECK a flag
permissions &= ~FLAG_WRITE;         // CLEAR a flag
permissions ^= FLAG_EXECUTE;        // TOGGLE a flag
```

### Shift as fast multiply/divide
```cpp
x << 1  // x * 2
x << 2  // x * 4
x << 3  // x * 8
x >> 1  // x / 2
x >> 2  // x / 4
```
The compiler does this optimization automatically for `* 2` and `/ 2`,
but explicit shifts are common in embedded and protocol code for clarity.

---

## Operator precedence (high to low)

| Priority | Operators |
|----------|-----------|
| Highest | `++` `--` (postfix) |
| | `++` `--` (prefix), `!`, `~` |
| | `*`, `/`, `%` |
| | `+`, `-` |
| | `<<`, `>>` |
| | `<`, `<=`, `>`, `>=` |
| | `==`, `!=` |
| | `&` |
| | `^` |
| | `\|` |
| | `&&` |
| | `\|\|` |
| | `<=>` |
| Lowest | `=`, `+=`, `-=`, etc. |

When in doubt — **use parentheses**. They cost nothing and make intent clear.
