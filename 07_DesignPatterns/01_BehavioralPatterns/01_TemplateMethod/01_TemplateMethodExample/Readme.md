# Template Method — `01_TemplateMethodExample`

> **Intent:** Defines the skeleton of an algorithm in a base class and lets subclasses override single steps without changing the order of the steps.

## Scenario

A report generator writes a header, one line per row and an optional footer. CSV, HTML and plain-text reports differ only in the single steps.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `ReportGenerator` | abstract class; `generate()` is the template method, `wantsFooter()` and `writeFooter()` are hooks |
| `CsvReport` / `HtmlReport` / `PlainReport` | concrete reports that fill in the steps |

## The pattern in short

- The base class owns the algorithm (`generate()`); the order of the steps cannot be changed by subclasses.
- Pure virtual functions are steps every subclass must provide; virtual functions with a default are hooks.

**Use it when:** Several classes share the same algorithm and differ only in some steps.

**Watch out:** Too many hooks make the base class hard to understand; prefer composition (Strategy) if the steps must change at run time.

## Output

```text
--- CSV report ---
number;text
1;apples
2;bread
3;coffee
rows: 3

--- HTML report ---
<ol>
  <li>apples</li>
  <li>bread</li>
  <li>coffee</li>
</ol>

--- plain text report ---
1. apples
2. bread
3. coffee
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_TemplateMethodExample
./01_TemplateMethodExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 19 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the template method pattern.
