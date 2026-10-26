#include <algorithm>  // std::copy
#include <cstdlib>    // malloc, calloc, realloc, free
#include <new>        // std::bad_alloc, std::nothrow, placement new
#include <print>
#include <string>

// =============================================================
// Helper class to visualize constructor/destructor calls
// =============================================================
class Sensor {
private:
    std::string name;
    double      value;

public:
    Sensor() : name("unnamed"), value(0.0) {
        std::println("[Sensor] Default constructor: '{}'", name);
    }

    Sensor(const std::string& name, double value)
        : name(name), value(value) {
        std::println("[Sensor] Constructor: '{}' value={}", name, value);
    }

    ~Sensor() {
        std::println("[Sensor] Destructor: '{}'", name);
    }

    void read() const {
        std::println("  Sensor '{}' reads: {}", name, value);
    }

    void setValue(double v) { value = v; }
    std::string getName() const { return name; }
};


int main()
{
    // === EXAMPLE 1: malloc — raw memory, no constructor ===
    std::println("=====================================================");
    std::println("Example 1: malloc — raw memory, no constructor");
    std::println("=====================================================");

    // malloc returns void* — must cast in C++
    // allocates RAW bytes — does NOT call constructor
    Sensor* s1 = static_cast<Sensor*>(malloc(sizeof(Sensor)));

    if (s1 != nullptr) {
        std::println("malloc allocated {} bytes", sizeof(Sensor));
        std::println("Constructor was called? NO — malloc only allocates memory");
        // s1->read();   // DANGEROUS — object not constructed, undefined behavior
        free(s1);        // free does NOT call destructor
        s1 = nullptr;
        std::println("Destructor was called? NO — free only releases memory");
    }


    // === EXAMPLE 2: new — allocation + constructor ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: new — allocation + constructor");
    std::println("=====================================================");

    // new allocates memory AND calls constructor
    Sensor* s2 = new Sensor("TempSensor", 36.6);
    std::println("new allocated memory AND called constructor");
    s2->read();

    // delete calls destructor AND releases memory
    delete s2;
    s2 = nullptr;
    std::println("delete called destructor AND freed memory");


    // === EXAMPLE 3: calloc — zeroed memory ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: calloc — zeroed memory");
    std::println("=====================================================");

    // calloc(count, size) — allocates count*size bytes, all set to ZERO
    int* intArray = static_cast<int*>(calloc(5, sizeof(int)));

    if (intArray != nullptr) {
        std::println("calloc(5, sizeof(int)) — all bytes zeroed:");
        std::print("Values: ");
        for (int i = 0; i < 5; ++i)
            std::print("{} ", intArray[i]);   // all 0
        std::println("");

        // Fill with data
        for (int i = 0; i < 5; ++i)
            intArray[i] = (i + 1) * 10;

        std::print("After fill: ");
        for (int i = 0; i < 5; ++i)
            std::print("{} ", intArray[i]);
        std::println("");

        free(intArray);
        intArray = nullptr;
    }


    // === EXAMPLE 4: new[] vs malloc for arrays ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: new[] vs malloc for arrays");
    std::println("=====================================================");

    std::println("");
    std::println("--- malloc array (no constructors) ---");
    Sensor* mallocArr = static_cast<Sensor*>(malloc(3 * sizeof(Sensor)));
    std::println("malloc: allocated {} bytes — NO constructors called",
                 3 * sizeof(Sensor));
    // mallocArr[0].read();  // UNDEFINED BEHAVIOR — not constructed
    free(mallocArr);          // NO destructors called
    mallocArr = nullptr;
    std::println("free: released memory — NO destructors called");

    std::println("");
    std::println("--- new[] array (constructors called) ---");
    Sensor* newArr = new Sensor[3];   // calls default constructor 3 times
    std::println("All 3 sensors constructed — safe to use:");
    for (int i = 0; i < 3; ++i)
        newArr[i].read();

    delete[] newArr;    // calls destructor 3 times, then frees memory
    newArr = nullptr;
    std::println("delete[]: all 3 destructors called");


    // === EXAMPLE 5: realloc — resize memory block ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: realloc — resize memory block");
    std::println("=====================================================");

    // Start with 3 ints
    int* dynArray = static_cast<int*>(malloc(3 * sizeof(int)));
    if (dynArray != nullptr) {
        dynArray[0] = 10; dynArray[1] = 20; dynArray[2] = 30;
        std::print("Original (3 ints): ");
        for (int i = 0; i < 3; ++i) std::print("{} ", dynArray[i]);
        std::println("");

        // Grow to 6 ints — may return same or new pointer
        int* temp = static_cast<int*>(realloc(dynArray, 6 * sizeof(int)));
        if (temp != nullptr) {
            dynArray = temp;   // IMPORTANT: use temp first, then reassign
            dynArray[3] = 40; dynArray[4] = 50; dynArray[5] = 60;
            std::print("After realloc (6 ints): ");
            for (int i = 0; i < 6; ++i) std::print("{} ", dynArray[i]);
            std::println("");
        } else {
            std::println("realloc failed — original pointer still valid");
            // dynArray still valid here — do NOT use temp
        }

        free(dynArray);
        dynArray = nullptr;
    }

    // There is NO C++ equivalent of realloc for new[]
    // With new[] you must: allocate new, copy, delete old
    std::println("");
    std::println("C++ equivalent of realloc with new[]:");
    int* oldArr = new int[3]{10, 20, 30};
    int* growArr = new int[6];                        // allocate larger
    std::copy(oldArr, oldArr + 3, growArr);           // copy old data
    growArr[3] = 40; growArr[4] = 50; growArr[5] = 60;
    delete[] oldArr;                                  // free old
    oldArr = growArr;                                 // reassign
    std::print("Grown array: ");
    for (int i = 0; i < 6; ++i) std::print("{} ", oldArr[i]);
    std::println("");
    delete[] oldArr;
    oldArr = nullptr;


    // === EXAMPLE 6: Placement new ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Placement new");
    std::println("=====================================================");

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
        std::println("Placement new: destructor called manually, memory freed with free()");
    }


    // === EXAMPLE 7: new with std::nothrow ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: new with std::nothrow");
    std::println("=====================================================");

    // Normal new throws std::bad_alloc on failure
    // std::nothrow version returns nullptr instead
    Sensor* safeAlloc = new (std::nothrow) Sensor("SafeSensor", 1.0);

    if (safeAlloc != nullptr) {
        safeAlloc->read();
        delete safeAlloc;
        safeAlloc = nullptr;
    } else {
        std::println("Allocation failed — returned nullptr (no exception)");
    }

    // Catching std::bad_alloc with try/catch
    std::println("");
    std::println("Catching std::bad_alloc:");
    try {
        // Intentionally try to allocate impossibly large amount
        // (commented out so the demo also runs under AddressSanitizer, which
        //  aborts on such a request instead of letting new throw — see Readme)
        // int* huge = new int[999999999999LL];

        Sensor* s = new Sensor("CatchTest", 5.5);
        s->read();
        delete s;
        std::println("Allocation succeeded");
    }
    catch (const std::bad_alloc& e) {
        std::println("Allocation failed: {}", e.what());
    }

    std::println("");
    std::println("=== End of MallocVsNew ===");
    return 0;
}
