# AlgorithmSTL — std::algorithm in C++23

The `<algorithm>` header provides over 100 generic algorithms that work on any
container via iterators. C++20 adds `<ranges>` for a cleaner pipeline syntax.

---

## Headers

```cpp
#include <algorithm>   // sort, find, transform, etc.
#include <numeric>     // accumulate, reduce, iota, etc.
#include <ranges>      // C++20 ranges and views
```

---

## Sorting

```cpp
std::sort(v.begin(), v.end());                          // O(n log n)
std::sort(v.begin(), v.end(), std::greater<int>());     // descending
std::sort(v.begin(), v.end(), [](auto& a, auto& b){ return a.score > b.score; });

std::stable_sort(v.begin(), v.end(), cmp); // preserves equal element order

std::partial_sort(v.begin(), v.begin()+K, v.end()); // only sort first K

std::nth_element(v.begin(), v.begin()+K, v.end()); // K-th element in place
```

---

## Searching

```cpp
std::find(v.begin(), v.end(), value)               // O(n) — returns iterator
std::find_if(v.begin(), v.end(), pred)             // first matching predicate
std::find_if_not(v.begin(), v.end(), pred)         // first NOT matching

std::binary_search(v.begin(), v.end(), value)      // O(log n) — sorted only
std::lower_bound(v.begin(), v.end(), value)        // first >= value
std::upper_bound(v.begin(), v.end(), value)        // first > value

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
std::transform(v.begin(), v.end(), out.begin(), [](int x){ return x*2; });

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

---

## Reordering

```cpp
std::reverse(v.begin(), v.end());
std::rotate(v.begin(), v.begin()+k, v.end());  // rotate left by k
std::shuffle(v.begin(), v.end(), rng);          // random shuffle
std::next_permutation(v.begin(), v.end());      // next lexicographic permutation
```

---

## Min / Max

```cpp
std::min(a, b)
std::max(a, b)
std::min_element(v.begin(), v.end())    // iterator to minimum
std::max_element(v.begin(), v.end())    // iterator to maximum
std::minmax_element(v.begin(), v.end()) // pair of iterators

std::clamp(value, lo, hi)   // keep value within [lo, hi]
```

---

## Numeric

```cpp
std::accumulate(v.begin(), v.end(), 0)          // sum
std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>()) // product
std::reduce(v.begin(), v.end(), 0)              // C++17, parallelizable
std::inner_product(a.begin(), a.end(), b.begin(), 0)  // dot product
std::partial_sum(v.begin(), v.end(), out.begin())     // prefix sums
std::adjacent_difference(v.begin(), v.end(), out.begin())
```

---

## Set operations (on sorted ranges)

```cpp
std::set_union(a.begin(),a.end(), b.begin(),b.end(), out);
std::set_intersection(a.begin(),a.end(), b.begin(),b.end(), out);
std::set_difference(a.begin(),a.end(), b.begin(),b.end(), out);
std::set_symmetric_difference(a.begin(),a.end(), b.begin(),b.end(), out);
std::includes(a.begin(),a.end(), b.begin(),b.end());
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

---

## Algorithm complexity summary

| Algorithm | Complexity | Notes |
|-----------|-----------|-------|
| `sort` | O(n log n) | introsort |
| `stable_sort` | O(n log² n) | |
| `find` | O(n) | linear |
| `binary_search` | O(log n) | sorted only |
| `count` | O(n) | |
| `transform` | O(n) | |
| `accumulate` | O(n) | |
| `set_union` | O(m+n) | sorted inputs |
