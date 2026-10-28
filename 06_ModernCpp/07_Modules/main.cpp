import Math;
import Geometry;

// stdlib via #include in main.cpp is fine — only module interfaces are problematic
#include <print>

int main()
{
    std::println("=====================================================");
    std::println("C++20 Modules — working on GCC 14");
    std::println("=====================================================");

    std::println("");
    std::println("--- import Math ---");
    std::println("PI              = {:.10f}", Math::PI);
    std::println("EULER           = {:.10f}", Math::EULER);
    std::println("add(10, 5)      = {}", Math::add(10, 5));
    std::println("subtract(10, 5) = {}", Math::subtract(10, 5));
    std::println("multiply(6, 7)  = {}", Math::multiply(6, 7));
    std::println("square(8)       = {}", Math::square(8));
    std::println("clamp(150,0,100)= {}", Math::clamp(150, 0, 100));
    std::println("version         = {}.{}", Math::versionMajor(), Math::versionMinor());

    std::println("");
    std::println("--- import Geometry ---");
    Geometry::Circle    c { {0.0, 0.0}, 5.0 };
    Geometry::Rectangle r { {0.0, 0.0}, 10.0, 4.0 };
    Geometry::Point     a { 0.0, 0.0 };
    Geometry::Point     b { 3.0, 4.0 };

    std::println("Circle area:          {:.4f}", Geometry::circleArea(c));
    std::println("Circle circumference: {:.4f}", Geometry::circleCircumference(c));
    std::println("Rectangle area:       {:.1f}", Geometry::rectangleArea(r));
    std::println("Distance (0,0)→(3,4): {:.1f}", Geometry::distance(a, b));
    return 0;
}
