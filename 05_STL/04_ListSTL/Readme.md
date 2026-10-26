# ListSTL — std::list in C++23

`std::list` is a **doubly linked list** — each element has pointers to the
previous and next element. Insertions and deletions anywhere are O(1) given
an iterator, but there is **no random access**.

---

## Header

```cpp
#include <list>
```

---

## Declaration

```cpp
std::list<int> empty;
std::list<int> sized(5);           // 5 elements, value 0
std::list<int> filled(5, 99);      // 5 elements, value 99
std::list<int> fromList = {1,2,3,4,5};
```

---

## Push and pop at both ends

```cpp
l.push_back(10);    // O(1)
l.push_front(10);   // O(1)
l.emplace_back(10); // construct in place at back
l.emplace_front(10);// construct in place at front

l.pop_back();       // O(1)
l.pop_front();      // O(1)
```

---

## Element access — front and back ONLY

```cpp
l.front()   // first element — O(1)
l.back()    // last element  — O(1)

// NO operator[] or at() — list has no random access!
// l[2]     // ERROR — does not compile (tested)
// l.at(2)  // ERROR — does not compile (tested)

// To reach middle: use std::advance (O(n))
auto it = l.begin();
std::advance(it, 2);   // move 2 steps
int val = *it;
```

---

## Insert and erase — O(1) with iterator

```cpp
auto it = l.begin();
std::advance(it, 2);   // find position

l.insert(it, 99);      // insert before it — O(1); `it` stays valid and still points to the old element
it = l.erase(it);      // erase at it — O(1); returns the iterator to the next element
l.erase(first, last);  // erase range — O(range size)
l.clear();
```

This is list's key advantage — no element shifting needed.
With vector, insert/erase in the middle is O(n) due to shifting.

**But:** finding the position (`std::advance`, `std::find`) is itself O(n). A list only
wins if you *already hold* the iterator, or insert/erase while you are traversing anyway.
Otherwise `vector` is usually faster even for middle inserts, because of cache behavior (see below).

---

## List-specific member functions

### sort
```cpp
l.sort();                          // ascending — O(n log n)
l.sort(std::greater<int>());       // descending
l.sort([](const T& a, const T& b) { return a.x < b.x; });
```
Note: `std::sort` and `std::ranges::sort` do **not** work on `std::list` (they need random
access iterators; compile error, tested). Use the member `.sort()` instead. It is stable.

### reverse
```cpp
l.reverse();   // O(n) — reverses in place
```

### unique
```cpp
l.sort();      // sort first, otherwise only *neighbouring* duplicates go
l.unique();    // removes consecutive duplicates — O(n)  ({1,2,1,1,3,1} → {1,2,1,3,1}, tested)
l.unique([](int a, int b) { return a == b; });   // custom predicate
```

### remove / remove_if
```cpp
l.remove(2);                                // remove ALL occurrences of 2 — O(n); returns the count since C++20
l.remove_if([](int x) { return x > 5; });  // remove all matching — O(n)
```

### merge
```cpp
list1.merge(list2);                         // merge two sorted lists — O(n)
list1.merge(list2, std::greater<int>());    // descending
// list2 becomes empty after merge
```

### splice — move of elements between lists (O(1) for the whole list or one element)
```cpp
dest.splice(pos, src);             // move ALL of src into dest at pos
dest.splice(pos, src, srcIt);      // move ONE element from src
dest.splice(pos, src, first, last);// move range from src
```

Splice is unique to `std::list` — no copying, just pointer relinking. Iterators and references to the
moved elements stay valid, but now belong to the destination list (tested).
Moving a *range* `first, last` between two different lists is O(distance) in libstdc++ and the standard
(it has to update the sizes), so the "O(1)" in the table applies to whole-list and single-element splice only.

---

## Iterators

```cpp
for (const auto& x : l) { }           // range-based
for (auto it = l.begin(); it != l.end(); ++it) { }
for (auto it = l.rbegin(); it != l.rend(); ++it) { }  // reverse
```

---

## List vs vector vs deque

| Operation | `vector` | `deque` | `list` |
|-----------|---------|---------|--------|
| `push_back` | O(1) | O(1) | O(1) |
| `push_front` | O(n) | O(1) | O(1) |
| `insert` with iterator | O(n) | O(n) | **O(1)** |
| `erase` with iterator | O(n) | O(n) | **O(1)** |
| Random access `[]` | O(1) | O(1) | ❌ none |
| `splice` | ❌ | ❌ | **O(1)** (whole list / one element) |
| `merge` sorted | ❌ | ❌ | O(n) |
| Memory layout | Contiguous | Chunks | Scattered |
| Cache performance | Best | Medium | Worst (measured below) |

---

## When to use list

✅ Frequent insertions/deletions in the **middle** of the sequence *through iterators you already hold*
✅ Pointers/references/iterators to elements must stay valid when other elements are inserted or erased (only the erased element's own iterator dies)
✅ Need `splice` or `merge` operations

❌ Need random access by index → use `vector`
❌ Cache-critical iteration → use `vector`
❌ Only adding/removing at ends → use `deque` or `vector`
❌ Small dataset or small element type — every node is a separate allocation with two pointers: a `list<int>` node is 24 bytes of payload+links per 4-byte `int` (tested)

---

## Cache performance — one measurement

GCC 13.3, `-O2`, 2,000,000 `int`s, in this sandbox (noisy, not a benchmark suite):

- summing 20 times with `std::accumulate`: `vector` ≈ 14 ms, `list` ≈ 203 ms (nodes allocated one after another, so this is the *best case* for a list; ~14×)
- sorting 2,000,000 shuffled `int`s: `std::sort` on a vector ≈ 185 ms, `list::sort` ≈ 1060 ms (~6×)

Your numbers will differ; the direction (list is much slower for traversal and sorting) is the point.

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>`/`<numeric>` are gone, needed includes added. The unused `copy` list from Example 1 is now printed.
- The "Topics covered" header listed an "Iterators" example that did not exist and put merge under #5; it now matches the real examples (5 sort/reverse/unique, 6 merge, 7 splice, 8 remove, 9 objects, 10 comparison).
- Lambda parameters and loop variables (`a`, `b`, `e`) no longer shadow other variables (`-Wshadow` clean).
- Corrected claims:
  - Insert/erase "O(1)" needs an iterator you already have; locating the position is O(n).
  - Splice of a *range* between two lists is not O(1).
  - "Iterator stability — inserting never invalidates" now says what is true: erasing invalidates only the erased element's iterator.
  - The README declaration used the variable name `list`; renamed to `fromList`.
  - `unique` without sorting only removes neighbours (shown with an example).

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the new `copy:` line and one extra blank line at the top from the banner helper); `l[2]`, `l.at(2)`, `std::sort`, `std::ranges::sort` rejected; iterator/pointer stability after insert and erase elsewhere; splice keeps iterators valid in the new list; `remove` return value; `unique` on unsorted data; node allocation size (24 bytes for `int` on libstdc++); the two timing measurements above.

## Not tested

Range splice complexity (taken from the standard/libstdc++ implementation, not measured), complexity of `merge` and `sort` (standard guarantees), the "Medium" cache rating of `deque` in the table, behavior on other standard libraries or with randomly scattered nodes (a fragmented heap makes lists slower still; my list was built with consecutive allocations).
