# Factory Method — `01_FactoryMethodExample`

> **Intent:** The base class defines the workflow and calls a virtual "factory method" to get the object it needs; subclasses decide which class is created.

## Scenario

A notifier sends a message through a channel. E-mail, SMS and push notifiers create different channel objects.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Channel` | product interface |
| `EmailChannel` / `SmsChannel` / `PushChannel` | concrete products |
| `Notifier` | creator; `notify()` calls the factory method `createChannel()` |
| `EmailNotifier` / `SmsNotifier` / `PushNotifier` | concrete creators |

## The pattern in short

- `notify()` works with the `Channel` interface only.
- The factory method returns `std::unique_ptr`, so ownership is clear.

**Use it when:** A class cannot know in advance which objects it has to create, or wants its subclasses to choose.

**Watch out:** One new product usually means one new creator class; a simple function or a lookup table can be enough.

## Output

```text
sending via email
  [email] subject line: Your parcel arrives tomorrow morning
sending via sms
  [sms] Your parcel arrives 
sending via push
  [push] Your parcel arrives tomorrow morning
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_FactoryMethodExample
./01_FactoryMethodExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 6 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the factory method pattern.
