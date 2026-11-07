// ============================================================
// Reader-Writer Lock - many readers at the same time, or one writer
//
// Topics:
//   1. std::shared_mutex with shared_lock and unique_lock
//   2. Readers run in parallel while a writer updates
//   3. Why a plain mutex is not enough
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o reader_writer
// ============================================================

#include <map>
#include <mutex>
#include <print>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

// Read-mostly data: many get(), few set().
class SettingsStore {
public:
    int get(const std::string& key) const
    {
        std::shared_lock lock(mutex_);           // many readers at once
        auto it = values_.find(key);
        return it == values_.end() ? -1 : it->second;
    }

    void set(const std::string& key, int value)
    {
        std::unique_lock lock(mutex_);           // exclusive: waits for all readers
        values_[key] = value;
    }

private:
    mutable std::shared_mutex    mutex_;
    std::map<std::string, int>   values_;
};

// === EXAMPLE 1: BASIC USE ===

void example1()
{
    std::println("=== EXAMPLE 1: BASIC USE ===");

    SettingsStore store;
    store.set("volume", 7);
    std::println("volume = {}", store.get("volume"));
    std::println("missing = {}", store.get("brightness"));
}

// === EXAMPLE 2: READERS WHILE A WRITER UPDATES ===

void example2()
{
    std::println("=== EXAMPLE 2: 4 READERS, 1 WRITER ===");

    SettingsStore store;
    store.set("version", 0);

    constexpr int readers = 4;
    std::vector<char> ok(readers, 1);            // one flag per reader, no sharing

    std::vector<std::thread> threads;
    for (int r = 0; r < readers; ++r) {
        threads.emplace_back([&, r] {
            int last = 0;
            for (int i = 0; i < 2000; ++i) {
                int v = store.get("version");
                if (v < last) {                  // a version must never go backwards
                    ok[r] = 0;
                }
                last = v;
            }
        });
    }

    std::thread writer([&] {
        for (int v = 1; v <= 100; ++v) {
            store.set("version", v);
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
    std::println("readers never saw an older version: {}", allOk);
    std::println("final version = {}", store.get("version"));
}

// === EXAMPLE 3: SHARED vs EXCLUSIVE ===

// The try_lock calls must come from ANOTHER thread: a thread that already
// owns the mutex may not lock it again (undefined behavior).
struct Probe {
    bool secondReader;
    bool writer;
};

Probe probeFromOtherThread(std::shared_mutex& m)
{
    Probe probe{false, false};
    std::thread other([&] {
        probe.secondReader = m.try_lock_shared();
        if (probe.secondReader) {
            m.unlock_shared();
        }
        probe.writer = m.try_lock();
        if (probe.writer) {
            m.unlock();
        }
    });
    other.join();
    return probe;
}

void example3()
{
    std::println("=== EXAMPLE 3: SHARED vs EXCLUSIVE ===");

    std::shared_mutex m;

    {
        std::shared_lock reader(m);                       // main thread is a reader
        Probe p = probeFromOtherThread(m);
        std::println("another reader can enter: {}", p.secondReader);
        std::println("a writer can enter while a reader is active: {}", p.writer);
    }

    Probe p = probeFromOtherThread(m);                    // nobody holds the lock now
    std::println("a writer can enter after the reader left: {}", p.writer);
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
