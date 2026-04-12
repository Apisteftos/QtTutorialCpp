# StackSTL — std::stack in C++23

`std::stack` is a **LIFO (Last In, First Out)** container adapter.
The last element pushed is the first one popped — like a stack of plates.

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
std::stack<int> s2(dq);   // top = 3 (last element)

// Choose underlying container
std::stack<int, std::vector<int>> sv;       // vector-backed (cache-friendly)
std::stack<int, std::list<int>>   sl;       // list-backed
```

---

## Core operations

```cpp
s.push(10);      // add to top
s.emplace(10);   // construct in place at top (preferred for objects)
s.top()          // access top element — O(1), no removal
s.pop()          // remove top element — O(1), no return value!
s.empty()        // true if no elements
s.size()         // number of elements
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
    redoStack = {};          // clear redo on new action
    currentState = newState;
}

void undo() {
    redoStack.push(currentState);
    currentState = undoStack.top();
    undoStack.pop();
}

void redo() {
    undoStack.push(currentState);
    currentState = redoStack.top();
    redoStack.pop();
}
```

---

## DFS with stack

```cpp
std::stack<int> stack;
stack.push(startNode);

while (!stack.empty()) {
    int node = stack.top(); stack.pop();
    if (visited[node]) continue;
    visited[node] = true;
    visit(node);
    for (int neighbor : graph[node])
        stack.push(neighbor);
}
```

---

## Underlying container comparison

| Container | push/pop | Cache | Notes |
|-----------|---------|-------|-------|
| `std::deque` (default) | O(1) | Medium | Good general choice |
| `std::vector` | O(1) amortized | Best | Preferred for performance |
| `std::list` | O(1) guaranteed | Worst | Only if stability needed |

Use `std::vector` as the underlying container for best performance:
```cpp
std::stack<int, std::vector<int>> s;
```

---

## What std::stack does NOT have

- No iterators — cannot loop through without popping
- No `operator[]` — no random access
- No `find` — cannot search
- No `insert` — only `push` at top

If you need these, use the underlying container directly (`vector`, `deque`).

---

## When to use stack

✅ Undo/redo history
✅ Expression parsing / evaluation (postfix, infix)
✅ DFS graph traversal
✅ Browser back/forward
✅ Call stack simulation
✅ Balanced bracket checking
✅ Recursive algorithm → iterative conversion

❌ Need FIFO (first in, first out) → use `std::queue`
❌ Need to iterate all elements → use `vector` directly
❌ Need random access → use `vector` directly
