# Chain of Responsibility — `02_ChainOfResponsibilityExample`

> **Intent:** Passes a request along a chain of handlers; each handler either handles it or forwards it.

## What this example contains

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `SupportHandler` | — |
| `Level1Support` | `SupportHandler` |
| `Level2Support` | `SupportHandler` |
| `ManagerSupport` | `SupportHandler` |

## The pattern in short

- A handler interface with a link to the next handler.
- Concrete handlers that decide whether to handle or pass on.

**Use it when:** More than one object may handle a request and the handler is not known in advance.

**Watch out:** A request can fall off the end of the chain unhandled, so decide what should happen then.

## Sample output (first lines)

```text
=== Customer Support System ===

Request 1 (Priority 1):
Level 1 Support: Handling issue - Password reset

Request 2 (Priority 2):
Level 1 Support: Escalating to next level...
Level 2 Support: Handling issue - Software installation issue

Request 3 (Priority 3):
Level 1 Support: Escalating to next level...
Level 2 Support: Escalating to next level...
Manager Support: Handling critical issue - System outage

```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_ChainOfResponsibilityExample
./02_ChainOfResponsibilityExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 18 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- Output of the converted program is byte-identical to the original `std::cout` version.

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

