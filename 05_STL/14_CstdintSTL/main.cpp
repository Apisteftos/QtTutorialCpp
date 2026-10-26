#include <cstddef>
#include <cstdint>
#include <limits>
#include <print>
#include <string_view>

// ============================================================
// std::cstdint — Fixed-Width Integer Types in C++23
// ============================================================
// Topics covered:
//   1.  Why fixed-width integers exist
//   2.  Exact-width types: int8_t, int16_t, int32_t, int64_t
//   3.  Unsigned types: uint8_t, uint16_t, uint32_t, uint64_t
//   4.  Minimum-width types: int_least8_t etc.
//   5.  Fastest types: int_fast8_t etc.
//   6.  Pointer-sized types: intptr_t, uintptr_t
//   7.  Max-width: intmax_t, uintmax_t
//   8.  Limits: INT8_MAX, UINT32_MAX etc.
//   9.  std::numeric_limits<T>
//  10.  Practical: binary packet header, hardware register
// ============================================================

namespace
{
    void banner(std::string_view title)
    {
        std::println("=====================================================");
        std::println("{}", title);
        std::println("=====================================================");
    }
}

int main()
{
    // === EXAMPLE 1: Why Fixed-Width Integers? ===
    banner("Example 1: Why Fixed-Width Integers?");

    std::println("Platform-dependent sizes (can vary!):");
    std::println("  sizeof(short):    {} bytes", sizeof(short));
    std::println("  sizeof(int):      {} bytes", sizeof(int));
    std::println("  sizeof(long):     {} bytes", sizeof(long));
    std::println("  sizeof(long long):{} bytes", sizeof(long long));

    std::println("");
    std::println("Fixed-width sizes (always guaranteed):");
    std::println("  sizeof(int8_t):   {} bytes (always 1)", sizeof(std::int8_t));
    std::println("  sizeof(int16_t):  {} bytes (always 2)", sizeof(std::int16_t));
    std::println("  sizeof(int32_t):  {} bytes (always 4)", sizeof(std::int32_t));
    std::println("  sizeof(int64_t):  {} bytes (always 8)", sizeof(std::int64_t));
    std::println("");


    // === EXAMPLE 2: Signed Fixed-Width Types ===
    banner("Example 2: Signed Fixed-Width Types");

    std::int8_t  i8  = 127;
    std::int16_t i16 = 32767;
    std::int32_t i32 = 2147483647;
    std::int64_t i64 = 9223372036854775807LL;

    std::println("int8_t   (1 byte):  {}", i8);
    std::println("int16_t  (2 bytes): {}", i16);
    std::println("int32_t  (4 bytes): {}", i32);
    std::println("int64_t  (8 bytes): {}", i64);

    // i8++ computes in int (127 + 1 = 128) and converts back to int8_t.
    // Since C++20 that conversion is modular, so the result is -128 (not UB).
    // Real signed overflow (e.g. INT32_MAX + 1 in int) IS undefined behavior.
    std::int8_t overflow = 127;
    overflow++;
    std::println("int8_t 127+1 = {} (wraps to -128!)", overflow);

    // Casting
    std::int32_t big   = 300;
    std::int8_t  small = static_cast<std::int8_t>(big);   // keeps the low 8 bits
    std::println("(int8_t)300 = {} (truncated)", small);
    std::println("");


    // === EXAMPLE 3: Unsigned Fixed-Width Types ===
    banner("Example 3: Unsigned Fixed-Width Types");

    std::uint8_t  u8  = 255;
    std::uint16_t u16 = 65535;
    std::uint32_t u32 = 4294967295U;
    std::uint64_t u64 = 18446744073709551615ULL;

    // std::println formats uint8_t as a number (only plain char is a character);
    // with std::cout << u8 it would print a character, so no cast is needed here.
    std::println("uint8_t  (1 byte):  {}", u8);
    std::println("uint16_t (2 bytes): {}", u16);
    std::println("uint32_t (4 bytes): {}", u32);
    std::println("uint64_t (8 bytes): {}", u64);

    // uint8_t is commonly used as a raw byte
    std::uint8_t byte = 0b10110100;
    std::print("byte (binary): ");
    for (int i = 7; i >= 0; --i)
        std::print("{}", (byte >> i) & 1);
    std::println(" = {}", byte);
    std::println("");


    // === EXAMPLE 4: Minimum-Width Types (int_least) ===
    banner("Example 4: Minimum-Width Types (int_least)");

    // int_leastN_t — at least N bits, smallest type available
    std::int_least8_t  l8  = 100;
    std::int_least16_t l16 = 1000;
    std::int_least32_t l32 = 100000;
    std::int_least64_t l64 = 1000000000LL;

    std::println("int_least8_t  size: {} bytes", sizeof(l8));
    std::println("int_least16_t size: {} bytes", sizeof(l16));
    std::println("int_least32_t size: {} bytes", sizeof(l32));
    std::println("int_least64_t size: {} bytes", sizeof(l64));
    std::println("");


    // === EXAMPLE 5: Fastest Types (int_fast) ===
    banner("Example 5: Fastest Types (int_fast)");

    // int_fastN_t — at least N bits, "fastest" type chosen by the implementation
    std::int_fast8_t  f8  = 42;
    std::int_fast16_t f16 = 1000;
    std::int_fast32_t f32 = 100000;
    std::int_fast64_t f64 = 1000000LL;

    std::println("int_fast8_t  size: {} bytes", sizeof(f8));
    std::println("int_fast16_t size: {} bytes", sizeof(f16));
    std::println("int_fast32_t size: {} bytes", sizeof(f32));
    std::println("int_fast64_t size: {} bytes", sizeof(f64));
    std::println("");


    // === EXAMPLE 6: Pointer-Sized Types ===
    banner("Example 6: Pointer-Sized Types");

    // intptr_t / uintptr_t — can hold a pointer value (optional types)
    int  x   = 42;
    int* ptr = &x;

    std::intptr_t  ip = reinterpret_cast<std::intptr_t>(ptr);
    std::uintptr_t up = reinterpret_cast<std::uintptr_t>(ptr);

    std::println("pointer size:       {} bytes", sizeof(ptr));
    std::println("intptr_t size:      {} bytes", sizeof(std::intptr_t));
    std::println("pointer as intptr:  {:x}", ip);
    std::println("pointer as uintptr: {:x}", up);

    // ptrdiff_t — difference between two pointers
    int arr[5] = {10, 20, 30, 40, 50};
    std::ptrdiff_t diff = &arr[4] - &arr[0];
    std::println("ptrdiff_t:          {} elements", diff);

    // size_t — result of sizeof, index type
    std::size_t sz = sizeof(double);
    std::println("size_t sizeof(double): {}", sz);
    std::println("");


    // === EXAMPLE 7: Maximum Width Types ===
    banner("Example 7: Maximum Width Types");

    std::intmax_t  imax =  9223372036854775807LL;
    std::uintmax_t umax = 18446744073709551615ULL;

    std::println("intmax_t  size: {} bytes", sizeof(std::intmax_t));
    std::println("uintmax_t size: {} bytes", sizeof(std::uintmax_t));
    std::println("intmax_t  max:  {}", imax);
    std::println("uintmax_t max:  {}", umax);
    std::println("");


    // === EXAMPLE 8: Compile-Time Limits (macros) ===
    banner("Example 8: Compile-Time Limits (macros)");

    // The limit macros are not in namespace std (they are preprocessor macros).
    std::println("INT8_MIN:   {}", INT8_MIN);
    std::println("INT8_MAX:   {}", INT8_MAX);
    std::println("UINT8_MAX:  {}", UINT8_MAX);
    std::println("INT16_MIN:  {}", INT16_MIN);
    std::println("INT16_MAX:  {}", INT16_MAX);
    std::println("INT32_MIN:  {}", INT32_MIN);
    std::println("INT32_MAX:  {}", INT32_MAX);
    std::println("UINT32_MAX: {}", UINT32_MAX);
    std::println("INT64_MAX:  {}", INT64_MAX);
    std::println("UINT64_MAX: {}", UINT64_MAX);
    std::println("");


    // === EXAMPLE 9: std::numeric_limits<T> ===
    banner("Example 9: std::numeric_limits<T>");

    std::println("int32_t:");
    std::println("  min:    {}", std::numeric_limits<std::int32_t>::min());
    std::println("  max:    {}", std::numeric_limits<std::int32_t>::max());
    std::println("  digits: {}", std::numeric_limits<std::int32_t>::digits);

    std::println("uint64_t:");
    std::println("  min:    {}", std::numeric_limits<std::uint64_t>::min());
    std::println("  max:    {}", std::numeric_limits<std::uint64_t>::max());

    std::println("float:");
    std::println("  min:     {}", std::numeric_limits<float>::min());
    std::println("  max:     {}", std::numeric_limits<float>::max());
    std::println("  epsilon: {}", std::numeric_limits<float>::epsilon());

    std::println("double:");
    std::println("  max:     {}", std::numeric_limits<double>::max());
    std::println("  epsilon: {}", std::numeric_limits<double>::epsilon());
    std::println("");


    // === EXAMPLE 10: Practical — Binary Packet Header ===
    banner("Example 10: Practical — Binary Packet Header");

    // Fixed-width integers are essential for binary file/network formats.
    struct PacketHeader {
        std::uint8_t  version;      // 1 byte: format version
        std::uint8_t  type;         // 1 byte: packet type
        std::uint16_t length;       // 2 bytes: total packet length
        std::uint32_t sequence;     // 4 bytes: sequence number
        std::uint32_t deviceId;     // 4 bytes: device identifier
        std::uint64_t timestamp;    // 8 bytes: Unix timestamp ms
    } __attribute__((packed));      // GCC/Clang: no padding between fields

    static_assert(sizeof(PacketHeader) == 20, "header must be exactly 20 bytes");

    std::println("PacketHeader size: {} bytes", sizeof(PacketHeader));

    PacketHeader packet;
    packet.version   = 0x01;
    packet.type      = 0x10;        // example packet type
    packet.length    = sizeof(PacketHeader);
    packet.sequence  = 12345U;
    packet.deviceId  = 7U;
    packet.timestamp = 1712917200000ULL;  // example ms timestamp

    // Copy members of a packed struct into locals before passing them on:
    // you must not bind a reference to a packed member.
    std::uint8_t  ver   = packet.version;
    std::uint8_t  ptype = packet.type;
    std::uint16_t len   = packet.length;
    std::uint32_t seq   = packet.sequence;
    std::uint32_t dev   = packet.deviceId;
    std::uint64_t ts    = packet.timestamp;

    std::println("version:     0x{:02X}", ver);
    std::println("type:        0x{:02X}", ptype);
    std::println("length:      {} bytes", len);
    std::println("sequence:    {}", seq);
    std::println("deviceId:    {}", dev);
    std::println("timestamp:   {}", ts);

    // Hardware register simulation (bit-fields)
    struct StatusRegister {
        std::uint8_t enabled  : 1;   // bit 0
        std::uint8_t ready    : 1;   // bit 1
        std::uint8_t error    : 1;   // bit 2
        std::uint8_t busy     : 1;   // bit 3
        std::uint8_t reserved : 4;   // bits 4-7
    };

    StatusRegister reg = {};
    reg.enabled = 1;
    reg.ready   = 1;
    reg.error   = 1;
    std::println("");
    std::println("StatusRegister size: {} byte", sizeof(reg));
    // println takes its arguments by reference and a bit-field cannot be bound
    // to one, so convert to a normal int first.
    std::println("enabled: {}", static_cast<int>(reg.enabled));
    std::println("ready:   {}", static_cast<int>(reg.ready));
    std::println("busy:    {}", static_cast<int>(reg.busy));

    std::println("");
    std::println("=== CstdintSTL complete ===");
    return 0;
}
