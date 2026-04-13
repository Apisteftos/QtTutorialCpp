#include <iostream>
#include <vector>
#include <map>
#include <list>

int main(int argc, char *argv[])
{


    std::cout << "========================================\n";
    std::cout << "Type 1: Traditional For Loop\n";
    std::cout << "========================================\n";
    // Syntax: for (initialization; condition; increment)

    std::cout << "Example 1a: Basic counting\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "i = " << i << "\n";
    }

    std::cout << "\nExample 1b: Counting backwards\n";
    for (int i = 10; i >= 0; i--) {
        std::cout << i << " ";
    }
    std::cout << "\nBlastoff!\n";

    std::cout << "\nExample 1c: Step by 2\n";
    for (int i = 0; i <= 10; i += 2) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "\nExample 1d: Multiple variables\n";
    for (int i = 0, j = 10; i < 5; i++, j--) {
        std::cout << "i=" << i << ", j=" << j << "\n";
    }

    std::cout << "\nExample 1e: Iterating through array\n";
    int arr[] = {10, 20, 30, 40, 50};
    int size = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < size; i++) {
        std::cout << "arr[" << i << "] = " << arr[i] << "\n";
    }


    std::cout << "\n========================================\n";
    std::cout << "Type 2: Range-Based For Loop (C++11)\n";
    std::cout << "========================================\n";
    // Syntax: for (type variable : container)
    // Most modern and clean way!

    std::cout << "Example 2a: Array\n";
    int numbers[] = {1, 2, 3, 4, 5};
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    std::cout << "Example 2aa:  2DArray\n";
    int numbers2d[4][4] = {
        {1,2,4,5},
        {4,8,9,0},
        {3,0,0,1},
        {9,8,8,2}
        };
    for(auto& numbers1: numbers2d){
        for (int num2: numbers1){
            std::cout << "{ " << num2 << " }";
        }
        std::cout << "\n";
    }





    std::cout << "\nExample 2b: std::vector\n";
    std::vector<std::string> fruits = {"Apple", "Banana", "Cherry", "Date"};
    for (std::string fruit : fruits) {
        std::cout << fruit << "\n";
    }

    std::cout << "\nExample 2c: With 'auto' keyword (type deduction)\n";
    std::vector<int> scores = {95, 87, 92, 78, 88};
    for (auto score : scores) {  // 'auto' figures out the type
        std::cout << score << " ";
    }
    std::cout << "\n";

    std::cout << "\nExample 2d: By reference (modify elements)\n";
    std::vector<int> values = {1, 2, 3, 4, 5};
    std::cout << "Before: ";
    for (auto val : values) std::cout << val << " ";

    for (auto& val : values) {  // '&' means reference - can modify!
        val *= 2;  // Double each value
    }

    std::cout << "\nAfter:  ";
    for (auto val : values) std::cout << val << " ";
    std::cout << "\n";

    std::cout << "\nExample 2e: Const reference (read-only, efficient)\n";
    std::vector<std::string> longstdstrings = {"Programming", "Computer", "Science"};
    for (const auto& str : longstdstrings) {  // Avoid copying, can't modify
        std::cout << str << " (length: " << str.length() << ")\n";
    }

    std::cout << "\nExample 2f: Map/Dictionary\n";
    std::map<std::string, int> ages = {{"Alice", 25}, {"Bob", 30}, {"Charlie", 35}};
    for (const auto& pair : ages) {
        std::cout << pair.first << " is " << pair.second << " years old\n";
    }

    std::cout << "\nExample 2g: Structured binding (C++17)\n";
    for (const auto& [name, age] : ages) {  // Unpack pair directly!
        std::cout << name << ": " << age << "\n";
    }


    std::cout << "\n========================================\n";
    std::cout << "Type 3: Iterator-Based For Loop\n";
    std::cout << "========================================\n";
    // Used with STL containers when you need more control

    std::cout << "Example 3a: std::vector iterator\n";
    std::vector<int> vec = {10, 20, 30, 40, 50};
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++) {
        std::cout << *it << " ";  // Dereference iterator to get value
    }
    std::cout << "\n";

    std::cout << "\nExample 3b: With auto (cleaner)\n";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "\nExample 3c: List iterator\n";
    std::list<std::string> myList = {"First", "Second", "Third"};
    for (auto it = myList.begin(); it != myList.end(); ++it) {
        std::cout << *it << " -> ";
    }
    std::cout << "END\n";

    std::cout << "\nExample 3d: Reverse iteration\n";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "\nExample 3e: Skip elements\n";
    std::vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (auto it = data.begin(); it != data.end(); it += 2) {
        std::cout << *it << " ";
    }
    std::cout << "\n";


    std::cout << "\n========================================\n";
    std::cout << "Type 4: Infinite For Loop\n";
    std::cout << "========================================\n";
    // for (;;) - all parts optional!

    std::cout << "Example 4a: Infinite loop with break\n";
    int counter = 0;
    for (;;) {  // Infinite loop!
        std::cout << counter << " ";
        counter++;
        if (counter >= 5) {
            break;  // Exit loop
        }
    }
    std::cout << "\n";

    std::cout << "\nExample 4b: Only condition (like while)\n";
    int x = 0;
    for (; x < 5;) {  // No init, no increment
        std::cout << x << " ";
        x++;
    }
    std::cout << "\n";

    std::cout << "\nExample 4c: No condition (infinite, use break)\n";
    int y = 0;
    for (int i = 0; ; i++) {  // No condition = always true
        if (i >= 3) break;
        std::cout << "Iteration " << i << "\n";
    }


    std::cout << "\n========================================\n";
    std::cout << "Bonus: Nested Loops\n";
    std::cout << "========================================\n";

    std::cout << "Example: Multiplication table\n";
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            std::cout << i * j << "\t";
        }
        std::cout << "\n";
    }


    std::cout << "\n========================================\n";
    std::cout << "Bonus: Loop Control Statements\n";
    std::cout << "========================================\n";

    std::cout << "Example with 'continue' (skip iteration):\n";
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // Skip even numbers
        }
        std::cout << i << " ";  // Only odd numbers printed
    }
    std::cout << "\n";

    std::cout << "\nExample with 'break' (exit loop early):\n";
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            break;  // Stop at 5
        }
        std::cout << i << " ";
    }
    std::cout << "\n";


    std::cout << "\n========================================\n";
    std::cout << "SUMMARY: When to Use Each Type\n";
    std::cout << "========================================\n";
    std::cout << "1. Traditional for: When you need index/counter\n";
    std::cout << "2. Range-based for: Modern, clean, use MOST of the time!\n";
    std::cout << "3. Iterator for: When you need iterator control\n";
    std::cout << "4. Infinite for: Rare, usually use while instead\n";




    return 0;
}
