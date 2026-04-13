# Swapping Arrays — C++23

Covers all methods to swap the contents of two arrays,
from manual element-by-element to modern STL one-liners.

---

## 1. Manual swap — temp variable

```cpp
void swapArrays(int arr1[], int arr2[], int size) {
    for (int i = 0; i < size; ++i) {
        int temp = arr1[i];   // save
        arr1[i]  = arr2[i];   // overwrite
        arr2[i]  = temp;      // restore
    }
}
```

The classic approach — understand this before using the STL versions.

---

## 2. std::swap per element — cleaner

```cpp
for (int i = 0; i < size; ++i)
    std::swap(arr1[i], arr2[i]);   // no temp variable needed
```

`std::swap(a, b)` does the same temp trick internally — just reads cleaner.

---

## 3. std::swap_ranges — whole array in one line

```cpp
#include <algorithm>

std::swap_ranges(arr1, arr1 + size, arr2);
// swaps arr1[0..size) with arr2[0..size)
```

Works on any two ranges of the same size — C arrays, vectors, arrays.

---

## 4. std::swap on std::vector — O(1)!

```cpp
std::vector<int> v1 = {1,2,3,4,5};
std::vector<int> v2 = {6,7,8,9,10};

std::swap(v1, v2);   // swaps internal pointers — O(1), no element copy!
v1.swap(v2);         // member function — equivalent
```

`std::swap` on a vector swaps **internal pointers**, not elements.
O(1) regardless of vector size — always prefer this over manual loops.

---

## 5. std::swap on std::array — O(N)

```cpp
std::array<int,5> a1 = {1,2,3,4,5};
std::array<int,5> a2 = {6,7,8,9,10};

std::swap(a1, a2);   // O(N) — no pointer trick for stack-allocated array
```

Unlike `vector`, `array` is stack-allocated — no pointers to swap.

---

## 6. Partial swap — subrange only

```cpp
// Swap only elements at indices 1 and 2
std::swap_ranges(arr1 + 1, arr1 + 3, arr2 + 1);
```

---

## 7. Swap rows of a 2D array

```cpp
int matrix[3][4] = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12} };

// Swap row 0 and row 2
std::swap_ranges(matrix[0], matrix[0] + 4, matrix[2]);
```

---

## Complexity summary

| Method | Complexity | Use when |
|--------|-----------|----------|
| Manual temp | O(N) | C arrays, learning |
| `std::swap` per element | O(N) | C arrays, clean |
| `std::swap_ranges` | O(N) | C arrays, one-liner |
| `std::swap(vector)` | **O(1)** | vectors — always prefer |
| `std::swap(array)` | O(N) | `std::array` |
| `swap_ranges` (partial) | O(K) | subrange only |

---

## sizeof trick for C array size

```cpp
int arr[] = {1, 2, 3, 4, 5};
int size = sizeof(arr) / sizeof(arr[0]);   // 5
```

This only works in the **same scope** where the array was declared.
If passed to a function, `arr` decays to a pointer and `sizeof` gives
the pointer size, not the array size. Always pass `size` explicitly.
