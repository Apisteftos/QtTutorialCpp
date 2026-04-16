# How to Count Complexity

## 📚 Overview

This guide teaches you **HOW TO COUNT** algorithm complexity step by step.

> **"Counting complexity is a skill. Follow the methodology, practice the patterns, and you'll be able to analyze any code."**

### What You'll Learn

- **Step-by-step counting methodology**
- **Pattern recognition** for common complexities
- **Rules for simplification**
- **How to analyze loops, recursion, and data structures**
- **Practice problems** with detailed solutions

---

## 🎯 The 5-Step Method

### Step 1: Identify All Operations

Find all loops, recursive calls, and operations.

### Step 2: Count Each Operation

Determine how many times each operation executes in terms of input size.

### Step 3: Combine Counts

- **Nested operations:** Multiply
- **Sequential operations:** Add

### Step 4: Simplify

- Drop constants
- Drop lower-order terms
- Keep only the dominant term

### Step 5: Express in Big O

Write the final complexity using Big O notation.

---

## 🔢 Counting Single Loops

### Pattern: Simple Loop

```cpp
for (int i = 0; i < n; i++) {
    // O(1) operation
}
```

**Count:**
- Loop runs from 0 to n-1
- **n iterations** × O(1) operation
- **Result: O(n)**

### Pattern: Loop with Step

```cpp
for (int i = 0; i < n; i += 2) {
    // O(1) operation
}
```

**Count:**
- i goes: 0, 2, 4, 6, ..., n
- **n/2 iterations** × O(1) operation
- Simplify: Drop constant 1/2
- **Result: O(n)**

### Pattern: Loop with Multiplier

```cpp
for (int i = 0; i < n; i += 10) {
    // O(1) operation
}
```

**Count:**
- i goes: 0, 10, 20, 30, ..., n
- **n/10 iterations** × O(1) operation
- Simplify: Drop constant 1/10
- **Result: O(n)**

### Pattern: Fixed Iterations

```cpp
for (int i = 0; i < 100; i++) {
    // O(1) operation
}
```

**Count:**
- Loop always runs **100 times**
- 100 is a constant
- **Result: O(1)**

---

## 🔁 Counting Nested Loops

### Pattern: Simple Nested (Same Size)

```cpp
for (int i = 0; i < n; i++) {           // n iterations
    for (int j = 0; j < n; j++) {       // n iterations
        // O(1) operation
    }
}
```

**Count:**
- Inner loop: n × O(1) = O(n)
- Outer loop: n × O(n) = O(n²)
- **Result: O(n²)**

### Pattern: Nested (Different Sizes)

```cpp
for (int i = 0; i < n; i++) {           // n iterations
    for (int j = 0; j < m; j++) {       // m iterations
        // O(1) operation
    }
}
```

**Count:**
- Inner loop: m × O(1) = O(m)
- Outer loop: n × O(m) = O(n*m)
- **Result: O(n*m)** ← Both variables stay!

### Pattern: Triangular Nested

```cpp
for (int i = 0; i < n; i++) {           // n iterations
    for (int j = 0; j < i; j++) {       // i iterations
        // O(1) operation
    }
}
```

**Count:**
- When i=0: 0 iterations
- When i=1: 1 iteration
- When i=2: 2 iterations
- ...
- When i=n-1: n-1 iterations
- **Total: 0 + 1 + 2 + ... + (n-1) = n(n-1)/2**
- Simplify: n²/2 - n/2 → drop constants and lower terms
- **Result: O(n²)**

### Pattern: Triple Nested

```cpp
for (int i = 0; i < n; i++) {           // n iterations
    for (int j = 0; j < n; j++) {       // n iterations
        for (int k = 0; k < n; k++) {   // n iterations
            // O(1) operation
        }
    }
}
```

**Count:**
- Innermost: n × O(1) = O(n)
- Middle: n × O(n) = O(n²)
- Outer: n × O(n²) = O(n³)
- **Result: O(n³)**

---

## 📉 Counting Logarithmic Patterns

### Pattern: Halving Loop

