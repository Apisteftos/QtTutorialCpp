/*
 * std::construct_at, std::destroy_at, std::make_unique_for_overwrite,
 * std::assume_aligned, std::to_address, std::ssize (<memory>, C++20)
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <iostream>
#include <memory>
#include <new>
#include <string>

struct Tracker {
    explicit Tracker(int id) : id(id) { std::cout << "  construct " << id << "\n"; }
    ~Tracker() { std::cout << "  destroy " << id << "\n"; }
    int id;
};

constexpr int constexprConstruct() {
    // construct_at is usable in constant expressions (placement new is not)
    std::allocator<int> alloc;
    int* p = alloc.allocate(1);
    std::construct_at(p, 41);
    int result = *p + 1;
    std::destroy_at(p);
    alloc.deallocate(p, 1);
    return result;
}

int main() {
    // === EXAMPLE 1: construct_at, destroy_at ===
    std::cout << "=== Example 1: construct_at, destroy_at ===\n";
    alignas(Tracker) unsigned char storage[sizeof(Tracker)];
    Tracker* t = std::construct_at(reinterpret_cast<Tracker*>(storage), 7);
    std::cout << "id = " << t->id << "\n";
    std::destroy_at(t);

    static_assert(constexprConstruct() == 42);
    std::cout << "constexpr use compiled: " << constexprConstruct() << "\n";

    // === EXAMPLE 2: make_unique_for_overwrite ===
    std::cout << "\n=== Example 2: make_unique_for_overwrite ===\n";
    // make_unique<int[]>(n) writes zeros first; for_overwrite leaves the elements
    // uninitialized, which is faster when they are overwritten right away.
    auto buffer = std::make_unique_for_overwrite<int[]>(4);
    for (int i = 0; i < 4; ++i) {
        buffer[i] = i * i;
    }
    std::cout << "buffer: " << buffer[0] << " " << buffer[1] << " " << buffer[2] << " " << buffer[3] << "\n";

    // === EXAMPLE 3: to_address, assume_aligned ===
    std::cout << "\n=== Example 3: to_address, assume_aligned ===\n";
    auto sp = std::make_shared<std::string>("hello");
    std::string* raw = std::to_address(sp);        // works for smart pointers and iterators
    std::cout << "to_address: " << *raw << "\n";
    alignas(32) static float data[8] = {};
    float* aligned = std::assume_aligned<32>(data);   // promise to the compiler: 32 byte aligned
    aligned[0] = 1.5f;
    std::cout << "aligned[0] = " << aligned[0] << "\n";
    return 0;
}
