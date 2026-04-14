#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdint>
#include <print>

using namespace std::chrono_literals;

// ============================================================
// VOLATILE in C++23
// ============================================================
// Topics covered:
//   1.  What volatile does — prevents compiler optimization
//   2.  Without volatile — optimizer may cache in register
//   3.  With volatile — always reads from memory
//   4.  volatile with hardware register simulation
//   5.  volatile with memory-mapped I/O
//   6.  volatile in signal handlers
//   7.  volatile vs atomic — the critical difference
//   8.  volatile pointer vs pointer to volatile
//   9.  volatile member functions
//  10.  When to use volatile (and when NOT to)
// ============================================================


// ============================================================
// Example 4 — Hardware register simulation
// In embedded systems, registers are at fixed memory addresses
// The hardware changes the value — compiler must not cache it
// ============================================================
struct StatusRegister {
    volatile uint8_t  rxReady;      // set by hardware when data received
    volatile uint8_t  txReady;      // set by hardware when ready to send
    volatile uint8_t  errorFlag;    // set by hardware on error
    volatile uint32_t dataRegister; // data read/written by hardware
};

// Simulate hardware setting the rxReady flag after a delay
void simulateHardware(StatusRegister* reg)
{
    std::this_thread::sleep_for(50ms);
    reg->rxReady      = 1;   // hardware "sets" the flag
    reg->dataRegister = 0xDEADBEEF;
}


// ============================================================
// Example 5 — Memory-mapped I/O simulation
// Each read/write may have side effects — must not be optimized away
// ============================================================
volatile uint32_t* const UART_DATA   = reinterpret_cast<volatile uint32_t*>(0);
volatile uint32_t* const UART_STATUS = reinterpret_cast<volatile uint32_t*>(0);

// In real embedded code:
// #define UART_BASE    0x40011000
// volatile uint32_t* const UART_DATA   = (volatile uint32_t*)(UART_BASE + 0x04);
// volatile uint32_t* const UART_STATUS = (volatile uint32_t*)(UART_BASE + 0x00);

void writeToUART(uint8_t byte)
{
    // In real embedded: wait until TX buffer empty
    // while (!(*UART_STATUS & 0x80)) {}   // spin until ready
    // *UART_DATA = byte;                   // write triggers hardware send
    std::println("  [UART] would write byte: 0x{:02X}", byte);
}


// ============================================================
// Example 6 — Signal handler flag
// Signal handlers run asynchronously — volatile needed for visibility
// ============================================================
volatile bool g_signalReceived = false;   // volatile: signal handler writes, main reads

void signalHandler(/* int sig */)
{
    g_signalReceived = true;   // async write — must not be optimized away
}


// ============================================================
// Example 8 — volatile pointer variants
// ============================================================
int           normalVar   = 42;
volatile int  volatileVar = 42;

int* const           ptrToNormal   = &normalVar;    // pointer to normal int
volatile int* const  ptrToVolatile = &volatileVar;  // pointer to volatile int


// ============================================================
// Example 9 — volatile member function
// ============================================================
class Sensor {
private:
    volatile int m_rawValue;   // updated by hardware/interrupt

public:
    explicit Sensor(int initial) : m_rawValue(initial) {}

    // volatile member function — can be called on volatile object
    int read() volatile {
        return m_rawValue;   // always reads from memory — never cached
    }

    void setValue(int v) volatile {
        m_rawValue = v;
    }

