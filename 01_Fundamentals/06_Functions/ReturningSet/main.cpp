#include <iostream>
#include <set>
#include <tuple>


std::set<std::tuple<int, int>> CoorPoints = {
    {-1,-1},  {0,-1},  {1,-1},
    {-1, 0},  {0, 0},  {1, 0},
    {-1, 1},  {0, 1},  {1, 1}
};



int main() {
    // Print the kernel matrix
    std::cout << "Blurring Kernel:" << std::endl;
    for (const auto& point : CoorPoints) {
        std::cout << "(" << std::get<0>(point) << ", " << std::get<1>(point) << ")" << std::endl;

    }

    return 0;
}
