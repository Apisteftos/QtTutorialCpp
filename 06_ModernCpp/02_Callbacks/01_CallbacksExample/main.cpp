#include <iostream>
#include <vector>
#include <functional>
using namespace std;

// Example 1: Using a function pointer as callback
void processArray(int arr[], int size, void (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);  // Call the callback function for each element
    }
}

void printDouble(int x) {
    cout << x * 2 << " ";
}

void printSquare(int x) {
    cout << x * x << " ";
}

// Example 2: Using std::function (more flexible)
void processVector(const vector<int>& vec, function<void(int)> callback) {
    for (int num : vec) {
        callback(num);
    }
}

int main() {
    // Example 1: Function pointer callbacks
    int numbers[] = {1, 2, 3, 4, 5};

    cout << "Doubled: ";
    processArray(numbers, 5, printDouble);  // Pass printDouble as callback
    cout << "\n";

    cout << "Squared: ";
    processArray(numbers, 5, printSquare);  // Pass printSquare as callback
    cout << "\n";

    // Example 2: Using lambda as callback (modern C++)
    vector<int> vec = {1, 2, 3, 4, 5};

    cout << "Custom operation: ";
    processVector(vec, [](int x) {
        cout << (x * x + x) << " ";  // Inline callback logic
    });
    cout << "\n";

    return 0;
}
