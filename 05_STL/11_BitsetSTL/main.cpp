#include <bitset>
#include <cstddef>
#include <cstdint>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>

// ============================================================
// std::bitset — Fixed-Size Bit Array in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  Setting, clearing, flipping bits
//   3.  Accessing bits: [], test, any, all, none, count
//   4.  Bitwise operators: &, |, ^, ~, <<, >>
//   5.  Converting: to_string, to_ulong, to_ullong
//   6.  Practical: permission flags
//   7.  Practical: sieve of Eratosthenes
//   8.  Practical: feature flags / bitmask
//   9.  Practical: connection status flags
//  10.  bitset vs uint32_t bitmask comparison
// ============================================================

// Print the "=====" banner that introduces an example
void banner(std::string_view title)
{
    std::println("");
    std::println("=====================================================");
    std::println("{}", title);
    std::println("=====================================================");
}

int main()
{
    // NOTE: std::bitset has no std::formatter, so it is printed with .to_string()
    // (leftmost character = highest bit, rightmost character = bit 0).

    // === EXAMPLE 1: Declaration and Initialization ===
    banner("Example 1: Declaration and Initialization");

    std::bitset<8> b1;                    // all zeros
    std::bitset<8> b2(0b10110100);        // from integer
    std::bitset<8> b3(0xA5);              // from hex
    std::bitset<8> b4("11001010");        // from string (MSB first)
    std::bitset<8> b5(std::string("10110001"), 0, 8);  // from std::string

    std::println("b1 (zero):   {}", b1.to_string());
    std::println("b2 (0b10110100): {}", b2.to_string());
    std::println("b3 (0xA5):   {}", b3.to_string());
    std::println("b4 (string): {}", b4.to_string());
    std::println("b5 (string): {}", b5.to_string());

    // Size is always compile-time constant
    std::println("size: {} bits", b1.size());


    // === EXAMPLE 2: Setting, Clearing, Flipping Bits ===
    banner("Example 2: Setting, Clearing, Flipping Bits");

    std::bitset<8> b;
    std::println("initial:        {}", b.to_string());

    b.set(3);          // set bit 3 to 1
    b.set(5);
    b.set(7);
    std::println("after set(3,5,7): {}", b.to_string());

    b.set();           // set ALL bits to 1
    std::println("after set():    {}", b.to_string());

    b.reset(3);        // clear bit 3 to 0
    b.reset(5);
    std::println("after reset(3,5): {}", b.to_string());

    b.reset();         // clear ALL bits
    std::println("after reset():  {}", b.to_string());

    b.set(2); b.set(4); b.set(6);
    b.flip(2);         // flip bit 2 (1->0)
    b.flip(3);         // flip bit 3 (0->1)
    std::println("after flip(2,3):{}", b.to_string());

    b.flip();          // flip ALL bits
    std::println("after flip():   {}", b.to_string());


    // === EXAMPLE 3: Accessing and Testing Bits ===
    banner("Example 3: Accessing and Testing Bits");

    std::bitset<8> c("10110100");
    std::println("bitset: {}", c.to_string());

    // operator[] — no bounds check. On a non-const bitset it returns a proxy
    // object, so convert it to bool before printing.
    std::println("c[0] (LSB): {}", static_cast<bool>(c[0]));
    std::println("c[2]:       {}", static_cast<bool>(c[2]));
    std::println("c[7] (MSB): {}", static_cast<bool>(c[7]));

    // test() — bounds checked, throws std::out_of_range
    std::println("test(4):    {}", c.test(4));
    try {
        bool bad = c.test(100);
        (void)bad;
    } catch (const std::out_of_range& e) {
        std::println("test(100) threw: {}", e.what());
    }

    // Count and check
    std::println("count():    {} bits set", c.count());
    std::println("size():     {}", c.size());
    std::println("any():      {} (any bit set?)", c.any());
    std::println("all():      {} (all bits set?)", c.all());
    std::println("none():     {} (no bits set?)", c.none());

    std::bitset<8> allSet; allSet.set();
    std::println("allSet.all():  {}", allSet.all());
    std::println("allSet.none(): {}", allSet.none());


    // === EXAMPLE 4: Bitwise Operators ===
    banner("Example 4: Bitwise Operators");

    std::bitset<8> x("11001010");
    std::bitset<8> y("10110101");

    std::println("x:     {}", x.to_string());
    std::println("y:     {}", y.to_string());
    std::println("x & y: {} (AND)", (x & y).to_string());
    std::println("x | y: {} (OR)", (x | y).to_string());
    std::println("x ^ y: {} (XOR)", (x ^ y).to_string());
    std::println("~x:    {} (NOT)", (~x).to_string());
    std::println("x<<2:  {} (left shift)", (x << 2).to_string());
    std::println("x>>2:  {} (right shift)", (x >> 2).to_string());

    // In-place
    std::bitset<8> z("11001010");
    z &= y;
    std::println("z &= y: {}", z.to_string());
    z = std::bitset<8>("11001010");
    z |= y;
    std::println("z |= y: {}", z.to_string());
    z = std::bitset<8>("11001010");
    z ^= y;
    std::println("z ^= y: {}", z.to_string());


    // === EXAMPLE 5: Converting to/from integer and string ===
    banner("Example 5: Converting to/from integer and string");

    std::bitset<8> d("10110100");

    // to integer
    std::println("to_ulong():  {}", d.to_ulong());
    std::println("to_ullong(): {}", d.to_ullong());

    // to string
    std::println("to_string(): {}", d.to_string());
    std::println("to_string('O','I'): {}", d.to_string('O', 'I'));  // custom chars

    // from integer (round trip)
    unsigned long val = 42;
    std::bitset<8> fromInt(val);
    std::println("bitset(42):  {} -> {}", fromInt.to_string(), fromInt.to_ulong());


    // === EXAMPLE 6: Permission Flags ===
    banner("Example 6: Permission Flags");

    // Unix-style file permissions: rwxrwxrwx (9 bits)
    // bit 8=owner_r, 7=owner_w, 6=owner_x, 5=group_r...
    enum Perm { OTHER_X=0, OTHER_W, OTHER_R,
                GROUP_X,   GROUP_W, GROUP_R,
                OWNER_X,   OWNER_W, OWNER_R };

    std::bitset<9> perms;
    perms.set(OWNER_R);
    perms.set(OWNER_W);
    perms.set(OWNER_X);
    perms.set(GROUP_R);
    perms.set(GROUP_X);
    perms.set(OTHER_R);

    std::println("permissions (rwxrwxrwx): {}", perms.to_string());
    std::println("octal value: {:o}", perms.to_ulong());   // 754 = rwxr-xr--

    std::println("owner read:  {}", perms.test(OWNER_R));
    std::println("other write: {}", perms.test(OTHER_W));


    // === EXAMPLE 7: Sieve of Eratosthenes ===
    banner("Example 7: Sieve of Eratosthenes");

    constexpr int LIMIT = 50;
    std::bitset<LIMIT + 1> isPrime;
    isPrime.set();        // assume all prime
    isPrime.reset(0);     // 0 is not prime
    isPrime.reset(1);     // 1 is not prime

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (isPrime.test(i)) {
            for (int j = i * i; j <= LIMIT; j += i) {
                isPrime.reset(j);
            }
        }
    }

    std::println("Primes up to {}:", LIMIT);
    std::print("  ");
    for (int i = 2; i <= LIMIT; ++i) {
        if (isPrime.test(i)) {
            std::print("{} ", i);
        }
    }
    std::println("");
    std::println("Count: {}", isPrime.count());


    // === EXAMPLE 8: Feature Flags ===
    banner("Example 8: Feature Flags");

    enum Feature {
        FEATURE_DARK_MODE     = 0,
        FEATURE_NOTIFICATIONS = 1,
        FEATURE_ANALYTICS     = 2,
        FEATURE_BETA_UI       = 3,
        FEATURE_OFFLINE_MODE  = 4
    };

    std::bitset<8> features;
    features.set(FEATURE_DARK_MODE);
    features.set(FEATURE_NOTIFICATIONS);
    features.set(FEATURE_OFFLINE_MODE);

    std::println("feature flags: {}", features.to_string());

    auto hasFeature = [&](Feature f) { return features.test(f); };
    std::println("dark mode:     {}", hasFeature(FEATURE_DARK_MODE));
    std::println("analytics:     {}", hasFeature(FEATURE_ANALYTICS));
    std::println("offline mode:  {}", hasFeature(FEATURE_OFFLINE_MODE));

    // Toggle feature
    features.flip(FEATURE_DARK_MODE);
    std::println("dark mode after toggle: {}", hasFeature(FEATURE_DARK_MODE));


    // === EXAMPLE 9: Connection Status Flags ===
    banner("Example 9: Connection Status Flags");

    // A status byte of a network connection
    enum ConnStatus {
        STATUS_ACTIVE        = 0,
        STATUS_AUTHENTICATED = 1,
        STATUS_RETRYING      = 2,
        STATUS_ERROR         = 3,
        STATUS_LOGGING       = 4,
        STATUS_ENCRYPTED     = 5,
        STATUS_COMPRESSED    = 6,
        STATUS_CONNECTED     = 7
    };

    std::bitset<8> connStatus;
    connStatus.set(STATUS_CONNECTED);
    connStatus.set(STATUS_ACTIVE);
    connStatus.set(STATUS_ENCRYPTED);

    std::println("status byte: 0x{:x} = {}", connStatus.to_ulong(), connStatus.to_string());

    std::println("connected:  {}", connStatus.test(STATUS_CONNECTED));
    std::println("error:      {}", connStatus.test(STATUS_ERROR));
    std::println("encrypted:  {}", connStatus.test(STATUS_ENCRYPTED));

    // An error occurs while authenticated
    connStatus.set(STATUS_ERROR);
    connStatus.set(STATUS_AUTHENTICATED);
    std::println("status after error: {}", connStatus.to_string());
    std::println("active bits: {}", connStatus.count());


    // === EXAMPLE 10: bitset vs uint32_t bitmask ===
    banner("Example 10: bitset vs uint32_t bitmask");

    // Traditional C-style bitmask
    std::uint32_t flags = 0;
    flags |= (1u << 3);   // set bit 3
    flags |= (1u << 5);
    bool bit3 = (flags >> 3) & 1;
    std::println("uint32_t bitmask:  0x{:x} bit3={}", flags, bit3);

    // std::bitset equivalent
    std::bitset<32> bFlags;
    bFlags.set(3);
    bFlags.set(5);
    std::println("std::bitset<32>:   0x{:x} bit3={}", bFlags.to_ulong(), bFlags.test(3));

    std::println("");
    std::println("=== BitsetSTL complete ===");
    return 0;
}
