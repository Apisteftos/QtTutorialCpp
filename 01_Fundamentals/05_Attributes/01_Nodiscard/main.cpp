// ============================================================
// [[nodiscard]] - make the compiler warn when a result is ignored
//
// Topics:
//   1. On a function
//   2. With a reason message (C++20)
//   3. On a type (class / enum): every function returning it is covered
//   4. On member functions (empty() vs clear())
//   5. RAII guard returned from a function
//   6. On a constructor (C++20)
//   7. Discarding on purpose: (void) and std::ignore
//   8. Standard library functions that are [[nodiscard]]
//
// See Readme.md for the explanation and the compiler warnings.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o nodiscard
// ============================================================

#include <future>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

// === EXAMPLE 1: ON A FUNCTION ===

[[nodiscard]] int parsePort(std::string_view text)
{
    int value = 0;
    for (char c : text) {
        value = value * 10 + (c - '0');
    }
    return value;
}

void example1()
{
    std::println("=== EXAMPLE 1: ON A FUNCTION ===");

    int port = parsePort("8080");           // result is used: no warning
    std::println("port = {}", port);

    // parsePort("8080");                   // would warn: result ignored
}

// === EXAMPLE 2: WITH A REASON MESSAGE (C++20) ===

[[nodiscard("the returned string is the new value, the input is not changed")]]
std::string trimmed(std::string_view text)
{
    std::size_t first = text.find_first_not_of(' ');
    if (first == std::string_view::npos) {
        return {};
    }
    std::size_t last = text.find_last_not_of(' ');
    return std::string(text.substr(first, last - first + 1));
}

void example2()
{
    std::println("=== EXAMPLE 2: WITH A REASON MESSAGE ===");

    std::string clean = trimmed("   hello   ");
    std::println("[{}]", clean);

    // trimmed("   hello   ");              // would warn and print the reason
}

// === EXAMPLE 3: ON A TYPE ===

// Every function that returns ErrorCode is automatically [[nodiscard]].
enum class [[nodiscard]] ErrorCode { Ok, NotFound, AccessDenied };

struct [[nodiscard]] Result {
    bool        ok;
    std::string message;
};

ErrorCode openFile(std::string_view name)
{
    if (name == "data.txt") {
        return ErrorCode::Ok;
    }
    return ErrorCode::NotFound;
}

Result validate(int age)
{
    if (age < 0) {
        return {false, "age must not be negative"};
    }
    return {true, "ok"};
}

void example3()
{
    std::println("=== EXAMPLE 3: ON A TYPE ===");

    ErrorCode code = openFile("data.txt");
    std::println("openFile ok? {}", code == ErrorCode::Ok);

    Result r = validate(-5);
    std::println("validate: {} ({})", r.ok, r.message);

    // openFile("missing.txt");             // would warn: error code ignored
    // validate(10);                        // would warn: Result ignored
}

// === EXAMPLE 4: ON MEMBER FUNCTIONS ===

class Stack {
public:
    void push(int value) { items_.push_back(value); }

    // empty() asks a question, clear() does something. Calling empty() and
    // dropping the answer is almost always a bug (the author meant clear()).
    [[nodiscard]] bool        empty() const { return items_.empty(); }
    [[nodiscard]] std::size_t size() const  { return items_.size(); }

    void clear() { items_.clear(); }

private:
    std::vector<int> items_;
};

void example4()
{
    std::println("=== EXAMPLE 4: ON MEMBER FUNCTIONS ===");

    Stack s;
    s.push(1);
    s.push(2);
    std::println("size = {}, empty = {}", s.size(), s.empty());

    s.clear();
    std::println("after clear: empty = {}", s.empty());

    // s.empty();                           // would warn: did you mean clear()?
}

// === EXAMPLE 5: RAII GUARD RETURNED FROM A FUNCTION ===

class ScopedTimer {
public:
    explicit ScopedTimer(std::string name) : name_(std::move(name))
    {
        std::println("  timer '{}' started", name_);
    }
    ~ScopedTimer() { std::println("  timer '{}' stopped", name_); }

    ScopedTimer(const ScopedTimer&)            = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

private:
    std::string name_;
};

// If the caller ignores the return value, the guard is destroyed at once
// and measures nothing. [[nodiscard]] catches that.
[[nodiscard]] ScopedTimer startTimer(std::string name)
{
    return ScopedTimer(std::move(name));
}

void example5()
{
    std::println("=== EXAMPLE 5: RAII GUARD ===");

    {
        auto timer = startTimer("work");    // lives until the end of the block
        std::println("  doing work");
    }

    // startTimer("oops");                  // would warn: started and stopped at once
}

// === EXAMPLE 6: ON A CONSTRUCTOR (C++20) ===

class Lock {
public:
    [[nodiscard]] explicit Lock(int id) : id_(id)
    {
        std::println("  lock {} acquired", id_);
    }
    ~Lock() { std::println("  lock {} released", id_); }

private:
    int id_;
};

void example6()
{
    std::println("=== EXAMPLE 6: ON A CONSTRUCTOR ===");

    {
        Lock lock(1);                       // named object: no warning
        std::println("  critical section");
    }

    // Lock(2);                             // would warn: temporary, released at once
}

// === EXAMPLE 7: DISCARDING ON PURPOSE ===

[[nodiscard]] bool tryLogin(std::string_view user)
{
    std::println("  login attempt for '{}'", user);
    return user == "admin";
}

void example7()
{
    std::println("=== EXAMPLE 7: DISCARDING ON PURPOSE ===");

    (void)tryLogin("guest");                // cast to void: "I know, I do not need it"
    std::ignore = tryLogin("admin");        // same idea, easy to search for
}

// === EXAMPLE 8: STANDARD LIBRARY FUNCTIONS ===

void example8()
{
    std::println("=== EXAMPLE 8: STANDARD LIBRARY ===");

    std::vector<int> v{1, 2, 3};
    std::println("empty() = {}", v.empty());       // [[nodiscard]]: do not ignore

    std::optional<int> opt = 5;
    std::println("value_or = {}", opt.value_or(0)); // [[nodiscard]] too

    // std::async returns a future that blocks in its destructor. Ignoring it
    // makes the call run synchronously, so std::async is [[nodiscard]].
    std::future<int> f = std::async(std::launch::async, [] { return 42; });
    std::println("async result = {}", f.get());

    // v.empty();                           // would warn
    // std::async(std::launch::async, [] { return 42; });   // would warn
}

int main()
{
    example1();
    std::println("");
    example2();
    std::println("");
    example3();
    std::println("");
    example4();
    std::println("");
    example5();
    std::println("");
    example6();
    std::println("");
    example7();
    std::println("");
    example8();
    return 0;
}
