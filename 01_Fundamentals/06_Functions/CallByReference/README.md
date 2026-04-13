# Call by Reference — C++

In **call by reference**, the function receives the **memory address** of the
argument via a pointer. Modifying the value through the pointer changes the
**original variable** in the caller.

---

## Example from this file

```cpp
void swap(int* x, int* y) {
    int temp = *x;   // dereference — read value at address x
    *x = *y;         // write value of y into address x
    *y = temp;       // write saved value into address y
}

int main() {
    int x = 500, y = 100;
    swap(&x, &y);    // pass addresses of x and y
    // x = 100, y = 500 — both changed
}
```

---

## Step by step

```
Before call:
  x = 500  (address: 0x100)
  y = 100  (address: 0x104)

Call: swap(&x, &y)
  pointer x inside swap → 0x100 (points to x in main)
  pointer y inside swap → 0x104 (points to y in main)

Inside swap:
  temp = *x     → temp = 500
  *x   = *y     → value at 0x100 = 100  (x in main becomes 100)
  *y   = temp   → value at 0x104 = 500  (y in main becomes 500)

After call:
  x = 100  ← changed
  y = 500  ← changed
```

---

## Why & is used at the call site

```cpp
swap(&x, &y);   // & = address-of operator — gives the address of x and y
```

Without `&` you would pass the **value** of x, not its address — swap
would work on copies and the originals would be unchanged.

---

## Pointer dereference inside the function

```cpp
void swap(int* x, int* y) {
    int temp = *x;   // *x = "value at address x" = 500
    *x = *y;         // store value at y into address x
    *y = temp;
}
```

| Syntax | Meaning |
|--------|---------|
| `x` | The address (pointer value) |
| `*x` | The value stored at that address |
| `&x` | The address of the pointer itself |

---

## Call by value vs call by reference

```cpp
// Call by value — copy, original unchanged
void changeByValue(int n) { n = 99; }
int a = 5;
changeByValue(a);   // a still 5

// Call by reference via pointer — original changed
void changeByRef(int* n) { *n = 99; }
int b = 5;
changeByRef(&b);    // b now 99
```

---

## Modern C++ alternative — reference parameter

```cpp
// Old style (C)
void swap(int* x, int* y);
swap(&x, &y);        // caller must use &

// Modern C++ — reference parameter, cleaner
void swap(int& x, int& y);
swap(x, y);          // caller uses variable directly — no &
```

Both achieve the same result — `int&` is preferred in modern C++
because it cannot be null and the call site is cleaner.

---

## When to use pointer parameters

✅ C API compatibility — C has no references
✅ When the pointer can be `nullptr` (optional parameter)
✅ Pointer arithmetic (arrays, buffer processing)

❌ In modern C++ — prefer `int&` for modifiable parameters
❌ When the parameter is never null — use reference instead
