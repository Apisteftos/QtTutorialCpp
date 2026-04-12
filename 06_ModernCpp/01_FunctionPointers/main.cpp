#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <print>
#include <functional>

// ============================================================
// FUNCTION POINTERS in C++23
// ============================================================
// Topics covered:
//   1.  Function address — functions live in memory
//   2.  Basic declaration and assignment
//   3.  Calling through a pointer (direct vs indirect)
//   4.  Function pointer as parameter (callback)
//   5.  Array of function pointers — dispatch table
//   6.  Returning a function pointer from a function
//   7.  typedef and using — readable aliases
//   8.  Function pointer vs std::function vs template
//   9.  Member function pointers
//  10.  Practical: calculator, menu system, event handler
// ============================================================


// ============================================================
// Free functions used across examples
// ============================================================
int  add      (int a, int b) { return a + b; }
int  subtract (int a, int b) { return a - b; }
int  multiply (int a, int b) { return a * b; }
int  divide   (int a, int b) { return b != 0 ? a / b : 0; }

void printDouble(int x) { std::cout << x * 2 << ' '; }
void printSquare(int x) { std::cout << x * x << ' '; }
void printNegate(int x) { std::cout << -x    << ' '; }

bool ascending (int a, int b) { return a < b; }
bool descending(int a, int b) { return a > b; }


// ============================================================
// Example 4 — Function pointer as parameter
// ============================================================
void processArray(const int arr[], int size, void (*callback)(int))
{
    for (int i = 0; i < size; ++i)
        callback(arr[i]);
}

void bubbleSort(int arr[], int size, bool (*compare)(int, int))
{
    for (int i = 0; i < size - 1; ++i)
        for (int j = 0; j < size - i - 1; ++j)
            if (compare(arr[j + 1], arr[j]))
                std::swap(arr[j], arr[j + 1]);
}


// ============================================================
// Example 6 — Returning a function pointer
// ============================================================
// Returns the correct math operation based on operator char
int (*getOperation(char op))(int, int)
{
    switch (op) {
    case '+': return add;
    case '-': return subtract;
    case '*': return multiply;
    case '/': return divide;
    default:  return nullptr;
    }
}

// Cleaner with 'using' alias (see Example 7)
using BinaryOp = int(*)(int, int);
BinaryOp selectOp(char op)
{
    switch (op) {
    case '+': return add;
    case '-': return subtract;
    case '*': return multiply;
    case '/': return divide;
    default:  return nullptr;
    }
}


// ============================================================
// Example 9 — Member function pointers
// ============================================================
class Calculator {
public:
    int add      (int a, int b) { return a + b; }
    int subtract (int a, int b) { return a - b; }
    int multiply (int a, int b) { return a * b; }

    void printResult(int result) const {
        std::println("  Calculator result: {}", result);
    }
};


// ============================================================
// Example 10 — Event handler system
// ============================================================
void onMouseClick  () { std::cout << "  [EVENT] Mouse clicked\n"; }
void onKeyPress    () { std::cout << "  [EVENT] Key pressed\n";   }
void onWindowResize() { std::cout << "  [EVENT] Window resized\n";}
void onWindowClose () { std::cout << "  [EVENT] Window closing\n";}

struct EventHandler {
    void      (*handler)();
    const char* name;
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Function address\n";
    std::cout << "=====================================================\n";

    // Every function has a memory address in the TEXT segment
    std::println("address of add():      {}", (void*)add);
    std::println("address of subtract(): {}", (void*)subtract);
    std::println("address of main():     {}", (void*)main);

    // Function name alone = its address (no call)
    // add      → address of add
    // &add     → also address of add (equivalent)
    // add()    → calls add (needs arguments)
    std::cout << "add == &add: " << (add == &add ? "true" : "false") << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Declaration and assignment\n";
    std::cout << "=====================================================\n";

    // Syntax: ReturnType (*pointerName)(ParameterTypes)
    int (*funcPtr)(int, int);       // uninitialized — declare only
    funcPtr = add;                  // assign: store address of add
    std::println("funcPtr = add  -> funcPtr(3, 4) = {}", funcPtr(3, 4));

    funcPtr = subtract;             // reassign: now points to subtract
    std::println("funcPtr = sub  -> funcPtr(10,3) = {}", funcPtr(10, 3));

