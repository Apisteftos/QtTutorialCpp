#include <iostream>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <climits>
#include <print>

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
//  10.  Practical: protocol buffer, MCX frame, hardware register
// ============================================================

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Why Fixed-Width Integers?\n";
    std::cout << "=====================================================\n";

    std::cout << "Platform-dependent sizes (can vary!):\n";
    std::cout << "  sizeof(short):    " << sizeof(short)     << " bytes\n";
    std::cout << "  sizeof(int):      " << sizeof(int)       << " bytes\n";
    std::cout << "  sizeof(long):     " << sizeof(long)      << " bytes\n";
    std::cout << "  sizeof(long long):" << sizeof(long long) << " bytes\n";

    std::cout << "\nFixed-width sizes (always guaranteed):\n";
    std::cout << "  sizeof(int8_t):   " << sizeof(int8_t)   << " bytes (always 1)\n";
    std::cout << "  sizeof(int16_t):  " << sizeof(int16_t)  << " bytes (always 2)\n";
    std::cout << "  sizeof(int32_t):  " << sizeof(int32_t)  << " bytes (always 4)\n";
    std::cout << "  sizeof(int64_t):  " << sizeof(int64_t)  << " bytes (always 8)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Signed Fixed-Width Types\n";
    std::cout << "=====================================================\n";

    int8_t  i8  = 127;
    int16_t i16 = 32767;
    int32_t i32 = 2147483647;
    int64_t i64 = 9223372036854775807LL;

    std::println("int8_t   (1 byte):  {}", i8);
    std::println("int16_t  (2 bytes): {}", i16);
    std::println("int32_t  (4 bytes): {}", i32);
    std::println("int64_t  (8 bytes): {}", i64);

    // Overflow behavior (wraps around for signed — UB, but shown for awareness)
    int8_t overflow = 127;
    overflow++;  // wraps to -128 (undefined behavior — shown for education)
    std::println("int8_t 127+1 = {} (wraps to -128!)", overflow);

    // Casting
    int32_t big  = 300;
    int8_t  small = static_cast<int8_t>(big);   // truncates
    std::println("(int8_t)300 = {} (truncated)", small);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Unsigned Fixed-Width Types\n";
    std::cout << "=====================================================\n";

    uint8_t  u8  = 255;
    uint16_t u16 = 65535;
    uint32_t u32 = 4294967295U;
    uint64_t u64 = 18446744073709551615ULL;

    // uint8_t prints as char — cast to int for numeric output
    std::println("uint8_t  (1 byte):  {}", (unsigned)u8);
    std::println("uint16_t (2 bytes): {}", u16);
    std::println("uint32_t (4 bytes): {}", u32);
    std::println("uint64_t (8 bytes): {}", u64);

    // uint8_t is commonly used as a raw byte
    uint8_t byte = 0b10110100;
    std::cout << "byte (binary): ";
    for (int i = 7; i >= 0; --i)
        std::cout << ((byte >> i) & 1);
    std::cout << " = " << (unsigned)byte << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Minimum-Width Types (int_least)\n";
    std::cout << "=====================================================\n";

    // int_leastN_t — at least N bits, smallest type available
    int_least8_t  l8  = 100;
    int_least16_t l16 = 1000;
    int_least32_t l32 = 100000;
    int_least64_t l64 = 1000000000LL;

    std::cout << "int_least8_t  size: " << sizeof(l8)  << " bytes\n";
    std::cout << "int_least16_t size: " << sizeof(l16) << " bytes\n";
    std::cout << "int_least32_t size: " << sizeof(l32) << " bytes\n";
    std::cout << "int_least64_t size: " << sizeof(l64) << " bytes\n";
    std::cout << "(may be larger than minimum on some platforms)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Fastest Types (int_fast)\n";
    std::cout << "=====================================================\n";

    // int_fastN_t — at least N bits, fastest type on this platform
    int_fast8_t  f8  = 42;
    int_fast16_t f16 = 1000;
    int_fast32_t f32 = 100000;
    int_fast64_t f64 = 1000000LL;

    std::cout << "int_fast8_t  size: " << sizeof(f8)  << " bytes\n";
    std::cout << "int_fast16_t size: " << sizeof(f16) << " bytes\n";
    std::cout << "int_fast32_t size: " << sizeof(f32) << " bytes\n";
    std::cout << "int_fast64_t size: " << sizeof(f64) << " bytes\n";
    std::cout << "(may be larger for CPU register alignment)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Pointer-Sized Types\n";
    std::cout << "=====================================================\n";

    // intptr_t / uintptr_t — same size as a pointer
    int  x   = 42;
    int* ptr = &x;

    intptr_t  ip = reinterpret_cast<intptr_t>(ptr);
    uintptr_t up = reinterpret_cast<uintptr_t>(ptr);

    std::cout << "pointer size:       " << sizeof(ptr)      << " bytes\n";
    std::cout << "intptr_t size:      " << sizeof(intptr_t) << " bytes\n";
    std::cout << "pointer as intptr:  " << std::hex << ip   << std::dec << '\n';
    std::cout << "pointer as uintptr: " << std::hex << up   << std::dec << '\n';

    // ptrdiff_t — difference between two pointers
    int arr[5] = {10,20,30,40,50};
    ptrdiff_t diff = &arr[4] - &arr[0];
    std::cout << "ptrdiff_t:          " << diff << " elements\n";

    // size_t — result of sizeof, index type
    std::size_t sz = sizeof(double);
    std::cout << "size_t sizeof(double): " << sz << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Maximum Width Types\n";
    std::cout << "=====================================================\n";

    intmax_t  imax =  9223372036854775807LL;
    uintmax_t umax = 18446744073709551615ULL;

    std::cout << "intmax_t  size: " << sizeof(intmax_t)  << " bytes\n";
    std::cout << "uintmax_t size: " << sizeof(uintmax_t) << " bytes\n";
    std::println("intmax_t  max:  {}", imax);
    std::println("uintmax_t max:  {}", umax);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Compile-Time Limits (macros)\n";
    std::cout << "=====================================================\n";

    std::cout << "INT8_MIN:   " << (int)INT8_MIN   << '\n';
    std::cout << "INT8_MAX:   " << (int)INT8_MAX   << '\n';
    std::cout << "UINT8_MAX:  " << (unsigned)UINT8_MAX  << '\n';
    std::cout << "INT16_MIN:  " << INT16_MIN  << '\n';
    std::cout << "INT16_MAX:  " << INT16_MAX  << '\n';
    std::cout << "INT32_MIN:  " << INT32_MIN  << '\n';
    std::cout << "INT32_MAX:  " << INT32_MAX  << '\n';
    std::cout << "UINT32_MAX: " << UINT32_MAX << '\n';
    std::cout << "INT64_MAX:  " << INT64_MAX  << '\n';
    std::cout << "UINT64_MAX: " << UINT64_MAX << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: std::numeric_limits<T>\n";
    std::cout << "=====================================================\n";

    std::cout << "int32_t:\n";
    std::println("  min:    {}", std::numeric_limits<int32_t>::min());
    std::println("  max:    {}", std::numeric_limits<int32_t>::max());
    std::println("  digits: {}", std::numeric_limits<int32_t>::digits);

    std::cout << "uint64_t:\n";
    std::println("  min:    {}", std::numeric_limits<uint64_t>::min());
    std::println("  max:    {}", std::numeric_limits<uint64_t>::max());

    std::cout << "float:\n";
    std::println("  min:     {}", std::numeric_limits<float>::min());
    std::println("  max:     {}", std::numeric_limits<float>::max());
    std::println("  epsilon: {}", std::numeric_limits<float>::epsilon());

    std::cout << "double:\n";
    std::println("  max:     {}", std::numeric_limits<double>::max());
    std::println("  epsilon: {}", std::numeric_limits<double>::epsilon());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Practical — MCX Protocol Frame\n";
    std::cout << "=====================================================\n";

    // Simulated MCX binary protocol frame
    // Fixed-width integers essential for binary protocol work
    struct McxFrameHeader {
        uint8_t  version;       // 1 byte: protocol version
        uint8_t  messageType;   // 1 byte: message type
        uint16_t length;        // 2 bytes: total frame length
        uint32_t callId;        // 4 bytes: unique call identifier
        uint32_t userId;        // 4 bytes: user identifier
        uint64_t timestamp;     // 8 bytes: Unix timestamp ms
    } __attribute__((packed));  // no padding between fields

    std::cout << "McxFrameHeader size: " << sizeof(McxFrameHeader) << " bytes\n";
    std::cout << "(version=1, type=1, length=2, callId=4, userId=4, ts=8 = 20 bytes)\n";

    McxFrameHeader frame;
    frame.version     = 0x01;
    frame.messageType = 0x10;   // GROUP_CALL_REQUEST
    frame.length      = sizeof(McxFrameHeader);
    frame.callId      = 12345U;
    frame.userId      = 7U;
    frame.timestamp   = 1712917200000ULL;  // example ms timestamp

    uint8_t  ver  = frame.version;
    uint8_t  mtype = frame.messageType;
    uint16_t len  = frame.length;
    uint32_t cid  = frame.callId;
    uint32_t uid  = frame.userId;
    uint64_t ts   = frame.timestamp;

    std::println("version:     0x{:02X}", ver);
    std::println("messageType: 0x{:02X}", mtype);
    std::println("length:      {} bytes", len);
    std::println("callId:      {}", cid);
    std::println("userId:      {}", uid);
    std::println("timestamp:   {}", ts);

    // Hardware register simulation
    struct StatusRegister {
        uint8_t connected  : 1;   // bit 0
        uint8_t active     : 1;   // bit 1
        uint8_t encrypted  : 1;   // bit 2
        uint8_t emergency  : 1;   // bit 3
        uint8_t reserved   : 4;   // bits 4-7
    };

    StatusRegister reg = {};
    reg.connected = 1;
    reg.active    = 1;
    reg.encrypted = 1;
    std::println("\nStatusRegister size: {} byte", sizeof(reg));
    std::println("connected: {}", (int)reg.connected);
    std::println("active:    {}", (int)reg.active);
    std::println("emergency: {}", (int)reg.emergency);

    std::cout << "\n=== CstdintSTL complete ===\n";
    return 0;
}
