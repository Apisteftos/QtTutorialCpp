/*
 * std::source_location (<source_location>), <bit> header: std::bit_cast,
 * popcount, countl_zero, rotl, has_single_bit, bit_ceil, endian (C++20)
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <bit>
#include <cstdint>
#include <iostream>
#include <source_location>
#include <string>

// The default argument is evaluated at the CALL site
void logMessage(const std::string& text, std::source_location where = std::source_location::current()) {
    std::cout << "[" << where.function_name() << " line " << where.line() << "] " << text << "\n";
}

void worker() {
    logMessage("working");
}

int main() {
    // === EXAMPLE 1: source_location ===
    std::cout << "=== Example 1: source_location ===\n";
    logMessage("from main");
    worker();
    auto here = std::source_location::current();
    std::cout << "file name ends with main.cpp: " << (std::string(here.file_name()).ends_with("main.cpp")) << "\n";

    // === EXAMPLE 2: bit operations ===
    std::cout << "\n=== Example 2: bit operations ===\n";
    std::uint32_t value = 0b0001'0110;
    std::cout << "popcount(0b10110) = " << std::popcount(value) << "\n";
    std::cout << "countl_zero       = " << std::countl_zero(value) << "\n";
    std::cout << "countr_zero       = " << std::countr_zero(value) << "\n";
    std::cout << "has_single_bit(64)= " << std::has_single_bit(64u) << ", (65)= " << std::has_single_bit(65u) << "\n";
    std::cout << "bit_ceil(100)     = " << std::bit_ceil(100u) << "\n";
    std::cout << "bit_floor(100)    = " << std::bit_floor(100u) << "\n";
    std::cout << "bit_width(255)    = " << std::bit_width(255u) << "\n";
    std::cout << "rotl(0x80000001, 1) = 0x" << std::hex << std::rotl(0x80000001u, 1) << std::dec << "\n";

    // === EXAMPLE 3: bit_cast and endian ===
    std::cout << "\n=== Example 3: bit_cast, endian ===\n";
    float f = 1.0f;
    auto bits = std::bit_cast<std::uint32_t>(f);   // reinterpret the bytes, without undefined behaviour
    std::cout << "bits of 1.0f: 0x" << std::hex << bits << std::dec << "\n";
    std::cout << "little endian machine: " << (std::endian::native == std::endian::little) << "\n";
    return 0;
}
