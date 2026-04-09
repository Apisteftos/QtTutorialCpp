#include <iostream>
#include <cstdlib>    // malloc, calloc, realloc, free
#include <cstring>    // memset, memcpy
#include <string>
#include <new>        // std::bad_alloc, std::nothrow

// =============================================================
// Helper class to visualize constructor/destructor calls
// =============================================================
class Sensor {
private:
    std::string name;
    double      value;

public:
    Sensor() : name("unnamed"), value(0.0) {
        std::cout << "[Sensor] Default constructor: '" << name << "'\n";
    }

    Sensor(const std::string& name, double value)
        : name(name), value(value) {
        std::cout << "[Sensor] Constructor: '" << name
                  << "' value=" << value << '\n';
    }

    ~Sensor() {
        std::cout << "[Sensor] Destructor: '" << name << "'\n";
    }

    void read() const {
        std::cout << "  Sensor '" << name << "' reads: " << value << '\n';
    }

    void setValue(double v) { value = v; }
    std::string getName() const { return name; }
};


int main()
{
    // =================================================
    // MALLOC — C style allocation
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: malloc — raw memory, no constructor\n";
    std::cout << "=====================================================\n";

    // malloc returns void* — must cast in C++
    // allocates RAW bytes — does NOT call constructor
    Sensor* s1 = static_cast<Sensor*>(malloc(sizeof(Sensor)));

    if (s1 != nullptr) {
        std::cout << "malloc allocated " << sizeof(Sensor) << " bytes\n";
        std::cout << "Constructor was called? NO — malloc only allocates memory\n";
        // s1->read();   // DANGEROUS — object not constructed, undefined behavior
        free(s1);        // free does NOT call destructor
        s1 = nullptr;
        std::cout << "Destructor was called? NO — free only releases memory\n";
    }


    // =================================================
    // NEW — C++ style allocation
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: new — allocation + constructor\n";
    std::cout << "=====================================================\n";

    // new allocates memory AND calls constructor
    Sensor* s2 = new Sensor("TempSensor", 36.6);
    std::cout << "new allocated memory AND called constructor\n";
    s2->read();

    // delete calls destructor AND releases memory
    delete s2;
    s2 = nullptr;
    std::cout << "delete called destructor AND freed memory\n";


    // =================================================
    // CALLOC — zeroed memory
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: calloc — zeroed memory\n";
    std::cout << "=====================================================\n";

    // calloc(count, size) — allocates count*size bytes, all set to ZERO
    int* intArray = static_cast<int*>(calloc(5, sizeof(int)));

    if (intArray != nullptr) {
        std::cout << "calloc(5, sizeof(int)) — all bytes zeroed:\n";
        std::cout << "Values: ";
        for (int i = 0; i < 5; ++i)
            std::cout << intArray[i] << " ";   // all 0
        std::cout << '\n';

        // Fill with data
        for (int i = 0; i < 5; ++i)
            intArray[i] = (i + 1) * 10;

        std::cout << "After fill: ";
        for (int i = 0; i < 5; ++i)
            std::cout << intArray[i] << " ";
        std::cout << '\n';

        free(intArray);
        intArray = nullptr;
    }


    // =================================================
    // NEW[] vs malloc for ARRAYS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: new[] vs malloc for arrays\n";
    std::cout << "=====================================================\n";

    std::cout << "\n--- malloc array (no constructors) ---\n";
    Sensor* mallocArr = static_cast<Sensor*>(malloc(3 * sizeof(Sensor)));
    std::cout << "malloc: allocated " << 3 * sizeof(Sensor)
              << " bytes — NO constructors called\n";
    // mallocArr[0].read();  // UNDEFINED BEHAVIOR — not constructed
    free(mallocArr);          // NO destructors called
    mallocArr = nullptr;
    std::cout << "free: released memory — NO destructors called\n";

    std::cout << "\n--- new[] array (constructors called) ---\n";
    Sensor* newArr = new Sensor[3];   // calls default constructor 3 times
    std::cout << "All 3 sensors constructed — safe to use:\n";
    for (int i = 0; i < 3; ++i)
        newArr[i].read();

    delete[] newArr;    // calls destructor 3 times, then frees memory
    newArr = nullptr;
    std::cout << "delete[]: all 3 destructors called\n";


    // =================================================
    // REALLOC — resize allocated memory
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: realloc — resize memory block\n";
    std::cout << "=====================================================\n";

    // Start with 3 ints
    int* dynArray = static_cast<int*>(malloc(3 * sizeof(int)));
    if (dynArray != nullptr) {
        dynArray[0] = 10; dynArray[1] = 20; dynArray[2] = 30;
        std::cout << "Original (3 ints): ";
        for (int i = 0; i < 3; ++i) std::cout << dynArray[i] << " ";
        std::cout << '\n';

        // Grow to 6 ints — may return same or new pointer
        int* temp = static_cast<int*>(realloc(dynArray, 6 * sizeof(int)));
        if (temp != nullptr) {
            dynArray = temp;   // IMPORTANT: use temp first, then reassign
            dynArray[3] = 40; dynArray[4] = 50; dynArray[5] = 60;
            std::cout << "After realloc (6 ints): ";
            for (int i = 0; i < 6; ++i) std::cout << dynArray[i] << " ";
            std::cout << '\n';
        } else {
            std::cout << "realloc failed — original pointer still valid\n";
            // dynArray still valid here — do NOT use temp
        }

        free(dynArray);
        dynArray = nullptr;
    }

    // There is NO C++ equivalent of realloc for new[]
    // With new[] you must: allocate new, copy, delete old
    std::cout << "\nC++ equivalent of realloc with new[]:\n";
    int* oldArr = new int[3]{10, 20, 30};
    int* growArr = new int[6];                        // allocate larger
    std::copy(oldArr, oldArr + 3, growArr);           // copy old data
    growArr[3] = 40; growArr[4] = 50; growArr[5] = 60;
    delete[] oldArr;                                  // free old
    oldArr = growArr;                                 // reassign
    std::cout << "Grown array: ";
    for (int i = 0; i < 6; ++i) std::cout << oldArr[i] << " ";
    std::cout << '\n';
    delete[] oldArr;
    oldArr = nullptr;


    // =================================================
    // MIXING malloc/free WITH new/delete — NEVER DO THIS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Never mix malloc/free with new/delete\n";
    std::cout << "=====================================================\n";

    std::cout << "The following combinations are UNDEFINED BEHAVIOR:\n";
    std::cout << "  Sensor* p = new Sensor(...);\n";
    std::cout << "  free(p);           // WRONG — destructor not called\n\n";
    std::cout << "  Sensor* p = (Sensor*)malloc(sizeof(Sensor));\n";
    std::cout << "  delete p;          // WRONG — constructor was never called\n\n";
    std::cout << "  int* arr = new int[5];\n";
    std::cout << "  delete arr;        // WRONG — must use delete[]\n\n";
    std::cout << "Rule: always pair malloc↔free and new↔delete and new[]↔delete[]\n";


    // =================================================
    // PLACEMENT NEW — construct in pre-allocated memory
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Placement new\n";
    std::cout << "=====================================================\n";

    // Allocate raw memory first
    void* rawMemory = malloc(sizeof(Sensor));

    if (rawMemory != nullptr) {
        // Construct object IN the pre-allocated memory
        Sensor* placedSensor = new (rawMemory) Sensor("PlacedSensor", 99.9);
        placedSensor->read();

        // MUST manually call destructor — do NOT use delete here
        placedSensor->~Sensor();   // explicit destructor call

        free(rawMemory);           // then free the raw memory
        rawMemory  = nullptr;
        placedSensor = nullptr;
        std::cout << "Placement new: destructor called manually, memory freed with free()\n";
    }


    // =================================================
    // NEW WITH std::nothrow — no exception on failure
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: new with std::nothrow\n";
    std::cout << "=====================================================\n";

    // Normal new throws std::bad_alloc on failure
    // std::nothrow version returns nullptr instead
    Sensor* safeAlloc = new (std::nothrow) Sensor("SafeSensor", 1.0);

    if (safeAlloc != nullptr) {
        safeAlloc->read();
        delete safeAlloc;
        safeAlloc = nullptr;
    } else {
        std::cout << "Allocation failed — returned nullptr (no exception)\n";
    }

    // Simulating bad_alloc with try/catch
    std::cout << "\nCatching std::bad_alloc:\n";
    try {
        // Intentionally try to allocate impossibly large amount
        // (commented out to avoid actual crash — shown for educational purpose)
        // int* huge = new int[999999999999LL];

        Sensor* s = new Sensor("CatchTest", 5.5);
        s->read();
        delete s;
        std::cout << "Allocation succeeded\n";
    }
    catch (const std::bad_alloc& e) {
        std::cout << "Allocation failed: " << e.what() << '\n';
    }

    std::cout << "\n=== End of MallocVsNew ===\n";
    return 0;
}