```cpp
int i = n;
while (i > 0) {
    // O(1) operation
    i = i / 2;
}
```

**Count:**
- i goes: n, n/2, n/4, n/8, ..., 1
- How many times until i reaches 1?
- n / 2^k = 1 → 2^k = n → **k = log₂(n)**
- **Result: O(log n)**

### Pattern: Doubling Loop

```cpp
int i = 1;
while (i < n) {
    // O(1) operation
    i = i * 2;
}
```

**Count:**
- i goes: 1, 2, 4, 8, 16, ..., n
- How many times until i reaches n?
- 2^k = n → **k = log₂(n)**
- **Result: O(log n)**

### Pattern: Nested with Halving

```cpp
for (int i = 0; i < n; i++) {           // n iterations
    int j = n;
    while (j > 0) {                     // log n iterations
        // O(1) operation
        j = j / 2;
    }
}
```

**Count:**
- Inner: log n × O(1) = O(log n)
- Outer: n × O(log n) = O(n log n)
- **Result: O(n log n)**

---

## ➕ Counting Sequential Operations

### Pattern: Multiple Loops (Same)

```cpp
// Loop 1
for (int i = 0; i < n; i++) {
    // O(1)
}

// Loop 2
for (int j = 0; j < n; j++) {
    // O(1)
}

// Loop 3
for (int k = 0; k < n; k++) {
    // O(1)
}
```

**Count:**
- Loop 1: O(n)
- Loop 2: O(n)
- Loop 3: O(n)
- **Total: O(n) + O(n) + O(n) = O(3n)**
- Simplify: Drop constant 3
- **Result: O(n)**

### Pattern: Different Complexities

```cpp
// Part 1: O(n²)
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        // O(1)
    }
}

// Part 2: O(n)
for (int i = 0; i < n; i++) {
    // O(1)
}

// Part 3: O(log n)
int i = n;
while (i > 0) {
    i /= 2;
}
```

**Count:**
- Part 1: O(n²)
- Part 2: O(n)
- Part 3: O(log n)
- **Total: O(n²) + O(n) + O(log n)**
- Keep dominant term: n² grows fastest
- **Result: O(n²)**

---

## 🔄 Counting Recursive Calls

### Pattern: Linear Recursion

```cpp
int func(int n) {
    if (n <= 0) return 0;
    return n + func(n - 1);
}
```

**Count:**
- Recurrence: T(n) = T(n-1) + O(1)
- Expansion:
  - T(n) = T(n-1) + 1
  - T(n-1) = T(n-2) + 1
  - T(n-2) = T(n-3) + 1
  - ...
  - T(1) = T(0) + 1
- **Total: 1 + 1 + 1 + ... (n times) = n**
- **Result: O(n)**

### Pattern: Binary Recursion

```cpp
int func(int n) {
    if (n <= 0) return 0;
    return func(n - 1) + func(n - 1);
}
```

**Count:**
- Recurrence: T(n) = 2 × T(n-1) + O(1)
- Expansion:
  - T(n) = 2 × T(n-1)
  - T(n) = 2 × [2 × T(n-2)] = 4 × T(n-2)
  - T(n) = 4 × [2 × T(n-3)] = 8 × T(n-3)
  - T(n) = 2^k × T(n-k)
  - When k=n: T(n) = 2^n × T(0)
- **Result: O(2^n)**

### Pattern: Divide and Conquer

```cpp
void mergeSort(int arr[], int left, int right) {
    if (left >= right) return;
    
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);      // T(n/2)
    mergeSort(arr, mid + 1, right); // T(n/2)
    merge(arr, left, mid, right);   // O(n)
}
```

**Count:**
- Recurrence: T(n) = 2 × T(n/2) + O(n)
- Using Master Theorem:
  - a = 2 (two recursive calls)
  - b = 2 (divide by 2)
  - f(n) = n
  - log_b(a) = log₂(2) = 1
  - f(n) = n^1
  - Case 2: f(n) = Θ(n^log_b(a))
- **Result: O(n log n)**

### Pattern: Halving Recursion

