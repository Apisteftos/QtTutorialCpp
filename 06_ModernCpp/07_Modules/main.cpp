import Math;
import Geometry;

// stdlib via #include in main.cpp is fine — only module interfaces are problematic
#include <iostream>
#include <print>

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "C++20 Modules — working on GCC 14\n";
    std::cout << "=====================================================\n";

    std::cout << "\n--- import Math ---\n";
    std::println("PI              = {:.10f}", Math::PI);
    std::println("EULER           = {:.10f}", Math::EULER);
    std::println("add(10, 5)      = {}", Math::add(10, 5));
    std::println("subtract(10, 5) = {}", Math::subtract(10, 5));
    std::println("multiply(6, 7)  = {}", Math::multiply(6, 7));
    std::println("square(8)       = {}", Math::square(8));
    std::println("clamp(150,0,100)= {}", Math::clamp(150, 0, 100));
    std::println("version         = {}.{}", Math::versionMajor(), Math::versionMinor());

    std::cout << "\n--- import Geometry ---\n";
    Geometry::Circle    c { {0.0, 0.0}, 5.0 };
    Geometry::Rectangle r { {0.0, 0.0}, 10.0, 4.0 };
    Geometry::Point     a { 0.0, 0.0 };
    Geometry::Point     b { 3.0, 4.0 };

    std::println("Circle area:          {:.4f}", Geometry::circleArea(c));
    std::println("Circle circumference: {:.4f}", Geometry::circleCircumference(c));
    std::println("Rectangle area:       {:.1f}", Geometry::rectangleArea(r));
    std::println("Distance (0,0)→(3,4): {:.1f}", Geometry::distance(a, b));

    std::cout << "\n--- GCC 14 Module limitation ---\n";
    std::cout << "Module interfaces cannot #include stdlib headers on GCC 14.\n";
    std::cout << "Workaround: use only built-in types (int, double) in .ixx\n";
    std::cout << "            use #include in main.cpp — works fine\n";
    std::cout << "Full stdlib support: MSVC or wait for GCC 15\n";

    std::cout << "\n=== Modules complete ===\n";
    return 0;
}
