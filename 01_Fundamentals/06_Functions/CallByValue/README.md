# Call by Value — C++

In **call by value**, the function receives a **copy** of the argument.
Any changes inside the function affect only the local copy —
the caller's variable is never modified.

---

## Example from this file

```cpp
void change(int data) {
    data = 5;   // modifies only the local copy
}

int main() {
    int data = 3;
    change(data);
    std::cout << data;   // still 3 — original unchanged
}
```

---

## Memory layout

```
main() stack:          change() stack:
┌───────────┐          ┌───────────┐
│ data = 3  │  ─copy→  │ data = 5  │  ← local copy, discarded on return
└───────────┘          └───────────┘
     ↑
 unchanged
```

---

## Key points

- The value is **copied** onto the function's stack frame
- Modifying the parameter inside the function has **no effect** on the caller
- The copy is **destroyed** when the function returns
- Safe — caller is protected from accidental modification

---

## When to use call by value

✅ Simple scalar types: `int`, `double`, `char`, `bool`
✅ When you want to work with the value without affecting the original
✅ When the function needs its own local copy to modify freely

❌ Large objects (`std::string`, `std::vector`) — copying is expensive
→ use `const T&` for large read-only objects instead

---

## Contrast with call by reference

```cpp
void changeByValue(int x) { x = 99; }   // caller unchanged
void changeByRef(int& x)  { x = 99; }   // caller IS changed

int n = 5;
changeByValue(n);   // n still 5
changeByRef(n);     // n now 99
```
