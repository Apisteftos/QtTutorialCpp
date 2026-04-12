#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <print>

// ============================================================
// std::exception / stdexcept — Exception Handling in C++23
// ============================================================
// Topics covered:
//   1.  Exception hierarchy
//   2.  try / catch / throw basics
//   3.  Standard exception types
//   4.  Custom exceptions
//   5.  Rethrowing exceptions
//   6.  noexcept specifier
//   7.  Exception in constructors / RAII
//   8.  std::exception_ptr — transfer across threads
//   9.  std::nested_exception
//  10.  Best practices
// ============================================================

// ============================================================
// Custom exception classes
// ============================================================
class AppException : public std::runtime_error {
public:
    explicit AppException(const std::string& msg)
        : std::runtime_error("AppException: " + msg) {}
};

class NetworkException : public AppException {
private:
    int m_errorCode;
public:
    NetworkException(const std::string& msg, int code)
        : AppException(msg), m_errorCode(code) {}

    int errorCode() const { return m_errorCode; }
};

class DatabaseException : public AppException {
public:
    explicit DatabaseException(const std::string& msg)
        : AppException("DB: " + msg) {}
};

// ============================================================
// Example 6 — noexcept
// ============================================================
int safeAdd(int a, int b) noexcept       { return a + b; }
int riskyDiv(int a, int b)               { if (b==0) throw std::invalid_argument("div by zero"); return a/b; }
// noexcept(false) is default if not specified


