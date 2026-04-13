#include <iostream>
#include <vector>

// ============================================================
// PASS BY VALUE vs PASS BY REFERENCE in C++23
// ============================================================
// Topics covered:
//   Part A — Scalars
//   1.  Pass-by-Value         (own copy, different address)
//   2.  Pass-by-Reference     via Pointer (int*)
//   3.  Pass-by-Reference     via Reference (int&)
//   4.  Address comparison    showing which is shared
//
//   Part B — Vectors
//   5.  Vector passed by value      (expensive copy, original unchanged)
//   6.  Vector passed by reference  (no copy, original modified)
//   7.  Vector passed by const ref  (read-only, no copy — best practice)
// ============================================================


// ============================================================
// Part A — Scalar examples
// ============================================================

// 1. Pass-by-Value — receives a COPY, different address
int square_byValue(int n)
{
    std::cout << "  [square_byValue]  address of n: " << &n << " (DIFFERENT)\n";
    n *= n;       // modifies only the local copy
    return n;     // caller's variable unchanged
}

// 2. Pass-by-Reference via Pointer — receives ADDRESS, dereference to modify
void square_byPointer(int* n)
{
    std::cout << "  [square_byPointer] address of n: " << n  << " (SAME)\n";
    *n *= *n;     // modifies the original via pointer dereference
}

// 3. Pass-by-Reference via Reference — alias for original, same address
void square_byReference(int& n)
{
    std::cout << "  [square_byReference] address of n: " << &n << " (SAME)\n";
    n *= n;       // modifies the original directly
}


// ============================================================
// Part B — Vector examples
// ============================================================

// 5. By value — entire vector is copied, original untouched
void modifyByValue(std::vector<int> arr)
{
    arr.push_back(42);
    std::cout << "  [modifyByValue]     inside size = " << arr.size()
              << " (local copy — original unaffected)\n";
}

// 6. By reference — no copy, modifies the original
void modifyByReference(std::vector<int>& arr)
{
    arr.push_back(42);
    std::cout << "  [modifyByReference] inside size = " << arr.size()
              << " (same vector — original changed)\n";
}

// 7. By const reference — read-only, no copy (best practice for read-only)
void printVector(const std::vector<int>& arr)
{
    std::cout << "  [printVector] elements: ";
    for (int x : arr) std::cout << x << ' ';
    std::cout << '\n';
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Part A — Scalars: Value vs Pointer vs Reference\n";
    std::cout << "=====================================================\n";

    // --- 1. Pass by value ---
    std::cout << "\n--- 1. Pass-by-Value ---\n";
    int n1 = 8;
    std::cout << "  [main] address of n1: " << &n1 << '\n';
    int result1 = square_byValue(n1);
    std::cout << "  square returned: " << result1 << '\n';
    std::cout << "  n1 after call:   " << n1 << " (UNCHANGED)\n";

    // --- 2. Pass by pointer ---
    std::cout << "\n--- 2. Pass-by-Reference via Pointer (int*) ---\n";
    int n2 = 8;
    std::cout << "  [main] address of n2: " << &n2 << '\n';
    square_byPointer(&n2);
    std::cout << "  n2 after call:   " << n2 << " (CHANGED)\n";

    // --- 3. Pass by reference ---
    std::cout << "\n--- 3. Pass-by-Reference via Reference (int&) ---\n";
    int n3 = 8;
    std::cout << "  [main] address of n3: " << &n3 << '\n';
    square_byReference(n3);
    std::cout << "  n3 after call:   " << n3 << " (CHANGED)\n";

    // --- 4. Summary ---
    std::cout << "\n--- 4. Summary ---\n";
    std::cout << "  Pass-by-Value:     caller's address != function's address\n";
    std::cout << "  Pass-by-Pointer:   caller's address == function's address\n";
    std::cout << "  Pass-by-Reference: caller's address == function's address\n";
    std::cout << '\n';
    std::cout << "  int*   — C style, can be null, caller must pass &x\n";
    std::cout << "  int&   — C++ style, cannot be null, cleaner syntax\n";
    std::cout << "  Prefer int& over int* in modern C++\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Part B — Vectors: Value vs Reference vs Const Ref\n";
    std::cout << "=====================================================\n";

    std::vector<int> myVector = {1, 2, 3, 4, 5};

    // --- 5. By value ---
    std::cout << "\n--- 5. Vector passed by value ---\n";
    std::cout << "  before: size = " << myVector.size() << '\n';
    modifyByValue(myVector);
    std::cout << "  after:  size = " << myVector.size() << " (unchanged)\n";
    printVector(myVector);

    // --- 6. By reference ---
    std::cout << "\n--- 6. Vector passed by reference ---\n";
    std::cout << "  before: size = " << myVector.size() << '\n';
    modifyByReference(myVector);
    std::cout << "  after:  size = " << myVector.size() << " (changed!)\n";
    printVector(myVector);

    // --- 7. By const reference ---
    std::cout << "\n--- 7. Vector passed by const reference (read-only) ---\n";
    printVector(myVector);
    std::cout << "  size = " << myVector.size() << " (unchanged — read-only)\n";

    // --- Summary ---
    std::cout << "\n--- Summary ---\n";
    std::cout << "  by value      — copy made, original safe, expensive for large data\n";
    std::cout << "  by reference  — no copy, original can be modified, fast\n";
    std::cout << "  by const ref  — no copy, original protected, fast — USE FOR READ-ONLY\n";

    return 0;
}
