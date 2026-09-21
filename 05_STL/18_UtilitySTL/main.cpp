// MoveSemantics — std::move example
// -----------------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <optional>

class Buffer {
public:
    explicit Buffer(size_t size) : m_size(size), m_data(new int[size]) {
        std::cout << "  Constructed Buffer(" << m_size << ")\n";
    }

    // Copy constructor — expensive: allocates + deep copies
    Buffer(const Buffer& other) : m_size(other.m_size), m_data(new int[other.m_size]) {
        std::copy(other.m_data, other.m_data + m_size, m_data);
        std::cout << "  COPY constructed (" << m_size << " ints copied)\n";
    }

    // Move constructor — cheap: steals the pointer, no allocation
    Buffer(Buffer&& other) noexcept
        : m_size(other.m_size), m_data(other.m_data) {
        other.m_data = nullptr;   // other no longer owns this memory
        other.m_size = 0;
        std::cout << "  MOVE constructed (pointer stolen, no copy)\n";
    }

    ~Buffer() {
        delete[] m_data;
    }

    size_t size() const { return m_size; }

private:
    size_t m_size;
    int* m_data;
};

int main() {
    std::cout << "=== Example 1: Copy vs Move on a custom class ===\n";
    Buffer a(1'000'000);
    Buffer b = a;                 // copy constructor runs
    Buffer c = std::move(a);      // move constructor runs
    std::cout << "  a.size() after move: " << a.size() << " (moved-from — do not rely on its data)\n\n";

    std::cout << "=== Example 2: Moving into a vector ===\n";
    std::vector<std::string> names;
    names.reserve(2);

    std::string s = "a fairly long string that would be expensive to copy";
    std::cout << "  Before push_back(s):        s = \"" << s << "\"\n";
    names.push_back(s);              // copy — s is untouched
    std::cout << "  After push_back(s)  (copy): s = \"" << s << "\"\n";

    names.push_back(std::move(s));   // move — s is now unspecified/likely empty
    std::cout << "  After push_back(std::move(s)): s = \"" << s << "\" (moved-from)\n\n";

    std::cout << "=== Example 3: unique_ptr ownership transfer ===\n";
    std::unique_ptr<Buffer> owner1 = std::make_unique<Buffer>(10);
    std::unique_ptr<Buffer> owner2 = std::move(owner1);   // REQUIRED — unique_ptr can't be copied

    std::cout << "  owner1 is " << (owner1 ? "still valid" : "nullptr") << "\n";
    std::cout << "  owner2 is " << (owner2 ? "valid" : "nullptr")
              << ", size = " << owner2->size() << "\n\n";

    std::cout << "=== Example 4: return by value — do NOT std::move it ===\n";
    auto buildVector = []() {
        std::vector<int> result(5, 42);
        return result;   // RVO/NRVO or implicit move handles this automatically
        // return std::move(result);  // <-- would PESSIMIZE, blocks RVO
    };
    std::vector<int> data = buildVector();
    std::cout << "  data.size() = " << data.size() << "\n\n";

    std::cout << "=== Example 5: std::pair ===\n";
    std::pair<std::string, int> p{"Kostas", 30};
    std::cout << "  p.first = " << p.first << ", p.second = " << p.second << "\n";
    auto [name, age] = p;   // structured bindings
    std::cout << "  structured binding -> name = " << name << ", age = " << age << "\n\n";

    std::cout << "=== Example 6: std::swap ===\n";
    std::vector<int> v1{1, 2, 3}, v2{4, 5};
    std::cout << "  before swap: v1.size() = " << v1.size() << ", v2.size() = " << v2.size() << "\n";
    std::swap(v1, v2);   // cheap — swaps internal pointers, no element copies
    std::cout << "  after  swap: v1.size() = " << v1.size() << ", v2.size() = " << v2.size() << "\n\n";

    std::cout << "=== Example 7: std::exchange (used inside a move) ===\n";
    int counter = 10;
    int old = std::exchange(counter, 20);   // replace, return old value
    std::cout << "  old = " << old << ", counter = " << counter << "\n\n";

    std::cout << "=== Example 8: std::cmp_less — safe signed/unsigned compare ===\n";
    int signedVal = -1;
    unsigned unsignedVal = 0;
    std::cout << std::boolalpha;
    std::cout << "  signedVal < unsignedVal        (unsafe) = " << (signedVal < unsignedVal) << "\n";
    std::cout << "  std::cmp_less(signedVal, unsignedVal)    = "
              << std::cmp_less(signedVal, unsignedVal) << "\n\n";

    std::cout << "=== Example 9: std::in_place — construct directly, no extra move ===\n";
    std::optional<Buffer> o(std::in_place, 100);   // Buffer built directly inside the optional
    std::cout << "  o->size() = " << o->size() << "\n";

    return 0;
}
