# volatile — C++23

`volatile` tells the compiler that a variable may be changed **outside the
program's control** — by hardware, a signal handler, or another execution context.
The compiler must **always read from memory** and **never cache** the value
in a register or optimize away reads/writes.

---

## What volatile does

```cpp
volatile int flag = 0;

// WITHOUT volatile — compiler may optimize this to: while (true) {}
// because it sees flag never changes inside the loop
while (flag == 0) {}

// WITH volatile — compiler must re-read flag from memory every iteration
// hardware or signal handler may set flag = 1
while (flag == 0) {}   // forced memory read each time
```

---

## Declaration syntax

```cpp
volatile int x;            // volatile int variable
volatile int* p;           // pointer to volatile int
int* volatile p;           // volatile pointer to int
volatile int* const p;     // const pointer to volatile int
const volatile int x;      // read-only volatile (hardware read register)
```

---

## Hardware register

The primary use case for `volatile`:

```cpp
// Hardware register at fixed memory address
volatile uint32_t* const UART_STATUS = (volatile uint32_t*)0x40011000;
volatile uint32_t* const UART_DATA   = (volatile uint32_t*)0x40011004;

// Spin-wait until hardware says TX buffer is ready
while (!(*UART_STATUS & 0x80)) {}   // must not be optimized away

// Write triggers hardware action — must not be skipped
*UART_DATA = 0x41;                  // sends byte 'A'
```

Without `volatile`, the compiler might eliminate "redundant" reads/writes
because it doesn't know the hardware is changing the value.

---

## Status register struct

```cpp
struct StatusRegister {
    volatile uint8_t  rxReady;       // set by hardware when data arrives
    volatile uint8_t  txReady;       // set by hardware when ready to send
    volatile uint8_t  errorFlag;     // set by hardware on error
    volatile uint32_t dataRegister;  // data buffer
};

StatusRegister* reg = /* hardware address */;

// Poll until data arrives
while (!reg->rxReady) {}
uint32_t data = reg->dataRegister;  // read data
```

---

## Signal handler flag

```cpp
volatile bool g_stop = false;   // volatile — written by signal, read by main

void signalHandler(int sig) {
    g_stop = true;   // async write — must reach main's read
}

// main loop
while (!g_stop) {
    doWork();
}
```

Without `volatile`, the compiler may hoist `g_stop` out of the loop,
reading it once and caching `false` forever.

---

## Pointer to volatile int

```cpp
volatile int reg = 0;

volatile int* p = &reg;    // p points to volatile int
*p = 1;                    // write is not optimized away
int v = *p;                // read is not cached
```

---

## volatile member function

```cpp
class Sensor {
    volatile int m_raw;
public:
    int read() volatile {       // callable on volatile objects
        return m_raw;           // always reads from memory
    }
};

volatile Sensor sensor(0);
sensor.read();     // OK — volatile function
// sensor.other(); // ERROR — non-volatile function on volatile object
```

---

## volatile vs const volatile

```cpp
volatile int rw_reg;          // read/write register — hardware and code both write
const volatile int ro_reg;    // read-only register  — only hardware writes, code reads
```

---

## volatile vs std::atomic

This is the most important distinction:

| | `volatile` | `std::atomic` |
|--|-----------|--------------|
| Prevents compiler caching | ✅ | ✅ |
| Prevents CPU reordering | ❌ | ✅ |
| Thread-safe | ❌ | ✅ |
| Atomic read-modify-write | ❌ | ✅ |
| Memory ordering guarantees | ❌ | ✅ |
| Use for hardware registers | ✅ | ❌ |
| Use for thread communication | ❌ | ✅ |

```cpp
// WRONG — volatile is NOT thread-safe
volatile int counter = 0;
counter++;   // read-modify-write is NOT atomic — race condition!

// CORRECT — atomic IS thread-safe
std::atomic<int> counter = 0;
counter++;   // indivisible operation — always correct
```

**Never use `volatile` for thread synchronization** — use `std::atomic`.

---

## What volatile does NOT guarantee

```
❌ Atomicity        — counter++ is still 3 operations (read, add, write)
❌ Memory ordering  — CPU may still reorder volatile ops relative to non-volatile
❌ Thread safety    — two threads writing volatile int → data race
❌ Mutual exclusion — use std::mutex
```

---

## When to use volatile

✅ Hardware registers in embedded/bare-metal code
✅ Memory-mapped I/O (`mmap` to device file)
✅ Signal handler flags (`volatile bool g_stop`)
✅ Variables modified by `setjmp`/`longjmp`
✅ Shared memory between CPU and DMA controller

---

## When NOT to use volatile

❌ Shared variables between threads → use `std::atomic`
❌ Mutually exclusive sections → use `std::mutex`
❌ Preventing compiler inlining or optimization in general
❌ "Making things safer" in multithreaded code — it doesn't help

---

## Summary

```cpp
// Hardware register — volatile is correct
volatile uint32_t* STATUS_REG = (volatile uint32_t*)0x40001000;
while (!(*STATUS_REG & READY_BIT)) {}

// Signal flag — volatile is correct
volatile bool g_stop = false;
void handler(int) { g_stop = true; }

// Thread communication — use atomic, NOT volatile
std::atomic<bool> g_ready = false;
// Thread A: g_ready.store(true);
// Thread B: while (!g_ready.load()) {}
```
