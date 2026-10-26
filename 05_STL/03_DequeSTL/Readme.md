# DequeSTL — std::deque in C++23

`std::deque` (double-ended queue) supports **constant-time insertion and removal at both ends**.
Unlike `std::vector`, it does not store elements in contiguous memory.

---

## Header

```cpp
#include <deque>
```

---

## Declaration

```cpp
std::deque<int> empty;
std::deque<int> sized(5);          // 5 elements, value 0
std::deque<int> filled(5, 42);     // 5 elements, value 42
std::deque<int> list = {1,2,3,4,5};
```

---

## Push and pop at both ends

```cpp
dq.push_back(10);    // add to end   — O(1) (no element is ever moved)
dq.push_front(10);   // add to front — O(1) ← key advantage over vector (vector has no push_front at all)
dq.emplace_back(10); // construct at end in place
dq.emplace_front(10);// construct at front in place

dq.pop_back();       // remove last  — O(1)
dq.pop_front();      // remove first — O(1) ← key advantage over vector
```

---

## Element access

```cpp
dq[2]        // no bounds check — O(1)
dq.at(2)     // bounds checked — throws std::out_of_range
dq.front()   // first element
dq.back()    // last element
```

Note: `std::deque` has **no `data()`, no `reserve()` and no `capacity()`** —
memory is not contiguous. All three are compile errors (tested).
It does have `shrink_to_fit()`.

Elements are stored in several fixed-size blocks. On GCC 13 a `deque<int>` of
1000 elements is contiguous only inside a block: 7 jumps between 1000 elements (tested,
libstdc++-specific; the block size differs between implementations).

---

## Insert and erase in the middle

```cpp
dq.insert(dq.begin() + 2, 99);          // O(n)
dq.insert(dq.begin() + 1, {11,12,13});  // O(n)
dq.erase(dq.begin() + 3);               // O(n)
dq.erase(dq.begin(), dq.begin() + 2);   // O(n)
dq.clear();
```

Middle operations are O(n) — same as vector. Deque's advantage is at the ends.
(The standard actually says linear in the distance to the *nearer* end.)

### Iterator and reference validity

Unlike `std::vector`, `push_back`/`push_front` on a deque invalidate **iterators** but
keep **references and pointers to existing elements** valid (a reference to `q[1]` was still
valid after 20000 pushes, tested with ASan). `erase` at the front keeps references to the
other elements valid, and so does `insert` at the front (tested). An `insert`/`erase` in the
*middle* invalidates all references and iterators.

---

## Sliding window pattern

Deque is ideal for maintaining a sliding window over a stream:

```cpp
std::deque<int> window;
constexpr std::size_t WINDOW_SIZE = 4;

for (int val : stream) {
    window.push_back(val);           // new value enters from back
    if (window.size() > WINDOW_SIZE)
        window.pop_front();          // oldest value exits from front
    // window always contains last N values
}
```

---

## Task queue with two priorities

```cpp
std::deque<Task> taskQueue;

taskQueue.push_back({2, "normal task"});    // normal priority → back
taskQueue.push_front({1, "URGENT task"});   // high priority → front

while (!taskQueue.empty()) {
    Task task = std::move(taskQueue.front());   // move out, then remove
    taskQueue.pop_front();
    process(task);
}
```

This only works for **two** levels. Urgent tasks pushed to the front come out
in reverse order (last urgent first): pushing "u1" then "u2" gives `u2 u1 <normal…>` (tested).
For more levels or a stable order use `std::priority_queue` or one deque per priority.

---

## deque vs vector

| Operation | `vector` | `deque` |
|-----------|---------|---------|
| `push_back` | O(1) amortized | O(1) |
| `push_front` | not available (`insert(begin(), x)` is O(n)) | **O(1)** |
| `pop_front` | not available (`erase(begin())` is O(n)) | **O(1)** |
| `operator[]` | O(1) | O(1) |
| Insert middle | O(n) | O(n) |
| Contiguous memory | ✅ | ❌ |
| `data()` raw pointer | ✅ | ❌ |
| `reserve()` | ✅ | ❌ |
| Cache friendliness | Better | Worse (contiguous only inside a block) |

---

## When to use deque

✅ Need fast insertion/removal at **both** front and back
✅ Sliding window algorithms
✅ Two-level queues where urgent items jump to front (not a real priority queue)
✅ BFS (breadth-first search) queue
✅ History buffers with a size limit (add to back, drop the oldest from front)

❌ Need raw pointer to data (`data()`) → use `vector`
❌ Cache-critical inner loop iteration → use `vector`
❌ Only adding/removing at back → use `vector`
❌ Fixed size → use `std::array`

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone, needed includes were added. The `copy` deque from Example 1 is now printed (it was unused).
- The comparison table printed at the end of Example 10 is now `println` text; its first rows were corrected like the README table (below).
- Corrected claims:
  - Deque `push_back`/`push_front` are plain **O(1)** in the standard, not "O(1) amortized" (vector's `push_back` is the amortized one).
  - `vector` does not have a slow `push_front`/`pop_front`, it has none; the O(n) refers to `insert(begin(), x)`/`erase(begin())`.
  - "Priority queue" was an overstatement: two levels only, and urgent items come out in reverse order.
  - "Undo/redo history" fits only a bounded history (drop from front); real undo needs removal at the back too.
- Sliding-window sketch uses `std::size_t` so `window.size() > WINDOW_SIZE` has no signed/unsigned comparison; the code uses index type `std::size_t` too (no `(int)` casts).
- `Task` is moved out of the queue instead of copied.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the new `copy:` line, the corrected O(1) table rows and one extra blank line at the top from the banner helper); `data`/`reserve`/`capacity` rejected by the compiler; `vector::push_front` rejected; 7 block jumps for 1000 ints; reference stability on push at both ends, erase at front and insert at front; reverse order of urgent tasks.

## Not tested

Performance claims ("Medium"/"Worse" cache friendliness — no measurement), the block size on other standard libraries (MSVC uses much smaller blocks), invalidation of references on middle insert/erase (taken from the standard, not run since it is UB), the O(n) / "nearer end" wording of middle operations.
