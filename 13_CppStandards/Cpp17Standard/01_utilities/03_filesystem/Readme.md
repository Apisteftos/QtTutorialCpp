# std::filesystem

Paths, files, directories, iteration, error codes.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::filesystem::path` | `<filesystem>` | C++17 |
| `exists, file_size, create_directories, copy_file, remove_all` | `<filesystem>` | C++17 |
| `recursive_directory_iterator` | `<filesystem>` | C++17 |
| overloads with std::error_code | <filesystem> | C++17 |

## What the example shows

- Example 1: path manipulation
- Example 2: create, write, inspect
- Example 3: iterate
- Example 4: errors without exceptions, copy and remove

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_filesystem
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 03_filesystem
./03_filesystem
```

## Output (GCC 15.2.0)

```text
=== Example 1: path ===
filename:    summary.final.txt
stem:        summary.final
extension:   .txt
parent:      /data/reports
replaced:    /data/reports/summary.final.csv
joined:      /data/reports/x.txt

=== Example 2: files and directories ===
a.txt exists: true
a.txt size:   5 bytes
sub is dir:   true

=== Example 3: recursive iteration ===
a.txt
sub/b.txt

=== Example 4: error_code, copy, remove ===
missing file: error code set: true
copy exists: true
removed 5 entries
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- The example works only inside a temporary directory that it creates and removes again.
- The order of directory iteration is not specified, so the example sorts the names.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::filesystem::path` | `<filesystem>` | C++17 |
| `exists, file_size, create_directories, copy_file, remove_all` | `<filesystem>` | C++17 |
| `recursive_directory_iterator` | `<filesystem>` | C++17 |
| overloads with std::error_code | <filesystem> | C++17 |
