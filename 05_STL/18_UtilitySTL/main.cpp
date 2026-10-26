// MoveSemantics — std::move example
// -----------------------------------------------------
#include <algorithm>
#include <cstddef>
#include <memory>
#include <optional>
#include <print>
#include <string>
#include <utility>
#include <vector>

class Buffer {
public:
    explicit Buffer(std::size_t size) : m_size(size), m_data(new int[size]) {
        std::println("  Constructed Buffer({})", m_size);
    }

    // Copy constructor — expensive: allocates + deep copies
    Buffer(const Buffer& other) : m_size(other.m_size), m_data(new int[other.m_size]) {
        std::copy(other.m_data, other.m_data + m_size, m_data);
        std::println("  COPY constructed ({} ints copied)", m_size);
    }

    // Move constructor — cheap: steals the pointer, no allocation
    Buffer(Buffer&& other) noexcept
        : m_size(other.m_size), m_data(other.m_data) {
        other.m_data = nullptr;   // other no longer owns this memory
        other.m_size = 0;
        std::println("  MOVE constructed (pointer stolen, no copy)");
    }

    // Copy assignment — expensive: allocates + deep copies
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            int* newData = new int[other.m_size];          // allocate first: if it throws, *this is unchanged
            std::copy(other.m_data, other.m_data + other.m_size, newData);
            delete[] m_data;
            m_data = newData;
            m_size = other.m_size;
            std::println("  COPY assigned ({} ints copied)", m_size);
        }
        return *this;
    }

    // Move assignment — cheap: frees our memory, steals the pointer
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_size = std::exchange(other.m_size, 0);
            m_data = std::exchange(other.m_data, nullptr);
            std::println("  MOVE assigned (pointer stolen, no copy)");
        }
        return *this;
    }

    ~Buffer() {
        delete[] m_data;
    }

    std::size_t size() const { return m_size; }

private:
    std::size_t m_size;
    int* m_data;
};

int main() {
    // === EXAMPLE 1: Copy vs Move on a custom class ===
    std::println("=== Example 1: Copy vs Move on a custom class ===");
    Buffer a(1'000'000);
    Buffer b = a;                 // copy constructor runs
    Buffer c = std::move(a);      // move constructor runs
    std::println("  a.size() after move: {} (moved-from — do not rely on its data)", a.size());

    Buffer d(10);
    d = b;                        // copy assignment runs
    d = std::move(c);             // move assignment runs
    std::println("");

    // === EXAMPLE 2: Moving into a vector ===
    std::println("=== Example 2: Moving into a vector ===");
    std::vector<std::string> names;
    names.reserve(2);

    std::string s = "a fairly long string that would be expensive to copy";
    std::println("  Before push_back(s):        s = \"{}\"", s);
    names.push_back(s);              // copy — s is untouched
    std::println("  After push_back(s)  (copy): s = \"{}\"", s);

    names.push_back(std::move(s));   // move — s is now unspecified/likely empty
    std::println("  After push_back(std::move(s)): s = \"{}\" (moved-from)", s);
    std::println("");

    // === EXAMPLE 3: unique_ptr ownership transfer ===
    std::println("=== Example 3: unique_ptr ownership transfer ===");
    std::unique_ptr<Buffer> owner1 = std::make_unique<Buffer>(10);
    std::unique_ptr<Buffer> owner2 = std::move(owner1);   // REQUIRED — unique_ptr can't be copied

    std::println("  owner1 is {}", owner1 ? "still valid" : "nullptr");
    std::println("  owner2 is {}, size = {}", owner2 ? "valid" : "nullptr", owner2->size());
    std::println("");

    // === EXAMPLE 4: return by value — do NOT std::move it ===
    std::println("=== Example 4: return by value — do NOT std::move it ===");
    auto buildVector = []() {
        std::vector<int> result(5, 42);
        return result;   // RVO/NRVO or implicit move handles this automatically
        // return std::move(result);  // <-- would PESSIMIZE, blocks RVO
    };
    std::vector<int> data = buildVector();
    std::println("  data.size() = {}", data.size());
    std::println("");

    // === EXAMPLE 5: std::pair ===
    std::println("=== Example 5: std::pair ===");
    std::pair<std::string, int> p{"Kostas", 30};
    std::println("  p.first = {}, p.second = {}", p.first, p.second);
    auto [name, age] = p;   // structured bindings (copies p; use auto& to avoid the copy)
    std::println("  structured binding -> name = {}, age = {}", name, age);
    std::println("");

    // === EXAMPLE 6: std::swap ===
    std::println("=== Example 6: std::swap ===");
    std::vector<int> v1{1, 2, 3}, v2{4, 5};
    std::println("  before swap: v1.size() = {}, v2.size() = {}", v1.size(), v2.size());
    std::swap(v1, v2);   // cheap — swaps internal pointers, no element copies
    std::println("  after  swap: v1.size() = {}, v2.size() = {}", v1.size(), v2.size());
    std::println("");

    // === EXAMPLE 7: std::exchange (used inside a move) ===
    std::println("=== Example 7: std::exchange (used inside a move) ===");
    int counter = 10;
    int old = std::exchange(counter, 20);   // replace, return old value
    std::println("  old = {}, counter = {}", old, counter);
    std::println("");

    // === EXAMPLE 8: std::cmp_less — safe signed/unsigned compare ===
    std::println("=== Example 8: std::cmp_less — safe signed/unsigned compare ===");
    int signedVal = -1;
    unsigned unsignedVal = 0;
    // (the compiler warns about the next line on purpose: -Wsign-compare)
    std::println("  signedVal < unsignedVal        (unsafe) = {}", signedVal < unsignedVal);
    std::println("  std::cmp_less(signedVal, unsignedVal)    = {}",
                 std::cmp_less(signedVal, unsignedVal));
    std::println("");

    // === EXAMPLE 9: std::in_place — construct directly, no extra move ===
    std::println("=== Example 9: std::in_place — construct directly, no extra move ===");
    std::optional<Buffer> o(std::in_place, 100);   // Buffer built directly inside the optional
    std::println("  o->size() = {}", o->size());

    // For contrast: a temporary is built first, then MOVED into the optional
    std::optional<Buffer> o1 = Buffer(50);
    std::println("  o1->size() = {}", o1->size());

    return 0;
}
