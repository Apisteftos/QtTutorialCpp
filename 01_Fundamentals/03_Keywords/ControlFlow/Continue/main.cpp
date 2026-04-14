#include <iostream>

int main() {
    std::cout << "Printing odd numbers between 1 and 10:\n";

    for (int i = 1; i <= 10; i++) {
        // If i is even, skip the rest of the loop body
        if (i % 2 == 0) {
            continue;
        }

        // This line only executes if i is odd
        std::cout << i << " ";
    }

    std::cout << "\nLoop finished!" << std::endl;
    return 0;
}
