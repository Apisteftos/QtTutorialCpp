#include <iostream>

int main() {
    std::cout << "Starting nested loops...\n";

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i == 2 && j == 2) {
                std::cout << "Condition met at i=2, j=2. Jumping to end.\n";
                goto end_of_loops; // Jump to the label
            }
            std::cout << "(" << i << "," << j << ") ";
        }
        std::cout << "\n";
    }

    // This label marks the destination of the jump
end_of_loops:
    std::cout << "\nSuccessfully jumped out of all loops!" << std::endl;

    return 0;
}
