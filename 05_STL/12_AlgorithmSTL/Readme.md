# AlgorithmSTL — std::algorithm in C++23

The `<algorithm>` header provides over 100 generic algorithms that work on any
container via iterators. C++20 adds `<ranges>` for a cleaner pipeline syntax.

---

## Headers

```cpp
#include <algorithm>   // sort, find, transform, etc.
#include <numeric>     // accumulate, reduce, iota, etc.
#include <ranges>      // C++20 ranges and views
#include <functional>  // std::greater, std::multiplies
#include <iterator>    // std::back_inserter, std::distance
```

Algorithms work on **iterator pairs** `[first, last)` that must come from the **same** container. Passing `begin()` of one
temporary and `end()` of another is undefined behavior — the old `main.cpp` did exactly that with
`std::includes(..., std::vector<int>{2,3,4}.begin(), std::vector<int>{2,3,4}.end())`, and AddressSanitizer reported a
heap-buffer-overflow (tested). Give the second vector a name.

---

## Sorting

```cpp
std::sort(v.begin(), v.end());                          // O(n log n)
std::sort(v.begin(), v.end(), std::greater<int>());     // descending
std::sort(v.begin(), v.end(), [](const auto& a, const auto& b){ return a.score > b.score; });
// (a non-const `auto&` parameter also compiled with GCC 13, tested, but comparators should take const references)

std::stable_sort(v.begin(), v.end(), cmp); // preserves equal element order

std::partial_sort(v.begin(), v.begin()+K, v.end()); // only sort first K

std::nth_element(v.begin(), v.begin()+K, v.end()); // K-th element in place, average O(n)
```

`sort`, `partial_sort`, `nth_element` need **random-access iterators**: `std::sort(l.begin(), l.end())` and
`std::ranges::sort(l)` on a `std::list` are compile errors (tested) — use `l.sort()`.
After `partial_sort(p.begin(), p.begin()+3, p.end())` the elements behind the sorted prefix are in unspecified order
(`{5,3,8,1,9,2,7}` → `1 2 3 8 9 5 7` on GCC 13, tested).

---

## Searching

```cpp
std::find(v.begin(), v.end(), value)               // O(n) — returns iterator
std::find_if(v.begin(), v.end(), pred)             // first matching predicate
std::find_if_not(v.begin(), v.end(), pred)         // first NOT matching

std::binary_search(v.begin(), v.end(), value)      // O(log n) comparisons — sorted only, returns bool
std::lower_bound(v.begin(), v.end(), value)        // first >= value
std::upper_bound(v.begin(), v.end(), value)        // first > value

std::search(v.begin(), v.end(), sub.begin(), sub.end())   // first occurrence of a subsequence

std::all_of(v.begin(), v.end(), pred)              // all match predicate
std::any_of(v.begin(), v.end(), pred)              // at least one matches
std::none_of(v.begin(), v.end(), pred)             // none match
```

---

## Counting

```cpp
std::count(v.begin(), v.end(), value)              // count occurrences
std::count_if(v.begin(), v.end(), pred)            // count matching predicate
```

---

## Modifying

```cpp
// transform — apply function
std::vector<int> out(v.size());      // the output range must already have room!
std::transform(v.begin(), v.end(), out.begin(), [](int x){ return x*2; });
// With an empty `out`, out.begin() writes into nothing: crashed with AddressSanitizer (SEGV, tested).
// Alternative that grows the vector: std::back_inserter(out) (needs <iterator>), after out.reserve(v.size()).

// replace
std::replace(v.begin(), v.end(), old, newVal);
std::replace_if(v.begin(), v.end(), pred, newVal);

// fill / generate
std::fill(v.begin(), v.end(), 0);
std::generate(v.begin(), v.end(), [n=0]() mutable { return n++; });
std::iota(v.begin(), v.end(), 1);    // 1, 2, 3, 4, ...

// copy_if
std::copy_if(src.begin(), src.end(), std::back_inserter(dst), pred);
```

