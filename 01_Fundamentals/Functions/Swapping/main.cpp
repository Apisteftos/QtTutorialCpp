#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <print>

// ============================================================
// SWAPPING ARRAYS in C++23
// ============================================================
// Topics covered:
//   1.  Manual swap — element by element with temp variable
//   2.  Manual swap using std::swap per element
//   3.  std::swap_ranges — swap entire C arrays
//   4.  std::swap on std::vector (O(1) — no copy!)
//   5.  std::swap on std::array
//   6.  Swap a subrange (partial swap)
//   7.  Swap rows of a 2D array
// ============================================================


// ============================================================
// Helper — print a C array
// ============================================================
void printArray(const std::string& label, const int arr[], int size)
{
    std::cout << label << ": ";
    for (int i = 0; i < size; ++i) std::cout << arr[i] << ' ';
    std::cout << '\n';
}


// ============================================================
// Example 1 — Manual swap (from file 1)
// ============================================================
void swapArrays_manual(int arr1[], int arr2[], int size)
{
    for (int i = 0; i < size; ++i) {
        int temp = arr1[i];   // save arr1[i]
        arr1[i]  = arr2[i];   // overwrite arr1[i]
        arr2[i]  = temp;      // restore into arr2[i]
    }
}


// ============================================================
// Example 2 — std::swap per element
// ============================================================
void swapArrays_stdSwap(int arr1[], int arr2[], int size)
{
    for (int i = 0; i < size; ++i)
        std::swap(arr1[i], arr2[i]);   // cleaner — no temp variable needed
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Manual swap — element by element\n";
    std::cout << "=====================================================\n";

    {
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {6, 7, 8, 9, 10};
        int size   = sizeof(arr1) / sizeof(arr1[0]);

        printArray("before arr1", arr1, size);
        printArray("before arr2", arr2, size);

        swapArrays_manual(arr1, arr2, size);

        printArray("after  arr1", arr1, size);
        printArray("after  arr2", arr2, size);

        std::cout << "\nHow it works:\n";
        std::cout << "  temp = arr1[i]\n";
        std::cout << "  arr1[i] = arr2[i]\n";
        std::cout << "  arr2[i] = temp\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: std::swap per element — cleaner\n";
    std::cout << "=====================================================\n";

    {
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {6, 7, 8, 9, 10};
        int size   = sizeof(arr1) / sizeof(arr1[0]);

        printArray("before arr1", arr1, size);
        printArray("before arr2", arr2, size);

        swapArrays_stdSwap(arr1, arr2, size);

        printArray("after  arr1", arr1, size);
        printArray("after  arr2", arr2, size);

        std::cout << "\nstd::swap(a, b) = temp trick under the hood\n";
        std::cout << "but reads cleaner — no manual temp variable\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: std::swap_ranges — swap entire C arrays\n";
    std::cout << "=====================================================\n";

    {
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {6, 7, 8, 9, 10};
        int size   = sizeof(arr1) / sizeof(arr1[0]);

        printArray("before arr1", arr1, size);
        printArray("before arr2", arr2, size);

        // swap_ranges(first1, last1, first2)
        std::swap_ranges(arr1, arr1 + size, arr2);

        printArray("after  arr1", arr1, size);
        printArray("after  arr2", arr2, size);

        std::cout << "\nstd::swap_ranges(begin1, end1, begin2)\n";
        std::cout << "works on any two ranges of the same size\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: std::swap on std::vector — O(1)!\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v1 = {1, 2, 3, 4, 5};
        std::vector<int> v2 = {6, 7, 8, 9, 10};

        std::cout << "before v1: "; for (int x : v1) std::cout << x << ' '; std::cout << '\n';
        std::cout << "before v2: "; for (int x : v2) std::cout << x << ' '; std::cout << '\n';

        std::swap(v1, v2);   // swaps internal pointers — O(1), no element copy!

        std::cout << "after  v1: "; for (int x : v1) std::cout << x << ' '; std::cout << '\n';
        std::cout << "after  v2: "; for (int x : v2) std::cout << x << ' '; std::cout << '\n';

        std::cout << "\nstd::swap on vector swaps internal pointers\n";
        std::cout << "O(1) — regardless of vector size — no element copy!\n";

        // member .swap() — equivalent
        v1.swap(v2);
        std::cout << "after v1.swap(v2):\n";
        std::cout << "v1: "; for (int x : v1) std::cout << x << ' '; std::cout << '\n';
        std::cout << "v2: "; for (int x : v2) std::cout << x << ' '; std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: std::swap on std::array\n";
    std::cout << "=====================================================\n";

    {
        std::array<int, 5> a1 = {1, 2, 3, 4, 5};
        std::array<int, 5> a2 = {6, 7, 8, 9, 10};

        std::cout << "before a1: "; for (int x : a1) std::cout << x << ' '; std::cout << '\n';
        std::cout << "before a2: "; for (int x : a2) std::cout << x << ' '; std::cout << '\n';

        std::swap(a1, a2);   // swaps element by element — O(N) for array

        std::cout << "after  a1: "; for (int x : a1) std::cout << x << ' '; std::cout << '\n';
        std::cout << "after  a2: "; for (int x : a2) std::cout << x << ' '; std::cout << '\n';

        std::cout << "\nNote: std::array swap is O(N) — no pointer trick\n";
        std::cout << "      std::vector swap is O(1) — just swaps pointers\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Partial swap — swap a subrange only\n";
    std::cout << "=====================================================\n";

    {
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {6, 7, 8, 9, 10};
        int size   = sizeof(arr1) / sizeof(arr1[0]);

        printArray("before arr1", arr1, size);
        printArray("before arr2", arr2, size);

        // Swap only elements [1..3) — indices 1 and 2
        std::swap_ranges(arr1 + 1, arr1 + 3, arr2 + 1);

        printArray("after  arr1", arr1, size);
        printArray("after  arr2", arr2, size);
        std::cout << "(only middle elements swapped)\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Swap rows of a 2D array\n";
    std::cout << "=====================================================\n";

    {
        int matrix[3][4] = {
            {1,  2,  3,  4},
            {5,  6,  7,  8},
            {9, 10, 11, 12}
        };

        auto printMatrix = [&]() {
            for (auto& row : matrix) {
                std::cout << "  ";
                for (int val : row) std::cout << val << "\t";
                std::cout << '\n';
            }
        };

        std::cout << "before:\n"; printMatrix();

        // Swap row 0 and row 2
        std::swap_ranges(matrix[0], matrix[0] + 4, matrix[2]);

        std::cout << "after swapping row 0 and row 2:\n"; printMatrix();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Summary — choosing the right swap\n";
    std::cout << "=====================================================\n";

    std::cout << "Method                      | Complexity | Use when\n";
    std::cout << "----------------------------|------------|------------------\n";
    std::cout << "manual temp variable        | O(N)       | C arrays, learning\n";
    std::cout << "std::swap per element       | O(N)       | C arrays, clean code\n";
    std::cout << "std::swap_ranges            | O(N)       | C arrays, ranges\n";
    std::cout << "std::swap(vector, vector)   | O(1)       | vectors — always prefer\n";
    std::cout << "std::swap(array, array)     | O(N)       | std::array\n";
    std::cout << "std::swap_ranges (partial)  | O(K)       | swap subrange only\n";

    std::cout << "\n=== Swapping Arrays complete ===\n";
    return 0;
}
