# Builder — `01_BuilderExample`

> **Intent:** Constructs a complex object step by step. The same steps can be reused by a director to make ready-made variants.

## Scenario

E-mails are built from recipient, subject, body and attachments. A director creates a welcome e-mail and a reminder with fixed steps.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Email` | the product |
| `EmailBuilder` | builder with chainable setters and `build()` |
| `EmailDirector` | director; knows which steps make a certain kind of e-mail |

## The pattern in short

- Setters return `*this`, so calls can be chained.
- `build()` moves the finished product out and starts a fresh one.

**Use it when:** An object has many optional parts or a construction that takes several steps.

**Watch out:** A builder is extra code for simple objects; aggregate initialisation or designated initialisers can be enough.

## Output

```text
To: sam@example.com
Subject: Invoice
Body: Please find the invoice attached.
Attachments: 2
  - invoice_2026_10.pdf
  - terms.pdf

To: robin@example.com
Subject: Welcome, Robin
Body: Your account is ready.
Attachments: 1
  - getting_started.pdf

To: robin@example.com
Subject: Reminder
Body: Your appointment is tomorrow.
Attachments: 0
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_BuilderExample
./01_BuilderExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 17 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the builder pattern.
