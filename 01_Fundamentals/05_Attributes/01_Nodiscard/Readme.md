# `[[nodiscard]]`: warn when a result is ignored

> **Intent:** Mark a function, type or constructor so that the compiler warns when the caller throws the result away.

`[[nodiscard]]` (C++17) is an attribute. It changes no behavior at run time. It only makes the compiler print a warning (`-Wunused-result`, part of `-Wall`) when the return value of a call is not used. With C++20 you can add a reason: `[[nodiscard("text")]]`, and the text appears in the warning.

## Examples in `main.cpp`

| # | What | Where the attribute goes | Ignoring the result means |
|---|---|---|---|
| 1 | Function | `[[nodiscard]] int parsePort(...)` | the parsed value is lost |
| 2 | Reason text (C++20) | `[[nodiscard("...")]] std::string trimmed(...)` | the reader learns the input is not changed in place |
| 3 | Type | `enum class [[nodiscard]] ErrorCode`, `struct [[nodiscard]] Result` | an error code or status is never checked |
| 4 | Member function | `[[nodiscard]] bool empty() const` | `empty()` was called when `clear()` was meant |
| 5 | RAII guard | `[[nodiscard]] ScopedTimer startTimer(...)` | the guard is destroyed at once and measures nothing |
| 6 | Constructor (C++20) | `[[nodiscard]] explicit Lock(int)` | a temporary `Lock(2);` is released immediately |
| 7 | Discarding on purpose | `(void)call();` or `std::ignore = call();` | nothing: you state that you do not need the result |
| 8 | Standard library | `std::vector::empty`, `std::optional::value_or`, `std::async` | the standard already marks them |

## Where to put it

```cpp
[[nodiscard]] int f();                 // function
[[nodiscard]] bool Stack::empty() const;   // member function
enum class [[nodiscard]] ErrorCode {}; // every function returning ErrorCode is covered
struct [[nodiscard]] Result {};        // same for a class or struct
[[nodiscard]] explicit Lock(int);      // constructor (C++20)
```

Put it before the declaration (or right after `class` / `enum class` for types).

## What the compiler says

Compiling the same calls without the `//` comments in front (GCC 15.2.0, `-Wall -Wextra`):

```text
warning: ignoring return value of 'int parsePort(std::string_view)', declared with attribute 'nodiscard' [-Wunused-result]
warning: ignoring return value of 'std::string trimmed(std::string_view)', declared with attribute 'nodiscard': 'the returned string is the new value, the input is not changed' [-Wunused-result]
warning: ignoring returned value of type 'ErrorCode', declared with attribute 'nodiscard' [-Wunused-result]
warning: ignoring returned value of type 'Result', declared with attribute 'nodiscard' [-Wunused-result]
warning: ignoring return value of 'bool Stack::empty() const', declared with attribute 'nodiscard' [-Wunused-result]
warning: ignoring return value of 'ScopedTimer startTimer(std::string)', declared with attribute 'nodiscard' [-Wunused-result]
warning: ignoring return value of 'Lock::Lock(int)', declared with attribute 'nodiscard' [-Wunused-result]
warning: ignoring return value of 'constexpr bool std::vector<...>::empty() const [...]', declared with attribute 'nodiscard' [-Wunused-result]
warning: ignoring return value of 'std::future<...> std::async(...)', declared with attribute 'nodiscard' [-Wunused-result]
```

All of them are only warnings. To turn them into errors use `-Werror=unused-result`.

## When to use it

Use it when ignoring the result is almost certainly a bug:

- **Error indicators:** error codes, `bool` success flags, status structs, `std::expected`.
- **Pure queries:** `empty()`, `size()`, `find()`, `parse()`, anything that only computes a value. Calling it for its side effect cannot work.
- **Ownership and handles:** factory functions returning a `unique_ptr`, a file handle, a `std::future` (see `ThreadPool::submit`), a lock or timer guard.
- **Functions where a similar name invites a mix-up:** `empty()` vs `clear()`, `remove` vs `erase`.

Do not use it when ignoring the result is normal. Functions such as `push_back` or `printf`-style calls would only produce noise, and a flood of warnings makes people stop reading warnings.

## Discarding on purpose

```cpp
(void)tryLogin("guest");           // C-style cast to void
std::ignore = tryLogin("admin");   // <tuple>, easy to search for
```

Both tell the compiler and the reader that dropping the result is intended.

## Notes

- `[[nodiscard]]` on a **type** applies to every function that returns it by value. It does not apply to functions returning a reference or pointer to it.
- A `[[nodiscard]]` function returning `void` makes no sense (nothing to discard), so do not write it.
- It does not prevent misuse. It is a hint for the compiler, and the warning can be silenced by the caller.
- The attribute is part of the interface, so put it on the declaration in the header.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o nodiscard
./nodiscard
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/nodiscard
```

## Output (GCC 15.2.0)

```text
=== EXAMPLE 1: ON A FUNCTION ===
port = 8080

=== EXAMPLE 2: WITH A REASON MESSAGE ===
[hello]

=== EXAMPLE 3: ON A TYPE ===
openFile ok? true
validate: false (age must not be negative)

=== EXAMPLE 4: ON MEMBER FUNCTIONS ===
size = 2, empty = false
after clear: empty = true

=== EXAMPLE 5: RAII GUARD ===
  timer 'work' started
  doing work
  timer 'work' stopped

=== EXAMPLE 6: ON A CONSTRUCTOR ===
  lock 1 acquired
  critical section
  lock 1 released

=== EXAMPLE 7: DISCARDING ON PURPOSE ===
  login attempt for 'guest'
  login attempt for 'admin'

=== EXAMPLE 8: STANDARD LIBRARY ===
empty() = false
value_or = 5
async result = 42
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`, and runs clean under AddressSanitizer and UBSan.
- The nine warnings listed above were produced by compiling a copy of `main.cpp` with the commented-out calls enabled.
- Not tested with Clang or MSVC. The attribute on constructors and with a message needs C++20 or newer.

## Reference

- Attribute: `[[nodiscard]]`, C++17; with message and on constructors, C++20.
- Standard library functions marked `[[nodiscard]]` include `std::vector::empty`, `std::optional::value_or`, `std::async` (headers `<vector>`, `<optional>`, `<future>`).
