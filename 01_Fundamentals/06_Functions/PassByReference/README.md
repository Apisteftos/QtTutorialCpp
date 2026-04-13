# Pass by Value and Reference — C++

Covers both scalar types and vectors, showing the address difference and
practical impact of each passing style.

---

## Part A — Scalars

### 1. Pass by Value — different address, original unchanged

```cpp
int square_byValue(int n) {
    n *= n;   // local copy only
    return n;
}

int n1 = 8;
int result = square_byValue(n1);
// result = 64, n1 still 8
```

The address of `n` inside the function is **different** from `&n1` in main —
a copy was made on the function's stack frame.

---

### 2. Pass by Pointer — same address, original changed

```cpp
void square_byPointer(int* n) {
    *n *= *n;   // dereference and modify original
}

int n2 = 8;
square_byPointer(&n2);
// n2 = 64
```

The pointer value equals `&n2` — same memory location.

---

### 3. Pass by Reference — same address, original changed

```cpp
void square_byReference(int& n) {
    n *= n;   // n is an alias — modifies original directly
}

int n3 = 8;
square_byReference(n3);
// n3 = 64
```

`&n` inside the function equals `&n3` in main — same address, no copy.

---

### Scalar comparison

| Method | Address | Original | Syntax to call | C++ style |
|--------|---------|----------|----------------|-----------|
| By value | Different | Unchanged | `fn(x)` | Always |
| By pointer | Same | Changed | `fn(&x)` | C legacy |
| By reference | Same | Changed | `fn(x)` | Modern C++ |

**Prefer `int&` over `int*`** in modern C++ — cleaner syntax, cannot be null.

---

## Part B — Vectors

### By value — expensive copy, original unchanged

```cpp
void modifyByValue(std::vector<int> arr) {
    arr.push_back(42);   // modifies local copy only
}

std::vector<int> v = {1,2,3,4,5};
modifyByValue(v);
// v.size() still 5 — original unchanged
```

Entire vector contents are **copied** — expensive for large vectors.

---

### By reference — no copy, original modified

```cpp
void modifyByReference(std::vector<int>& arr) {
    arr.push_back(42);   // modifies the original
}

modifyByReference(v);
// v.size() now 6 — original changed
```

No copy — fast, modifies caller's vector directly.

---

### By const reference — no copy, read-only

```cpp
void printVector(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << ' ';
}
```

Best practice for read-only access — no copy, no modification possible.

---

### Vector comparison

| Method | Copy made | Original | Use when |
|--------|-----------|----------|----------|
| By value | ✅ expensive | Unchanged | Need own copy to modify |
| By reference | ❌ | Changed | Need to modify original |
| By const ref | ❌ | Protected | Read-only — **default choice** |

---

## Golden rule

```
Small type (int, double) → by value
Large type, read-only    → const T&
Large type, modify       → T&
```
