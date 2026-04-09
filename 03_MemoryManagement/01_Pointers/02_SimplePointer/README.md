# Simple Pointers in C++23

A pointer stores the **memory address** of another variable.
Instead of holding a value directly, it holds the location where a value lives.

---

## Declaration & Basic Usage

```cpp
int  number = 42;
int* ptr    = &number;   // ptr holds the ADDRESS of number
```

| Operator | Name | Meaning |
|----------|------|---------|
| `&` | Address-of | Gets the memory address of a variable |
| `*` | Dereference | Gets the value at the address the pointer holds |
| `->` | Arrow | Dereference + member access for structs/classes |

```cpp
std::cout << number;   // 42        — the value
std::cout << &number;  // 0x7fff... — the address
std::cout << ptr;      // 0x7fff... — same address (ptr stores it)
std::cout << *ptr;     // 42        — dereference: value at that address
```

---

## Example 1 — Basic pointer

```cpp
int number = 42;
int* ptr   = &number;

*ptr = 99;             // modify number THROUGH the pointer
std::cout << number;   // prints 99
```

The pointer and the variable refer to the **same memory location**.
Changing one changes the other.

---

## Example 2 — Pointer arithmetic

Pointers can be incremented/decremented to move through contiguous memory (arrays):

```cpp
int arr[5] = {10, 20, 30, 40, 50};
int* p = arr;          // points to arr[0]

*(p + 0)  // 10 — arr[0]
*(p + 1)  // 20 — arr[1]
*(p + 2)  // 30 — arr[2]
```

Each `+1` advances by **one element size** (`sizeof(int)` = 4 bytes on x86_64),
not by 1 byte. This is why pointer arithmetic is type-aware.

```cpp
int* pFirst = arr;
int* pLast  = &arr[4];
pLast - pFirst;   // = 4 — distance in elements, not bytes
```

---

## Example 3 — Pointer to pointer

```cpp
int   value = 99;
int*  p     = &value;    // pointer to int
int** pp    = &p;        // pointer to pointer to int

**pp = 777;   // double dereference — modifies value
```

Used in: dynamic 2D arrays, function arguments that modify a pointer,
C-style string arrays (`char**`), Qt plugin interfaces.

---

## Example 4 — Dynamic memory

```cpp
int* p = new int(55);    // allocate single int on heap
*p = 100;
delete p;                // MUST free — otherwise memory leak
p = nullptr;             // prevent dangling pointer

int* arr = new int[4];   // allocate array on heap
delete[] arr;            // use delete[] for arrays
arr = nullptr;
```

### Heap vs Stack
| | Stack | Heap |
|--|-------|------|
| Allocated by | Compiler automatically | `new` / `malloc` |
| Freed by | Compiler automatically | `delete` / `free` — your responsibility |
| Size | Limited (~1-8 MB) | Large (limited by RAM) |
| Speed | Fast | Slower (OS call) |
| Use for | Local variables | Large data, unknown size, long lifetime |

---

## Example 5 — const pointer variations

Read the declaration **right to left**:

```cpp
const int* p      // pointer to const int   — value is fixed, pointer can move
int* const p      // const pointer to int   — pointer is fixed, value can change
const int* const p // const pointer to const int — both fixed
```

| Declaration | Can change `*p`? | Can change `p`? |
|-------------|-----------------|-----------------|
| `int* p` | ✅ | ✅ |
| `const int* p` | ❌ | ✅ |
| `int* const p` | ✅ | ❌ |
| `const int* const p` | ❌ | ❌ |

---

## Common pointer mistakes

```cpp
int* p;
*p = 5;          // ❌ CRASH — p is uninitialized, points to random memory

int* p = new int(10);
delete p;
*p = 99;         // ❌ CRASH — dangling pointer, memory already freed

int* p = new int[5];
delete p;        // ❌ WRONG — must use delete[] for arrays
```

### Rules to avoid pointer bugs
1. Always initialize pointers (`= nullptr` or `= &something`)
2. Always `delete` what you `new` — exactly once
3. Use `delete[]` for arrays allocated with `new[]`
4. Set pointer to `nullptr` after `delete`
5. Always check for `nullptr` before dereferencing
6. Prefer `std::unique_ptr` / `std::shared_ptr` in modern C++
