/*
 * std::mdspan, std::extents, layout_left / layout_right / layout_stride
 * (C++23, <mdspan>)
 *
 * A non-owning multidimensional view over a flat array. C++23 also allows
 * several indices in operator[]:  m[row, col].
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <print>
#include <vector>
#include <version>

#ifdef __cpp_lib_mdspan
#include <mdspan>
#endif

#ifndef __cpp_lib_mdspan
int main() {
    // <mdspan> arrives in libstdc++ 16 (GCC 15 does not have it)
    std::println("std::mdspan is not available in this standard library");
    return 0;
}
#else
int main() {
    std::array<int, 12> storage{};
    for (int i = 0; i < 12; ++i) {
        storage[i] = i;
    }

    // === EXAMPLE 1: 2D view with multi-index operator[] ===
    std::println("=== Example 1: 3 x 4 matrix ===");
    std::mdspan matrix(storage.data(), 3, 4);   // 3 rows, 4 columns (layout_right: row major)
    for (std::size_t r = 0; r < matrix.extent(0); ++r) {
        for (std::size_t c = 0; c < matrix.extent(1); ++c) {
            std::print("{:3}", matrix[r, c]);
        }
        std::println("");
    }
    std::println("rank {}, size {}", matrix.rank(), matrix.size());

    matrix[1, 2] = 99;                       // writes into the original array
    std::println("storage[6] is now {}", storage[6]);

    // === EXAMPLE 2: layouts ===
    std::println("");
    std::println("=== Example 2: layout_right and layout_left ===");
    using Extents = std::extents<std::size_t, 3, 4>;   // sizes known at compile time
    std::mdspan<int, Extents, std::layout_right> rowMajor(storage.data());
    std::mdspan<int, Extents, std::layout_left> columnMajor(storage.data());
    std::println("row major:    element [1, 2] = {}", rowMajor[1, 2]);
    std::println("column major: element [1, 2] = {}", columnMajor[1, 2]);

    // === EXAMPLE 3: dynamic and fixed extents mixed, 3 dimensions ===
    std::println("");
    std::println("=== Example 3: 3D view ===");
    std::vector<double> volume(2 * 3 * 4, 1.5);
    std::mdspan<double, std::dextents<std::size_t, 3>> cube(volume.data(), 2, 3, 4);
    cube[1, 2, 3] = 7.0;
    double sum = 0;
    for (std::size_t i = 0; i < cube.extent(0); ++i) {
        for (std::size_t j = 0; j < cube.extent(1); ++j) {
            for (std::size_t k = 0; k < cube.extent(2); ++k) {
                sum += cube[i, j, k];
            }
        }
    }
    std::println("sum of all elements: {}", sum);

    // === EXAMPLE 4: layout_stride, a column of the matrix ===
    std::println("");
    std::println("=== Example 4: layout_stride ===");
    std::layout_stride::mapping<std::dextents<std::size_t, 1>> mapping(
        std::dextents<std::size_t, 1>(3), std::array<std::size_t, 1>{4});   // 3 elements, step 4
    std::mdspan column(storage.data() + 1, mapping);   // column 1 of the matrix
    for (std::size_t r = 0; r < column.extent(0); ++r) {
        std::print("{} ", column[r]);
    }
    std::println("");
    return 0;
}
#endif
