#include <iostream>
#include <string>
#include <print>

// ============================================================
// FUNCTIONS in C++23 — Fundamentals
// ============================================================
// Topics covered:
//   1.  Basic function — declaration, definition, call
//   2.  Function with parameters
//   3.  Function with return value
//   4.  void function — no return value
//   5.  Function declaration (prototype) before main
//   6.  Multiple parameters, different types
//   7.  Default parameters
//   8.  Function overloading — same name, different signature
//   9.  Inline function
//  10.  Practical: greet, calculate, validate
// ============================================================


// ============================================================
// Example 5 — Declaration (prototype) before main
// Lets us define the function AFTER main
// ============================================================
int multiply(int a, int b);       // prototype — just the signature
double circleArea(double radius); // prototype


// ============================================================
// Example 9 — inline function
// Compiler may replace the call with the function body
// Best for small, frequently called functions
// ============================================================
inline int square(int x) { return x * x; }
inline int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Basic function — no params, no return\n";
    std::cout << "=====================================================\n";

    // Define a simple function as a lambda for inline demo
    // (full named functions shown below in later examples)
    auto sayHello = []() {
        std::cout << "Hello from a function!\n";
    };

    sayHello();   // call the function
    sayHello();   // call it again — reusable
    sayHello();

    std::cout << "\nFunction anatomy:\n";
    std::cout << "  returnType functionName(parameters) { body }\n";
    std::cout << "  void       sayHello    ()           { cout... }\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Function with parameters\n";
    std::cout << "=====================================================\n";

    // Named function defined outside main
    // greet(name) defined below — shows prototype usage
    auto greet = [](const std::string& name) {
        std::println("Hello, {}!", name);
    };

    greet("Kostas");
    greet("Andre");
    greet("World");

    // Parameters vs Arguments
    std::cout << "\nTerminology:\n";
    std::cout << "  parameter — variable in function definition: (string& name)\n";
    std::cout << "  argument  — value passed at call site:        greet(\"Kostas\")\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Function with return value\n";
    std::cout << "=====================================================\n";

    auto add = [](int a, int b) -> int {
        return a + b;
    };

    auto divide = [](double a, double b) -> double {
        if (b == 0.0) return 0.0;   // guard against division by zero
        return a / b;
    };

    int    sum    = add(10, 25);
    double result = divide(10.0, 3.0);

    std::println("add(10, 25)      = {}", sum);
    std::println("divide(10, 3)    = {:.4f}", result);
    std::println("add(add(1,2), 3) = {}", add(add(1, 2), 3));  // nested calls


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: void function — performs action, no return\n";
    std::cout << "=====================================================\n";

    auto printSeparator = [](char c, int n) {
        for (int i = 0; i < n; ++i) std::cout << c;
        std::cout << '\n';
    };

    auto printBox = [&](const std::string& text) {
        int width = (int)text.size() + 4;
        printSeparator('=', width);
        std::cout << "| " << text << " |\n";
        printSeparator('=', width);
    };

    printSeparator('-', 20);
    printBox("MCX Session");
    printSeparator('*', 20);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Declaration (prototype) before main\n";
    std::cout << "=====================================================\n";

    // multiply and circleArea declared above main — defined below
    std::println("multiply(6, 7)      = {}", multiply(6, 7));
    std::println("circleArea(5.0)     = {:.4f}", circleArea(5.0));
    std::println("circleArea(1.0)     = {:.4f}", circleArea(1.0));

    std::cout << "\nWhy prototypes?\n";
    std::cout << "  Lets you call functions defined later in the file\n";
    std::cout << "  Usually put in a header file (.h) in real projects\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Multiple parameters, mixed types\n";
    std::cout << "=====================================================\n";

    auto describeUser = [](const std::string& name, int age, bool active) {
        std::println("Name: {}  Age: {}  Active: {}",
                     name, age, active ? "yes" : "no");
    };

    auto calcBMI = [](double weight, double height) -> double {
        return weight / (height * height);
    };

    describeUser("Kostas", 41, true);
    describeUser("Andre",  28, false);

    double bmi = calcBMI(80.0, 1.78);
    std::println("BMI(80kg, 1.78m) = {:.1f}", bmi);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Default parameters\n";
    std::cout << "=====================================================\n";

    auto connect = [](const std::string& host,
                      int port     = 5060,
                      bool secure  = false) {
        std::println("connecting to {}:{} secure={}", host, port, secure);
    };

    connect("192.168.1.1");              // uses defaults: 5060, false
    connect("192.168.1.1", 443);         // uses default: false
    connect("192.168.1.1", 443, true);   // all specified

    std::cout << "\nRule: default params must be at the END\n";
    std::cout << "  connect(host, port=5060, secure=false)  OK\n";
    std::cout << "  connect(host=x, port, secure)           ERROR\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Function overloading\n";
    std::cout << "=====================================================\n";

    // Same name — compiler picks correct version by parameter types
    auto printValue = [](int x)         { std::println("int:    {}", x);    };
    auto printValue2 = [](double x)     { std::println("double: {}", x);    };
    auto printValue3 = [](const std::string& x) { std::println("string: {}", x); };

    // In real C++ (not lambdas) you overload with same function name:
    // void print(int x)         { ... }
    // void print(double x)      { ... }
    // void print(const string&) { ... }

    printValue(42);
    printValue2(3.14);
    printValue3("hello");

    // Named function overloading example (using multiply prototype above)
    std::println("multiply(int,int): {}", multiply(4, 5));

    std::cout << "\nOverloading rules:\n";
    std::cout << "  same name, different parameter types or count\n";
    std::cout << "  return type alone is NOT enough to overload\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: inline function\n";
    std::cout << "=====================================================\n";

    std::println("square(5)       = {}", square(5));
    std::println("square(12)      = {}", square(12));
    std::println("clamp(15, 0,10) = {}", clamp(15, 0, 10));
    std::println("clamp(-3, 0,10) = {}", clamp(-3, 0, 10));
    std::println("clamp(7,  0,10) = {}", clamp(7,  0, 10));

    std::cout << "\ninline hint:\n";
    std::cout << "  compiler may replace call with function body\n";
    std::cout << "  no function call overhead for small functions\n";
    std::cout << "  compiler ignores the hint if it decides otherwise\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Practical — greet, calculate, validate\n";
    std::cout << "=====================================================\n";

    // greet
    auto greetFormal = [](const std::string& name,
                          const std::string& title = "Mr") {
        std::println("Good day, {}. {}!", title, name);
    };
    greetFormal("Angeloulis");
    greetFormal("Schmidt", "Dr");

    // calculate
    auto percentOf = [](double percent, double total) -> double {
        return (percent / 100.0) * total;
    };
    std::println("15% of 200 = {}", percentOf(15.0, 200.0));
    std::println("7.5% of 80 = {:.2f}", percentOf(7.5, 80.0));

    // validate
    auto isValidPort = [](int port) -> bool {
        return port >= 1 && port <= 65535;
    };
    auto isValidIP = [](const std::string& ip) -> bool {
        return !ip.empty() && ip.find('.') != std::string::npos;
    };

    std::println("isValidPort(5060):       {}", isValidPort(5060));
    std::println("isValidPort(0):          {}", isValidPort(0));
    std::println("isValidPort(99999):      {}", isValidPort(99999));
    std::println("isValidIP(\"192.168.1.1\"):{}", isValidIP("192.168.1.1"));
    std::println("isValidIP(\"\"):           {}", isValidIP(""));

    std::cout << "\n--- Function summary ---\n";
    std::cout << "  void fn()              — no params, no return\n";
    std::cout << "  void fn(int x)         — with params, no return\n";
    std::cout << "  int  fn(int x)         — with params, with return\n";
    std::cout << "  int  fn(int x, int y=0)— with default param\n";
    std::cout << "  inline int fn(int x)   — inline hint\n";

    std::cout << "\n=== Functions complete ===\n";
    return 0;
}


// ============================================================
// Definitions of functions declared above main (prototypes)
// ============================================================
int multiply(int a, int b)
{
    return a * b;
}

double circleArea(double radius)
{
    constexpr double PI = 3.14159265358979;
    return PI * radius * radius;
}
