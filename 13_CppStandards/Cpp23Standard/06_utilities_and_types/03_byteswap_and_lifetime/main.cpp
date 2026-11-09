/*
 * std::byteswap (<bit>), std::start_lifetime_as (<memory>) (C++23)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <print>
#include <vector>

struct Header {
    std::uint32_t magic;
    std::uint16_t version;
    std::uint16_t flags;
};

int main() {
    // === EXAMPLE 1: byteswap ===
    std::println("=== Example 1: byteswap ===");
    std::uint32_t value = 0x11223344;
    std::uint32_t swapped = std::byteswap(value);
    std::println("{:#010x} -> {:#010x}", value, swapped);
    std::println("16 bit: {:#06x} -> {:#06x}", std::uint16_t{0xAABB}, std::byteswap(std::uint16_t{0xAABB}));

    // network order (big endian) to host order
    std::uint32_t network = 0x0001'0000;   // the bytes 00 00 01 00 (256 in big endian), read on a little endian machine
    std::uint32_t host = (std::endian::native == std::endian::little) ? std::byteswap(network) : network;
    std::println("host value: {}", host);

    // === EXAMPLE 2: start_lifetime_as ===
    std::println("");
    std::println("=== Example 2: start_lifetime_as ===");
    // A byte buffer (for example read from a file or a socket)
    alignas(Header) std::byte raw[sizeof(Header)];
    Header source{0xCAFEBABE, 3, 7};
    std::memcpy(raw, &source, sizeof(Header));

#if defined(__cpp_lib_start_lifetime_as)
    // Tells the compiler "an object of this type lives here" - no copy, no undefined behaviour
    const Header* header = std::start_lifetime_as<Header>(raw);
    std::println("magic {:#x}, version {}, flags {}", header->magic, header->version, header->flags);
#else
    // Fallback: copy into a real object (always valid)
    Header header;
    std::memcpy(&header, raw, sizeof(Header));
    std::println("(start_lifetime_as not available in this library, used memcpy) magic {:#x}, version {}",
                 header.magic, header.version);
#endif
    return 0;
}
