# Chain of Responsibility — `01_ChainOfResponsibilityExample`

> **Intent:** Passes a request along a chain of handlers. Each handler decides to process it or to forward it to the next one.

## Scenario

Expenses go to the team lead, then to the director, then to the finance board. Each has a limit; larger amounts move up the chain, and amounts above the last limit are rejected.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Approver` | handler base class; `handle()` decides between approving and forwarding |
| `TeamLead` / `Director` / `FinanceBoard` | concrete handlers with different limits |
| `Expense` | the request |

## The pattern in short

- `setNext()` returns the next handler, so a chain can be built in one line.
- The client only talks to the first handler.

**Use it when:** More than one object may handle a request and the handler is not known in advance.

**Watch out:** A request can fall off the end of the chain; always decide what happens then (here: rejected).

## Output

```text
team lead approved 'team lunch' (80)

team lead passes 'new laptops' on
director approved 'new laptops' (900)

team lead passes 'conference trip' on
director passes 'conference trip' on
finance board approved 'conference trip' (4500)

team lead passes 'office renovation' on
director passes 'office renovation' on
'office renovation' (25000) was rejected: nobody may approve it
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_ChainOfResponsibilityExample
./01_ChainOfResponsibilityExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 12 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the chain of responsibility pattern.
