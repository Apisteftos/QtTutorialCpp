# Recursion — C++

A function that **calls itself** to solve a smaller version of the same problem.
Every recursive function needs a **base case** that stops the recursion.

---

## Example from this file

```cpp
int factorial(int n) {
    if (n < 0)  return -1;   // error — wrong value
    if (n == 0) return  1;   // base case — stops recursion
    return n * factorial(n - 1);  // recursive case
}

int main() {
    int fact = factorial(5);
    std::cout << fact;   // 120
}
```

Note: the file declares `factorial` **inside** `main()` — this is valid C++
but unusual. Normally prototypes are declared at file scope.

---

## Call stack for factorial(5)

```
factorial(5)
  5 * factorial(4)
        4 * factorial(3)
              3 * factorial(2)
                    2 * factorial(1)
                          1 * factorial(0)
                                returns 1    ← base case
                          returns 1
                    returns 2
              returns 6
        returns 24
  returns 120
```

---

## The two parts of every recursive function

```cpp
int factorial(int n) {
    if (n == 0) return 1;           // 1. BASE CASE  — stops recursion
    return n * factorial(n - 1);    // 2. RECURSIVE CASE — smaller problem
}
```

Without the base case → **infinite recursion → stack overflow**.

---

## Common recursive algorithms

```cpp
// Fibonacci
int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);
}

// Sum of array
int sum(int arr[], int n) {
    if (n == 0) return 0;
    return arr[n-1] + sum(arr, n-1);
}

// Power
int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp-1);
}
```

---

## Recursion vs iteration

```cpp
// Recursive factorial — clean but uses stack space
int factRecursive(int n) { return (n<=1) ? 1 : n * factRecursive(n-1); }

// Iterative factorial — uses O(1) stack space
int factIterative(int n) {
    int result = 1;
    for (int i=2; i<=n; ++i) result *= i;
    return result;
}
```

| | Recursion | Iteration |
|--|----------|-----------|
| Readability | ✅ natural for tree/graph | ❌ more code |
| Stack usage | O(n) — call per level | O(1) |
| Risk | Stack overflow for large n | None |

---

## When to use recursion

✅ Tree traversal — inherently recursive structure
✅ Divide and conquer — merge sort, quicksort, binary search
✅ Backtracking — sudoku, maze solving
✅ When the problem naturally splits into smaller same-problems

❌ Simple loops — use iteration (factorial, sum)
❌ Very deep recursion (n > ~10000) — use iteration or `std::stack`
