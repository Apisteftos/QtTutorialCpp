#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <memory>
#include <print>

// ============================================================
// LAMBDAS in C++23
// ============================================================
// Topics covered:
//   1.  Basic lambda syntax
//   2.  Capture modes: [], [=], [&], [x], [&x], [this]
//   3.  Lambda with parameters and return type
//   4.  Mutable lambda
//   5.  Lambda stored in auto and std::function
//   6.  Lambda as algorithm argument
//   7.  Generic lambda (C++14) and templated lambda (C++20)
//   8.  Immediately Invoked Lambda Expression (IILE)
//   9.  Lambda returning lambda (higher-order functions)
//  10.  Practical: MCX event filters, pipelines, comparators
// ============================================================


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Basic Lambda Syntax\n";
    std::cout << "=====================================================\n";

    // Minimal lambda — no parameters, no return
    auto hello = []() { std::cout << "Hello from lambda!\n"; };
    hello();

    // Lambda called immediately
    []() { std::cout << "Immediately invoked!\n"; }();

    // Lambda with body
    auto greet = [](const std::string& name) {
        std::println("Hello, {}!", name);
    };
    greet("Kostas");
    greet("World");

    // Lambda with return value
    auto square = [](int x) { return x * x; };
    std::println("square(7) = {}", square(7));

    // Explicit return type
    auto divide = [](double a, double b) -> double {
        if (b == 0.0) return 0.0;
        return a / b;
    };
    std::println("divide(10.0, 3.0) = {:.4f}", divide(10.0, 3.0));

    // Lambda anatomy:
    // [ captures ] ( parameters ) -> return_type { body }
    //     ^              ^               ^            ^
    //  what outer      inputs        optional      code
    //  variables                      explicit


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Capture Modes\n";
    std::cout << "=====================================================\n";

    int  x      = 10;
    int  y      = 20;
    std::string label = "value";

    // [] — capture nothing (no access to outer variables)
    auto noCapture = []() { std::cout << "no capture\n"; };
    noCapture();

    // [=] — capture ALL by value (copies)
    auto captureAllByVal = [=]() {
        std::println("[=] x={} y={} label={}", x, y, label);
        // x and y are COPIES — modifying them here won't affect outer
    };
    captureAllByVal();

    // [&] — capture ALL by reference
    auto captureAllByRef = [&]() {
        x += 100;   // modifies the OUTER x!
        std::println("[&] x={} y={}", x, y);
    };
    captureAllByRef();
    std::println("outer x after [&] lambda: {}", x);

    x = 10;  // reset

    // [x] — capture only x by value
    auto captureXByVal = [x]() {
        std::println("[x] x={} (copy)", x);
    };
    captureXByVal();

    // [&x] — capture only x by reference
    auto captureXByRef = [&x]() {
        x *= 2;
        std::println("[&x] x={} (reference, modified)", x);
    };
    captureXByRef();
    std::println("outer x after [&x] lambda: {}", x);  // changed!

    x = 10;  // reset

    // [=, &y] — capture all by value EXCEPT y by reference
    auto mixed = [=, &y]() {
        y += 5;    // y is by ref — modifies outer
        std::println("[=,&y] x={} (copy) y={} (ref)", x, y);
    };
    mixed();
    std::println("y after mixed lambda: {}", y);  // changed!

    // [&, x] — capture all by reference EXCEPT x by value
    auto mixed2 = [&, x]() {
        std::println("[&,x] x={} (copy) y={} (ref)", x, y);
    };
    mixed2();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Parameters and Return Types\n";
    std::cout << "=====================================================\n";

    // Multiple parameters
    auto add = [](int a, int b) { return a + b; };
    std::println("add(3, 4) = {}", add(3, 4));

    // Default parameter values (C++14)
    auto greetDefault = [](const std::string& name,
                           const std::string& greeting = "Hello") {
        std::println("{}, {}!", greeting, name);
    };
    greetDefault("Alice");
    greetDefault("Bob", "Hi");

    // Explicit return type — needed when body has multiple return paths
    auto classify = [](int n) -> std::string {
        if (n < 0)  return "negative";
        if (n == 0) return "zero";
        return "positive";
    };
    std::println("classify(-5): {}", classify(-5));
    std::println("classify(0):  {}", classify(0));
    std::println("classify(7):  {}", classify(7));

    // Variadic parameters via forwarding (C++20)
    auto printAll = [](auto&&... args) {
        ((std::cout << args << ' '), ...);
        std::cout << '\n';
    };
    std::cout << "printAll: ";
    printAll(1, "hello", 3.14, true);

    // noexcept lambda
    auto safeAdd = [](int a, int b) noexcept { return a + b; };
    std::println("safeAdd(5,6) = {}", safeAdd(5, 6));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Mutable Lambda\n";
    std::cout << "=====================================================\n";

    // By default, captured-by-value variables are const inside lambda
    int counter = 0;

    // auto bad = [counter]() { counter++; };  // ERROR: counter is const!

    // mutable — allows modifying captured copies
    auto counting = [counter]() mutable {
        ++counter;   // modifies the COPY — outer counter unchanged
        std::println("  inside mutable lambda: counter={}", counter);
    };

    counting();   // 1
    counting();   // 2
    counting();   // 3
    std::println("outer counter after mutable calls: {} (unchanged)", counter);

    // Contrast: capture by reference modifies outer
    auto countingRef = [&counter]() {
        ++counter;
        std::println("  inside ref lambda: counter={}", counter);
    };
    countingRef();   // 1
    countingRef();   // 2
    std::println("outer counter after ref calls: {} (changed)", counter);

    // Mutable lambda as stateful generator
    auto makeCounter = [n = 0]() mutable -> int {
        return ++n;   // n is owned by the lambda — persists between calls!
    };
    std::cout << "generator: ";
    for (int i = 0; i < 5; ++i)
        std::cout << makeCounter() << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Storing Lambdas\n";
    std::cout << "=====================================================\n";

    // auto — lambda's own unique type (most efficient)
    auto fn1 = [](int x) { return x * 2; };
    std::println("auto stored lambda: fn1(5) = {}", fn1(5));

    // std::function — type-erased, stores any callable
    std::function<int(int)> fn2 = [](int x) { return x * 3; };
    std::println("std::function:      fn2(5) = {}", fn2(5));

    // std::function accepts capturing lambda
    int factor = 4;
    std::function<int(int)> fn3 = [factor](int x) { return x * factor; };
    std::println("std::function cap:  fn3(5) = {}", fn3(5));

    // Reassign std::function at runtime
    fn2 = [](int x) { return x * x; };
    std::println("fn2 reassigned:     fn2(5) = {}", fn2(5));

    // Store in vector
    std::vector<std::function<int(int)>> transforms;
    transforms.push_back([](int x) { return x + 1;  });
    transforms.push_back([](int x) { return x * 2;  });
    transforms.push_back([](int x) { return x * x;  });
    transforms.push_back([](int x) { return -x;     });

    std::cout << "applying transforms to 5: ";
    for (auto& t : transforms)
        std::cout << t(5) << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Lambda with STL Algorithms\n";
    std::cout << "=====================================================\n";

    std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    // sort with lambda comparator
    std::sort(v.begin(), v.end(), [](int a, int b) { return a < b; });
    std::cout << "sorted asc:    ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    std::cout << "sorted desc:   ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // find_if
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x < 4; });
    if (it != v.end()) std::println("first < 4: {}", *it);

    // count_if
    int evens = std::count_if(v.begin(), v.end(), [](int x) { return x%2==0; });
    std::println("even count: {}", evens);

    // transform
    std::vector<int> squared(v.size());
    std::transform(v.begin(), v.end(), squared.begin(),
                   [](int x) { return x*x; });
    std::cout << "squared:       ";
    for (int x : squared) std::cout << x << ' '; std::cout << '\n';

    // remove_if + erase
    std::sort(v.begin(), v.end());
    v.erase(std::remove_if(v.begin(), v.end(),
                           [](int x) { return x % 2 == 0; }), v.end());
    std::cout << "odds only:     ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // for_each
    std::vector<int> nums = {1, 2, 3, 4, 5};
    int runSum = 0;
    std::for_each(nums.begin(), nums.end(), [&runSum](int x) { runSum += x; });
    std::println("for_each sum: {}", runSum);

    // accumulate with lambda
    int product = std::accumulate(nums.begin(), nums.end(), 1,
                                  [](int acc, int x) { return acc * x; });
    std::println("product: {}", product);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Generic and Templated Lambdas\n";
    std::cout << "=====================================================\n";

    // Generic lambda (C++14) — auto parameters
    auto printAny = [](auto x) { std::cout << x << ' '; };
    printAny(42);
    printAny(3.14);
    printAny("hello");
    printAny(true);
    std::cout << '\n';

    // Generic lambda with two auto params
    auto addAny = [](auto a, auto b) { return a + b; };
    std::println("addAny(3, 4):       {}", addAny(3, 4));
    std::println("addAny(1.5, 2.5):   {}", addAny(1.5, 2.5));

    // Templated lambda (C++20) — explicit template syntax
    auto typed = []<typename T>(T a, T b) -> T { return a + b; };
    std::println("typed<int>(3,4):    {}", typed(3, 4));
    std::println("typed<double>(1,2): {}", typed(1.1, 2.2));

    // Templated lambda with constraints (C++20 concepts)
    auto onlyNumeric = []<std::integral T>(T a, T b) { return a + b; };
    std::println("onlyNumeric(5,3):   {}", onlyNumeric(5, 3));
    // onlyNumeric(1.5, 2.5);  // ERROR — not integral

    // Generic sort comparator
    auto byAbsValue = [](auto a, auto b) { return std::abs(a) < std::abs(b); };
    std::vector<int> absVec = {-5, 3, -1, 8, -2};
    std::sort(absVec.begin(), absVec.end(), byAbsValue);
    std::cout << "sorted by abs: ";
    for (int x : absVec) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Immediately Invoked Lambda (IILE)\n";
    std::cout << "=====================================================\n";

    // Called immediately — result used right away
    int result = [](int a, int b) { return a * b + 5; }(3, 4);
    std::println("IILE result: {}", result);

    // IILE for complex initialization (const variable initialized via complex logic)
    const std::string greeting = [&]() -> std::string {
        int hour = 14;   // simulate current hour
        if (hour < 12) return "Good morning";
        if (hour < 18) return "Good afternoon";
        return "Good evening";
    }();
    std::println("IILE greeting: {}", greeting);

    // IILE for block scope with return
    const int config = []() -> int {
        // Could read from file, parse args, etc.
        int base = 100;
        base *= 2;
        base += 50;
        return base;
    }();
    std::println("IILE config: {}", config);

    // IILE in expressions
    std::println("IILE inline: {}",
                 [](int n) { return n * n + 2 * n + 1; }(5));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Lambda Returning Lambda\n";
    std::cout << "=====================================================\n";

    // Factory: returns a lambda that multiplies by n
    auto multiplierFactory = [](int n) {
        return [n](int x) { return x * n; };
    };

    auto times2  = multiplierFactory(2);
    auto times10 = multiplierFactory(10);

    std::println("times2(7)  = {}", times2(7));
    std::println("times10(7) = {}", times10(7));

    // Adder factory
    auto adderFactory = [](int n) {
        return [n](int x) { return x + n; };
    };
    auto add5  = adderFactory(5);
    auto add100 = adderFactory(100);
    std::println("add5(42)   = {}", add5(42));
    std::println("add100(42) = {}", add100(42));

    // Compose two lambdas: compose(f, g)(x) = f(g(x))
    auto compose = [](auto f, auto g) {
        return [f, g](auto x) { return f(g(x)); };
    };

    auto doubleSquare = compose(times2, [](int x){ return x*x; });
    auto squareDouble = compose([](int x){ return x*x; }, times2);

    std::println("doubleSquare(3) = {} (square then double)", doubleSquare(3));
    std::println("squareDouble(3) = {} (double then square)", squareDouble(3));

    // Partial application
    auto partialAdd = [](int a) {
        return [a](int b) { return a + b; };
    };
    auto addTo10 = partialAdd(10);
    std::println("addTo10(5)  = {}", addTo10(5));
    std::println("addTo10(25) = {}", addTo10(25));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Practical — MCX filters, pipeline, comparators\n";
    std::cout << "=====================================================\n";

    // --- MCX message filtering ---
    struct McxMessage {
        int         id;
        std::string type;
        int         priority;
        bool        encrypted;
    };

    std::vector<McxMessage> messages = {
        {1, "GROUP_CALL",      2, true },
        {2, "INDIVIDUAL_CALL", 3, false},
        {3, "EMERGENCY_CALL",  1, true },
        {4, "FLOOR_REQUEST",   2, true },
        {5, "INDIVIDUAL_CALL", 3, true },
        {6, "EMERGENCY_CALL",  1, false}
    };

    // Filter: only emergency calls
    auto isEmergency = [](const McxMessage& m) {
        return m.type == "EMERGENCY_CALL";
    };
    std::cout << "Emergency calls:\n";
    for (const auto& m : messages)
        if (isEmergency(m))
            std::println("  [{}] {} pri={}", m.id, m.type, m.priority);

    // Filter: encrypted and high priority
    int threshold = 2;
    auto isHighPriEncrypted = [threshold](const McxMessage& m) {
        return m.priority <= threshold && m.encrypted;
    };
    std::cout << "High priority + encrypted:\n";
    for (const auto& m : messages)
        if (isHighPriEncrypted(m))
            std::println("  [{}] {} pri={} enc={}", m.id, m.type, m.priority, m.encrypted);

    // Sort by priority
    std::sort(messages.begin(), messages.end(),
              [](const McxMessage& a, const McxMessage& b) {
                  return a.priority < b.priority;
              });
    std::cout << "Sorted by priority:\n";
    for (const auto& m : messages)
        std::println("  pri={} [{}] {}", m.priority, m.id, m.type);

    // --- Processing pipeline ---
    std::cout << "\nData pipeline:\n";
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto filterEven  = [](const std::vector<int>& v) {
        std::vector<int> r;
        std::copy_if(v.begin(), v.end(), std::back_inserter(r),
                     [](int x) { return x % 2 == 0; });
        return r;
    };
    auto squareAll   = [](const std::vector<int>& v) {
        std::vector<int> r(v.size());
        std::transform(v.begin(), v.end(), r.begin(),
                       [](int x) { return x * x; });
        return r;
    };
    auto sumAll      = [](const std::vector<int>& v) {
        return std::accumulate(v.begin(), v.end(), 0);
    };

    auto filteredEvens  = filterEven(data);
    auto squaredEvens   = squareAll(filteredEvens);
    auto total          = sumAll(squaredEvens);
    std::println("sum of squares of evens [1..10]: {}", total);  // 4+16+36+64+100=220

    // --- Custom comparators ---
    std::cout << "\nCustom sort comparators:\n";
    std::vector<std::string> words = {"banana","fig","apple","kiwi","date","cherry"};

    // Sort by length
    std::sort(words.begin(), words.end(),
              [](const std::string& a, const std::string& b) {
                  return a.size() < b.size();
              });
    std::cout << "by length:  ";
    for (const auto& w : words) std::cout << w << ' '; std::cout << '\n';

    // Sort alphabetically
    std::sort(words.begin(), words.end());
    std::cout << "alpha:      ";
    for (const auto& w : words) std::cout << w << ' '; std::cout << '\n';

    // Sort by last character
    std::sort(words.begin(), words.end(),
              [](const std::string& a, const std::string& b) {
                  return a.back() < b.back();
              });
    std::cout << "last char:  ";
    for (const auto& w : words) std::cout << w << ' '; std::cout << '\n';

    std::cout << "\n=== Lambdas complete ===\n";
    return 0;
}
