#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
using namespace std;

// ============================================
// 1. REGULAR FUNCTION
// ============================================
void regular_function(int id) {
    cout << "1. Regular function called with id: " << id << "\n";
}

// ============================================
// 2. FUNCTION WITH MULTIPLE PARAMETERS
// ============================================
void function_with_params(int a, double b, string c) {
    cout << "2. Function with params: " << a << ", " << b << ", " << c << "\n";
}

// ============================================
// 3. FUNCTION RETURNING VALUE (result ignored by thread)
// ============================================
int function_with_return(int x) {
    cout << "3. Function with return value: " << x << "\n";
    return x * 2;  // Return value is ignored when used with std::thread
}

// ============================================
// 4. FUNCTION WITH REFERENCE PARAMETERS
// ============================================
void function_with_reference(int& counter) {
    // Must use std::ref() when passing to thread
    counter++;
    cout << "4. Function with reference, counter now: " << counter << "\n";
}

// ============================================
// 5. FUNCTION POINTER
// ============================================
void simple_task() {
    cout << "5. Function pointer called\n";
}

// ============================================
// 6. LAMBDA EXPRESSION (Anonymous Function)
// ============================================
// Lambdas are defined inline - see main() for examples

// ============================================
// 7. FUNCTOR (Function Object / Callable Object)
// ============================================
class Functor {
public:
    void operator()(int id) const {
        cout << "7. Functor called with id: " << id << "\n";
    }
};

// ============================================
// 8. FUNCTOR WITH STATE
// ============================================
class Counter {
private:
    int count;
public:
    Counter(int initial) : count(initial) {}

    void operator()() {
        count++;
        cout << "8. Stateful functor, count: " << count << "\n";
    }
};

// ============================================
// 9. CLASS MEMBER FUNCTION (Non-static)
// ============================================
class MyClass {
private:
    int value;
public:
    MyClass(int v) : value(v) {}

    void member_function(int x) {
        cout << "9. Member function: value=" << value << ", param=" << x << "\n";
    }

    void const_member_function(int x) const {
        cout << "   Const member function: value=" << value << ", param=" << x << "\n";
    }
};

// ============================================
// 10. STATIC MEMBER FUNCTION
// ============================================
class StaticClass {
public:
    static void static_function(int id) {
        cout << "10. Static member function with id: " << id << "\n";
    }
};

// ============================================
// 11. STD::FUNCTION (Function Wrapper)
// ============================================
void wrapped_function(int x) {
    cout << "11. std::function wrapper with x: " << x << "\n";
}

// ============================================
// 12. STD::BIND (Bound Function)
// ============================================
void bind_example(int a, int b, int c) {
    cout << "12. std::bind - a=" << a << ", b=" << b << ", c=" << c << "\n";
}

// ============================================
// 13. VARIADIC TEMPLATE FUNCTION
// ============================================
template<typename... Args>
void variadic_function(Args... args) {
    cout << "13. Variadic function with " << sizeof...(args) << " arguments\n";
}

// ============================================
// 14. CALLABLE WITH MOVE-ONLY PARAMETERS
// ============================================
void move_only_param(unique_ptr<int> ptr) {
    cout << "14. Move-only parameter: " << *ptr << "\n";
}