---

## Removing (erase-remove idiom)

```cpp
// Remove specific value
v.erase(std::remove(v.begin(), v.end(), 2), v.end());

// Remove matching predicate
v.erase(std::remove_if(v.begin(), v.end(), pred), v.end());

// Remove consecutive duplicates (sort first)
v.erase(std::unique(v.begin(), v.end()), v.end());
```

`std::remove` / `remove_if` / `unique` do **not** shrink the container: they move the kept elements to the front and
return the new logical end; the elements behind it are in an unspecified state. `erase` then really removes them.

Since C++20 there are one-liners for `std::vector` (and the other standard containers):

```cpp
std::erase(v, 2);                                   // returns the number of removed elements
std::erase_if(v, [](int x) { return x % 2 == 0; }); // (both tested: removed 3 evens, 1 value)
```

---

## Reordering

```cpp
std::reverse(v.begin(), v.end());
std::rotate(v.begin(), v.begin()+k, v.end());  // rotate left by k
std::shuffle(v.begin(), v.end(), rng);          // random shuffle
std::next_permutation(v.begin(), v.end());      // next lexicographic permutation
```

- `next_permutation` returns `false` when it wraps around to the sorted order. To visit **all** permutations,
  start from a **sorted** sequence: starting from `{3,1,2}` a `do … while` loop gives only 2 permutations, not 6 (tested).
- `shuffle` needs a random generator (`std::mt19937`). `std::random_shuffle` was removed from the standard in C++17;
  GCC 13 still compiles it but warns that it is deprecated (tested).

---

## Min / Max

```cpp
std::min(a, b)     // returns a REFERENCE to a or b
std::max(a, b)
std::min_element(v.begin(), v.end())    // iterator to minimum
std::max_element(v.begin(), v.end())    // iterator to maximum
std::minmax_element(v.begin(), v.end()) // pair of iterators

std::clamp(value, lo, hi)   // keep value within [lo, hi] (lo must not be greater than hi)
```

Because `min`/`max`/`clamp` return references, binding the result of a call with a temporary argument to a reference
dangles: `const int& m = std::min(a + 1, 10);` — AddressSanitizer reports `stack-use-after-scope` when `m` is used
(tested). Store the result in a plain `int`.

---

## Numeric

```cpp
std::accumulate(v.begin(), v.end(), 0)          // sum — the type of the init value decides the arithmetic type:
                                                 // for a vector<double> {1.5, 2.5} it returns 3 with `0` and 4 with `0.0` (tested)
std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>()) // product
std::reduce(v.begin(), v.end(), 0)              // C++17; order of operations unspecified, so the operation should be
                                                 // associative and commutative (floating-point sums can differ from accumulate).
                                                 // Parallel only with an execution policy, e.g. std::execution::par (not tested here)
std::inner_product(a.begin(), a.end(), b.begin(), 0)  // dot product
std::partial_sum(v.begin(), v.end(), out.begin())     // prefix sums
std::adjacent_difference(v.begin(), v.end(), out.begin())
```

---

## Set operations (on sorted ranges)

```cpp
// `out` is an output iterator, e.g. std::back_inserter(result) (a std::inserter(set, set.end()) works for sets)
std::set_union(a.begin(),a.end(), b.begin(),b.end(), std::back_inserter(result));
std::set_intersection(a.begin(),a.end(), b.begin(),b.end(), std::back_inserter(result));
std::set_difference(a.begin(),a.end(), b.begin(),b.end(), std::back_inserter(result));
std::set_symmetric_difference(a.begin(),a.end(), b.begin(),b.end(), std::back_inserter(result));
std::includes(a.begin(),a.end(), b.begin(),b.end());   // is b a subset of a? (both sorted, same container rules as above)
```

---

## C++20 Ranges — cleaner syntax

