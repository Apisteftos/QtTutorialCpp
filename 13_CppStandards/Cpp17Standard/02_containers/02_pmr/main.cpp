/*
 * Polymorphic memory resources: std::pmr::monotonic_buffer_resource,
 * std::pmr::vector, std::pmr::string (<memory_resource>, C++17)
 *
 * Containers get their memory from a resource chosen at run time, for
 * example a buffer on the stack.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

// A resource that counts how often it is asked for memory
class CountingResource : public std::pmr::memory_resource {
public:
    int allocations = 0;

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        ++allocations;
        return std::pmr::new_delete_resource()->allocate(bytes, alignment);
    }
    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
    }
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override { return this == &other; }
};

int main() {
    // === EXAMPLE 1: a stack buffer as memory source ===
    std::cout << "=== Example 1: monotonic_buffer_resource on the stack ===\n";
    CountingResource upstream;
    std::array<std::byte, 1024> buffer;
    std::pmr::monotonic_buffer_resource pool(buffer.data(), buffer.size(), &upstream);

    std::pmr::vector<int> numbers(&pool);
    for (int i = 0; i < 20; ++i) {
        numbers.push_back(i);
    }
    std::cout << "20 ints stored, heap allocations: " << upstream.allocations << "\n";

    // === EXAMPLE 2: when the buffer is too small, the upstream resource is used ===
    std::cout << "\n=== Example 2: buffer too small ===\n";
    std::pmr::vector<int> big(&pool);
    big.resize(10000);
    std::cout << "10000 ints stored, heap allocations: " << upstream.allocations << "\n";

    // === EXAMPLE 3: pmr strings propagate their resource ===
    std::cout << "\n=== Example 3: pmr::string in a pmr::vector ===\n";
    std::pmr::monotonic_buffer_resource pool2;
    std::pmr::vector<std::pmr::string> words(&pool2);
    words.emplace_back("a string that is long enough to need heap memory");
    words.emplace_back("short");
    std::cout << words[0] << "\n";
    std::cout << "string uses the vector's resource: " << std::boolalpha
              << (words[0].get_allocator().resource() == &pool2) << "\n";
    return 0;
}
