# Command — `01_CommandExample`

> **Intent:** Turns a request into an object, so it can be stored, passed around and executed later by code that does not know the receiver.

## Scenario

A remote-control button runs a command on a speaker. A simple command calls one function; a complex command calls several functions with data given at creation time.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Command` | interface with `execute()` |
| `MuteCommand` | simple command: one call on the receiver |
| `PartyModeCommand` | complex command: several calls, with volume and track list stored inside |
| `Speaker` | receiver; does the real work |
| `RemoteButton` | invoker; only knows the `Command` interface |

## The pattern in short

- The invoker never calls the receiver directly.
- Because commands are objects, they can be put in a queue or a history (see the other Command examples for undo and macros).

**Use it when:** You want to queue, log, schedule or undo operations, or to decouple the sender of a request from its receiver.

**Watch out:** Every action needs its own class; keep commands small and let the receiver contain the logic.

## Output

```text
button: nothing assigned
button: pressed
  speaker: muted
button: pressed
  speaker: volume 8
  speaker: queued 'Intro'
  speaker: queued 'Dance floor'
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_CommandExample
./01_CommandExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 7 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the command pattern.
