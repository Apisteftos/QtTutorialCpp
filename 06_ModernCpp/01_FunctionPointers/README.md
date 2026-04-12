# FunctionPointers — C++23

A **function pointer** stores the memory address of a function.
Just as a data pointer points to a value, a function pointer points to executable code.

---

## Why functions have addresses

When compiled, every function becomes machine code stored in the **TEXT segment** of memory.
Each function has a unique starting address — exactly like a variable has an address.

```
TEXT SEGMENT (read-only):
┌────────────────────────────────┐
│  add()      Address: 0x400650  │ ← function pointer points here
│  subtract() Address: 0x400700  │
│  main()     Address: 0x400500  │
└────────────────────────────────┘
```

```cpp
// function name alone = its address (no call)
// &add  == add   → both give the address of add
// add() → calls add (needs arguments)
std::cout << (void*)add;   // prints address
std::cout << (void*)&add;  // same address
```

---

## Declaration syntax

```
int (*funcPtr)(int, int);
│    │             │
│    │             └── parameter types
│    └── pointer name — (*name) is critical!
└── return type
```

**The parentheses `(*funcPtr)` are mandatory:**

```cpp
int *funcPtr(int, int);    // WRONG — declares function returning int*
int (*funcPtr)(int, int);  // CORRECT — pointer to function returning int
```

---

## Declaration examples

```cpp
void (*p1)();                    // void func()
int  (*p2)(int);                 // int func(int)
double (*p3)(int, double);       // double func(int, double)
bool (*p4)(int, int);            // bool func(int, int)
int  (*ops[4])(int, int);        // array of 4 function pointers
```

---

## Assignment and calling

```cpp
int (*fp)(int, int);   // declare
fp = add;              // assign — store address of add
fp = &add;             // same — & is optional

// Calling
int r1 = fp(5, 3);     // implicit dereference (preferred)
int r2 = (*fp)(5, 3);  // explicit dereference (equivalent)

// Initialize at declaration
int (*fp2)(int, int) = add;

// Safe default
int (*fp3)(int, int) = nullptr;
if (fp3) fp3(1, 2);   // always check before calling
```

---

## Function pointer as parameter (callback)

```cpp
void processArray(const int arr[], int n, void (*callback)(int)) {
    for (int i = 0; i < n; ++i)
        callback(arr[i]);
}

void printDouble(int x) { std::cout << x*2 << ' '; }
void printSquare(int x) { std::cout << x*x << ' '; }

int nums[] = {1,2,3,4,5};
processArray(nums, 5, printDouble);  // pass function by name
processArray(nums, 5, printSquare);

// Sort with custom comparator
bool ascending (int a, int b) { return a < b; }
bool descending(int a, int b) { return a > b; }

void bubbleSort(int arr[], int n, bool (*cmp)(int,int));
bubbleSort(arr, 7, ascending);
bubbleSort(arr, 7, descending);
```

---

## Array of function pointers — dispatch table

```cpp
int (*ops[4])(int,int) = {add, subtract, multiply, divide};

// O(1) dispatch — no if-else chain
int result = ops[userChoice](a, b);

// Event table
struct Handler { void (*fn)(); const char* name; };
Handler events[] = {
    {onMouseClick,  "CLICK"},
    {onKeyPress,    "KEY"},
    {onWindowClose, "CLOSE"}
};
for (auto& e : events) e.fn();
```

---

## Returning a function pointer

```cpp
// Ugly syntax — use 'using' instead
int (*getOp(char op))(int,int) { ... }

// Clean with using alias (preferred)
using BinaryOp = int(*)(int,int);

BinaryOp selectOp(char op) {
    switch (op) {
        case '+': return add;
        case '-': return subtract;
        default:  return nullptr;
    }
}

BinaryOp fn = selectOp('+');
if (fn) fn(10, 5);   // 15
```

---

## typedef and using — readable aliases

