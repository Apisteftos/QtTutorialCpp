#include <iostream>

int main() {
    int choice;

    do {
        std::cout << "\n--- System Menu ---\n";
        std::cout << "1. Send Message\n";
        std::cout << "2. Receive Message\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Action: Sending...\n";
        } else if (choice == 2) {
            std::cout << "Action: Receiving...\n";
        } else if (choice != 0) {
            std::cout << "Invalid choice, try again.\n";
        }

    } while (choice != 0); // Condition is checked AFTER the code runs

    std::cout << "Exiting program. Goodbye!" << std::endl;
    return 0;
}
