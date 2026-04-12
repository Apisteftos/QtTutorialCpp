# QueueSTL — std::queue in C++23

`std::queue` is a **FIFO (First In, First Out)** container adapter.
The first element pushed is the first one popped — like a queue at a ticket office.

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
std::queue<int> q2(dq);              // front=1, back=3

std::queue<int, std::list<int>> ql;  // list-backed
```

Note: `std::vector` **cannot** back a queue — queue needs `push_front` which vector lacks.

---

## Core operations

```cpp
q.push(10);     // add to BACK
q.emplace(10);  // construct in place at back
q.front()       // access FRONT element — O(1), no removal
q.back()        // access BACK element — O(1), no removal
q.pop()         // remove FRONT element — O(1), no return value!
q.empty()       // true if no elements
q.size()        // number of elements
```

**Important:** `pop()` does NOT return a value. Use `front()` first:

```cpp
int val = q.front();   // peek at front
q.pop();               // then remove
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
    process(tasks.front());
    tasks.pop();
}
// processes in order: Download, Parse, Save
```

---

## BFS with queue

```cpp
std::queue<int> q;
q.push(startNode);
visited[startNode] = true;

while (!q.empty()) {
    int node = q.front(); q.pop();
    visit(node);
    for (int neighbor : graph[node])
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            q.push(neighbor);
        }
}
// BFS visits nodes level by level
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

If you need these, use `std::deque` directly.

---

## When to use queue

✅ Tasks processed in arrival order
✅ BFS graph traversal
✅ Message/event queues (MCX packets, Qt events)
✅ Print spoolers, job schedulers
✅ Producer/consumer buffer

❌ Need LIFO → use `std::stack`
❌ Need both ends + random access → use `std::deque`
❌ Need to iterate → use `std::deque` directly
