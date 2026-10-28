#include <thread>
#include <functional>
#include <chrono>
#include <print>

// ============================================
// 1. REGULAR FUNCTION
// ============================================
void regular_function(int id) {
    std::println("1. Regular function called with id: {}", id);
}

// ============================================
// 2. FUNCTION WITH MULTIPLE PARAMETERS
// ============================================
void function_with_params(int a, double b, std::string c) {
    std::println("2. Function with params: {}, {}, {}", a, b, c);
}

// ============================================
// 3. FUNCTION RETURNING VALUE (result ignored by thread)
// ============================================
int function_with_return(int x) {
    std::println("3. Function with return value: {}", x);
    return x * 2;  // Return value is ignored when used with std::thread
}

// ============================================
// 4. FUNCTION WITH REFERENCE PARAMETERS
// ============================================
void function_with_reference(int& counter) {
    // Must use std::ref() when passing to thread
    counter++;
    std::println("4. Function with reference, counter now: {}", counter);
}

// ============================================
// 5. FUNCTION POINTER
// ============================================
void simple_task() {
    std::println("5. Function pointer called");
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
        std::println("7. Functor called with id: {}", id);
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
        std::println("8. Stateful functor, count: {}", count);
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
        std::println("9. Member function: value={}, param={}", value, x);
    }

    void const_member_function(int x) const {
        std::println("   Const member function: value={}, param={}", value, x);
    }
};

// ============================================
// 10. STATIC MEMBER FUNCTION
// ============================================
class StaticClass {
public:
    static void static_function(int id) {
        std::println("10. Static member function with id: {}", id);
    }
};

// ============================================
// 11. STD::FUNCTION (Function Wrapper)
// ============================================
void wrapped_function(int x) {
    std::println("11. std::function wrapper with x: {}", x);
}

// ============================================
// 12. STD::BIND (Bound Function)
// ============================================
void bind_example(int a, int b, int c) {
    std::println("12. std::bind - a={}, b={}, c={}", a, b, c);
}

// ============================================
// 13. VARIADIC TEMPLATE FUNCTION
// ============================================
template<typename... Args>
void variadic_function(Args... args) {
    std::println("13. Variadic function with {} arguments", sizeof...(args));
}

// ============================================
// 14. CALLABLE WITH MOVE-ONLY PARAMETERS
// ============================================
void move_only_param(std::unique_ptr<int> ptr) {
    std::println("14. Move-only parameter: {}", *ptr);
}