```cpp
// No begin/end needed
std::ranges::sort(v);
std::ranges::find(v, 5);
std::ranges::count_if(v, pred);

// Views — lazy pipelines
auto result = v
    | std::views::filter([](int x){ return x%2==0; })
    | std::views::transform([](int x){ return x*x; });

// take/drop
v | std::views::take(3)   // first 3
v | std::views::drop(5)   // skip first 5

// Projections — sort by field
std::ranges::sort(people, {}, &Person::age);
```

Views are **lazy** — no computation until you iterate.

Gotchas (tested with GCC 13.3):

- A `filter_view` (and anything built on it) **cannot be iterated through a `const` reference** — a function taking
  `const auto& r` and looping over it does not compile. Take the range by value or by forwarding reference
  (`Range&& r`), as the `printAll` helper in `main.cpp` does.
- An rvalue container piped into a view (`make() | std::views::filter(...)`) compiles: since the C++20 defect report P2415
  the view then *owns* the container.
- A view that refers to an lvalue container dangles if the container is destroyed or reallocated first.
- `std::ranges::to` (C++23, converts a view into a container) is not available in the GCC 13 library
  (`__cpp_lib_ranges_to_container` undefined), so it is not used here.

---

## Algorithm complexity summary

| Algorithm | Complexity | Notes |
|-----------|-----------|-------|
| `sort` | O(n log n) | libstdc++ uses introsort (an implementation detail) |
| `partial_sort` | O(n log k) | k = sorted prefix length |
| `nth_element` | O(n) average | |
| `stable_sort` | O(n log n) with enough extra memory, otherwise O(n log² n) | the standard allows both |
| `find` | O(n) | linear |
| `binary_search` | O(log n) comparisons | sorted only; on a `std::list` it compiles, but moving the iterators costs O(n) |
| `count` | O(n) | |
| `transform` | O(n) | |
| `accumulate` | O(n) | |
| `set_union` | O(m+n) | sorted inputs |

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` and the unused `<array>`/`<list>` are gone, and `<functional>`, `<iterator>` and `<cstddef>` were added. A `printAll` helper replaced the many `for (...) cout ...; cout << '\n';` one-liners; `(int)std::distance(...)` casts were dropped (`println` formats `ptrdiff_t` directly).
- **Real bug fixed (undefined behavior):** Example 9 called `std::includes(a.begin(), a.end(), std::vector<int>{2,3,4}.begin(), std::vector<int>{2,3,4}.end())` — `begin()` and `end()` of two *different* temporary vectors. AddressSanitizer reported a heap-buffer-overflow for the original (it only "worked" without the sanitizer). The second range is now a named vector `part`.
- The ranges example `printAll` uses a forwarding reference because a filter view can't be iterated through `const&` (see above).
- Output changes: `bool` results print as `true`/`false`; "rotate(2):" got the missing space; "sorted by age:" is now a heading with indented entries (the original printed the label and the first person on the same line); the `shuffle` line is random on every run.
- README: `stable_sort` complexity corrected (O(n log n) with enough extra memory, O(n log² n) otherwise); the `transform`/set-operation sketches now show where the output goes (`out` must have room / `std::back_inserter`).

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (compared with the original after fixing its `includes` bug; output identical apart from `true`/`false`, the space after `rotate(2):`, the "sorted by age" layout, the split "(only first 3 sorted)" line and the random shuffle); non-const `auto&` comparator; `std::sort`/`ranges::sort` on `std::list` rejected; `binary_search` on a list; `erase`/`erase_if`; `accumulate` init type; `next_permutation` from an unsorted start; `transform` into an empty vector; dangling `std::min`; filter view through `const&`; rvalue container into a view; `random_shuffle` deprecation; `ranges::to` availability.

## Not tested

Parallel execution policies (`std::execution::par` needs TBB with libstdc++), the O(…) complexities in the table beyond the cases above (they are standard guarantees), `std::reduce` vs `accumulate` differences for floating-point, other C++23 range additions (only the absence of `ranges::to` was checked), other standard libraries.
