/*
 * Parallel algorithms: execution policies (<execution>, C++17)
 *
 * std::execution::seq, par, par_unseq are passed as the first argument.
 * With GCC the parallel backend is Intel TBB: install it (for example the
 * package libtbb-dev) and link with -ltbb (without it the link fails when the
 * TBB headers are installed). If the TBB headers are not installed, GCC uses a
 * sequential backend and the code still runs.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example -ltbb
 */

#include <algorithm>
#include <atomic>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> data(100000);
    std::iota(data.begin(), data.end(), 1);

    // === EXAMPLE 1: for_each with a policy ===
    std::cout << "=== Example 1: for_each(par) ===\n";
    std::atomic<long long> sum{0};   // the lambda can run on several threads: shared data needs protection
    std::for_each(std::execution::par, data.begin(), data.end(), [&sum](int x) { sum += x; });
    std::cout << "sum = " << sum << "\n";

    // === EXAMPLE 2: sort and transform ===
    std::cout << "\n=== Example 2: sort(par), transform(par_unseq) ===\n";
    std::vector<int> values = {9, 4, 7, 1, 8, 2, 6, 3, 5, 0};
    std::sort(std::execution::par, values.begin(), values.end());
    for (int v : values) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    std::vector<int> doubled(data.size());
    std::transform(std::execution::par_unseq, data.begin(), data.end(), doubled.begin(), [](int x) { return x * 2; });
    std::cout << "doubled.back() = " << doubled.back() << "\n";

    // === EXAMPLE 3: reduce ===
    std::cout << "\n=== Example 3: reduce(par) ===\n";
    long long total = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
    std::cout << "total = " << total << "\n";
    // The policy functions may throw only std::terminate; do not throw from the callable.
    return 0;
}
