// ============================================================
// Fork-Join - split a task into parts, run the parts in parallel (fork),
// wait for all of them and combine the results (join)
//
// Topics:
//   1. Split a vector into chunks, one std::async per chunk
//   2. Recursive fork-join with a size threshold
//   3. The parallel result equals the sequential result
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o fork_join
// ============================================================

#include <cstddef>
#include <future>
#include <numeric>
#include <print>
#include <span>
#include <vector>

// === EXAMPLE 1: ONE TASK PER CHUNK ===

long long parallelSum(std::span<const int> data, std::size_t parts)
{
    std::vector<std::future<long long>> futures;
    std::size_t chunk = (data.size() + parts - 1) / parts;

    // FORK: one asynchronous task per chunk.
    for (std::size_t begin = 0; begin < data.size(); begin += chunk) {
        std::size_t end = std::min(begin + chunk, data.size());
        futures.push_back(std::async(std::launch::async, [data, begin, end] {
            return std::accumulate(data.begin() + begin, data.begin() + end, 0LL);
        }));
    }

    // JOIN: wait for every part and combine.
    long long total = 0;
    for (auto& f : futures) {
        total += f.get();
    }
    return total;
}

void example1()
{
    std::println("=== EXAMPLE 1: ONE TASK PER CHUNK ===");

    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);

    std::println("parallel sum (4 parts) = {}", parallelSum(data, 4));
}

// === EXAMPLE 2: RECURSIVE FORK-JOIN ===

long long recursiveSum(std::span<const int> data, std::size_t threshold, int depth)
{
    if (data.size() <= threshold || depth == 0) {
        return std::accumulate(data.begin(), data.end(), 0LL);   // small: do it here
    }

    std::size_t middle = data.size() / 2;

    // Fork the left half, compute the right half in this thread, then join.
    auto left = std::async(std::launch::async, recursiveSum,
                           data.first(middle), threshold, depth - 1);
    long long right = recursiveSum(data.subspan(middle), threshold, depth - 1);
    return left.get() + right;
}

void example2()
{
    std::println("=== EXAMPLE 2: RECURSIVE FORK-JOIN ===");

    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);

    // depth 3 means at most 2^3 = 8 pieces, so at most 7 extra threads.
    std::println("recursive sum = {}", recursiveSum(data, 10'000, 3));
}

// === EXAMPLE 3: SAME RESULT AS SEQUENTIAL ===

void example3()
{
    std::println("=== EXAMPLE 3: COMPARE WITH SEQUENTIAL ===");

    std::vector<int> data(123'457);          // odd size on purpose
    std::iota(data.begin(), data.end(), 1);

    long long sequential = std::accumulate(data.begin(), data.end(), 0LL);
    long long parallel   = parallelSum(data, 5);
    long long recursive  = recursiveSum(data, 1000, 4);

    std::println("sequential = {}", sequential);
    std::println("all three equal: {}", sequential == parallel && sequential == recursive);
}

int main()
{
    example1();
    std::println("");
    example2();
    std::println("");
    example3();
    return 0;
}
