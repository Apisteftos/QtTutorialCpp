#include <iostream>

int main() {
    int magicNumber = 7;

    std::cout << "Searching for the magic number (7) in a loop from 1 to 100...\n";

    for (int i = 1; i <= 100; i++) {
        if (i == magicNumber) {
            std::cout << "Found " << i << "! Breaking out of the loop.\n";
            break; // The loop stops here immediately
        }

        std::cout << "Checking: " << i << "...\n";
    }

    std::cout << "The loop is over. The program continues here." << std::endl;
    return 0;
}
