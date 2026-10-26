# PriorityQueueSTL — std::priority_queue in C++23

`std::priority_queue` is a **max-heap** by default — the largest element is always at the top.
Push in any order, always pop in sorted (highest first) order.
Internally it is a **binary heap** (a complete binary tree stored in a `std::vector`):
`push` and `pop` are O(log n), `top` is O(1).

---

## Header

```cpp
#include <queue>
```

---

## Declaration

```cpp
// Max-heap (default)
std::priority_queue<int> maxPq;

// Min-heap
std::priority_queue<int, std::vector<int>, std::greater<int>> minPq;

// From range (builds the heap in O(n), like std::make_heap)
std::vector<int> v = {3, 1, 4, 1, 5};
std::priority_queue<int> pq(v.begin(), v.end());
```

---

## Core operations

```cpp
pq.push(10);    // add element — O(log n)
pq.emplace(10); // construct in place — O(log n)
pq.top()        // access top (max/min) — O(1), returns a CONST reference
pq.pop()        // remove top — O(log n)
pq.empty()      // true if empty
pq.size()       // element count
pq.swap(other)  // swap two queues of the same type
```

`top()`/`pop()` on an empty queue is undefined behavior (aborts with `-D_GLIBCXX_ASSERTIONS`, tested).
Because `top()` is `const`, you cannot move the element out of the queue; `std::move(pq.top())` copies (tested: the source string stays intact).

**`pop()` does not return a value** — use `top()` first:
```cpp
int val = pq.top();   // peek
pq.pop();             // remove
```

---

## Max-heap vs Min-heap

```cpp
// Max-heap — largest on top
std::priority_queue<int> maxPq;
maxPq.push(30); maxPq.push(10); maxPq.push(50);
maxPq.top()   // 50

// Min-heap — smallest on top
std::priority_queue<int, std::vector<int>, std::greater<int>> minPq;
minPq.push(30); minPq.push(10); minPq.push(50);
minPq.top()   // 10
```

---

## Custom comparator

The comparator works like for `std::sort`, but the result is *reversed* in effect:
`comp(a, b) == true` means "**a has lower priority than b**" — `b` comes out first.
That is why the default `std::less` gives a max-heap.

```cpp
// Sort by string length (longest first)
auto cmp = [](const std::string& a, const std::string& b) {
    return a.size() < b.size();   // a is "less" than b -> b (longer) comes out first
};

std::priority_queue<std::string,
                    std::vector<std::string>,
                    decltype(cmp)> pq(cmp);
```

Or use `operator<` in the struct (the default `std::less<Task>` calls it):
```cpp
struct Task {
    int priority;
    std::string name;
    bool operator<(const Task& o) const {
        return priority > o.priority;  // "less" = has the HIGHER number -> lower number comes out first
    }
};

std::priority_queue<Task> taskPq;   // uses operator<
```

A struct that only has `operator>` does **not** work with the default `std::priority_queue<Task>`
(compile error: no match for `operator<`, tested) — pass `std::greater<Task>` as in the scheduler below.

---

## Task scheduler pattern

```cpp
struct Task {
    int priority;
    std::string name;
    bool operator>(const Task& o) const { return priority > o.priority; }
};

// Min-heap: lowest number = most urgent
std::priority_queue<Task, std::vector<Task>, std::greater<Task>> scheduler;

scheduler.push({1, "CRITICAL fix"});
scheduler.push({3, "Nice to have"});
scheduler.push({2, "Important"});

// Always processes most urgent first
while (!scheduler.empty()) {
    execute(scheduler.top()); scheduler.pop();
}
```

**Ties are not first-in-first-out.** `priority_queue` is not stable: elements with equal priority come out in
an unspecified order. Example from `main.cpp`: pushing `"cat"`, `"elephant"`, `"dog"`, … by length pops `"dog"` before `"cat"`
(tested). If you need FIFO among equals, add a running sequence number to the compared key.

---

## Dijkstra with priority queue

```cpp
// Min-heap of {distance, node}
std::priority_queue<std::pair<int,int>,
                    std::vector<std::pair<int,int>>,
                    std::greater<>> pq;
pq.push({0, startNode});

while (!pq.empty()) {
    auto [dist, u] = pq.top(); pq.pop();
    if (dist > distances[u]) continue;       // outdated entry (a shorter path was found later)
    for (auto [weight, v] : graph[u])       // graph edges: {weight, destination}
        if (dist + weight < distances[v]) {
            distances[v] = dist + weight;
            pq.push({distances[v], v});
        }
}
```

