/*
 * std::ispanstream, std::ospanstream, std::spanstream (C++23, <spanstream>)
 *
 * Stream classes that read from / write into a caller-owned buffer.
 * No allocation happens, unlike std::stringstream.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <print>
#include <span>
#include <spanstream>
#include <string>

int main() {
    // === EXAMPLE 1: ospanstream writes into a fixed buffer ===
    std::println("=== Example 1: ospanstream ===");
    std::array<char, 32> buffer{};
    std::ospanstream out{std::span<char>(buffer)};
    out << "id=" << 42 << " ok";
    auto written = out.span();   // the part of the buffer that was written
    std::println("wrote {} characters: {}", written.size(),
                 std::string(written.data(), written.size()));

    // === EXAMPLE 2: ispanstream reads from existing memory ===
    std::println("");
    std::println("=== Example 2: ispanstream ===");
    char text[] = "10 20 30";
    std::ispanstream in{std::span<char>(text)};
    int a = 0;
    int b = 0;
    int c = 0;
    in >> a >> b >> c;
    std::println("read {} {} {}, sum {}", a, b, c, a + b + c);

    // === EXAMPLE 3: output that does not fit ===
    std::println("");
    std::println("=== Example 3: buffer too small ===");
    std::array<char, 4> tiny{};
    std::ospanstream small{std::span<char>(tiny)};
    small << "too long for this buffer";
    std::println("stream is good: {}", small.good());
    return 0;
}