// ============================================
// MAIN - DEMONSTRATING ALL CALLABLES
// ============================================
int main() {
    cout << "=== CALLABLES IN MULTITHREADING ===\n\n";

    // 1. Regular Function
    cout << "--- 1. REGULAR FUNCTION ---\n";
    thread t1(regular_function, 100);
    t1.join();
    cout << "\n";

    // 2. Function with Multiple Parameters
    cout << "--- 2. FUNCTION WITH MULTIPLE PARAMETERS ---\n";
    thread t2(function_with_params, 42, 3.14, "Hello");
    t2.join();
    cout << "\n";

    // 3. Function with Return Value (ignored)
    cout << "--- 3. FUNCTION WITH RETURN VALUE ---\n";
    thread t3(function_with_return, 21);
    t3.join();
    cout << "Note: Return value is ignored by std::thread\n\n";

    // 4. Function with Reference Parameters
    cout << "--- 4. FUNCTION WITH REFERENCE ---\n";
    int counter = 10;
    cout << "Counter before: " << counter << "\n";
    thread t4(function_with_reference, ref(counter));  // Must use std::ref()
    t4.join();
    cout << "Counter after: " << counter << "\n\n";

    // 5. Function Pointer
    cout << "--- 5. FUNCTION POINTER ---\n";
    void (*func_ptr)() = simple_task;
    thread t5(func_ptr);
    t5.join();
    cout << "\n";

    // 6. Lambda Expressions
    cout << "--- 6. LAMBDA EXPRESSIONS ---\n";

    // Simple lambda
    thread t6a([]() {
        cout << "   a) Simple lambda (no parameters)\n";
    });
    t6a.join();

    // Lambda with parameters
    thread t6b([](int x, string s) {
        cout << "   b) Lambda with params: " << x << ", " << s << "\n";
    }, 99, "World");
    t6b.join();

    // Lambda with capture
    int captured_value = 555;
    thread t6c([captured_value]() {
        cout << "   c) Lambda with capture [=]: " << captured_value << "\n";
    });
    t6c.join();

    // Lambda with reference capture
    int modify_me = 10;
    thread t6d([&modify_me]() {
        modify_me = 20;
        cout << "   d) Lambda with reference capture [&]: " << modify_me << "\n";
    });
    t6d.join();
    cout << "   Value after thread: " << modify_me << "\n";

    // Lambda with mutable
    thread t6e([captured_value]() mutable {
        captured_value++;
        cout << "   e) Mutable lambda: " << captured_value << "\n";
    });
    t6e.join();
    cout << "\n";

    // 7. Functor (Function Object)
    cout << "--- 7. FUNCTOR ---\n";
    Functor functor_obj;
    thread t7(functor_obj, 777);
    t7.join();
    cout << "\n";

    // 8. Stateful Functor
    cout << "--- 8. STATEFUL FUNCTOR ---\n";
    Counter counter_obj(100);
    thread t8(counter_obj);  // Creates a copy
    t8.join();
    cout << "\n";

    // 9. Class Member Function
    cout << "--- 9. CLASS MEMBER FUNCTION ---\n";
    MyClass obj(42);
    // Syntax: &ClassName::method, &object, parameters
    thread t9a(&MyClass::member_function, &obj, 10);
    t9a.join();

    thread t9b(&MyClass::const_member_function, &obj, 20);
    t9b.join();
    cout << "\n";

    // 10. Static Member Function
    cout << "--- 10. STATIC MEMBER FUNCTION ---\n";
    thread t10(StaticClass::static_function, 888);
    // Or: thread t10(&StaticClass::static_function, 888);
    t10.join();
    cout << "\n";

    // 11. std::function (Function Wrapper)
    cout << "--- 11. STD::FUNCTION ---\n";
    function<void(int)> func_wrapper = wrapped_function;
    thread t11(func_wrapper, 999);
    t11.join();

    // std::function can wrap lambda
    function<void()> lambda_wrapper = []() {
        cout << "   std::function wrapping lambda\n";
    };
    thread t11b(lambda_wrapper);
    t11b.join();
    cout << "\n";

    // 12. std::bind
    cout << "--- 12. STD::BIND ---\n";
    // Bind some parameters, leave others for thread creation
    auto bound_func = bind(bind_example, 1, placeholders::_1, 3);
    thread t12(bound_func, 2);  // 2 fills placeholders::_1
    t12.join();

    // Bind all parameters
    auto fully_bound = bind(bind_example, 10, 20, 30);
    thread t12b(fully_bound);
    t12b.join();
    cout << "\n";

    // 13. Variadic Template Function
    cout << "--- 13. VARIADIC TEMPLATE ---\n";
    thread t13(variadic_function<int, double, string>, 1, 2.5, "test");
    t13.join();
    cout << "\n";

    // 14. Move-only Parameters
    cout << "--- 14. MOVE-ONLY PARAMETERS ---\n";
    unique_ptr<int> ptr = make_unique<int>(12345);
    thread t14(move_only_param, move(ptr));
    // ptr is now nullptr
    t14.join();
    cout << "\n";

    // ============================================
    // BONUS: Passing by Value vs Reference
    // ============================================
    cout << "=== BONUS: VALUE vs REFERENCE ===\n";

    int value = 100;

    // By value (default) - changes don't affect original
    thread tv([](int x) {
        x = 200;
        cout << "Inside thread (by value): " << x << "\n";
    }, value);
    tv.join();
    cout << "Outside thread: " << value << " (unchanged)\n\n";

    // By reference (using std::ref) - changes affect original
    thread tr([](int& x) {
        x = 300;
        cout << "Inside thread (by ref): " << x << "\n";
    }, ref(value));
    tr.join();
    cout << "Outside thread: " << value << " (changed!)\n\n";

    // ============================================
    // SUMMARY
    // ============================================
    cout << "=== SUMMARY OF CALLABLES ===\n";
    cout << "1.  Regular function\n";
    cout << "2.  Function with parameters\n";
    cout << "3.  Function with return value\n";
    cout << "4.  Function with reference parameters\n";
    cout << "5.  Function pointer\n";
    cout << "6.  Lambda expressions (with captures)\n";
    cout << "7.  Functors (callable objects)\n";
    cout << "8.  Stateful functors\n";
    cout << "9.  Class member functions\n";
    cout << "10. Static member functions\n";
    cout << "11. std::function wrappers\n";
    cout << "12. std::bind bound functions\n";
    cout << "13. Variadic template functions\n";
    cout << "14. Functions with move-only parameters\n";

    return 0;
}