// ============================================
// MAIN - DEMONSTRATING ALL CALLABLES
// ============================================
int main() {
    std::println("=== CALLABLES IN MULTITHREADING ===");
    std::println("");

    // 1. Regular Function
    std::println("--- 1. REGULAR FUNCTION ---");
    std::thread t1(regular_function, 100);
    t1.join();
    std::println("");

    // 2. Function with Multiple Parameters
    std::println("--- 2. FUNCTION WITH MULTIPLE PARAMETERS ---");
    std::thread t2(function_with_params, 42, 3.14, "Hello");
    t2.join();
    std::println("");

    // 3. Function with Return Value (ignored)
    std::println("--- 3. FUNCTION WITH RETURN VALUE ---");
    std::thread t3(function_with_return, 21);
    t3.join();
    std::println("Note: Return value is ignored by std::thread");
    std::println("");

    // 4. Function with Reference Parameters
    std::println("--- 4. FUNCTION WITH REFERENCE ---");
    int counter = 10;
    std::println("Counter before: {}", counter);
    std::thread t4(function_with_reference, std::ref(counter));  // Must use std::ref()
    t4.join();
    std::println("Counter after: {}", counter);
    std::println("");

    // 5. Function Pointer
    std::println("--- 5. FUNCTION POINTER ---");
    void (*func_ptr)() = simple_task;
    std::thread t5(func_ptr);
    t5.join();
    std::println("");

    // 6. Lambda Expressions
    std::println("--- 6. LAMBDA EXPRESSIONS ---");

    // Simple lambda
    std::thread t6a([]() {
        std::println("   a) Simple lambda (no parameters)");
    });
    t6a.join();

    // Lambda with parameters
    std::thread t6b([](int x, std::string s) {
        std::println("   b) Lambda with params: {}, {}", x, s);
    }, 99, "World");
    t6b.join();

    // Lambda with capture
    int captured_value = 555;
    std::thread t6c([captured_value]() {
        std::println("   c) Lambda with capture [=]: {}", captured_value);
    });
    t6c.join();

    // Lambda with reference capture
    int modify_me = 10;
    std::thread t6d([&modify_me]() {
        modify_me = 20;
        std::println("   d) Lambda with reference capture [&]: {}", modify_me);
    });
    t6d.join();
    std::println("   Value after thread: {}", modify_me);

    // Lambda with mutable
    std::thread t6e([captured_value]() mutable {
        captured_value++;
        std::println("   e) Mutable lambda: {}", captured_value);
    });
    t6e.join();
    std::println("");

    // 7. Functor (Function Object)
    std::println("--- 7. FUNCTOR ---");
    Functor functor_obj;
    std::thread t7(functor_obj, 777);
    t7.join();
    std::println("");

    // 8. Stateful Functor
    std::println("--- 8. STATEFUL FUNCTOR ---");
    Counter counter_obj(100);
    std::thread t8(counter_obj);  // Creates a copy
    t8.join();
    std::println("");

    // 9. Class Member Function
    std::println("--- 9. CLASS MEMBER FUNCTION ---");
    MyClass obj(42);
    // Syntax: &ClassName::method, &object, parameters
    std::thread t9a(&MyClass::member_function, &obj, 10);
    t9a.join();

    std::thread t9b(&MyClass::const_member_function, &obj, 20);
    t9b.join();
    std::println("");

    // 10. Static Member Function
    std::println("--- 10. STATIC MEMBER FUNCTION ---");
    std::thread t10(StaticClass::static_function, 888);
    // Or: thread t10(&StaticClass::static_function, 888);
    t10.join();
    std::println("");

    // 11. std::function (Function Wrapper)
    std::println("--- 11. STD::FUNCTION ---");
    std::function<void(int)> func_wrapper = wrapped_function;
    std::thread t11(func_wrapper, 999);
    t11.join();

    // std::function can wrap lambda
    std::function<void()> lambda_wrapper = []() {
        std::println("   std::function wrapping lambda");
    };
    std::thread t11b(lambda_wrapper);
    t11b.join();
    std::println("");

    // 12. std::bind
    std::println("--- 12. STD::BIND ---");
    // Bind some parameters, leave others for thread creation
    auto bound_func = std::bind(bind_example, 1, std::placeholders::_1, 3);
    std::thread t12(bound_func, 2);  // 2 fills placeholders::_1
    t12.join();

    // Bind all parameters
    auto fully_bound = std::bind(bind_example, 10, 20, 30);
    std::thread t12b(fully_bound);
    t12b.join();
    std::println("");

    // 13. Variadic Template Function
    std::println("--- 13. VARIADIC TEMPLATE ---");
    std::thread t13(variadic_function<int, double, std::string>, 1, 2.5, "test");
    t13.join();
    std::println("");

    // 14. Move-only Parameters
    std::println("--- 14. MOVE-ONLY PARAMETERS ---");
    std::unique_ptr<int> ptr = std::make_unique<int>(12345);
    std::thread t14(move_only_param, move(ptr));
    // ptr is now nullptr
    t14.join();
    std::println("");

    // ============================================
    // BONUS: Passing by Value vs Reference
    // ============================================
    std::println("=== BONUS: VALUE vs REFERENCE ===");

    int value = 100;

    // By value (default) - changes don't affect original
    std::thread tv([](int x) {
        x = 200;
        std::println("Inside thread (by value): {}", x);
    }, value);
    tv.join();
    std::println("Outside thread: {} (unchanged)", value);
    std::println("");

    // By reference (using std::ref) - changes affect original
    std::thread tr([](int& x) {
        x = 300;
        std::println("Inside thread (by ref): {}", x);
    }, std::ref(value));
    tr.join();
    std::println("Outside thread: {} (changed!)", value);
    return 0;
}
