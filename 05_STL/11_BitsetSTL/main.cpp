#include <iostream>
#include <bitset>
#include <string>
#include <print>

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
//   9.  Practical: MCX status flags
//  10.  bitset vs uint32_t bitmask comparison
// ============================================================

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    std::bitset<8> b1;                    // all zeros
    std::bitset<8> b2(0b10110100);        // from integer
    std::bitset<8> b3(0xA5);             // from hex
    std::bitset<8> b4("11001010");       // from string (MSB first)
    std::bitset<8> b5(std::string("10110001"), 0, 8);  // from std::string

    std::cout << "b1 (zero):   " << b1 << '\n';
    std::cout << "b2 (0b10110100): " << b2 << '\n';
    std::cout << "b3 (0xA5):   " << b3 << '\n';
    std::cout << "b4 (string): " << b4 << '\n';
    std::cout << "b5 (string): " << b5 << '\n';

    // Size is always compile-time constant
    std::cout << "size: " << b1.size() << " bits\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Setting, Clearing, Flipping Bits\n";
    std::cout << "=====================================================\n";

    std::bitset<8> b;
    std::cout << "initial:        " << b << '\n';

    b.set(3);          // set bit 3 to 1
    b.set(5);
    b.set(7);
    std::cout << "after set(3,5,7): " << b << '\n';

    b.set();           // set ALL bits to 1
    std::cout << "after set():    " << b << '\n';

    b.reset(3);        // clear bit 3 to 0
    b.reset(5);
    std::cout << "after reset(3,5): " << b << '\n';

    b.reset();         // clear ALL bits
    std::cout << "after reset():  " << b << '\n';

    b.set(2); b.set(4); b.set(6);
    b.flip(2);         // flip bit 2 (1->0)
    b.flip(3);         // flip bit 3 (0->1)
    std::cout << "after flip(2,3):" << b << '\n';

    b.flip();          // flip ALL bits
    std::cout << "after flip():   " << b << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Accessing and Testing Bits\n";
    std::cout << "=====================================================\n";

    std::bitset<8> c("10110100");
    std::cout << "bitset: " << c << '\n';

    // operator[] — no bounds check
    std::cout << "c[0] (LSB): " << c[0] << '\n';
    std::cout << "c[2]:       " << c[2] << '\n';
    std::cout << "c[7] (MSB): " << c[7] << '\n';

    // test() — bounds checked, throws std::out_of_range
    std::cout << "test(4):    " << c.test(4) << '\n';
    try {
        c.test(100);
    } catch (const std::out_of_range& e) {
        std::cout << "test(100) threw: " << e.what() << '\n';
    }

    // Count and check
    std::cout << "count():    " << c.count() << " bits set\n";
    std::cout << "size():     " << c.size()  << '\n';
    std::cout << "any():      " << c.any()   << " (any bit set?)\n";
    std::cout << "all():      " << c.all()   << " (all bits set?)\n";
    std::cout << "none():     " << c.none()  << " (no bits set?)\n";

    std::bitset<8> allSet; allSet.set();
    std::cout << "allSet.all():  " << allSet.all()  << '\n';
    std::cout << "allSet.none(): " << allSet.none() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Bitwise Operators\n";
    std::cout << "=====================================================\n";

    std::bitset<8> x("11001010");
    std::bitset<8> y("10110101");

    std::cout << "x:     " << x << '\n';
    std::cout << "y:     " << y << '\n';
    std::cout << "x & y: " << (x & y) << " (AND)\n";
    std::cout << "x | y: " << (x | y) << " (OR)\n";
    std::cout << "x ^ y: " << (x ^ y) << " (XOR)\n";
    std::cout << "~x:    " << (~x)    << " (NOT)\n";
    std::cout << "x<<2:  " << (x<<2)  << " (left shift)\n";
    std::cout << "x>>2:  " << (x>>2)  << " (right shift)\n";

    // In-place
    std::bitset<8> z("11001010");
    z &= y;
    std::cout << "z &= y: " << z << '\n';
    z = std::bitset<8>("11001010");
    z |= y;
    std::cout << "z |= y: " << z << '\n';
    z = std::bitset<8>("11001010");
    z ^= y;
    std::cout << "z ^= y: " << z << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Converting to/from integer and string\n";
    std::cout << "=====================================================\n";

    std::bitset<8> d("10110100");

    // to integer
    std::cout << "to_ulong():  " << d.to_ulong()  << '\n';
    std::cout << "to_ullong(): " << d.to_ullong() << '\n';

    // to string
    std::cout << "to_string(): " << d.to_string() << '\n';
    std::cout << "to_string('O','I'): " << d.to_string('O', 'I') << '\n';  // custom chars

    // from integer (round trip)
    unsigned long val = 42;
    std::bitset<8> fromInt(val);
    std::cout << "bitset(42):  " << fromInt
              << " -> " << fromInt.to_ulong() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Permission Flags\n";
    std::cout << "=====================================================\n";

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

    std::cout << "permissions (rwxrwxrwx): " << perms << '\n';
    std::cout << "octal value: " << perms.to_ulong() << '\n';

    std::cout << "owner read:  " << perms.test(OWNER_R) << '\n';
    std::cout << "other write: " << perms.test(OTHER_W) << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Sieve of Eratosthenes\n";
    std::cout << "=====================================================\n";

    const int LIMIT = 50;
    std::bitset<LIMIT + 1> isPrime;
    isPrime.set();        // assume all prime
    isPrime.reset(0);     // 0 is not prime
    isPrime.reset(1);     // 1 is not prime

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (isPrime.test(i)) {
            for (int j = i * i; j <= LIMIT; j += i)
                isPrime.reset(j);
        }
    }

    std::cout << "Primes up to " << LIMIT << ":\n  ";
    for (int i = 2; i <= LIMIT; ++i)
        if (isPrime.test(i)) std::cout << i << ' ';
    std::cout << '\n';
    std::cout << "Count: " << isPrime.count() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Feature Flags\n";
    std::cout << "=====================================================\n";

    enum Feature {
        FEATURE_DARK_MODE     = 0,
        FEATURE_NOTIFICATIONS = 1,
        FEATURE_ANALYTICS     = 2,
        FEATURE_BETA_UI       = 3,
        FEATURE_OFFLINE_MODE  = 4,
        FEATURE_COUNT         = 8
    };

    std::bitset<8> features;
    features.set(FEATURE_DARK_MODE);
    features.set(FEATURE_NOTIFICATIONS);
    features.set(FEATURE_OFFLINE_MODE);

    std::cout << "feature flags: " << features << '\n';

    auto hasFeature = [&](Feature f) { return features.test(f); };
    std::cout << "dark mode:     " << hasFeature(FEATURE_DARK_MODE)     << '\n';
    std::cout << "analytics:     " << hasFeature(FEATURE_ANALYTICS)     << '\n';
    std::cout << "offline mode:  " << hasFeature(FEATURE_OFFLINE_MODE)  << '\n';

    // Toggle feature
    features.flip(FEATURE_DARK_MODE);
    std::cout << "dark mode after toggle: " << hasFeature(FEATURE_DARK_MODE) << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: MCX Status Flags\n";
    std::cout << "=====================================================\n";

    // MCX/MCPTT call status word
    enum McxStatus {
        STATUS_CALL_ACTIVE      = 0,
        STATUS_FLOOR_GRANTED    = 1,
        STATUS_FLOOR_REQUESTED  = 2,
        STATUS_EMERGENCY        = 3,
        STATUS_RECORDING        = 4,
        STATUS_ENCRYPTED        = 5,
        STATUS_DUPLEX           = 6,
        STATUS_CONNECTED        = 7
    };

    std::bitset<8> callStatus;
    callStatus.set(STATUS_CONNECTED);
    callStatus.set(STATUS_CALL_ACTIVE);
    callStatus.set(STATUS_ENCRYPTED);

    std::cout << "call status byte: 0x"
              << std::hex << callStatus.to_ulong()
              << std::dec << " = " << callStatus << '\n';

    std::cout << "connected:  " << callStatus.test(STATUS_CONNECTED)  << '\n';
    std::cout << "emergency:  " << callStatus.test(STATUS_EMERGENCY)  << '\n';
    std::cout << "encrypted:  " << callStatus.test(STATUS_ENCRYPTED)  << '\n';

    // Emergency declared
    callStatus.set(STATUS_EMERGENCY);
    callStatus.set(STATUS_FLOOR_GRANTED);
    std::cout << "emergency call status: " << callStatus << '\n';
    std::cout << "active bits: " << callStatus.count() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: bitset vs uint32_t bitmask\n";
    std::cout << "=====================================================\n";

    // Traditional C-style bitmask
    uint32_t flags = 0;
    flags |= (1u << 3);   // set bit 3
    flags |= (1u << 5);
    bool bit3 = (flags >> 3) & 1;
    std::cout << "uint32_t bitmask:  0x" << std::hex << flags
              << std::dec << " bit3=" << bit3 << '\n';

    // std::bitset equivalent
    std::bitset<32> bFlags;
    bFlags.set(3);
    bFlags.set(5);
    std::cout << "std::bitset<32>:   " << bFlags.to_ulong()
              << " bit3=" << bFlags.test(3) << '\n';

    std::cout << "\nbitset advantages:\n";
    std::cout << "  + count() — count set bits easily\n";
    std::cout << "  + any/all/none — readability\n";
    std::cout << "  + string output for debugging\n";
    std::cout << "  + bounds checking with test()\n";
    std::cout << "  + arbitrary size (not limited to 64 bits)\n";
    std::cout << "uint32_t advantages:\n";
    std::cout << "  + arithmetic operations\n";
    std::cout << "  + C interop\n";
    std::cout << "  + runtime-determined size\n";

    std::cout << "\n=== BitsetSTL complete ===\n";
    return 0;
}
