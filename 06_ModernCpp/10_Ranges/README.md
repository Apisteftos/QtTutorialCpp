# C++20/23 Ranges

## What are Ranges?

Ranges generalize iterators — any type with `begin()` and `end()` is a range.
**Views** are lazy, composable transformations over ranges using the `|` pipe operator.

---

## Pipeline concept

```mermaid
flowchart LR
    data["vector&lt;int&gt;\n{1..15}"]
    f["views::filter\nn % 2 == 0"]
    t["views::transform\nn * 3"]
    tk["views::take(4)"]
    out["result\n{6,12,18,24}"]

    data --> f --> t --> tk --> out
    note["LAZY — no intermediate vectors\nData flows through pipeline on demand"]
    style note fill:#2a2a2a,color:#aaa
```

---

## Key views

```mermaid
flowchart TD
    subgraph Views ["std::views:: (namespace)"]
        filter["filter(pred)\nkeep elements where pred is true"]
        transform["transform(fn)\napply fn to each element"]
        take["take(n)\nfirst n elements"]
        drop["drop(n)\nskip first n elements"]
        take_while["take_while(pred)\ntake until pred is false"]
        drop_while["drop_while(pred)\nskip until pred is false"]
        iota["iota(start) / iota(start,end)\ngenerate sequence"]
        reverse["reverse\niterate backwards"]
        keys["keys\nfirst of each pair"]
        values["values\nsecond of each pair"]
        enumerate["enumerate (C++23)\nindex + value pairs"]
    end
```

---

## Common patterns

```cpp
namespace views = std::views;

std::vector<int> v = {1,2,3,4,5,6,7,8,9,10};

// Filter
v | views::filter([](int n){ return n % 2 == 0; })
// → 2 4 6 8 10

// Transform
v | views::transform([](int n){ return n * n; })
// → 1 4 9 16 25 36 49 64 81 100

// Chain
v | views::filter([](int n){ return n % 2 == 0; })
  | views::transform([](int n){ return n * 3; })
  | views::take(3)
// → 6 12 18

// Generate
views::iota(1) | views::take(5)
// → 1 2 3 4 5

views::iota(1, 6)
// → 1 2 3 4 5
```

---

## Lazy evaluation

```mermaid
sequenceDiagram
    participant Main
    participant View as Pipeline view
    participant Data as vector data

    Main->>View: create pipeline (filter|transform|take)
    Note over View: Nothing computed yet!

    Main->>View: begin() — start iteration
    View->>Data: get element 1 → filter? no → skip
    View->>Data: get element 2 → filter? yes → transform → yield
    Main->>View: ++it
    View->>Data: get element 3 → filter? no → skip
    View->>Data: get element 4 → filter? yes → transform → yield
    Note over View: take(2) satisfied → STOP
    Note over Data: Elements 5-10 never processed!
```

---

## Range algorithms

```cpp
namespace ranges = std::ranges;

std::vector<int> v = {5,2,8,1,9,3};

ranges::sort(v);                          // sort in place
ranges::find(v, 8);                       // find iterator
ranges::count_if(v, [](int n){ return n%2==0; });
ranges::min_element(v);
ranges::max_element(v);
ranges::all_of(v,  [](int n){ return n > 0; });
ranges::any_of(v,  [](int n){ return n > 8; });
ranges::none_of(v, [](int n){ return n < 0; });
ranges::contains(v, 7);                   // C++23
```

---

## views vs std:: algorithms

```mermaid
flowchart LR
    subgraph Old ["Old — std:: algorithms"]
        O1["std::sort(v.begin(), v.end())"]
        O2["std::find(v.begin(), v.end(), x)"]
        O3["Always need begin() and end()"]
    end

    subgraph New ["New — ranges::"]
        N1["ranges::sort(v)"]
        N2["ranges::find(v, x)"]
        N3["Works directly on range"]
    end
```

---

## MCX session filtering example

```cpp
// Active GRP-ALPHA sessions sorted by priority
std::vector<McxSession> alpha;
for (const auto& s : sessions
    | views::filter([](const McxSession& s){
        return s.active && s.group == "GRP-ALPHA";
      }))
    alpha.push_back(s);
ranges::sort(alpha, {}, &McxSession::priority);

// All active session IDs
for (const auto& id : sessions
    | views::filter([](const auto& s){ return s.active; })
    | views::transform([](const auto& s){ return s.id; }))
    std::cout << id << '\n';
```

---

## When to use Ranges

✅ Complex filtering and transformation pipelines
✅ Replacing nested loops with readable pipelines
✅ Lazy evaluation — avoid intermediate containers
✅ Infinite sequences — `views::iota` + `views::take`
✅ Functional programming style in C++

❌ Simple single-step operations — a plain loop is clearer
❌ Performance-critical code needing manual SIMD