```cpp
// typedef (C++03)
typedef void (*IntTransform)(int);
typedef int  (*MathOp)(int, int);

// using (C++11 — preferred)
using IntTransform = void(*)(int);
using MathOp       = int(*)(int, int);
using VoidCallback = void(*)();

// Usage
MathOp    op  = add;
IntTransform t = printDouble;
```

---

## Member function pointers

```cpp
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    int mul(int a, int b) { return a * b; }
};

// Syntax: ReturnType (ClassName::*name)(params)
int (Calculator::*fp)(int, int) = &Calculator::add;

Calculator c;
int r1 = (c.*fp)(10, 5);     // via object — 15

Calculator* cp = &c;
int r2 = (cp->*fp)(10, 5);   // via pointer — 15

fp = &Calculator::mul;
int r3 = (c.*fp)(6, 7);      // 42

// Array of member function pointers
int (Calculator::*ops[2])(int,int) = {
    &Calculator::add,
    &Calculator::mul
};
(c.*ops[0])(3, 4);   // add — 7
(c.*ops[1])(3, 4);   // mul — 12
```

---

## Function pointer vs std::function vs template

| | Function pointer | `std::function` | Template |
|--|----------------|----------------|---------|
| Syntax | `void(*)(int)` | `std::function<void(int)>` | `template<typename F>` |
| Lambda (no capture) | ✅ | ✅ | ✅ |
| Lambda (with capture) | ❌ | ✅ | ✅ |
| Storable | ✅ | ✅ | ❌ |
| C-compatible | ✅ | ❌ | ❌ |
| Performance | Fastest | Small overhead | Fastest (inlined) |
| Null check | `!= nullptr` | `operator bool` | N/A |

```cpp
// Function pointer — cannot store capturing lambda
int factor = 3;
void (*fp)(int) = [factor](int x){ ... };  // ERROR — has capture

// std::function — accepts anything
std::function<void(int)> fn = [factor](int x){ std::cout << x*factor; };

// Template — fastest, inlined by compiler
template<typename F>
void apply(int x, F fn) { fn(x); }
apply(5, [factor](int x){ std::cout << x*factor; });
```

---

## Common mistakes

```cpp
// 1. Missing parentheses — wrong type entirely
int *fp(int, int);     // declares function returning int*
int (*fp)(int, int);   // correct — pointer to function

// 2. Calling without assignment — undefined behavior
int (*fp)(int, int);
fp(5, 3);              // UB — fp is uninitialized!
fp = add;
fp(5, 3);              // OK

// 3. Signature mismatch — won't compile
void print(int x) { ... }
int (*fp)(int, int) = print;  // ERROR — wrong return type and arity

// 4. Calling the function instead of taking its address
int (*fp)(int,int) = add();   // ERROR — add() calls with no args
int (*fp)(int,int) = add;     // CORRECT

// 5. Forgetting null check before calling
int (*fp)(int,int) = nullptr;
fp(5, 3);              // CRASH — always check!
if (fp) fp(5, 3);      // safe
```

---

## Memory layout

```
STACK:
┌──────────────────────────────────┐
│  funcPtr  (8 bytes on 64-bit)    │
│  value: 0x400650 ────────────┐   │
└──────────────────────────────┼───┘
                               │
TEXT SEGMENT:                  │
┌──────────────────────────────▼───┐
│  add() at 0x400650               │
│  MOV EAX, [EBP+8]                │
│  ADD EAX, [EBP+12]               │
│  RET                             │
└──────────────────────────────────┘
```

---

## When to use function pointers

✅ C-compatible APIs and callbacks (`qsort`, `pthread_create`)
✅ Dispatch tables — O(1) runtime function selection
✅ Embedded systems — no heap, no exceptions, minimal overhead
✅ Plugin systems — function loaded from shared library
✅ When you need to store and call a stateless function

❌ Need to capture state → use `std::function` or lambda
❌ Generic algorithms → use templates
❌ OOP polymorphism → use virtual functions
❌ Qt signals/slots → use Qt's own mechanism