    funcPtr = multiply;
    std::println("funcPtr = mul  -> funcPtr(6, 7) = {}", funcPtr(6, 7));

    // Initialize at declaration
    int (*opPtr)(int, int) = add;
    std::println("opPtr(100,50)  = {}", opPtr(100, 50));

    // nullptr — safe default
    int (*safePtr)(int, int) = nullptr;
    if (safePtr == nullptr)
        std::cout << "safePtr is null — not yet assigned\n";

    safePtr = divide;
    std::println("safePtr(20, 4) = {}", safePtr(20, 4));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Calling — direct vs indirect\n";
    std::cout << "=====================================================\n";

    int (*fp)(int, int) = add;

    // All three call the same function
    int r1 = add(5, 3);          // direct call
    int r2 = fp(5, 3);           // indirect — implicit dereference
    int r3 = (*fp)(5, 3);        // indirect — explicit dereference

    std::println("direct:            add(5,3)   = {}", r1);
    std::println("indirect (fp):     fp(5,3)    = {}", r2);
    std::println("indirect (*fp):    (*fp)(5,3) = {}", r3);
    std::cout << "(all three produce the same result)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Function pointer as parameter\n";
    std::cout << "=====================================================\n";

    int numbers[] = {1, 2, 3, 4, 5};

    std::cout << "printDouble: ";
    processArray(numbers, 5, printDouble);
    std::cout << '\n';

    std::cout << "printSquare: ";
    processArray(numbers, 5, printSquare);
    std::cout << '\n';

    std::cout << "printNegate: ";
    processArray(numbers, 5, printNegate);
    std::cout << '\n';

    // Custom sort with function pointer comparator
    int arr[] = {5, 3, 8, 1, 9, 2, 7};
    bubbleSort(arr, 7, ascending);
    std::cout << "sorted asc:  ";
    for (int x : arr) std::cout << x << ' '; std::cout << '\n';

    bubbleSort(arr, 7, descending);
    std::cout << "sorted desc: ";
    for (int x : arr) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Array of function pointers — dispatch table\n";
    std::cout << "=====================================================\n";

    // Array of pointers — all same signature
    int (*ops[4])(int, int) = {add, subtract, multiply, divide};
    const char* opNames[]   = {"add", "sub", "mul", "div"};

    int a = 20, b = 4;
    for (int i = 0; i < 4; ++i)
        std::println("ops[{}] {} ({},{}) = {}", i, opNames[i], a, b, ops[i](a, b));

    // Dispatch by index — O(1) instead of if-else chain
    int choice = 2;   // multiply
    std::println("dispatch[{}] = {}", choice, ops[choice](a, b));

    // Array of void(*)(int) callbacks
    void (*transforms[3])(int) = {printDouble, printSquare, printNegate};
    const char* tNames[]       = {"double", "square", "negate"};

