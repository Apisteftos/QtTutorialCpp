#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // Example 1: Simple lambda with no capture
    auto greet = []() {
        cout << "Hello from lambda!\n";
    };
    greet();  // Call it like a regular function


    // Example 2: Lambda with parameters
    auto add = [](int a, int b) {
        return a + b;
    };
    cout << "5 + 3 = " << add(5, 3) << "\n";


    // Example 3: Lambda with capture - accessing outside variables
    int multiplier = 10;

    auto multiply = [multiplier](int x) {  // Capture by value
        return x * multiplier;
    };
    cout << "7 * 10 = " << multiply(7) << "\n";


    // Example 4: Capture by reference (can modify the variable)
    int counter = 0;

    auto increment = [&counter]() {  // Capture by reference
        counter++;
    };

    increment();
    increment();
    increment();
    cout << "Counter: " << counter << "\n";  // Output: 3


    // Example 5: Using lambda with STL algorithms
    vector<int> numbers = {5, 2, 8, 1, 9, 3};

    // Sort in descending order using lambda
    sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a > b;  // Compare function
    });

    cout << "Sorted descending: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << "\n";


    // Example 6: Lambda as callback (replacing function pointers)
    vector<int> values = {1, 2, 3, 4, 5};

    // Process each element with a lambda
    for_each(values.begin(), values.end(), [](int x) {
        cout << x * x << " ";  // Print squares
    });
    cout << "\n";


    // Example 7: Capture all by value or reference
    int a = 5, b = 10;

    auto captureAll = [=]() {  // [=] captures ALL variables by value
        cout << "a=" << a << ", b=" << b << "\n";
    };
    captureAll();

    auto captureAllRef = [&]() {  // [&] captures ALL by reference
        a += 10;
        b += 10;
    };
    captureAllRef();
    cout << "After modification: a=" << a << ", b=" << b << "\n";


    // Example 8: Explicit return type
    auto divide = [](double x, double y) -> double {
        if (y == 0) return 0;
        return x / y;
    };
    cout << "10 / 3 = " << divide(10, 3) << "\n";

    return 0;
}
