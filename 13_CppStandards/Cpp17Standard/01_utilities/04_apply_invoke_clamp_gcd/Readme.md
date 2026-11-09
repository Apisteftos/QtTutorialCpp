# apply, invoke, not_fn, clamp, gcd, lcm, size/data/empty, byte

Small utilities added in C++17.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::apply, std::make_from_tuple` | `<tuple>` | C++17 |
| `std::invoke, std::not_fn` | `<functional>` | C++17 |
| `std::clamp` | `<algorithm>` | C++17 |
| `std::gcd, std::lcm` | `<numeric>` | C++17 |
| `std::size, std::data, std::empty` | `<iterator>` | C++17 |
| `std::as_const` | `<utility>` | C++17 |
| `std::byte` | `<cstddef>` | C++17 |

## What the example shows

- Example 1: apply and make_from_tuple
- Example 2: invoke and not_fn
- Example 3: clamp, gcd, lcm
- Example 4: size, data, empty, as_const, byte

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/04_apply_invoke_clamp_gcd
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 04_apply_invoke_clamp_gcd
./04_apply_invoke_clamp_gcd
```

## Output (GCC 15.2.0)

```text
=== Example 1: apply, make_from_tuple ===
apply(add, {3,4}) = 7
point = 3, 4

=== Example 2: invoke, not_fn ===
invoke(function):      3
invoke(member function): 17
invoke(data member):     3
odd numbers: 3

=== Example 3: clamp, gcd, lcm ===
clamp(15, 0, 10) = 10
clamp(-3, 0, 10) = 0
gcd(12, 18) = 6, lcm(4, 6) = 12

=== Example 4: size, data, empty, as_const, byte ===
size(array) = 5, empty(vector) = 0
first via data(): 1
as_const: abc
byte: 240
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- `std::invoke` calls function pointers, member functions and data members with one syntax.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::apply, std::make_from_tuple` | `<tuple>` | C++17 |
| `std::invoke, std::not_fn` | `<functional>` | C++17 |
| `std::clamp` | `<algorithm>` | C++17 |
| `std::gcd, std::lcm` | `<numeric>` | C++17 |
| `std::size, std::data, std::empty` | `<iterator>` | C++17 |
| `std::as_const` | `<utility>` | C++17 |
| `std::byte` | `<cstddef>` | C++17 |
