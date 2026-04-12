# DequeSTL — std::deque in C++23

`std::deque` (double-ended queue) supports **O(1) insertion and removal at both ends**.
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
dq.push_back(10);    // add to end   — O(1)
dq.push_front(10);   // add to front — O(1) ← key advantage over vector
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

Note: `std::deque` has **no `data()` method** and **no `reserve()`** —
memory is not contiguous.

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

---

## Sliding window pattern

Deque is ideal for maintaining a sliding window over a stream:

```cpp
std::deque<int> window;
const int WINDOW_SIZE = 4;

for (int val : stream) {
    window.push_back(val);           // new value enters from back
    if (window.size() > WINDOW_SIZE)
        window.pop_front();          // oldest value exits from front
    // window always contains last N values
}
```

---

## Task queue with priority

```cpp
std::deque<Task> queue;

queue.push_back({2, "normal task"});    // normal priority → back
queue.push_front({1, "URGENT task"});   // high priority → front

while (!queue.empty()) {
    auto task = queue.front();
    queue.pop_front();
    process(task);
}
```

---

## deque vs vector

| Operation | `vector` | `deque` |
|-----------|---------|---------|
| `push_back` | O(1) amortized | O(1) amortized |
| `push_front` | O(n) | **O(1) amortized** |
| `pop_front` | O(n) | **O(1)** |
| `operator[]` | O(1) | O(1) |
| Insert middle | O(n) | O(n) |
| Contiguous memory | ✅ | ❌ |
| `data()` raw pointer | ✅ | ❌ |
| `reserve()` | ✅ | ❌ |
| Cache friendliness | Better | Worse |

---

## When to use deque

✅ Need fast insertion/removal at **both** front and back
✅ Sliding window algorithms
✅ Priority queues where urgent items jump to front
✅ BFS (breadth-first search) queue
✅ Undo/redo history (add to back, remove from front)

❌ Need raw pointer to data (`data()`) → use `vector`
❌ Cache-critical inner loop iteration → use `vector`
❌ Only adding/removing at back → use `vector`
❌ Fixed size → use `std::array`
