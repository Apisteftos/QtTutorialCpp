# QueueSTL — std::queue in C++23

`std::queue` is a **FIFO (First In, First Out)** container adapter.
The first element pushed is the first one popped — like a queue at a ticket office:

- `push` (enqueue) — joins at the **back**
- `pop` (dequeue) — leaves from the **front**
- the first person to arrive is the first to be served

---

## Header

```cpp
#include <queue>
```

---

## Declaration

```cpp
std::queue<int> q;                   // default — backed by std::deque

std::deque<int> dq = {1, 2, 3};
std::queue<int> q2(dq);              // front=1, back=3 (the queue gets a COPY of dq, tested)

std::queue<int, std::list<int>> ql;  // list-backed
```

Note: `std::vector` **cannot** back a queue — a queue needs `pop_front()` (it removes from the front), which
vector lacks (compile error: `'class std::vector<int>' has no member named 'pop_front'`, tested).
The underlying container needs `front()`, `back()`, `push_back()`, `pop_front()`, so `deque` and `list` work.

---

## Core operations

```cpp
q.push(10);     // add to BACK
q.emplace(10);  // construct in place at back
q.front()       // access FRONT element — O(1), no removal
q.back()        // access BACK element — O(1), no removal
q.pop()         // remove FRONT element — O(1), no return value!
                // front()/back()/pop() on an EMPTY queue is undefined behavior —
                // check empty() first (with -D_GLIBCXX_ASSERTIONS it aborts, tested)
q.empty()       // true if no elements
q.size()        // number of elements
q.swap(other)   // swap two queues (same type); also ==, <, <=> compare element-wise
q = {};         // empty the queue (no clear() member; tested)
```

**Important:** `pop()` does NOT return a value. Use `front()` first:

```cpp
int val = q.front();   // peek at front (copy)
q.pop();               // then remove
```

A *reference* to the front element is dangling after `pop()`:

```cpp
const auto& msg = q.front();
use(msg);
q.pop();               // msg must not be used after this line
```

---

## FIFO order

```cpp
std::queue<int> q;
q.push(10);   // first in
q.push(20);
q.push(30);   // last in

q.front()  // 10 — first in, first out
q.pop();   // removes 10
q.front()  // 20
```

---

## Task processing

```cpp
std::queue<std::string> tasks;
tasks.push("Download");
tasks.push("Parse");
tasks.push("Save");

while (!tasks.empty()) {
    process(tasks.front());   // use the front element ...
    tasks.pop();              // ... then remove it
}
// processes in order: Download, Parse, Save
```

---

## BFS with queue

```cpp
std::queue<int> q;
q.push(startNode);
visited[startNode] = true;     // mark when pushing, not when popping: every node enters at most once

while (!q.empty()) {
    int node = q.front(); q.pop();
    visit(node);
    for (int neighbor : graph[node])
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            q.push(neighbor);
        }
}
// BFS visits nodes level by level (unlike DFS with a stack)
```

---

## Queue vs Stack

| | `std::queue` | `std::stack` |
|--|-------------|-------------|
| Order | FIFO | LIFO |
| Add at | Back | Top |
| Remove from | Front | Top |
| Access | `front()` + `back()` | `top()` |
| Use for | BFS, task queues | DFS, undo/redo |

---

## What std::queue does NOT have

- No iterators
- No `operator[]` or random access
- No `find` or search
- No `insert` in the middle

All of these are compile errors (tested). If you need them, use `std::deque` directly.

---

## When to use queue

✅ Tasks processed in arrival order
✅ BFS graph traversal
✅ Message/event queues (network packets, GUI events)
✅ Print spoolers, job schedulers
✅ Producer/consumer buffer — but `std::queue` is **not thread-safe**: with several threads you need a mutex (and usually a condition variable) around it

❌ Need LIFO → use `std::stack`
❌ Need both ends + random access → use `std::deque`
❌ Need to iterate → use `std::deque` directly

---

## Queue vs Stack vs Deque

- `queue` — FIFO (first in, first out), exposes `push`, `pop`, `front`, `back`, `empty`, `size`
- `stack` — LIFO (last in, first out)
- `deque` — both ends, random access, full container interface

`std::queue` is a **container adapter**: it wraps a `deque` (default) or a `list` and hides everything else.

## Real-world uses

- Task/job processing (in the order received)
- BFS graph traversal
- Message queues (network packets, events)
- Print spooler
- Producer/consumer buffer

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone, the duplicate `#include <queue>` was removed, and `<deque>` (used for `std::deque` but only included indirectly) was added.
- Removed the two lecture-only examples (they only printed text): **"What is a Queue (FIFO)"** (now in the intro of this README) and **"Queue characteristics"** (now the sections above). The remaining examples are renumbered 1–7 and the "Topics covered" list was updated.
- The old Example 8 used message types from a specific protocol domain; it is now a neutral message queue (`LOGIN_REQUEST`, `DATA_UPLOAD`, …) with a local `Message` struct. The same applies to the "packets" remark in this README.
- **Wrong claim fixed:** "vector cannot back a queue because queue needs `push_front`". A queue never calls `push_front`; it needs `pop_front()`, which vector lacks (compile error shown above).
- The task example moves the string out of the queue instead of copying it; the message loop uses `const auto&` and has a comment that the reference dies at `pop()`.
- The Qt remark "Event queues in Qt (QCoreApplication event loop)" is now just "GUI events": Qt's event loop has its own event queue and does not use `std::queue`, and I did not verify its internals.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the removed lecture examples, the renumbering and the neutral message example); vector as underlying container rejected with the `pop_front` error; `q = {}`; `queue(dq)` copies; `swap`, `<`, `==`; list-backed queue incl. `emplace`; `front()` on empty aborts with `-D_GLIBCXX_ASSERTIONS`; iterators/`[]`/`clear`/`insert` rejected.

## Not tested

Dangling `front()` reference after `pop()` is undefined behavior; ASan did **not** report it in my test (a deque keeps its memory block), so "no error" proves nothing. Not tested either: thread-safety statement (standard guarantee: no synchronization), other standard libraries.
