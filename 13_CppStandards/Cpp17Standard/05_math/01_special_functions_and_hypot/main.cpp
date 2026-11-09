/*
 * Mathematical special functions (<cmath>) and the 3-argument std::hypot
 * (C++17)
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    std::cout << std::fixed << std::setprecision(6);

    // === EXAMPLE 1: hypot with three arguments ===
    std::cout << "=== Example 1: hypot ===\n";
    std::cout << "hypot(3, 4)    = " << std::hypot(3.0, 4.0) << "\n";
    std::cout << "hypot(1, 2, 2) = " << std::hypot(1.0, 2.0, 2.0) << "   (length of a 3D vector)\n";

    // === EXAMPLE 2: special functions ===
    std::cout << "\n=== Example 2: special functions ===\n";
    std::cout << "riemann_zeta(2) = " << std::riemann_zeta(2.0) << "   (pi^2 / 6 = " << (M_PI * M_PI / 6) << ")\n";
    std::cout << "beta(2, 3)      = " << std::beta(2.0, 3.0) << "\n";
    std::cout << "expint(1)       = " << std::expint(1.0) << "\n";
    std::cout << "erf(1) (C++11)  = " << std::erf(1.0) << "\n";
    std::cout << "legendre(2, 0.5)= " << std::legendre(2, 0.5) << "\n";
    std::cout << "ellint_1(0, pi/2)= " << std::ellint_1(0.0, M_PI / 2) << "   (= pi/2)\n";
    std::cout << "cyl_bessel_j(0, 1) = " << std::cyl_bessel_j(0.0, 1.0) << "\n";
    std::cout << "assoc_laguerre(2, 1, 0.5) = " << std::assoc_laguerre(2, 1, 0.5) << "\n";
    return 0;
}
