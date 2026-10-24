#include <print>
#include <string>

int main()
{
    // =================================================
    // BASIC POINTER — declaration and initialization
    // =================================================
    std::println("=====================================================");
    std::println("Example 1: Basic Pointer Declaration");
    std::println("=====================================================");

    int  number  = 42;
    int* ptr     = &number;    // ptr holds the ADDRESS of number

    std::println("number value:      {}", number);
    std::println("number address:    {}", static_cast<const void*>(&number));
    std::println("ptr (address):     {}", static_cast<const void*>(ptr));  // same as &number
    std::println("ptr value (*ptr):  {}", *ptr);  // dereference — gets the value


    // =================================================
    // MODIFYING VALUE THROUGH POINTER
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Modifying Value Through Pointer");
    std::println("=====================================================");

    int score = 100;
    int* pScore = &score;

    std::println("Before: score = {}", score);

    *pScore = 250;    // modify score through the pointer

    std::println("After (*pScore = 250): score = {}", score);
    std::println("score and *pScore are the same: {}", (*pScore == score ? "YES" : "NO"));


    // =================================================
    // POINTER ARITHMETIC
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Pointer Arithmetic");
    std::println("=====================================================");

    int arr[5] = {10, 20, 30, 40, 50};
    int* pArr  = arr;    // pointer to first element (arr decays to &arr[0])

    std::println("Array traversal using pointer arithmetic:");
    for (int i = 0; i < 5; ++i) {
        std::println("pArr + {} → address: {} → value: {}", i, static_cast<const void*>(pArr + i), *(pArr + i));
    }

    // Increment pointer
    std::println("");
    std::println("Moving pointer forward:");
    std::println("*pArr              = {}", *pArr);  // 10
    ++pArr;
    std::println("*pArr after ++pArr = {}", *pArr);  // 20
    ++pArr;
    std::println("*pArr after ++pArr = {}", *pArr);  // 30

    // Pointer difference
    int* pFirst = arr;
    int* pLast  = &arr[4];
    std::println("");
    std::println("Distance between pFirst and pLast: {} elements", (pLast - pFirst));


    // =================================================
    // POINTER TO POINTER
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Pointer to Pointer");
    std::println("=====================================================");

    int   value  = 99;
    int*  pVal   = &value;    // pointer to int
    int** ppVal  = &pVal;     // pointer to pointer to int

    std::println("value:         {}", value);
    std::println("*pVal:         {}", *pVal);
    std::println("**ppVal:       {}", **ppVal);  // double dereference

    **ppVal = 777;    // modify value through double pointer
    std::println("After **ppVal = 777 → value = {}", value);


    // =================================================
    // POINTER AND DYNAMIC MEMORY
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Dynamic Memory (heap allocation)");
    std::println("=====================================================");

    // Single value on heap
    int* pHeap = new int(55);
    std::println("Heap value:   {}", *pHeap);
    std::println("Heap address: {}", static_cast<const void*>(pHeap));
    delete pHeap;       // ALWAYS delete what you new
    pHeap = nullptr;    // good practice — prevent dangling pointer
    std::println("After delete + nullptr: pHeap = {}", static_cast<const void*>(pHeap));

    // Array on heap
    int size = 4;
    int* pDynArr = new int[size]{100, 200, 300, 400};
    std::println("");
    std::print("Dynamic array: ");
    for (int i = 0; i < size; ++i)
        std::print("{} ", pDynArr[i]);
    std::println("");
    delete[] pDynArr;   // use delete[] for arrays
    pDynArr = nullptr;


    // =================================================
    // CONST POINTER VARIATIONS
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: const Pointer Variations");
    std::println("=====================================================");

    int a = 10, b = 20;

    // Pointer to const — cannot modify the VALUE, but can point elsewhere
    const int* pToConst = &a;
    std::println("const int* pToConst = {}", *pToConst);
    // *pToConst = 99;   // ERROR — value is const
    pToConst = &b;       // OK — pointer itself can change
    std::println("pToConst now points to b: {}", *pToConst);

    // Const pointer — cannot point elsewhere, but CAN modify the value
    int* const constPtr = &a;
    *constPtr = 99;      // OK — value can change
    // constPtr = &b;    // ERROR — pointer itself is const
    std::println("int* const constPtr → a is now: {}", a);

    // Const pointer to const — cannot do either
    const int* const fullConst = &b;
    std::println("const int* const fullConst = {}", *fullConst);
    // *fullConst = 99;   // ERROR
    // fullConst  = &a;   // ERROR

    return 0;
}
