// ============================================================
// Immutable Object - data that never changes after construction needs no lock
//
// Topics:
//   1. An object with only const members
//   2. "Changing" it means creating a new object
//   3. Publishing new versions to readers with std::atomic<std::shared_ptr>
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o immutable_object
// ============================================================

#include <atomic>
#include <memory>
#include <print>
#include <string>
#include <thread>
#include <vector>

// Immutable: all members are const, there are no setters.
class Settings {
public:
    Settings(std::string name, int limit) : name_(std::move(name)), limit_(limit) {}

    const std::string& name() const { return name_; }
    int                limit() const { return limit_; }

    // "Modifying" returns a new object. The old one stays valid for everyone
    // who still holds it.
    Settings withLimit(int newLimit) const { return Settings(name_, newLimit); }

private:
    const std::string name_;
    const int         limit_;
};

// === EXAMPLE 1: CONST MEMBERS ===

void example1()
{
    std::println("=== EXAMPLE 1: CONST MEMBERS ===");

    Settings s("basic", 10);
    std::println("{} / {}", s.name(), s.limit());

    //! s.limit_ = 20;                     // error: limit_ is private and const
}

// === EXAMPLE 2: A CHANGE CREATES A NEW OBJECT ===

void example2()
{
    std::println("=== EXAMPLE 2: A CHANGE CREATES A NEW OBJECT ===");

    Settings original("basic", 10);
    Settings updated = original.withLimit(99);

    std::println("original limit = {}", original.limit());
    std::println("updated  limit = {}", updated.limit());
}

// === EXAMPLE 3: PUBLISHING VERSIONS TO READERS ===

void example3()
{
    std::println("=== EXAMPLE 3: ATOMIC PUBLICATION ===");

    std::atomic<std::shared_ptr<const Settings>> current{
        std::make_shared<const Settings>("v0", 0)};

    constexpr int readers = 3;
    std::vector<char> ok(readers, 1);

    std::vector<std::thread> threads;
    for (int r = 0; r < readers; ++r) {
        threads.emplace_back([&, r] {
            for (int i = 0; i < 5000; ++i) {
                std::shared_ptr<const Settings> snap = current.load();   // a stable snapshot
                // The name and the limit of ONE snapshot always belong together.
                if (snap->name() != "v" + std::to_string(snap->limit())) {
                    ok[r] = 0;
                }
            }
        });
    }

    std::thread writer([&] {
        for (int v = 1; v <= 200; ++v) {
            current.store(std::make_shared<const Settings>("v" + std::to_string(v), v));
        }
    });

    writer.join();
    for (auto& t : threads) {
        t.join();
    }

    bool allOk = true;
    for (char flag : ok) {
        allOk = allOk && flag;
    }
    std::println("every snapshot was consistent: {}", allOk);
    std::println("final version: {}", current.load()->name());
}

int main()
{
    example1();
    std::println("");
    example2();
    std::println("");
    example3();
    return 0;
}
