#include "mydynlib.h"
#include <iostream>
#include <vector>
#include <print>

// ============================================================
// Dynamic Library Usage Example
// ============================================================
// This program links against libmydynlib.so at BUILD time
// and the OS loads it at RUNTIME.
//
// The program needs at runtime:
//   libmydynlib.so  (in system path or LD_LIBRARY_PATH)
// ============================================================

int main()
{
    MyDynLib::printBanner();

    std::cout << "\n=====================================================\n";
    std::cout << "Math utilities (from shared library)\n";
    std::cout << "=====================================================\n";

    std::println("add(10, 5)        = {}", MyDynLib::add(10, 5));
    std::println("multiply(6, 7)    = {}", MyDynLib::multiply(6, 7));
    std::println("divide(10.0, 4.0) = {:.2f}", MyDynLib::divide(10.0, 4.0));

    try {
        MyDynLib::divide(1.0, 0.0);
    } catch (const std::exception& e) {
        std::println("Exception caught: {}", e.what());
    }


    std::cout << "\n=====================================================\n";
    std::cout << "String utilities (from shared library)\n";
    std::cout << "=====================================================\n";

    std::string s = "  dynamic library demo  ";
    std::println("original:    '{}'", s);
    std::println("trim:        '{}'", MyDynLib::trim(s));
    std::println("toUpper:     '{}'", MyDynLib::toUpperCase(MyDynLib::trim(s)));


    std::cout << "\n=====================================================\n";
    std::cout << "Collection utilities (from shared library)\n";
    std::cout << "=====================================================\n";

    std::vector<int> data = {10, 20, 30, 40, 50};
    std::println("avg([10,20,30,40,50]) = {:.1f}", MyDynLib::avgVector(data));

    std::println("Library version: {}", MyDynLib::version());

    std::cout << "\n=== Dynamic Library example complete ===\n";
    return 0;
}
