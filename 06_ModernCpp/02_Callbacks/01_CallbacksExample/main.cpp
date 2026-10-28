#include <vector>
#include <functional>
#include <print>

// Example 1: Using a function pointer as callback
void processArray(int arr[], int size, void (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);  // Call the callback function for each element
    }
}

void printDouble(int x) {
    std::print("{} ", x * 2);
}

void printSquare(int x) {
    std::print("{} ", x * x);
}

// Example 2: Using std::function (more flexible)
void processVector(const std::vector<int>& vec, std::function<void(int)> callback) {
    for (int num : vec) {
        callback(num);
    }
}

int main() {
    // Example 1: Function pointer callbacks
    int numbers[] = {1, 2, 3, 4, 5};

    std::print("Doubled: ");
    processArray(numbers, 5, printDouble);  // Pass printDouble as callback
    std::println("");

    std::print("Squared: ");
    processArray(numbers, 5, printSquare);  // Pass printSquare as callback
    std::println("");

    // Example 2: Using lambda as callback (modern C++)
    std::vector<int> vec = {1, 2, 3, 4, 5};

    std::print("Custom operation: ");
    processVector(vec, [](int x) {
        std::print("{} ", (x * x + x));  // Inline callback logic
    });
    std::println("");

    return 0;
}
