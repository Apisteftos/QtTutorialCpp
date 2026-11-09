# chrono calendar

Dates, weekdays and durations in `<chrono>`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `year_month_day, weekday, sys_days, last` | `<chrono>` | C++20 |
| `duration_cast, floor, hh_mm_ss, std::format with chrono types` | `<chrono>, <format>` | C++20 |

## What the example shows

- Example 1: calendar dates
- Example 2: day arithmetic through sys_days
- Example 3: durations and format
- Example 4: clocks

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/05_chrono_calendar
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 05_chrono_calendar
./05_chrono_calendar
```

## Output (GCC 15.2.0)

```text
=== Example 1: year_month_day ===
date: 2024-02-28, valid: 1
leap year: 1
2023-02-29 valid: 0

=== Example 2: arithmetic ===
28 Feb 2024 + 2 days = 2024-03-01
weekday: Wed
days until 25 Dec: 301
last day of Feb 2024: 2024-02-29

=== Example 3: durations ===
total seconds: 9015
as minutes (floor): 150
formatted: 9015s
date formatted: 28.02.2024
hh:mm:ss: 02:30:15

=== Example 4: clocks ===
steady_clock is monotonic: true
system_clock is after 2020: true
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- Time zones (`std::chrono::zoned_time`) are not part of this example because they need the system time zone database.
- A namespace alias (`namespace chrono = std::chrono;`) keeps the lines short without a using directive.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `year_month_day, weekday, sys_days, last` | `<chrono>` | C++20 |
| `duration_cast, floor, hh_mm_ss, std::format with chrono types` | `<chrono>, <format>` | C++20 |
