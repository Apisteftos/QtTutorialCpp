#include "mylibrary.h"
#include <iostream>
#include <vector>
#include <print>

// ============================================================
// Static Library Usage Example
// ============================================================
// This program links against libmylibrary.a
// It only needs:  mylibrary.h  (declarations)
//                 libmylibrary.a (compiled code)
// It does NOT need mylibrary.cpp
// ============================================================

int main()
{
    MyLib::printBanner();

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Math utilities\n";
    std::cout << "=====================================================\n";

    std::println("add(10, 5)         = {}", MyLib::add(10, 5));
    std::println("subtract(10, 5)    = {}", MyLib::subtract(10, 5));
    std::println("multiply(6, 7)     = {}", MyLib::multiply(6, 7));
    std::println("divide(10.0, 3.0)  = {:.4f}", MyLib::divide(10.0, 3.0));
    std::println("clamp(150, 0, 100) = {}", MyLib::clamp(150, 0, 100));
    std::println("power(2, 10)       = {}", MyLib::power(2.0, 10));

    try {
        MyLib::divide(5.0, 0.0);
    } catch (const std::exception& e) {
        std::println("Exception: {}", e.what());
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "String utilities\n";
    std::cout << "=====================================================\n";

    std::string s = "  Hello, MCXApp!  ";
    std::println("original:     '{}'", s);
    std::println("trim:         '{}'", MyLib::trim(s));
    std::println("toUpperCase:  '{}'", MyLib::toUpperCase(MyLib::trim(s)));
    std::println("toLowerCase:  '{}'", MyLib::toLowerCase(MyLib::trim(s)));

    std::string url = "https://bdbos.de";
    std::println("startsWith https: {}", MyLib::startsWith(url, "https"));
    std::println("endsWith   .de:   {}", MyLib::endsWith(url, ".de"));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Collection utilities\n";
    std::cout << "=====================================================\n";

    std::vector<int> data = {3, 1, 8, 5, 2, 9, 4, 7, 6};
    std::println("data:  3 1 8 5 2 9 4 7 6");
    std::println("sum:   {}", MyLib::sumVector(data));
    std::println("avg:   {:.2f}", MyLib::avgVector(data));
    std::println("max:   {}", MyLib::maxVector(data));
    std::println("min:   {}", MyLib::minVector(data));

    std::cout << "\n=== Static Library example complete ===\n";
    return 0;
}
