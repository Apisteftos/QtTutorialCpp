# PriorityQueueSTL — std::priority_queue in C++23

`std::priority_queue` is a **max-heap** by default — the largest element is always at the top.
Push in any order, always pop in sorted (highest first) order.

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

// From range
std::vector<int> v = {3, 1, 4, 1, 5};
std::priority_queue<int> pq(v.begin(), v.end());
```

---

## Core operations

```cpp
pq.push(10);    // add element — O(log n)
pq.emplace(10); // construct in place — O(log n)
pq.top()        // access top (max/min) — O(1)
pq.pop()        // remove top — O(log n)
pq.empty()      // true if empty
pq.size()       // element count
```

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

```cpp
// Sort by string length (longest first)
auto cmp = [](const std::string& a, const std::string& b) {
    return a.size() < b.size();   // longer = higher priority
};

std::priority_queue<std::string,
                    std::vector<std::string>,
                    decltype(cmp)> pq(cmp);
```

Or use `operator<` in the struct:
```cpp
struct Task {
    int priority;
    std::string name;
    bool operator<(const Task& o) const {
        return priority > o.priority;  // lower number = higher priority (min-heap)
    }
};

std::priority_queue<Task> taskPq;   // uses operator<
```

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
    for (auto [weight, v] : graph[u])
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
    if (topK.size() > K)
        topK.pop();   // remove smallest — keeps only K largest
}
// topK now contains the K largest elements
```

---

## Heap operations on vector

```cpp
std::vector<int> v = {3, 1, 4, 1, 5};
std::make_heap(v.begin(), v.end());     // O(n) — build heap
std::push_heap(v.begin(), v.end());     // O(log n) — after push_back
std::pop_heap(v.begin(), v.end());      // O(log n) — moves max to end
std::sort_heap(v.begin(), v.end());     // O(n log n) — destroys heap
```

---

## Complexity

| Operation | Complexity |
|-----------|-----------|
| `push` | O(log n) |
| `pop` | O(log n) |
| `top` | O(1) |
| Build from range | O(n) |

---

## When to use priority_queue

✅ Always need the max/min element quickly
✅ Task scheduling by urgency
✅ Dijkstra / A* pathfinding
✅ Top-K largest/smallest elements
✅ Merge K sorted lists
✅ Event-driven simulation (next event by time)
✅ MCX call prioritization (emergency > group > individual)

❌ Need to search for arbitrary element → use `set`
❌ Need to iterate all elements → use `vector` + `sort`
❌ Need FIFO → use `queue`