    // non-volatile version — for non-volatile objects
    int readCached() const {
        return m_rawValue;
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What volatile does\n";
    std::cout << "=====================================================\n";

    std::cout << "volatile tells the compiler:\n";
    std::cout << "  - Do NOT cache this variable in a register\n";
    std::cout << "  - Do NOT reorder reads/writes to this variable\n";
    std::cout << "  - ALWAYS read from memory, ALWAYS write to memory\n";
    std::cout << "  - The value may change outside the program's control\n\n";
    std::cout << "Use cases:\n";
    std::cout << "  - Hardware registers (embedded systems)\n";
    std::cout << "  - Memory-mapped I/O\n";
    std::cout << "  - Signal handler flags\n";
    std::cout << "  - setjmp/longjmp local variables\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Without volatile — optimizer may eliminate reads\n";
    std::cout << "=====================================================\n";

    {
        int nonVolatile = 0;

        // With optimisation enabled, the compiler MAY transform this into:
        //   if (0 == 1) { ... }  → remove the loop entirely!
        // because it 'knows' nonVolatile never changes inside the loop
        for (int i = 0; i < 3; ++i) {
            if (nonVolatile == 1)
                std::cout << "  value changed! (compiler may never check this)\n";
        }

        std::cout << "Without volatile: compiler may cache nonVolatile in register\n";
        std::cout << "  loop condition may never re-read from memory\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: With volatile — always reads from memory\n";
    std::cout << "=====================================================\n";

    {
        volatile int flag = 0;

        // Compiler CANNOT optimize away these reads
        // Every iteration must read flag from actual memory
        for (int i = 0; i < 3; ++i) {
            int current = flag;   // forced memory read every time
            std::println("  iteration {}: flag={} (read from memory)", i, current);
        }

        std::cout << "With volatile: every read goes to memory\n";
        std::cout << "  an external source could change flag between reads\n";

        // Volatile loop — waiting for external change
        volatile bool ready = false;
        // In real code: while (!ready) {}  // spin-wait for hardware/interrupt
        std::cout << "  spin-wait pattern: while (!ready) {}  ← volatile prevents optimization\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Hardware register simulation\n";
    std::cout << "=====================================================\n";

    {
        StatusRegister reg = {0, 0, 0, 0};

        std::cout << "waiting for hardware to set rxReady...\n";

        // Start "hardware" in background thread
        std::thread hw(simulateHardware, &reg);

        // Poll the volatile register — every read is a real memory read
        while (!reg.rxReady) {
            std::this_thread::sleep_for(10ms);
            std::cout << "  polling rxReady...\n";
        }

        // Without volatile, compiler could hoist rxReady read out of loop:
        //   bool ready = reg.rxReady;
        //   while (!ready) {}  ← infinite loop, never re-reads!

        std::println("  rxReady set! data=0x{:08X}", reg.dataRegister);
        hw.join();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Memory-mapped I/O\n";
    std::cout << "=====================================================\n";

    std::cout << "In embedded systems, hardware registers are at fixed addresses:\n\n";
    std::cout << "  volatile uint32_t* UART_DATA = (volatile uint32_t*)0x40011004;\n\n";
    std::cout << "  // Read — hardware may update value between reads\n";
    std::cout << "  uint32_t status = *UART_STATUS;  // always reads from hardware\n\n";
    std::cout << "  // Write — triggers hardware action\n";
    std::cout << "  *UART_DATA = 0x41;               // always writes to hardware\n\n";
    std::cout << "  // Without volatile, compiler may skip 'redundant' reads/writes!\n";

    writeToUART(0x41);
    writeToUART(0x42);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Signal handler flag\n";
    std::cout << "=====================================================\n";

    {
        // Signal handlers run asynchronously on the same thread
        // The main loop reads g_signalReceived — must not be cached
        std::cout << "global flag:  volatile bool g_signalReceived = false\n";
        std::cout << "signal sets:  g_signalReceived = true\n";
        std::cout << "main checks:  while (!g_signalReceived) {}\n\n";

        // Simulate: signal fires in background
        std::thread signalSim([]() {
            std::this_thread::sleep_for(40ms);
            signalHandler();
            std::println("  [signal] g_signalReceived set to true");
        });

        std::cout << "  waiting for signal...\n";
        while (!g_signalReceived)
            std::this_thread::sleep_for(10ms);

        std::cout << "  signal received — g_signalReceived=" << g_signalReceived << '\n';
        signalSim.join();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: volatile vs atomic — critical difference\n";
    std::cout << "=====================================================\n";

    {
        volatile int  vol   = 0;   // prevents optimization, NOT thread-safe!
        std::atomic<int> atm = 0;  // prevents optimization AND thread-safe

        std::cout << "volatile:\n";
        std::cout << "  + Prevents compiler caching/reordering\n";
        std::cout << "  + Correct for hardware registers, signal handlers\n";
        std::cout << "  - NOT thread-safe — no memory ordering guarantees\n";
        std::cout << "  - NOT atomic — read-modify-write not indivisible\n\n";

        std::cout << "std::atomic:\n";
        std::cout << "  + Thread-safe — memory ordering guaranteed\n";
        std::cout << "  + Atomic operations — read-modify-write indivisible\n";
        std::cout << "  + Prevents compiler AND CPU reordering\n";
        std::cout << "  - Slight overhead vs volatile\n\n";

        std::cout << "Rule:\n";
        std::cout << "  Hardware register / signal handler → volatile\n";
        std::cout << "  Shared variable between threads    → std::atomic\n";
        std::cout << "  NEVER use volatile for thread communication!\n";

        // Demonstrate: volatile does NOT prevent race condition
        int sharedVol = 0;
        // Two threads doing sharedVol++ may produce wrong result
        // even with volatile — not atomic!

        // atomic is correct for this:
        std::atomic<int> sharedAtm = 0;
        std::thread t1([&](){ for(int i=0;i<10000;++i) sharedAtm++; });
        std::thread t2([&](){ for(int i=0;i<10000;++i) sharedAtm++; });
        t1.join(); t2.join();
        std::println("\natomic result (expected 20000): {}", sharedAtm.load());
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: volatile pointer variants\n";
    std::cout << "=====================================================\n";

    {
        int  a = 10;
        int  b = 20;
        volatile int c = 30;
        volatile int d = 40;

        // 1. Pointer to volatile int — value may change, pointer can change
        volatile int* p1 = &c;
        std::println("volatile int* p1 = &c:  *p1={}", *p1);
        p1 = &d;               // OK — can point elsewhere
        // *p1 = 99;            // OK — can write (but d might be hardware reg)

        // 2. const pointer to volatile int — pointer fixed, value may change
        volatile int* const p2 = &c;
        std::println("volatile int* const p2: *p2={}", *p2);
        // p2 = &d;             // ERROR — const pointer cannot be reassigned

        // 3. volatile pointer to non-volatile int (rare)
        int* volatile p3 = &a;
        std::println("int* volatile p3 = &a:  *p3={}", *p3);
        p3 = &b;               // write to pointer itself is volatile (unoptimized)

        std::cout << "\nSyntax summary:\n";
        std::cout << "  volatile int* p    — ptr to volatile int\n";
        std::cout << "  int* volatile p    — volatile ptr to int\n";
        std::cout << "  volatile int* const p — const ptr to volatile int\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: volatile member function\n";
    std::cout << "=====================================================\n";

    {
        Sensor sensor(100);

        std::println("sensor.read(): {}", sensor.read());
        sensor.setValue(200);
        std::println("after setValue(200): {}", sensor.read());

        // volatile object — can only call volatile member functions
        volatile Sensor vSensor(50);
        std::println("volatile sensor.read(): {}", vSensor.read());
        // vSensor.readCached();  // ERROR — non-volatile function on volatile object

        std::cout << "\nvolatile member function:\n";
        std::cout << "  void fn() volatile {}  ← can be called on volatile objects\n";
        std::cout << "  Used when the object itself is volatile (hardware register map)\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: When to use volatile (and when NOT to)\n";
    std::cout << "=====================================================\n";

    std::cout << "USE volatile for:\n";
    std::cout << "  Hardware registers       — value changed by hardware\n";
    std::cout << "  Memory-mapped I/O        — read/write have side effects\n";
    std::cout << "  Signal handler flags     — async write from signal handler\n";
    std::cout << "  setjmp/longjmp locals    — value may change on longjmp\n\n";

    std::cout << "DO NOT use volatile for:\n";
    std::cout << "  Thread synchronization   — use std::atomic or std::mutex\n";
    std::cout << "  Shared data between threads — volatile is NOT thread-safe\n";
    std::cout << "  Performance optimization — volatile hurts performance\n";
    std::cout << "  Preventing inlining      — use other mechanisms\n\n";

    std::cout << "volatile does NOT guarantee:\n";
    std::cout << "  Atomicity of operations  — use std::atomic\n";
    std::cout << "  Memory ordering          — use std::atomic with memory_order\n";
    std::cout << "  Mutual exclusion         — use std::mutex\n";
    std::cout << "  Visibility across CPUs   — use std::atomic (memory fence)\n";

    std::cout << "\n=== Volatile complete ===\n";
    return 0;
}
