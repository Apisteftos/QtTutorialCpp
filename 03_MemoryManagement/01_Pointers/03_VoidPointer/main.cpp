#include <print>
#include <bit>
#include <cstddef>
#include <cstdlib>
#include <cstring>

// =============================================================
// void* — a pointer that can point to ANY type
// but CANNOT be dereferenced without casting first
// =============================================================

// Generic print function using void* (C-style, educational)
void printValue(void* ptr, char type)
{
    switch (type) {
    case 'i': std::println("int:    {}", *(static_cast<int*>(ptr))); break;
    case 'd': std::println("double: {}", *(static_cast<double*>(ptr))); break;
    case 'c': std::println("char:   {}", *(static_cast<char*>(ptr))); break;
    case 'f': std::println("float:  {}", *(static_cast<float*>(ptr))); break;
    default:  std::println("Unknown type"); break;
    }
}

// Generic swap using void* and memcpy (like C's qsort internals)
// Only valid for trivially copyable types (int, double, plain structs...),
// NOT for std::string, std::vector, ... (their bytes contain internal pointers)
void genericSwap(void* a, void* b, std::size_t size)
{
    // Temporary buffer on stack
    unsigned char temp[256];
    if (size > sizeof(temp)) {
        std::println("genericSwap: size {} does not fit the {}-byte buffer", size, sizeof(temp));
        return;
    }
    std::memcpy(temp, a, size);
    std::memcpy(a, b, size);
    std::memcpy(b, temp, size);
}


int main()
{
    // =================================================
    // BASIC void* — point to anything
    // =================================================
    std::println("=====================================================");
    std::println("Example 1: void* Basic Usage");
    std::println("=====================================================");

    int    i = 42;
    double d = 3.14;
    char   c = 'K';
    float  f = 1.5f;

    void* vptr;   // can point to any type

    vptr = &i;
    std::println("vptr points to int,    address: {}", vptr);

    vptr = &d;
    std::println("vptr points to double, address: {}", vptr);

    vptr = &c;
    std::println("vptr points to char,   address: {}", vptr);

    vptr = &f;
    std::println("vptr points to float,  address: {}", vptr);


    // =================================================
    // MUST CAST BEFORE DEREFERENCING
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Must Cast Before Dereferencing");
    std::println("=====================================================");

    int number = 100;
    void* vp = &number;

    // std::println("{}", *vp);                         // ERROR — cannot dereference void*
    std::println("{}", *(static_cast<int*>(vp)));  // OK — cast first, then dereference

    // Assign through void pointer
    *(static_cast<int*>(vp)) = 999;
    std::println("After assign through void*: number = {}", number);


    // =================================================
    // VOID* AS GENERIC FUNCTION PARAMETER
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: void* as Generic Function Parameter");
    std::println("=====================================================");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Generic Swap Using void*");
    std::println("=====================================================");

    int x = 10, y = 20;
    std::println("Before swap: x = {}, y = {}", x, y);
    genericSwap(&x, &y, sizeof(int));
    std::println("After swap:  x = {}, y = {}", x, y);

    double da = 1.11, db = 9.99;
    std::println("");
    std::println("Before swap: da = {}, db = {}", da, db);
    genericSwap(&da, &db, sizeof(double));
    std::println("After swap:  da = {}, db = {}", da, db);


    // =================================================
    // VOID* FOR MEMORY OPERATIONS (like malloc/memcpy)
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: void* in Memory Operations");
    std::println("=====================================================");

    // malloc returns void* — must cast in C++
    // (use new/delete in modern C++, but good to understand for C interop)
    void* rawMemory = std::malloc(sizeof(int) * 4);

    if (rawMemory != nullptr) {
        int* intArray = static_cast<int*>(rawMemory);
        for (int idx = 0; idx < 4; ++idx)
            intArray[idx] = (idx + 1) * 10;

        std::print("Array via void* + malloc: ");
        for (int idx = 0; idx < 4; ++idx)
            std::print("{} ", intArray[idx]);
        std::println("");

        std::free(rawMemory);   // use free() with malloc()
        rawMemory = nullptr;
    }

    // memcpy uses void* — copies raw bytes regardless of type
    int   src[3]  = {1, 2, 3};
    int   dst[3]  = {0, 0, 0};
    std::memcpy(dst, src, sizeof(src));   // internally uses void*
    std::print("After memcpy: dst = ");
    for (int v : dst) std::print("{} ", v);
    std::println("");


    // =================================================
    // void* LIMITATIONS vs modern C++ alternatives
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: void* Limitations");
    std::println("=====================================================");

    void* voidPtr = &myInt;

    // No arithmetic on void* (unlike int*)
    // voidPtr++;         // ERROR in ISO C++ — compiler doesn't know element size
    // voidPtr + 1;       // ERROR — same reason
    // (GCC accepts both as an extension and only warns with -Wpointer-arith;
    //  -pedantic-errors turns it into an error.)

    // No dereference without cast
    // *voidPtr;          // ERROR

    // No type safety — you can cast to wrong type (dangerous!)
    // (Casting to double* would read 8 bytes from a 4-byte int: a stack buffer overflow.
    //  float has the same size as int, so the damage stays small - but it is still UB.)
    float* wrongCast = static_cast<float*>(voidPtr);   // compiles but WRONG
    std::println("Wrong cast result (undefined behavior): {}", *wrongCast);
    // The well-defined way to reinterpret the bytes:
    std::println("std::bit_cast<float>(myInt) (well-defined): {}", std::bit_cast<float>(myInt));

    std::println("");
    std::println("void* summary:");
    std::println("  ✅ Can point to any type");
    std::println("  ✅ Useful for C interop (malloc, memcpy, callbacks)");
    std::println("  ❌ Cannot dereference without cast");
    std::println("  ❌ No pointer arithmetic");
    std::println("  ❌ No type safety");
    std::println("  → Prefer templates or std::any in modern C++");

    return 0;
}
