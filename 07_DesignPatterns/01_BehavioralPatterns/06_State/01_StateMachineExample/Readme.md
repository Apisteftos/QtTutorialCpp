# State — `01_StateExample`

> **Intent:** Lets an object change its behaviour when its internal state changes. Each state is its own class and decides which state comes next.

## Scenario

A media player is Stopped, Playing or Paused. The same button (`play`, `pause`, `stop`) does something different in each state.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `PlayerState` | state interface |
| `StoppedState` / `PlayingState` / `PausedState` | concrete states; they replace themselves via `MediaPlayer::setState()` |
| `MediaPlayer` | context; forwards every request to the current state |

## The pattern in short

- No `if (state == ...)` chains in the context.
- A state object replaces itself while one of its own functions runs, so nothing may touch its members after `setState()`.

**Use it when:** An object behaves differently depending on its state and has many state-dependent operations.

**Watch out:** Many small classes; for few states and events a transition table (see `02_StateMachineExample`) can be simpler.

## Output

```text
state: Stopped
  cannot pause: nothing is playing
  start playing
state: Playing
  already playing
  pause playback
state: Paused
  resume playing
  stop playback
state: Stopped
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_StateExample
./01_StateExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 10 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the state pattern.
