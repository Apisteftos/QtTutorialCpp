#include <exception>
#include <memory>
#include <new>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

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


// ============================================================
// Example 9 — std::nested_exception
// ============================================================
void loadConfig() {
    try {
        throw std::runtime_error("file 'app.cfg' not found");
    } catch (...) {
        // wraps the currently handled exception inside a new one
        std::throw_with_nested(AppException("cannot load configuration"));
    }
}

// Prints the exception and every exception nested inside it
void printNested(const std::exception& e, int level = 0) {
    std::println("{}{}", std::string(level * 2, ' '), e.what());
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& inner) {
        printNested(inner, level + 1);
    }
}


namespace
{
    void banner(std::string_view title)
    {
        std::println("=====================================================");
        std::println("{}", title);
        std::println("=====================================================");
    }

    template<typename Base, typename Derived>
    void showDerivedFrom(std::string_view derived, std::string_view base)
    {
        static_assert(std::is_base_of_v<Base, Derived>);
        std::println("  {} derives from {}", derived, base);
    }
}


int main()
{
    // === EXAMPLE 1: Exception Hierarchy ===
    banner("Example 1: Exception Hierarchy");

    // The hierarchy is checked at compile time with std::is_base_of_v
    showDerivedFrom<std::exception,     std::logic_error>    ("logic_error",     "exception");
    showDerivedFrom<std::logic_error,   std::invalid_argument>("invalid_argument", "logic_error");
    showDerivedFrom<std::logic_error,   std::domain_error>   ("domain_error",    "logic_error");
    showDerivedFrom<std::logic_error,   std::length_error>   ("length_error",    "logic_error");
    showDerivedFrom<std::logic_error,   std::out_of_range>   ("out_of_range",    "logic_error");
    showDerivedFrom<std::exception,     std::runtime_error>  ("runtime_error",   "exception");
    showDerivedFrom<std::runtime_error, std::range_error>    ("range_error",     "runtime_error");
    showDerivedFrom<std::runtime_error, std::overflow_error> ("overflow_error",  "runtime_error");
    showDerivedFrom<std::runtime_error, std::underflow_error>("underflow_error", "runtime_error");
    showDerivedFrom<std::exception,     std::bad_alloc>      ("bad_alloc",       "exception");
    std::println("  logic_error derives from runtime_error? {}",
                 std::is_base_of_v<std::runtime_error, std::logic_error>);
    std::println("");


    // === EXAMPLE 2: try / catch / throw basics ===
    banner("Example 2: try / catch / throw basics");

    // Basic try-catch
    try {
        std::println("about to throw...");
        throw std::runtime_error("something went wrong");
        std::println("this line is never reached");
    } catch (const std::runtime_error& e) {
        std::println("caught runtime_error: {}", e.what());
    }

    // Multiple catch blocks
    auto testException = [](int choice) {
        try {
            if (choice == 1) throw std::invalid_argument("bad argument");
            if (choice == 2) throw std::out_of_range("index out of range");
            if (choice == 3) throw std::runtime_error("runtime failure");
            std::println("no exception");
        }
        catch (const std::invalid_argument& e) { std::println("invalid_arg: {}", e.what()); }
        catch (const std::out_of_range& e)     { std::println("out_of_range: {}", e.what()); }
        catch (const std::runtime_error& e)    { std::println("runtime_error: {}", e.what()); }
        catch (const std::exception& e)        { std::println("exception: {}", e.what()); }
        catch (...)                            { std::println("unknown exception"); }
    };

    testException(1);
    testException(2);
    testException(3);
    testException(0);
    std::println("");


    // === EXAMPLE 3: Standard Exception Types ===
    banner("Example 3: Standard Exception Types");

    // logic_error — programming errors (wrong arguments etc.)
    try {
        std::vector<int> v = {1,2,3};
        int x = v.at(10);  // throws std::out_of_range
        (void)x;
    } catch (const std::out_of_range& e) {
        std::println("out_of_range: {}", e.what());   // text is implementation-specific
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
        // A real failure would be e.g. a huge new[] / allocation request.
        // (std::vector<int> huge(SIZE_MAX) throws std::length_error, not bad_alloc.)
        throw std::bad_alloc();
    } catch (const std::bad_alloc& e) {
        std::println("bad_alloc: {}", e.what());
    }
    std::println("");


    // === EXAMPLE 4: Custom Exceptions ===
    banner("Example 4: Custom Exceptions");

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
    std::println("");


    // === EXAMPLE 5: Rethrowing ===
    banner("Example 5: Rethrowing");

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
    std::println("");


    // === EXAMPLE 6: noexcept ===
    banner("Example 6: noexcept");

    std::println("safeAdd(3,4) = {}", safeAdd(3, 4));
    std::println("safeAdd noexcept: {}", noexcept(safeAdd(1,2)));
    std::println("riskyDiv noexcept: {}", noexcept(riskyDiv(1,2)));

    try {
        std::println("{}", riskyDiv(10, 2));
        std::println("{}", riskyDiv(10, 0));  // throws
    } catch (const std::invalid_argument& e) {
        std::println("caught: {}", e.what());
    }
    std::println("");


    // === EXAMPLE 7: RAII — exception-safe resources ===
    banner("Example 7: RAII — exception-safe resources");

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
    std::println("");


    // === EXAMPLE 8: std::exception_ptr — across threads ===
    banner("Example 8: std::exception_ptr — across threads");

    std::exception_ptr eptr;

    // Capture exception from lambda/thread
    auto worker = [&eptr]() {
        try {
            throw std::runtime_error("error from worker");
        } catch (...) {
            eptr = std::current_exception();
        }
    };

    worker();   // (a real std::thread works the same way — see the README)

    // Rethrow in calling context
    if (eptr) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception& e) {
            std::println("rethrown from worker: {}", e.what());
        }
    }
    std::println("");


    // === EXAMPLE 9: std::nested_exception ===
    banner("Example 9: std::nested_exception");

    try {
        loadConfig();
    } catch (const std::exception& e) {
        printNested(e);
    }

    std::println("");
    std::println("=== StdexceptSTL complete ===");
    return 0;
}