---

## Top-K elements pattern

```cpp
// Find K largest elements using min-heap of size K
std::priority_queue<int, std::vector<int>, std::greater<int>> topK;

for (int x : data) {
    topK.push(x);
    if (topK.size() > K)           // K is a std::size_t
        topK.pop();               // remove smallest — keeps only K largest
}
// topK now contains the K largest elements
```

---

## Heap operations on vector

```cpp
std::vector<int> v = {3, 1, 4, 1, 5};
std::make_heap(v.begin(), v.end());     // O(n) — build heap
std::push_heap(v.begin(), v.end());     // O(log n) — after push_back
std::pop_heap(v.begin(), v.end());      // O(log n) — moves max to the END; then v.pop_back()
std::sort_heap(v.begin(), v.end());     // O(n log n) — result is sorted ascending, no longer a heap
```

Unlike `priority_queue`, a vector kept as a heap can be iterated and lets you move the top element out
(`pop_heap`, then `std::move(v.back())`, then `pop_back()`).

---

## Complexity

| Operation | Complexity |
|-----------|-----------|
| `push` | O(log n) |
| `pop` | O(log n) |
| `top` | O(1) |
| Build from range (iterator constructor / `make_heap`) | O(n) |
| `swap` | O(1) |

Measured with a counting comparator on 1,000,000 random `int`s (GCC 13.3): `make_heap` needs
**1.65 comparisons per element** (linear, the standard allows up to 3·n), pushing the same elements
one by one needs 2.28 per element, and a `pop` needs about 20 comparisons (log₂ n ≈ 19.9).

Not available: iteration, `clear()`, random access, search, decrease-key / changing the priority of an element
(all compile errors, tested, except decrease-key which simply does not exist). Reset with `pq = {}`.

---

## When to use priority_queue

✅ Always need the max/min element quickly
✅ Task scheduling by urgency
✅ Dijkstra / A* pathfinding
✅ Top-K largest/smallest elements
✅ Merge K sorted lists
✅ Event-driven simulation (next event by time)
✅ Ticket / request triage (urgent > normal > low)

❌ Need to search for an arbitrary element → use `set`
❌ Need to change the priority of an element already inside → use `set` (erase + insert) or re-insert and skip outdated entries (as in Dijkstra)
❌ Need FIFO among equal priorities → add a sequence number to the key
❌ Need to iterate all elements → use `vector` + `sort`
❌ Need FIFO → use `queue`

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` and `<climits>` are gone. `std::numeric_limits` needs `<limits>`, `std::pair` `<utility>` and `std::make_heap` `<algorithm>` — these were missing and only worked through other headers.
- Removed the lecture-only **"What is a Priority Queue"** example (its text is now in the intro of this README); the remaining examples are renumbered 1–9 and the "Topics covered" list was updated (item 5 said "with struct", the code uses a lambda).
- The old Example 9 used a call-prioritization example from a specific protocol domain; it is now a neutral support-ticket queue (`Ticket` with `operator<`), and the matching bullet in "When to use" was replaced.
- Example "Custom Comparator with Lambda": the comment said "shortest first = min-heap by length" while the code (and output) is **longest first**. The comment now explains `comp(a, b)` semantics.
- `K` in Top-K is a `std::size_t`, so `topK.size() > K` does not compare signed with unsigned; the Dijkstra node count also uses `std::size_t`. `maxHeap` was declared but never used; it is now printed.
- The README Dijkstra sketch lacked the "outdated entry" check from `main.cpp`. It stays correct without it (tested on the sample graph: same distances), but processes stale entries again, which costs time on bigger graphs.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the removed lecture example, the renumbering and the neutral ticket example); counting-comparator measurements above; ties not FIFO (`dog` before `cat`); default `priority_queue<T>` with only `operator>` rejected; iteration/`[]`/`clear` rejected; `top()` returns `const&`; `top()` on empty aborts with `-D_GLIBCXX_ASSERTIONS`; `std::greater<>` on pairs; Dijkstra without the outdated check.

## Not tested

The `std::from_range` constructor (C++23, not in GCC 13), exact tie order on other standard libraries (unspecified by the standard, so it may differ), complexity of `sort_heap` (standard guarantee), and wall-clock speed.