```cpp
int func(int n) {
    if (n <= 1) return 1;
    return func(n / 2);
}
```

**Count:**
- Recurrence: T(n) = T(n/2) + O(1)
- After k steps: n/2^k = 1 → k = log₂(n)
- **Result: O(log n)**

---

## 📊 Master Theorem (Quick Reference)

For recurrence: **T(n) = a × T(n/b) + f(n)**

Where:
- **a** = number of recursive calls
- **b** = factor by which problem size is divided
- **f(n)** = work done per level

### Three Cases:

**Case 1:** f(n) < n^(log_b(a))
→ **Result: O(n^log_b(a))**

**Case 2:** f(n) = n^(log_b(a))
→ **Result: O(n^log_b(a) × log n)**

**Case 3:** f(n) > n^(log_b(a))
→ **Result: O(f(n))**

### Examples:

```
T(n) = 2T(n/2) + O(1)      → O(n)
T(n) = 2T(n/2) + O(n)      → O(n log n)  [Merge Sort]
T(n) = 4T(n/2) + O(n)      → O(n²)
T(n) = T(n/2) + O(n)       → O(n)
T(n) = 2T(n/2) + O(n²)     → O(n²)
```

---

## 🔧 Simplification Rules

### Rule 1: Drop Constants

```
5n → n
10n² → n²
n/2 → n
```

### Rule 2: Drop Lower-Order Terms

```
n² + n → n²
n³ + n² + n → n³
n log n + n → n log n
```

### Rule 3: Keep Dominant Term

```
Growth rates (slowest to fastest):
O(1) < O(log n) < O(√n) < O(n) < O(n log n) < O(n²) < O(n³) < O(2^n) < O(n!)
```

### Rule 4: Different Variables Stay

```
O(n + m) → O(n + m)  (cannot simplify)
O(n × m) → O(n × m)  (cannot simplify)
```

### Examples:

```
3n² + 2n log n + 5n + 10
→ Keep dominant: n²
→ Result: O(n²)

2^n + n³ + n² + n
→ Keep dominant: 2^n
→ Result: O(2^n)

n*m + n + m
→ Keep dominant when both large: n*m
→ Result: O(n*m)
```

---

## 📚 Common Patterns Quick Reference

| Pattern | Code | Complexity |
|---------|------|------------|
| **Simple loop** | `for (i=0; i<n; i++)` | O(n) |
| **Nested loops (same)** | `for (i...) for (j...)` | O(n²) |
| **Nested loops (different)** | `for (i<n) for (j<m)` | O(n*m) |
| **Triangular** | `for (i<n) for (j<i)` | O(n²) |
| **Halving** | `while (n>0) n/=2` | O(log n) |
| **Doubling** | `for (i=1; i<n; i*=2)` | O(log n) |
| **Square root** | `for (i*i<n)` | O(√n) |
| **Linear recursion** | `T(n)=T(n-1)+c` | O(n) |
| **Binary recursion** | `T(n)=2T(n-1)+c` | O(2^n) |
| **Divide & conquer** | `T(n)=2T(n/2)+n` | O(n log n) |

---

## 🎓 Practice Problems

### Problem 1

```cpp
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            // O(1)
        }
    }
}
```

**Solution:**
1. Innermost: n × O(1) = O(n)
2. Middle: n × O(n) = O(n²)
3. Outer: n × O(n²) = O(n³)
4. **Answer: O(n³)**

### Problem 2

```cpp
for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
        // O(1)
    }
}
```

**Solution:**
1. When i=0: n iterations
2. When i=1: n-1 iterations
3. When i=2: n-2 iterations
4. Total: n + (n-1) + (n-2) + ... + 1 = n(n+1)/2
5. Simplify: n²/2 + n/2 → n²
6. **Answer: O(n²)**

### Problem 3

```cpp
for (int i = 0; i < n; i++) {
    // O(1)
}

int j = n;
while (j > 0) {
    // O(1)
    j /= 2;
}
```

**Solution:**
1. First loop: O(n)
2. Second loop: O(log n)
3. Total: O(n) + O(log n)
4. Keep dominant: O(n)
5. **Answer: O(n)**