    for (int i = 0; i < 3; ++i) {
        std::cout << tNames[i] << ": ";
        for (int n : {1, 2, 3, 4, 5}) transforms[i](n);
        std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Returning a function pointer\n";
    std::cout << "=====================================================\n";

    // getOperation returns int(*)(int,int)
    char operators[] = {'+', '-', '*', '/'};
    for (char op : operators) {
        auto fn = getOperation(op);
        if (fn)
            std::println("getOperation('{}')(12,3) = {}", op, fn(12, 3));
    }

    // Cleaner with using alias
    BinaryOp fn = selectOp('*');
    std::println("selectOp('*')(7,8) = {}", fn ? fn(7, 8) : -1);

    // nullptr check
    BinaryOp bad = selectOp('?');
    std::cout << "selectOp('?') = " << (bad == nullptr ? "nullptr" : "valid") << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: typedef and using — readable aliases\n";
    std::cout << "=====================================================\n";

    // Without alias — hard to read
    void (*p1)(int) = printDouble;

    // typedef (C++03 style)
    typedef void (*IntTransform)(int);
    IntTransform p2 = printSquare;

    // using (C++11 style — preferred)
    using IntPredicate = bool(*)(int, int);
    using MathOp       = int(*)(int, int);
    using VoidCallback = void(*)();

    MathOp    mathFn  = add;
    IntTransform tfn  = printNegate;

    std::cout << "IntTransform(printDouble): "; p1(5); std::cout << '\n';
    std::cout << "IntTransform(printSquare): "; p2(5); std::cout << '\n';
    std::cout << "MathOp(add)(8,2):          " << mathFn(8, 2) << '\n';
    std::cout << "IntTransform(printNegate): "; tfn(5); std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Function pointer vs std::function vs template\n";
    std::cout << "=====================================================\n";

    // Function pointer — no state, no lambda with capture
    void (*fpNoCapture)(int) = printDouble;
    std::cout << "function pointer: ";
    fpNoCapture(5); std::cout << '\n';

    // std::function — accepts lambdas with capture
    int multiplier = 10;
    std::function<void(int)> sfn = [multiplier](int x) {
        std::cout << x * multiplier << ' ';
    };
    std::cout << "std::function:    "; sfn(5); std::cout << '\n';

    // Function pointer CANNOT hold a capturing lambda:
    // void (*bad)(int) = [multiplier](int x){ ... };  // ERROR!

    // Template — zero overhead, inlined
    auto callWith5 = [](auto fn) { fn(5); };
    std::cout << "template:         ";
    callWith5([](int x) { std::cout << x * 3; });
    std::cout << '\n';

    std::cout << "\nComparison:\n";
    std::cout << "  function ptr  — fastest, no capture, C-compatible\n";
    std::cout << "  std::function — flexible, captures, small overhead\n";
    std::cout << "  template      — fastest, inlined, compile-time only\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Member function pointers\n";
    std::cout << "=====================================================\n";

    Calculator calc;

    // Member function pointer syntax: ReturnType (ClassName::*name)(params)
    int (Calculator::*memberFp)(int, int) = &Calculator::add;

    // Call via object
    int res1 = (calc.*memberFp)(10, 5);
    std::println("(calc.*memberFp)(10,5) add = {}", res1);

    memberFp = &Calculator::multiply;
    int res2 = (calc.*memberFp)(6, 7);
    std::println("(calc.*memberFp)(6,7) mul  = {}", res2);

    // Via pointer to object
    Calculator* calcPtr = &calc;
    memberFp = &Calculator::subtract;
    int res3 = (calcPtr->*memberFp)(20, 8);
    std::println("(calcPtr->*fp)(20,8) sub   = {}", res3);

    // Array of member function pointers
    int (Calculator::*mathOps[3])(int, int) = {
        &Calculator::add,
        &Calculator::subtract,
        &Calculator::multiply
    };
    const char* mathNames[] = {"add", "sub", "mul"};

    std::cout << "member function dispatch:\n";
    for (int i = 0; i < 3; ++i)
        std::println("  {}(15, 3) = {}", mathNames[i], (calc.*mathOps[i])(15, 3));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Practical — calculator, menu, events\n";
    std::cout << "=====================================================\n";

    // --- Calculator ---
    std::cout << "Calculator:\n";
    struct Calculation { char op; int a; int b; };
    Calculation calcs[] = {{'+',10,5},{'-',10,5},{'*',10,5},{'/',10,5}};
    for (auto& c : calcs) {
        BinaryOp op = selectOp(c.op);
        if (op) std::println("  {} {} {} = {}", c.a, c.op, c.b, op(c.a, c.b));
    }

    // --- Menu system ---
    std::cout << "\nMenu system:\n";
    void (*menuActions[4])() = {
        []() { std::cout << "  [1] New file created\n";   },
        []() { std::cout << "  [2] File opened\n";        },
        []() { std::cout << "  [3] File saved\n";         },
        []() { std::cout << "  [4] Application quit\n";   }
    };
    const char* menuLabels[] = {"New", "Open", "Save", "Quit"};

    for (int i = 0; i < 4; ++i) {
        std::println("  executing menu[{}] '{}':", i+1, menuLabels[i]);
        menuActions[i]();
    }

    // --- Event handler table ---
    std::cout << "\nEvent handler system:\n";
    EventHandler events[] = {
        {onMouseClick,   "MOUSE_CLICK"},
        {onKeyPress,     "KEY_PRESS"},
        {onWindowResize, "WINDOW_RESIZE"},
        {onWindowClose,  "WINDOW_CLOSE"}
    };

    for (auto& ev : events) {
        std::println("  firing event: {}", ev.name);
        ev.handler();
    }

    std::cout << "\n=== FunctionPointers complete ===\n";
    return 0;
}
