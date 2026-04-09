#include <iostream>
#include <string>

int main()
{
    // =================================================
    // BASIC POINTER — declaration and initialization
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Basic Pointer Declaration\n";
    std::cout << "=====================================================\n";

    int  number  = 42;
    int* ptr     = &number;    // ptr holds the ADDRESS of number

    std::cout << "number value:      " << number  << '\n';
    std::cout << "number address:    " << &number << '\n';
    std::cout << "ptr (address):     " << ptr     << '\n';   // same as &number
    std::cout << "ptr value (*ptr):  " << *ptr    << '\n';   // dereference — gets the value


    // =================================================
    // MODIFYING VALUE THROUGH POINTER
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Modifying Value Through Pointer\n";
    std::cout << "=====================================================\n";

    int score = 100;
    int* pScore = &score;

    std::cout << "Before: score = " << score << '\n';

    *pScore = 250;    // modify score through the pointer

    std::cout << "After (*pScore = 250): score = " << score << '\n';
    std::cout << "score and *pScore are the same: " << (*pScore == score ? "YES" : "NO") << '\n';


    // =================================================
    // POINTER ARITHMETIC
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Pointer Arithmetic\n";
    std::cout << "=====================================================\n";

    int arr[5] = {10, 20, 30, 40, 50};
    int* pArr  = arr;    // pointer to first element (arr decays to &arr[0])

    std::cout << "Array traversal using pointer arithmetic:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "pArr + " << i << " → address: " << (pArr + i)
                  << " → value: " << *(pArr + i) << '\n';
    }

    // Increment pointer
    std::cout << "\nMoving pointer forward:\n";
    std::cout << "*pArr     = " << *pArr     << '\n';   // 10
    ++pArr;
    std::cout << "*pArr++   = " << *pArr     << '\n';   // 20
    ++pArr;
    std::cout << "*pArr++   = " << *pArr     << '\n';   // 30

    // Pointer difference
    int* pFirst = arr;
    int* pLast  = &arr[4];
    std::cout << "\nDistance between pFirst and pLast: " << (pLast - pFirst) << " elements\n";


    // =================================================
    // POINTER TO POINTER
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Pointer to Pointer\n";
    std::cout << "=====================================================\n";

    int   value  = 99;
    int*  pVal   = &value;    // pointer to int
    int** ppVal  = &pVal;     // pointer to pointer to int

    std::cout << "value:         " << value   << '\n';
    std::cout << "*pVal:         " << *pVal   << '\n';
    std::cout << "**ppVal:       " << **ppVal << '\n';   // double dereference

    **ppVal = 777;    // modify value through double pointer
    std::cout << "After **ppVal = 777 → value = " << value << '\n';


    // =================================================
    // POINTER AND DYNAMIC MEMORY
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Dynamic Memory (heap allocation)\n";
    std::cout << "=====================================================\n";

    // Single value on heap
    int* pHeap = new int(55);
    std::cout << "Heap value:   " << *pHeap << '\n';
    std::cout << "Heap address: " << pHeap  << '\n';
    delete pHeap;       // ALWAYS delete what you new
    pHeap = nullptr;    // good practice — prevent dangling pointer
    std::cout << "After delete + nullptr: pHeap = " << pHeap << '\n';

    // Array on heap
    int size = 4;
    int* pDynArr = new int[size]{100, 200, 300, 400};
    std::cout << "\nDynamic array: ";
    for (int i = 0; i < size; ++i)
        std::cout << pDynArr[i] << " ";
    std::cout << '\n';
    delete[] pDynArr;   // use delete[] for arrays
    pDynArr = nullptr;


    // =================================================
    // CONST POINTER VARIATIONS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: const Pointer Variations\n";
    std::cout << "=====================================================\n";

    int a = 10, b = 20;

    // Pointer to const — cannot modify the VALUE, but can point elsewhere
    const int* pToConst = &a;
    std::cout << "const int* pToConst = " << *pToConst << '\n';
    // *pToConst = 99;   // ERROR — value is const
    pToConst = &b;       // OK — pointer itself can change
    std::cout << "pToConst now points to b: " << *pToConst << '\n';

    // Const pointer — cannot point elsewhere, but CAN modify the value
    int* const constPtr = &a;
    *constPtr = 99;      // OK — value can change
    // constPtr = &b;    // ERROR — pointer itself is const
    std::cout << "int* const constPtr → a is now: " << a << '\n';

    // Const pointer to const — cannot do either
    const int* const fullConst = &b;
    std::cout << "const int* const fullConst = " << *fullConst << '\n';
    // *fullConst = 99;   // ERROR
    // fullConst  = &a;   // ERROR

    return 0;
}
