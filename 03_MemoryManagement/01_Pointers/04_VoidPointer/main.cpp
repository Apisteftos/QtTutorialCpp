#include <iostream>
#include <cstring>   // memcpy

// =============================================================
// void* — a pointer that can point to ANY type
// but CANNOT be dereferenced without casting first
// =============================================================

// Generic print function using void* (C-style, educational)
void printValue(void* ptr, char type)
{
    switch (type) {
    case 'i': std::cout << "int:    " << *(static_cast<int*>(ptr))    << '\n'; break;
    case 'd': std::cout << "double: " << *(static_cast<double*>(ptr)) << '\n'; break;
    case 'c': std::cout << "char:   " << *(static_cast<char*>(ptr))   << '\n'; break;
    case 'f': std::cout << "float:  " << *(static_cast<float*>(ptr))  << '\n'; break;
    default:  std::cout << "Unknown type\n"; break;
    }
}

// Generic swap using void* and memcpy (like C's qsort internals)
void genericSwap(void* a, void* b, size_t size)
{
    // Temporary buffer on stack
    unsigned char temp[256];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}


int main()
{
    // =================================================
    // BASIC void* — point to anything
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: void* Basic Usage\n";
    std::cout << "=====================================================\n";

    int    i = 42;
    double d = 3.14;
    char   c = 'K';
    float  f = 1.5f;

    void* vptr;   // can point to any type

    vptr = &i;
    std::cout << "vptr points to int,    address: " << vptr << '\n';

    vptr = &d;
    std::cout << "vptr points to double, address: " << vptr << '\n';

    vptr = &c;
    std::cout << "vptr points to char,   address: " << vptr << '\n';


    // =================================================
    // MUST CAST BEFORE DEREFERENCING
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Must Cast Before Dereferencing\n";
    std::cout << "=====================================================\n";

    int number = 100;
    void* vp = &number;

    // std::cout << *vp;                         // ERROR — cannot dereference void*
    std::cout << *(static_cast<int*>(vp)) << '\n';  // OK — cast first, then dereference

    // Assign through void pointer
    *(static_cast<int*>(vp)) = 999;
    std::cout << "After assign through void*: number = " << number << '\n';


    // =================================================
    // VOID* AS GENERIC FUNCTION PARAMETER
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: void* as Generic Function Parameter\n";
    std::cout << "=====================================================\n";

    int    myInt    = 42;
    double myDouble = 9.81;
    char   myChar   = 'Z';
    float  myFloat  = 2.71f;

    printValue(&myInt,    'i');
    printValue(&myDouble, 'd');
    printValue(&myChar,   'c');
    printValue(&myFloat,  'f');


    // =================================================
    // GENERIC SWAP WITH void*
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Generic Swap Using void*\n";
    std::cout << "=====================================================\n";

    int x = 10, y = 20;
    std::cout << "Before swap: x = " << x << ", y = " << y << '\n';
    genericSwap(&x, &y, sizeof(int));
    std::cout << "After swap:  x = " << x << ", y = " << y << '\n';

    double da = 1.11, db = 9.99;
    std::cout << "\nBefore swap: da = " << da << ", db = " << db << '\n';
    genericSwap(&da, &db, sizeof(double));
    std::cout << "After swap:  da = " << da << ", db = " << db << '\n';


    // =================================================
    // VOID* FOR MEMORY OPERATIONS (like malloc/memcpy)
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: void* in Memory Operations\n";
    std::cout << "=====================================================\n";

    // malloc returns void* — must cast in C++
    // (use new/delete in modern C++, but good to understand for C interop)
    void* rawMemory = malloc(sizeof(int) * 4);

    if (rawMemory != nullptr) {
        int* intArray = static_cast<int*>(rawMemory);
        for (int idx = 0; idx < 4; ++idx)
            intArray[idx] = (idx + 1) * 10;

        std::cout << "Array via void* + malloc: ";
        for (int idx = 0; idx < 4; ++idx)
            std::cout << intArray[idx] << " ";
        std::cout << '\n';

        free(rawMemory);   // use free() with malloc()
        rawMemory = nullptr;
    }

    // memcpy uses void* — copies raw bytes regardless of type
    int   src[3]  = {1, 2, 3};
    int   dst[3]  = {0, 0, 0};
    memcpy(dst, src, sizeof(src));   // internally uses void*
    std::cout << "After memcpy: dst = ";
    for (int v : dst) std::cout << v << " ";
    std::cout << '\n';


    // =================================================
    // void* LIMITATIONS vs modern C++ alternatives
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: void* Limitations\n";
    std::cout << "=====================================================\n";

    void* voidPtr = &myInt;

    // No arithmetic on void* (unlike int*)
    // voidPtr++;         // ERROR — compiler doesn't know element size
    // voidPtr + 1;       // ERROR — same reason

    // No dereference without cast
    // *voidPtr;          // ERROR

    // No type safety — you can cast to wrong type (dangerous!)
    double* wrongCast = static_cast<double*>(voidPtr);   // compiles but WRONG
    std::cout << "Wrong cast result (undefined behavior): " << *wrongCast << '\n';

    std::cout << "\nvoid* summary:\n";
    std::cout << "  ✅ Can point to any type\n";
    std::cout << "  ✅ Useful for C interop (malloc, memcpy, callbacks)\n";
    std::cout << "  ❌ Cannot dereference without cast\n";
    std::cout << "  ❌ No pointer arithmetic\n";
    std::cout << "  ❌ No type safety\n";
    std::cout << "  → Prefer templates or std::any in modern C++\n";

    return 0;
}
