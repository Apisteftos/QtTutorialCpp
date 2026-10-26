# StackSTL — std::stack in C++23

`std::stack` is a **LIFO (Last In, First Out)** container adapter.
The last element pushed is the first one popped — like a stack of plates:

- `push` — a plate goes on the **top**
- `pop` — a plate comes from the **top** (last added, first removed)
- `top` — peek at the top plate

---

## Header

```cpp
#include <stack>
```

---

## Declaration

```cpp
std::stack<int> s;                          // default — backed by std::deque

// Initialize from existing container
std::deque<int> dq = {1, 2, 3};
std::stack<int> s2(dq);   // top = 3 (last element); the stack gets a COPY, dq is unchanged (tested)

// Choose underlying container
std::stack<int, std::vector<int>> sv;       // vector-backed (cache-friendly)
std::stack<int, std::list<int>>   sl;       // list-backed
```

---

## Core operations

```cpp
s.push(10);      // add to top
s.emplace(10);   // construct in place at top (saves a temporary for non-trivial types; same as push for int)
s.top()          // access top element — O(1), no removal
s.pop()          // remove top element — O(1), no return value!
                 // top()/pop() on an EMPTY stack is undefined behavior —
                 // check empty() first (with -D_GLIBCXX_ASSERTIONS it aborts, tested)
s.empty()        // true if no elements
s.size()         // number of elements
s.swap(other)    // swap two stacks (same type); also ==, <, <=> compare element-wise
```

**Important:** `pop()` does NOT return the value — call `top()` first:

```cpp
int val = s.top();   // peek
s.pop();             // then remove
```

---

## LIFO order

```cpp
std::stack<int> s;
s.push(10);   // bottom
s.push(20);
s.push(30);   // top

s.top()  // 30 — last in, first out
s.pop(); // removes 30
s.top()  // 20
```

---

## Balanced parentheses checker

```cpp
bool isBalanced(const std::string& expr) {
    std::stack<char> st;
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{')
            st.push(c);
        else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char top = st.top(); st.pop();
            if (c == ')' && top != '(') return false;
            if (c == ']' && top != '[') return false;
            if (c == '}' && top != '{') return false;
        }
    }
    return st.empty();
}
```

---

## Undo/Redo system

```cpp
std::stack<State> undoStack, redoStack;

void doAction(State newState) {
    undoStack.push(currentState);
    redoStack = {};          // clear redo on new action (std::stack has no clear(); tested)
    currentState = newState;
}

void undo() {
    if (undoStack.empty()) return;      // nothing to undo
    redoStack.push(currentState);
    currentState = undoStack.top();
    undoStack.pop();
}

void redo() {
    if (redoStack.empty()) return;      // nothing to redo
    undoStack.push(currentState);
    currentState = redoStack.top();
    redoStack.pop();
}
```

---

## DFS with stack

```cpp
std::stack<int> pending;
pending.push(startNode);

while (!pending.empty()) {
    int node = pending.top(); pending.pop();
    if (visited[node]) continue;
    visited[node] = true;
    visit(node);
    // push in reverse, so the FIRST neighbor ends up on top and is visited first
    for (int i = static_cast<int>(graph[node].size()) - 1; i >= 0; --i)
        pending.push(graph[node][i]);
}
```

Without the reverse loop the neighbors are visited in reverse order. For the graph in
`main.cpp` (`0→{1,2}`, `1→{0,3,4}`, `2→{0,5}`) the plain `for (int n : graph[node])` version
visits `0 2 5 1 4 3`, the version above visits `0 1 3 4 2 5` (both tested). Both are valid depth-first orders.

---

## Underlying container comparison

| Container | push/pop | Cache | Notes |
|-----------|---------|-------|-------|
| `std::deque` (default) | O(1) | Medium | Good general choice, elements never move on push |
| `std::vector` | O(1) amortized | Best | Usually fastest; push may reallocate, so references to elements are invalidated |
| `std::list` | O(1) | Worst | One heap allocation per element |

The underlying container only needs `back()`, `push_back()`, `pop_back()` (+ `empty()`, `size()`).
`std::array` does not work (compile error, tested).

Rough measurement (GCC 13.3, `-O2`, 5 × [5,000,000 pushes + 5,000,000 pops], `int`, one noisy sandbox run):
`vector` ≈ 64 ms, `deque` ≈ 130 ms, `list` ≈ 867 ms. For a stack that grows and shrinks a lot,
`std::stack<int, std::vector<int>>` is a good choice; the default `deque` is fine unless profiling says otherwise.

---

## What std::stack does NOT have

- No iterators — cannot loop through without popping
- No `operator[]` — no random access
- No `find` — cannot search
- No `insert`, no `clear()` — only `push` at top (reset with `s = {}`)

All of these are compile errors (tested). If you need them, use the underlying container directly (`vector`, `deque`).

---

## When to use stack

✅ Undo/redo history (one stack for undo, one for redo)
✅ Expression parsing / evaluation (postfix, infix)
✅ DFS graph traversal
✅ Browser back/forward (two stacks)
✅ Call stack simulation
✅ Balanced bracket checking
✅ Recursive algorithm → iterative conversion

❌ Need FIFO (first in, first out) → use `std::queue`
❌ Need to iterate all elements → use `vector` directly (or copy the stack and drain the copy, as `main.cpp` does)
❌ Need random access → use `vector` directly

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone, needed includes added (`<cctype>`, `<utility>`).
- Removed the two lecture-only examples (they only printed text): **"What is a Stack (LIFO)"** (now the plate description at the top) and **"Stack characteristics"** (now "What std::stack does NOT have", the container table and "When to use stack"). The remaining examples are renumbered 1–8 and the "Topics covered" list was updated.
- Example "Undo/Redo": the comment on the third `undo()` said "nothing to undo beyond this", but three undos still had states to restore (the third returns to the empty text). A fourth `undo()` now actually prints "nothing to undo". The redo stack is cleared with `m_redoStack = {};` instead of a pop loop.
- Postfix evaluator: `std::isdigit` now gets an `unsigned char` (negative `char` values are undefined behavior) and `<cctype>` is included. It only handles single-digit operands and valid input: `"12 3 +"` evaluates to `5` (tested), and `top()` on an empty stack would be undefined behavior.
- DFS: the README version did not reverse the neighbor order and produced a different visiting order from `main.cpp` (see the DFS section). A local variable named `stack` was renamed `pending`.
- The Balanced-parentheses sketch only checked `)`; now it checks all three bracket kinds like `main.cpp`.
- The old Example 10 text "vector — no push_front overhead" made no sense for a stack (it never uses `push_front`), and "list — O(1) *guaranteed*" suggested the others are not: `deque` push/pop is also O(1). Both were dropped from the container table. "Only if stability needed" was misleading too: `deque` also keeps element references valid on push.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the removed lecture examples, the renumbering and the extra "nothing to undo" line); `redoStack = {}`; `stack(dq)` copies; `swap`, `<`, `==`; `top()` on empty aborts with `-D_GLIBCXX_ASSERTIONS`; iterators/`[]`/`clear`/`insert` rejected; `std::array` as container rejected; both DFS orders; multi-digit postfix; the timing run above.

## Not tested

The C++23 constructors taking iterator pairs or `std::from_range` beyond the iterator-pair form (GCC 13 has no `from_range`), behavior on other standard libraries, and timings beyond the single noisy run.
