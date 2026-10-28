# Adapter — `01_Adapter`

> **Intent:** Converts the interface of an existing class into the interface the client expects.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: MEDIA PLAYER ADAPTER
- EXAMPLE 2: SHAPE ADAPTER (GEOMETRIC)
- EXAMPLE 3: TEMPERATURE SENSOR ADAPTER
- EXAMPLE 4: PAYMENT GATEWAY ADAPTER
- EXAMPLE 5: DATABASE ADAPTER
- CLIENT CODE & DEMONSTRATIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `MediaPlayer` | — |
| `MP4Player` | — |
| `VLCPlayer` | — |
| `MediaAdapter` | `MediaPlayer` |
| `AudioPlayer` | `MediaPlayer` |
| `LegacyRectangle` | — |
| `Shape` | — |
| `Circle` | `Shape` |
| `RectangleAdapter` | `Shape` |
| `FahrenheitSensor` | — |
| `TemperatureSensor` | — |
| `TemperatureAdapter` | `TemperatureSensor` |
| `PayPalAPI` | — |
| `StripeAPI` | — |
| `PaymentProcessor` | — |
| `PayPalAdapter` | `PaymentProcessor` |
| `StripeAdapter` | `PaymentProcessor` |
| `MySQLDatabase` | — |
| `MongoDB` | — |
| `Database` | — |
| `MySQLAdapter` | `Database` |
| `MongoDBAdapter` | `Database` |

## The pattern in short

- A target interface.
- An adaptee with an incompatible interface.
- An adapter that implements the target and forwards to the adaptee.

**Use it when:** You want to reuse a class whose interface does not match what your code needs.

**Watch out:** Adds a layer of indirection; keep adapters thin.

## Sample output (first lines)

```text
========================================
ADAPTER PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: MEDIA PLAYER           ║
╚════════════════════════════════════╝

🎵 Playing MP3 file: song.mp3
🎬 Playing MP4 file: video.mp4
🎥 Playing VLC file: movie.vlc
❌ Invalid media type: avi

╔════════════════════════════════════╗
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_Adapter
./01_Adapter
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 1 warning(s) (-Wunused-parameter). They come from the original code and were left as they were.
- Runs to completion (exit code 0) and prints 103 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored)..

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### BENEFITS OF ADAPTER PATTERN

- ✅ Single Responsibility: Separates interface conversion
- ✅ Open/Closed: Add new adapters without changing existing code
- ✅ Works with legacy code without modification
- ✅ Integrates third-party libraries easily
- ✅ Makes incompatible interfaces work together
- ✅ Promotes code reusability

### WHEN TO USE ADAPTER

1. Use existing class with incompatible interface
2. Create reusable class for unrelated classes
3. Work with legacy code you can't modify
4. Integrate third-party libraries
5. Need uniform interface for similar classes

### OBJECT ADAPTER vs CLASS ADAPTER

**Object Adapter (Composition):**

- Uses composition
- More flexible (can adapt multiple classes)
- Preferred in C++ (no multiple inheritance issues)
- Example: All examples above

**Class Adapter (Inheritance):**

- Uses multiple inheritance
- Less flexible
- Can override adaptee behavior
- Not commonly used in C++

### REAL-WORLD EXAMPLES (closing summary)

- Power plug adapters (US to EU)
- Card readers (SD card to USB)
- Media players (different formats)
- Database drivers (ODBC, JDBC)
- Payment gateways (PayPal, Stripe)
- Legacy system integration
- Third-party API wrappers
- Temperature converters
