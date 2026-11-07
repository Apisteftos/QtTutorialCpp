// ============================================================
// Guarded Suspension and Balking - two answers to "the object is not in
// the right state for this call"
//
//   Guarded Suspension: wait until the state is right, then continue
//   Balking:            do not wait, give up immediately
//
// Topics:
//   1. Balking: save() returns false when there is nothing to do
//   2. Balking: a second save while one is running is refused
//   3. Guarded suspension: wait until a version has been published
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o guarded_balking
// ============================================================

#include <condition_variable>
#include <mutex>
#include <print>
#include <thread>

class Document {
public:
    void edit()
    {
        {
            std::lock_guard lock(mutex_);
            dirty_ = true;
            ++version_;
        }
        changed_.notify_all();
    }

    // BALKING: if there is nothing to save, or a save is already running,
    // return false at once instead of waiting.
    bool beginSave()
    {
        std::lock_guard lock(mutex_);
        if (!dirty_ || saving_) {
            return false;
        }
        saving_ = true;
        return true;
    }

    void finishSave()
    {
        std::lock_guard lock(mutex_);
        saving_ = false;
        dirty_  = false;
    }

    // GUARDED SUSPENSION: block until the document has reached this version.
    void waitForVersion(int version)
    {
        std::unique_lock lock(mutex_);
        changed_.wait(lock, [&] { return version_ >= version; });
    }

    int version() const
    {
        std::lock_guard lock(mutex_);
        return version_;
    }

private:
    mutable std::mutex      mutex_;
    std::condition_variable changed_;
    bool                    dirty_   = false;
    bool                    saving_  = false;
    int                     version_ = 0;
};

// === EXAMPLE 1: BALK WHEN THERE IS NOTHING TO SAVE ===

void example1()
{
    std::println("=== EXAMPLE 1: NOTHING TO SAVE ===");

    Document doc;
    std::println("beginSave on an unchanged document: {}", doc.beginSave());

    doc.edit();
    std::println("beginSave after an edit:            {}", doc.beginSave());
    doc.finishSave();
    std::println("beginSave right after saving:       {}", doc.beginSave());
}

// === EXAMPLE 2: BALK WHILE A SAVE IS RUNNING ===

void example2()
{
    std::println("=== EXAMPLE 2: SECOND SAVE IS REFUSED ===");

    Document doc;
    doc.edit();

    bool first  = doc.beginSave();       // starts saving
    bool second = doc.beginSave();       // a save is already running: balk
    std::println("first beginSave:  {}", first);
    std::println("second beginSave: {}", second);

    doc.finishSave();
    doc.edit();
    std::println("after finishSave and a new edit: {}", doc.beginSave());
    doc.finishSave();
}

// === EXAMPLE 3: WAIT UNTIL THE STATE IS RIGHT ===

void example3()
{
    std::println("=== EXAMPLE 3: GUARDED SUSPENSION ===");

    Document doc;
    int      seen = 0;

    std::thread waiter([&] {
        doc.waitForVersion(3);           // sleeps until version 3 exists
        seen = doc.version();
    });

    for (int i = 0; i < 3; ++i) {
        doc.edit();
    }
    waiter.join();

    std::println("waiter continued with version >= 3: {}", seen >= 3);
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
