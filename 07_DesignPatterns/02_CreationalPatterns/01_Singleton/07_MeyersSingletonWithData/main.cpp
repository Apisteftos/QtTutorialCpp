// ============================================================
// Meyers' Singleton WITH DATA - when do you need a mutex?
//
// The static local variable makes the CREATION thread-safe.
// It does not protect the DATA inside the object.
// Rule: protect the data, not the singleton.
//
// Topics:
//   1. Creation only: no mutex needed (const data, constructed once)
//   2. One value written by many threads: std::atomic
//   3. Several values that belong together: std::mutex
//   4. Many readers, one writer: std::shared_mutex
//
// See Readme.md for the decision table, the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o meyers_data
// ============================================================

#include <atomic>
#include <chrono>
#include <cstddef>
#include <map>
#include <mutex>
#include <print>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

std::atomic<int> g_constructions{0};

// Prints the title of an example inside a box (ASCII titles only).
void printBanner(std::string_view title)
{
    std::string line;
    for (std::size_t i = 0; i < title.size() + 2; ++i) {
        line += "═";
    }

    std::println("╔{}╗", line);
    std::println("║ {} ║", title);
    std::println("╚{}╝", line);
}

// --- 1. Creation only ------------------------------------------------------

class CreationOnly {
public:
    static CreationOnly& instance()
    {
        static CreationOnly object;           // thread-safe creation (C++11), no mutex written
        return object;
    }

    CreationOnly(const CreationOnly&)            = delete;
    CreationOnly& operator=(const CreationOnly&) = delete;

    int value() const { return value_; }      // const data: many readers are safe without a lock

private:
    CreationOnly() : value_(42)
    {
        g_constructions.fetch_add(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));  // the other threads wait here
    }

    const int value_;
};

// --- 2. One independent value: atomic -------------------------------------

class AtomicCounter {
public:
    static AtomicCounter& instance()
    {
        static AtomicCounter object;
        return object;
    }

    AtomicCounter(const AtomicCounter&)            = delete;
    AtomicCounter& operator=(const AtomicCounter&) = delete;

    void increment() { count_.fetch_add(1); }
    int  get() const { return count_.load(); }

private:
    AtomicCounter() = default;

    std::atomic<int> count_{0};
};

// --- 3. Several values that must stay consistent: mutex -------------------

class Settings {
public:
    struct Snapshot {
        std::string name;
        int         version;
    };

    static Settings& instance()
    {
        static Settings object;
        return object;
    }

    Settings(const Settings&)            = delete;
    Settings& operator=(const Settings&) = delete;

    // The lock is inside the class, so a caller cannot forget it.
    void update(std::string name, int version)
    {
        std::lock_guard lock(mutex_);
        name_    = std::move(name);           // both fields change under ONE lock
        version_ = version;
    }

    // Returns a copy: the caller never sees a half-updated state.
    Snapshot snapshot() const
    {
        std::lock_guard lock(mutex_);
        return {name_, version_};
    }

private:
    Settings() = default;

    mutable std::mutex mutex_;
    std::string        name_    = "config-0";
    int                version_ = 0;
};

// --- 4. Many readers, one writer: shared_mutex ------------------------------

class Cache {
public:
    static Cache& instance()
    {
        static Cache object;
        return object;
    }

    Cache(const Cache&)            = delete;
    Cache& operator=(const Cache&) = delete;

    void put(const std::string& key, int value)
    {
        std::unique_lock lock(mutex_);        // exclusive: only one writer, no readers
        data_[key] = value;
    }

    bool contains(const std::string& key) const
    {
        std::shared_lock lock(mutex_);        // shared: many readers at the same time
        return data_.contains(key);
    }

    std::size_t size() const
    {
        std::shared_lock lock(mutex_);
        return data_.size();
    }

private:
    Cache() = default;

    mutable std::shared_mutex    mutex_;
    std::map<std::string, int>   data_;
};

// === EXAMPLE 1: CREATION ONLY, NO MUTEX ===

void example1()
{
    printBanner("EXAMPLE 1: CREATION ONLY");

    std::vector<int>         seen(4, 0);
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&seen, i] { seen[i] = CreationOnly::instance().value(); });
    }
    for (auto& t : threads) {
        t.join();
    }

    std::println("constructor ran {} time(s)", g_constructions.load());
    std::println("values seen by the 4 threads: {} {} {} {}", seen[0], seen[1], seen[2], seen[3]);
}

// === EXAMPLE 2: ONE VALUE, std::atomic ===

void example2()
{
    printBanner("EXAMPLE 2: ATOMIC COUNTER");

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([] {
            for (int n = 0; n < 1000; ++n) {
                AtomicCounter::instance().increment();
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    std::println("4 threads x 1000 increments = {}", AtomicCounter::instance().get());
}

// === EXAMPLE 3: SEVERAL VALUES, std::mutex ===

void example3()
{
    printBanner("EXAMPLE 3: MUTEX FOR RELATED VALUES");

    int inconsistent[2] = {0, 0};

    std::thread writer([] {
        for (int i = 1; i <= 1000; ++i) {
            Settings::instance().update("config-" + std::to_string(i), i);
        }
    });

    std::vector<std::thread> readers;
    for (int r = 0; r < 2; ++r) {
        readers.emplace_back([&inconsistent, r] {
            for (int n = 0; n < 1000; ++n) {
                auto s = Settings::instance().snapshot();
                if (s.name != "config-" + std::to_string(s.version)) {
                    ++inconsistent[r];        // name and version do not belong together
                }
            }
        });
    }

    writer.join();
    for (auto& t : readers) {
        t.join();
    }

    auto last = Settings::instance().snapshot();
    std::println("inconsistent snapshots seen by the readers: {}", inconsistent[0] + inconsistent[1]);
    std::println("final state: {} / version {}", last.name, last.version);
}

// === EXAMPLE 4: MANY READERS, std::shared_mutex ===

void example4()
{
    printBanner("EXAMPLE 4: SHARED MUTEX");

    std::thread writer([] {
        for (int i = 0; i < 100; ++i) {
            Cache::instance().put("key" + std::to_string(i), i);
        }
    });

    std::vector<std::thread> readers;
    for (int r = 0; r < 3; ++r) {
        readers.emplace_back([] {
            for (int n = 0; n < 1000; ++n) {
                (void)Cache::instance().contains("key" + std::to_string(n % 100));
            }
        });
    }

    writer.join();
    for (auto& t : readers) {
        t.join();
    }

    std::println("entries in the cache: {}", Cache::instance().size());
    std::println("contains key99: {}", Cache::instance().contains("key99"));
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
    return 0;
}
