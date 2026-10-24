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

## Example 1 — Basic pointer (main.cpp Example 1)

```cpp
int number = 42;
int* ptr   = &number;
// *ptr == 42, ptr == &number
```

`main.cpp` prints the pointers through `static_cast<const void*>(...)` because `std::println` cannot format `int*`; addresses differ from run to run.

`->` combines dereference and member access: for `P* pp = &p;` the expressions `pp->x` and `(*pp).x` are the same ✅ tested.

---

## Example 2 — Modifying a value through a pointer (main.cpp Example 2)

```cpp
*ptr = 99;             // modify number THROUGH the pointer
std::cout << number;   // prints 99
```

The pointer and the variable refer to the **same memory location**.
Changing one changes the other.

---

## Example 3 — Pointer arithmetic (main.cpp Example 3)

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

Pointer arithmetic is only valid inside one array (plus the one-past-the-end position, e.g. `arr + 5`); going further, or subtracting pointers of different arrays, is undefined behavior. `main.cpp` Example 3 also moves the pointer with `++pArr`; its labels used to read `*pArr++`, but the code increments *before* printing, so they now say `*pArr after ++pArr`.

---

## Example 4 — Pointer to pointer (main.cpp Example 4)

```cpp
int   value = 99;
int*  p     = &value;    // pointer to int
int** pp    = &p;        // pointer to pointer to int

**pp = 777;   // double dereference — modifies value
```

Used in: dynamic 2D arrays, function arguments that modify a pointer,
C-style string arrays (`char**`, e.g. `argv`).

---

## Example 5 — Dynamic memory (main.cpp Example 5)

```cpp
int* p = new int(55);    // allocate single int on heap
*p = 100;
delete p;                // MUST free — otherwise memory leak
p = nullptr;             // prevent dangling pointer

int* arr = new int[4];   // allocate array on heap (elements uninitialized)
delete[] arr;            // use delete[] for arrays
arr = nullptr;
```

### Heap vs Stack
| | Stack | Heap |
|--|-------|------|
| Allocated by | Compiler automatically | `new` / `malloc` |
| Freed by | Compiler automatically (at scope exit) | `delete` / `free` — your responsibility |
| Size | Limited (typically 1-8 MB per thread, system dependent) | Large (limited by RAM) |
| Speed | Fast | Slower (allocator bookkeeping, sometimes an OS call) |
| Use for | Local variables | Large data, unknown size, long lifetime |

---

## Example 6 — const pointer variations (main.cpp Example 6)

Read the declaration **right to left**:

```cpp
const int* p      // pointer to const int   — value is fixed, pointer can move
int* const p      // const pointer to int   — pointer is fixed, value can change
const int* const p // const pointer to const int — both fixed
```
(Three separate declarations - they would clash if put in one scope. "Value is fixed" means *through this pointer*; the variable itself may still be changed by other means, e.g. `a = 5;`.)

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
*p = 5;          // ❌ undefined behavior — p is uninitialized (typically a crash)

int* p = new int(10);
delete p;
*p = 99;         // ❌ undefined behavior — dangling pointer, memory already freed

int* p = new int[5];
delete p;        // ❌ WRONG — must use delete[] for arrays (undefined behavior;
                 //    AddressSanitizer reports alloc-dealloc-mismatch ✅ tested)
```
Each of the three snippets is separate (they all declare `p`).

### Rules to avoid pointer bugs
1. Always initialize pointers (`= nullptr` or `= &something`)
2. Always `delete` what you `new` — exactly once
3. Use `delete[]` for arrays allocated with `new[]`
4. Set pointer to `nullptr` after `delete`
5. Always check for `nullptr` before dereferencing
6. Prefer `std::unique_ptr` in modern C++, and `std::shared_ptr` only when ownership is really shared (Core Guidelines [R.11](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rr-newdelete), [R.21](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rr-unique))

See the nullptr folder for null pointers and dangling pointers in more detail.


---

## 🚀 How to run

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o pointers
./pointers
```
`main.cpp` uses `std::println` (`<print>`), so it needs GCC 14+ or a recent Clang with `-std=c++23`.

---

## 🧾 Notes on main.cpp

No lecture text to move. Changes:

- `std::cout` → `std::println` / `std::print`; pointers are printed via `static_cast<const void*>` (a null pointer prints `0x0`, `cout` printed `0`).
- Example 3: labels `*pArr++` corrected to `*pArr after ++pArr` (the code increments before printing).
- ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer (and Clang); the output is identical apart from the label change, `0x0`, and the run-dependent addresses.

---

## ⚠️ Open points

- The README's examples 1–5 did not match `main.cpp` (1–6); the headings now use the `main.cpp` numbers.
- This folder has no number or Previous/Next links.
- The Core Guidelines anchors `#rr-newdelete` and `#rr-unique` come from the guidelines text; I did not open the pages.
