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
std::list<int> list = {1,2,3,4,5};
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
// l[2]     // ERROR — does not compile
// l.at(2)  // ERROR — does not compile

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

l.insert(it, 99);      // insert before it — O(1)
l.erase(it);           // erase at it — O(1)
l.erase(first, last);  // erase range — O(range size)
l.clear();
```

This is list's key advantage — no element shifting needed.
With vector, insert/erase in the middle is O(n) due to shifting.

---

## List-specific member functions

### sort
```cpp
l.sort();                          // ascending — O(n log n)
l.sort(std::greater<int>());       // descending
l.sort([](const T& a, const T& b) { return a.x < b.x; });
```
Note: `std::sort` does **not** work on `std::list` (requires random access iterators).
Use the member `.sort()` instead.

### reverse
```cpp
l.reverse();   // O(n) — reverses in place
```

### unique
```cpp
l.sort();      // must sort first
l.unique();    // removes consecutive duplicates — O(n)
l.unique([](int a, int b) { return a == b; });   // custom predicate
```

### remove / remove_if
```cpp
l.remove(2);                                // remove ALL occurrences of 2 — O(n)
l.remove_if([](int x) { return x > 5; });  // remove all matching — O(n)
```

### merge
```cpp
list1.merge(list2);                         // merge two sorted lists — O(n)
list1.merge(list2, std::greater<int>());    // descending
// list2 becomes empty after merge
```

### splice — O(1) move of elements between lists
```cpp
dest.splice(pos, src);             // move ALL of src into dest at pos
dest.splice(pos, src, srcIt);      // move ONE element from src
dest.splice(pos, src, first, last);// move range from src
```

Splice is unique to `std::list` — no copying, just pointer relinking.

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
| `splice` | ❌ | ❌ | **O(1)** |
| `merge` sorted | ❌ | ❌ | O(n) |
| Memory layout | Contiguous | Chunks | Scattered |
| Cache performance | Best | Medium | Worst |

---

## When to use list

✅ Frequent insertions/deletions in the **middle** of the sequence
✅ Elements must not be invalidated by inserts/erases elsewhere
✅ Need `splice` or `merge` operations
✅ Iterator stability — inserting never invalidates other iterators

❌ Need random access by index → use `vector`
❌ Cache-critical iteration → use `vector`
❌ Only adding/removing at ends → use `deque` or `vector`
❌ Small dataset — linked list overhead (pointer per node) hurts
