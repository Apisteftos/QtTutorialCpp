#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <print>

// ============================================================
// std::iomanip — I/O Manipulation in C++23
// ============================================================
// Topics covered:
//   1.  setw — field width
//   2.  setfill — fill character
//   3.  left, right, internal alignment
//   4.  setprecision — floating point precision
//   5.  fixed, scientific, defaultfloat
//   6.  boolalpha — bool as text
//   7.  hex, oct, dec, showbase
//   8.  showpos, uppercase
//   9.  Formatted table output
//  10.  C++23 std::print / std::println formatting
// ============================================================

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: setw — Field Width\n";
    std::cout << "=====================================================\n";

    std::cout << std::setw(10) << "hello"   << '\n';  // right-aligned (default)
    std::cout << std::setw(10) << 42        << '\n';
    std::cout << std::setw(10) << 3.14      << '\n';

    // setw only affects the NEXT output
    std::cout << std::setw(5) << 1
              << std::setw(5) << 2
              << std::setw(5) << 3 << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: setfill — Fill Character\n";
    std::cout << "=====================================================\n";

    std::cout << std::setfill('*') << std::setw(10) << "hi"    << '\n';
    std::cout << std::setfill('-') << std::setw(10) << 42      << '\n';
    std::cout << std::setfill('0') << std::setw(8)  << 255     << '\n';  // zero-pad
    std::cout << std::setfill(' ');  // reset to space


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Alignment\n";
    std::cout << "=====================================================\n";

    std::cout << "[" << std::right   << std::setw(10) << "right"    << "]\n";
    std::cout << "[" << std::left    << std::setw(10) << "left"     << "]\n";
    std::cout << "[" << std::internal<< std::setw(10) << -42        << "]\n"; // sign left, value right
    std::cout << std::right;  // reset


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: setprecision — Floating Point\n";
    std::cout << "=====================================================\n";

    double pi = 3.141592653589793;

    std::cout << "default:          " << pi              << '\n';
    std::cout << "setprecision(2):  " << std::setprecision(2)  << pi << '\n';
    std::cout << "setprecision(5):  " << std::setprecision(5)  << pi << '\n';
    std::cout << "setprecision(10): " << std::setprecision(10) << pi << '\n';
    std::cout << std::setprecision(6);  // reset


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: fixed, scientific, defaultfloat\n";
    std::cout << "=====================================================\n";

    double val = 123456.789;

    std::cout << "defaultfloat:  " << std::defaultfloat << val << '\n';
    std::cout << "fixed(2):      " << std::fixed << std::setprecision(2) << val << '\n';
    std::cout << "fixed(4):      " << std::fixed << std::setprecision(4) << val << '\n';
    std::cout << "scientific(2): " << std::scientific << std::setprecision(2) << val << '\n';
    std::cout << "scientific(4): " << std::scientific << std::setprecision(4) << val << '\n';

    // Reset
    std::cout << std::defaultfloat << std::setprecision(6);

    double small = 0.000123456;
    std::cout << "small default:    " << small << '\n';
    std::cout << "small fixed(8):   " << std::fixed << std::setprecision(8) << small << '\n';
    std::cout << std::defaultfloat << std::setprecision(6);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: boolalpha\n";
    std::cout << "=====================================================\n";

    bool t = true, f = false;
    std::cout << "default:   " << t << " " << f << '\n';
    std::cout << "boolalpha: " << std::boolalpha << t << " " << f << '\n';
    std::cout << std::noboolalpha;  // reset


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: hex, oct, dec, showbase\n";
    std::cout << "=====================================================\n";

    int n = 255;
    std::cout << "decimal: " << std::dec     << n << '\n';
    std::cout << "hex:     " << std::hex     << n << '\n';
    std::cout << "octal:   " << std::oct     << n << '\n';
    std::cout << "hex+showbase: " << std::showbase << std::hex << n << '\n';
    std::cout << "oct+showbase: " << std::oct << n << '\n';
    std::cout << std::dec << std::noshowbase;  // reset

    // uppercase hex
    std::cout << "hex upper: " << std::uppercase << std::hex << 0xDEADBEEF
              << std::nouppercase << std::dec << '\n';

    // showpos
    std::cout << "showpos: " << std::showpos << 42 << " " << -42 << std::noshowpos << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Formatted Table Output\n";
    std::cout << "=====================================================\n";

    struct Product {
        std::string name;
        int         qty;
        double      price;
    };

    std::vector<Product> inventory = {
        {"Widget",       150,  2.99},
        {"Gadget",        42, 24.99},
        {"Doohickey",    300,  0.49},
        {"Thingamajig",   15, 99.95}
    };

    // Header
    std::cout << std::left
              << std::setw(16) << "Product"
              << std::right
              << std::setw(8)  << "Qty"
              << std::setw(10) << "Price"
              << std::setw(12) << "Total"
              << '\n';
    std::cout << std::string(46, '-') << '\n';

    double grandTotal = 0;
    for (const auto& p : inventory) {
        double total = p.qty * p.price;
        grandTotal += total;
        std::cout << std::left  << std::setw(16) << p.name
                  << std::right << std::setw(8)  << p.qty
                  << std::fixed << std::setprecision(2)
                  << std::setw(10) << p.price
                  << std::setw(12) << total
                  << '\n';
    }
    std::cout << std::string(46, '-') << '\n';
    std::cout << std::left  << std::setw(16) << "TOTAL"
              << std::right << std::setw(30) << grandTotal << '\n';
    std::cout << std::defaultfloat << std::setprecision(6);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Number formatting utilities\n";
    std::cout << "=====================================================\n";

    // Zero-padded numbers
    std::cout << "Zero padded:    ";
    for (int i = 1; i <= 5; ++i)
        std::cout << std::setfill('0') << std::setw(3) << i << ' ';
    std::cout << '\n';
    std::cout << std::setfill(' ');

    // Hex dump style
    std::cout << "Hex dump:       ";
    unsigned char bytes[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x01, 0xFF};
    for (auto b : bytes)
        std::cout << std::uppercase << std::hex
                  << std::setfill('0') << std::setw(2)
                  << (int)b << ' ';
    std::cout << std::dec << std::nouppercase << std::setfill(' ') << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: C++23 std::print / std::println formatting\n";
    std::cout << "=====================================================\n";

    // std::print is the modern replacement for printf and cout+iomanip
    double pi2 = 3.14159265;
    std::println("default:      {}", pi2);
    std::println("fixed 2dp:    {:.2f}", pi2);
    std::println("scientific:   {:e}", pi2);
    std::println("width 10:     {:10.4f}", pi2);
    std::println("left align:   {:<10.2f}|", pi2);
    std::println("right align:  {:>10.2f}|", pi2);
    std::println("fill zeros:   {:010.2f}", pi2);
    std::println("showpos:      {:+.2f}", pi2);
    std::println("hex int:      {:x}", 255);
    std::println("HEX int:      {:X}", 255);
    std::println("octal:        {:o}", 255);
    std::println("binary:       {:b}", 42);
    std::println("bool:         {}", true);

    std::cout << "\n=== IomanipSTL complete ===\n";
    return 0;
}