// ============================================================
// Example 7 — RAII resource in exception path
// ============================================================
class FileResource {
public:
    explicit FileResource(const std::string& name) : m_name(name) {
        std::println("[FileResource] opened '{}'", name);
    }
    ~FileResource() {
        std::println("[FileResource] closed '{}'", m_name);
    }
    void write(const std::string& data) {
        if (data.empty()) throw std::invalid_argument("empty data");
        std::println("[FileResource] wrote to '{}'", m_name);
    }
private:
    std::string m_name;
};


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Exception Hierarchy\n";
    std::cout << "=====================================================\n";

    std::cout << "std::exception\n";
    std::cout << "  ├── std::logic_error\n";
    std::cout << "  │     ├── std::invalid_argument\n";
    std::cout << "  │     ├── std::domain_error\n";
    std::cout << "  │     ├── std::length_error\n";
    std::cout << "  │     └── std::out_of_range\n";
    std::cout << "  └── std::runtime_error\n";
    std::cout << "        ├── std::range_error\n";
    std::cout << "        ├── std::overflow_error\n";
    std::cout << "        └── std::underflow_error\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: try / catch / throw basics\n";
    std::cout << "=====================================================\n";

    // Basic try-catch
    try {
        std::cout << "about to throw...\n";
        throw std::runtime_error("something went wrong");
        std::cout << "this line is never reached\n";
    } catch (const std::runtime_error& e) {
        std::println("caught runtime_error: {}", e.what());
    }

    // Multiple catch blocks
    auto testException = [](int choice) {
        try {
            if (choice == 1) throw std::invalid_argument("bad argument");
            if (choice == 2) throw std::out_of_range("index out of range");
            if (choice == 3) throw std::runtime_error("runtime failure");
            std::cout << "no exception\n";
        }
        catch (const std::invalid_argument& e) { std::println("invalid_arg: {}", e.what()); }
        catch (const std::out_of_range& e)     { std::println("out_of_range: {}", e.what()); }
        catch (const std::runtime_error& e)    { std::println("runtime_error: {}", e.what()); }
        catch (const std::exception& e)        { std::println("exception: {}", e.what()); }
        catch (...)                            { std::cout << "unknown exception\n"; }
    };

    testException(1);
    testException(2);
    testException(3);
    testException(0);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Standard Exception Types\n";
    std::cout << "=====================================================\n";

    // logic_error — programming errors (wrong arguments etc.)
    try {
        std::vector<int> v = {1,2,3};
        int x = v.at(10);  // throws std::out_of_range
        (void)x;
    } catch (const std::out_of_range& e) {
        std::println("out_of_range: {}", e.what());
    }

    // runtime_error — errors at runtime
    try {
        throw std::overflow_error("integer overflow detected");
    } catch (const std::overflow_error& e) {
        std::println("overflow_error: {}", e.what());
    }

    // invalid_argument
    try {
        throw std::invalid_argument("negative size not allowed");
    } catch (const std::invalid_argument& e) {
        std::println("invalid_argument: {}", e.what());
    }

    // bad_alloc
    try {
        // std::vector<int> huge(SIZE_MAX);  // would throw bad_alloc
        throw std::bad_alloc();
    } catch (const std::bad_alloc& e) {
        std::println("bad_alloc: {}", e.what());
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Custom Exceptions\n";
    std::cout << "=====================================================\n";

    // Catch custom by base class
    try {
        throw NetworkException("connection refused", 503);
    }
    catch (const NetworkException& e) {
        std::println("NetworkException: {} (code {})", e.what(), e.errorCode());
    }
    catch (const AppException& e) {
        std::println("AppException: {}", e.what());
    }

    try {
        throw DatabaseException("table not found");
    }
    catch (const AppException& e) {   // catches DatabaseException via base
        std::println("AppException caught: {}", e.what());
    }

    // Catch hierarchy — most derived first
    try {
        throw NetworkException("timeout", 408);
    }
    catch (const std::exception& e) {  // catches everything
        std::println("std::exception: {}", e.what());
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Rethrowing\n";
    std::cout << "=====================================================\n";

    auto processData = [](int val) {
        try {
            if (val < 0) throw std::invalid_argument("negative value");
            std::println("processed: {}", val);
        }
        catch (const std::exception& e) {
            std::println("  [processData] caught: {} — rethrowing", e.what());
            throw;  // rethrow same exception
        }
    };

    try {
        processData(-5);
    } catch (const std::exception& e) {
        std::println("  [main] caught rethrown: {}", e.what());
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: noexcept\n";
    std::cout << "=====================================================\n";

    std::cout << "safeAdd(3,4) = " << safeAdd(3, 4) << '\n';
    std::cout << "safeAdd noexcept: " << noexcept(safeAdd(1,2)) << '\n';
    std::cout << "riskyDiv noexcept: " << noexcept(riskyDiv(1,2)) << '\n';

    try {
        std::cout << riskyDiv(10, 2) << '\n';
        std::cout << riskyDiv(10, 0) << '\n';  // throws
    } catch (const std::invalid_argument& e) {
        std::println("caught: {}", e.what());
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: RAII — exception-safe resources\n";
    std::cout << "=====================================================\n";

    // RAII ensures cleanup even when exception thrown
    try {
        FileResource file("output.txt");   // constructor opens
        file.write("some data");
        file.write("");                    // throws!
        file.write("more data");           // never reached
    } catch (const std::invalid_argument& e) {
        std::println("caught: {}", e.what());
    }
    // FileResource destructor called — file closed — no leak


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: std::exception_ptr — across threads\n";
    std::cout << "=====================================================\n";

    std::exception_ptr eptr;

    // Capture exception from lambda/thread
    auto worker = [&eptr]() {
        try {
            throw std::runtime_error("error from worker");
        } catch (...) {
            eptr = std::current_exception();
        }
    };

    worker();

    // Rethrow in calling context
    if (eptr) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception& e) {
            std::println("rethrown from worker: {}", e.what());
        }
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Exception safety levels\n";
    std::cout << "=====================================================\n";

    std::cout << "Exception safety guarantees:\n\n";
    std::cout << "  No-throw (strongest):\n";
    std::cout << "    Function never throws — noexcept\n";
    std::cout << "    Use for: destructors, swap, move operations\n\n";
    std::cout << "  Strong guarantee:\n";
    std::cout << "    If exception thrown, state unchanged (commit-or-rollback)\n";
    std::cout << "    Use copy-then-swap idiom\n\n";
    std::cout << "  Basic guarantee:\n";
    std::cout << "    If exception thrown, object in valid state (but changed)\n";
    std::cout << "    No resource leaks\n\n";
    std::cout << "  No guarantee (avoid):\n";
    std::cout << "    Exception may leave object in invalid/corrupt state\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Best Practices\n";
    std::cout << "=====================================================\n";

    std::cout << "DO:\n";
    std::cout << "  throw by value:       throw MyException(msg)\n";
    std::cout << "  catch by const ref:   catch (const std::exception& e)\n";
    std::cout << "  rethrow without arg:  throw;  (preserves type)\n";
    std::cout << "  use RAII for cleanup: destructors always run\n";
    std::cout << "  mark noexcept when safe (enables optimizations)\n";
    std::cout << "  derive from std::exception or its subclasses\n\n";

    std::cout << "DON'T:\n";
    std::cout << "  catch by value — loses derived type info\n";
    std::cout << "  throw from destructors — undefined behavior\n";
    std::cout << "  use catch(...) to swallow all exceptions silently\n";
    std::cout << "  throw raw pointers — memory leak if not caught\n";
    std::cout << "  use exceptions for normal control flow\n";

    std::cout << "\n=== StdexceptSTL complete ===\n";
    return 0;
}
