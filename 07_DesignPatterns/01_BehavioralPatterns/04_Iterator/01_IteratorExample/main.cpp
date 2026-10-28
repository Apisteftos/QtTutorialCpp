/*
 * Iterator: a ring buffer that can be used in a range-based for loop
 *
 * Intent: traverse a collection without exposing how it stores its elements.
 * Here the iterator follows the C++ convention (begin/end, operator*, ++, !=),
 * so the standard range-for loop and algorithms work with it.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <cstddef>
#include <print>
#include <string>

template <typename T, std::size_t Capacity>
class RingBuffer {
public:
    // When the buffer is full, the oldest element is overwritten.
    void push(const T& value) {
        data_[(head_ + size_) % Capacity] = value;
        if (size_ < Capacity) {
            ++size_;
        } else {
            head_ = (head_ + 1) % Capacity;
        }
    }

    std::size_t size() const { return size_; }

    class Iterator {
    public:
        Iterator(const RingBuffer* buffer, std::size_t position)
            : buffer_(buffer), position_(position) {}

        const T& operator*() const {
            return buffer_->data_[(buffer_->head_ + position_) % Capacity];
        }
        Iterator& operator++() {
            ++position_;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return position_ != other.position_; }

    private:
        const RingBuffer* buffer_;
        std::size_t position_;
    };

    Iterator begin() const { return Iterator(this, 0); }
    Iterator end() const { return Iterator(this, size_); }

private:
    std::array<T, Capacity> data_{};
    std::size_t head_ = 0;
    std::size_t size_ = 0;
};

int main() {
    RingBuffer<int, 4> numbers;
    for (int i = 1; i <= 6; ++i) {
        numbers.push(i);
    }

    std::println("numbers (capacity 4, pushed 1..6):");
    for (int value : numbers) {
        std::println("  {}", value);
    }

    RingBuffer<std::string, 3> words;
    words.push("alpha");
    words.push("beta");

    std::println("words ({} stored):", words.size());
    for (const auto& word : words) {
        std::println("  {}", word);
    }
    return 0;
}