### Problem 4

```cpp
void func(int n) {
    if (n <= 1) return;
    func(n / 2);
    func(n / 2);
    for (int i = 0; i < n; i++) {
        // O(1)
    }
}
```

**Solution:**
1. Recurrence: T(n) = 2T(n/2) + O(n)
2. Master Theorem: a=2, b=2, f(n)=n
3. log_b(a) = log₂(2) = 1
4. f(n) = n = n^1
5. Case 2: f(n) = Θ(n^log_b(a))
6. **Answer: O(n log n)**

### Problem 5

```cpp
for (int i = 1; i < n; i *= 2) {
    for (int j = 0; j < n; j++) {
        // O(1)
    }
}
```

**Solution:**
1. Outer: log n iterations (doubling)
2. Inner: n iterations
3. Total: log n × n
4. **Answer: O(n log n)**

---

## 🎯 Step-by-Step Analysis Template

Use this template for any code:

```
1. IDENTIFY:
   - What are the loops?
   - What are the recursive calls?
   - What are the data structure operations?

2. COUNT EACH:
   - How many iterations?
   - In terms of what variables?
   - What work per iteration?

3. COMBINE:
   - Nested? → Multiply
   - Sequential? → Add
   
4. SIMPLIFY:
   - Drop constants
   - Drop lower-order terms
   - Keep dominant term
   
5. RESULT:
   - Write as O(...)
```

---

## ✅ Checklist for Analysis

When analyzing code, ask yourself:

```
□ Have I identified all loops?
□ Have I counted iterations correctly?
□ Have I analyzed recursive calls?
□ Did I multiply nested operations?
□ Did I add sequential operations?
□ Have I simplified correctly?
□ Did I keep the dominant term?
□ Did I drop constants?
□ For different variables, did I keep both?
□ Is my final answer in Big O notation?
```

---

## 💡 Common Mistakes to Avoid

### Mistake 1: Forgetting to Simplify

```
❌ O(3n + 5)
✅ O(n)
```

### Mistake 2: Not Dropping Lower Terms

```
❌ O(n² + n)
✅ O(n²)
```

### Mistake 3: Simplifying Different Variables

```
❌ O(n*m) → O(n)  [WRONG!]
✅ O(n*m)  [Keep both]
```

### Mistake 4: Miscounting Triangular Loops

```cpp
for (i=0; i<n; i++)
    for (j=0; j<i; j++)

❌ O(n) [WRONG!]
✅ O(n²) [Correct - sum is n²/2]
```

### Mistake 5: Confusing Halving with Linear

```cpp
while (n > 0) { n /= 2; }

❌ O(n) [WRONG!]
✅ O(log n) [Correct - logarithmic]
```

---

## 📈 Growth Rate Comparison

Remember these relative growth rates:

```
For n = 1,000:

O(1):        1 operation
O(log n):    ~10 operations
O(√n):       ~31 operations
O(n):        1,000 operations
O(n log n):  ~10,000 operations
O(n²):       1,000,000 operations
O(n³):       1,000,000,000 operations
O(2^n):      More than atoms in universe
O(n!):       Completely impossible
```

---

## 🎓 Key Takeaways

1. **Follow the methodology** - identify, count, combine, simplify
2. **Practice pattern recognition** - learn common patterns
3. **Always simplify** - drop constants and lower terms
4. **Count carefully** - especially triangular and logarithmic
5. **Different variables stay** - O(n*m) doesn't simplify to O(n)
6. **Recursion** - write recurrence, use Master Theorem
7. **Check your work** - use the checklist

---

## 📚 Further Practice

To master complexity counting:

1. **Analyze your own code** - practice on real examples
2. **Try LeetCode problems** - with complexity analysis
3. **Review algorithms** - understand why they have their complexity
4. **Compare alternatives** - see complexity differences
5. **Teach others** - best way to solidify understanding

---

**Remember: Counting complexity is a skill that improves with practice. Use this methodology consistently, and you'll master it!**