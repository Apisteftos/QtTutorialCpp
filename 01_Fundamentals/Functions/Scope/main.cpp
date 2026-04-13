#include <iostream>
#include <string>
#include <print>

// ============================================================
// SCOPE in C++23
// ============================================================
// Topics covered:
//   1.  Global scope
//   2.  Local (function) scope
//   3.  Block scope — if, for, while, {}
//   4.  Nested scope — inner shadows outer
//   5.  Scope resolution operator ::
//   6.  Class scope — private, public, static
//   7.  Namespace scope
//   8.  RAII — scope-based resource lifetime
//   9.  static local variable — persists across calls
//  10.  Practical: avoiding naming conflicts
// ============================================================


// ============================================================
// Example 1 — Global scope
// Lives for the entire program, accessible from everywhere
// ============================================================
int g_appVersion  = 23;
std::string g_appName = "MCXApp";

void printAppInfo()
{
    // Global variables are accessible from any function
    std::println("[printAppInfo] app: {} v{}", g_appName, g_appVersion);
}


// ============================================================
// Example 5 — Scope resolution operator ::
// ============================================================
int counter = 0;   // global counter

void scopeResolutionDemo()
{
    int counter = 99;                         // local counter — shadows global
    std::println("local  counter: {}", counter);   // 99
    std::println("global counter: {}", ::counter); // 0 — :: accesses global
    ::counter += 1;                           // modify global from inside function
}


// ============================================================
// Example 6 — Class scope
// ============================================================
class McxSession {
private:
    int    m_sessionId;       // private — only accessible inside class
    bool   m_connected;

public:
    static int s_totalSessions;   // static — shared across all instances

    explicit McxSession(int id)
        : m_sessionId(id), m_connected(false)
    {
        ++s_totalSessions;
        std::println("[McxSession] created session {}", m_sessionId);
    }

    void connect() {
        m_connected = true;   // accessible — we are inside the class
        std::println("[McxSession] session {} connected", m_sessionId);
    }

    bool isConnected() const { return m_connected; }
    int  id()          const { return m_sessionId; }

    ~McxSession() {
        --s_totalSessions;
        std::println("[McxSession] session {} destroyed", m_sessionId);
    }
};

int McxSession::s_totalSessions = 0;


// ============================================================
// Example 7 — Namespace scope
// ============================================================
namespace Audio {
int sampleRate = 44100;
void play() { std::println("[Audio::play] sampleRate={}", sampleRate); }
}

namespace Network {
int sampleRate = 1000;   // same name — different namespace, no conflict
void play() { std::println("[Network::play] sampleRate={}", sampleRate); }
}


// ============================================================
// Example 9 — static local variable
// Initialized once, persists between calls
// ============================================================
int generateCallId()
{
    static int s_nextId = 1000;   // initialized only on first call
    return s_nextId++;
}

void countCalls()
{
    static int s_count = 0;
    ++s_count;
    std::println("[countCalls] called {} time(s)", s_count);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Global scope\n";
    std::cout << "=====================================================\n";

    {
        // Global variables accessible from anywhere
        std::println("g_appName:    {}", g_appName);
        std::println("g_appVersion: {}", g_appVersion);
        printAppInfo();

        // Modifying global — affects everyone
        g_appVersion = 24;
        std::println("g_appVersion after change: {}", g_appVersion);
        printAppInfo();

        std::cout << "\nWarning: avoid globals — creates hidden dependencies!\n";
        std::cout << "Prefer: pass as parameters or use class members\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Local (function) scope\n";
    std::cout << "=====================================================\n";

    {
        // Local variables live only inside their function/block
        int localVar = 42;
        std::println("localVar inside block: {}", localVar);

        std::string message = "I only exist here";
        std::println("message: {}", message);
    }
    // localVar and message are DESTROYED here — cannot access below

    std::cout << "localVar and message are out of scope — destroyed\n";
    // std::println("{}", localVar);   // ERROR — uncomment to see compiler error


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Block scope — if, for, while, {}\n";
    std::cout << "=====================================================\n";

    {
        int x = 10;

        // if block — condition variable scoped to if (C++17)
        if (int result = x * 2; result > 15) {
            std::println("if-init: result={} (only exists inside if)", result);
        }
        // result does NOT exist here

        // for loop — i is scoped to the loop
        std::cout << "loop values: ";
        for (int i = 0; i < 5; ++i) {
            int doubled = i * 2;   // doubled is scoped to each iteration
            std::cout << doubled << ' ';
        }
        std::cout << '\n';
        // i and doubled do NOT exist here

        // while block
        int countdown = 3;
        while (countdown > 0) {
            int tick = countdown * 100;   // scoped to this iteration
            std::println("  tick={}", tick);
            --countdown;
        }
        // tick does NOT exist here

        // Manual block scope — limit lifetime deliberately
        {
            std::string tempBuffer = "large temporary data";
            std::println("  using: {}", tempBuffer);
        }   // tempBuffer destroyed here — memory freed immediately
        std::cout << "  tempBuffer is gone\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Nested scope — inner shadows outer\n";
    std::cout << "=====================================================\n";

    {
        int value = 100;
        std::println("outer value: {}", value);

        {
            int value = 200;   // shadows outer value — separate variable!
            std::println("  inner value: {}", value);   // 200

            {
                int value = 300;   // shadows both above
                std::println("    innermost value: {}", value);   // 300
            }

            std::println("  back to inner: {}", value);   // 200
        }

        std::println("back to outer: {}", value);   // 100 — unchanged!

        std::cout << "\nWarning: shadowing hides bugs — keep names distinct\n";
        std::cout << "Many compilers warn about this with -Wshadow\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Scope resolution operator ::\n";
    std::cout << "=====================================================\n";

    {
        std::println("global counter before: {}", ::counter);
        scopeResolutionDemo();
        std::println("global counter after:  {}", ::counter);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Class scope\n";
    std::cout << "=====================================================\n";

    {
        std::println("sessions before: {}", McxSession::s_totalSessions);

        {
            McxSession s1(1);
            McxSession s2(2);
            std::println("sessions active: {}", McxSession::s_totalSessions);

            s1.connect();
            std::println("s1 connected: {}", s1.isConnected());

            // s1.m_sessionId   // ERROR — private, uncomment to see
            std::println("s1 id (via public method): {}", s1.id());
        }   // s1 and s2 destroyed here — destructors called

        std::println("sessions after scope: {}", McxSession::s_totalSessions);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Namespace scope\n";
    std::cout << "=====================================================\n";

    {
        // Both have sampleRate and play() — no conflict
        Audio::play();
        Network::play();

        std::println("Audio::sampleRate:   {}", Audio::sampleRate);
        std::println("Network::sampleRate: {}", Network::sampleRate);

        // using — bring one into local scope
        using Audio::sampleRate;
        std::println("sampleRate (using Audio): {}", sampleRate);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: RAII — scope-based resource lifetime\n";
    std::cout << "=====================================================\n";

    {
        // RAII: Resource Acquisition Is Initialization
        // Constructor acquires, destructor releases — guaranteed by scope
        class ScopedLock {
        public:
            explicit ScopedLock(const std::string& name) : m_name(name) {
                std::println("[ScopedLock] '{}' ACQUIRED", m_name);
            }
            ~ScopedLock() {
                std::println("[ScopedLock] '{}' RELEASED", m_name);
            }
        private:
            std::string m_name;
        };

        std::cout << "entering critical section\n";
        {
            ScopedLock lock("floorMutex");
            std::cout << "  doing protected work...\n";
            // Even if exception thrown here, lock released by destructor
        }   // lock.~ScopedLock() called automatically — always unlocks!
        std::cout << "exited critical section\n";

        // This is how std::lock_guard works — same RAII pattern
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: static local — persists across calls\n";
    std::cout << "=====================================================\n";

    {
        // static local: initialized once, lives until program ends
        std::cout << "generating call IDs: ";
        for (int i = 0; i < 5; ++i)
            std::cout << generateCallId() << ' ';
        std::cout << '\n';

        // Each call increments the static counter
        countCalls();
        countCalls();
        countCalls();

        std::cout << "\nstatic local vs global:\n";
        std::cout << "  static local — only accessible inside its function\n";
        std::cout << "  global       — accessible from everywhere\n";
        std::cout << "  Prefer static local — encapsulated, safe\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Practical — avoiding naming conflicts\n";
    std::cout << "=====================================================\n";

    {
        // Pattern 1: use namespaces to group related code
        Audio::sampleRate = 48000;
        Network::sampleRate = 500;
        std::println("Audio: {}Hz  Network: {}Hz",
                     Audio::sampleRate, Network::sampleRate);

        // Pattern 2: prefix member variables with m_ to avoid shadowing
        // int id;                  // BAD — shadows member m_id
        // int m_id vs int id       // GOOD — clearly different

        // Pattern 3: limit scope to minimum needed
        {
            std::string tempResult = "intermediate";
            std::println("using temp: {}", tempResult);
        }   // freed immediately — not polluting outer scope

        // Pattern 4: use const to protect variables from accidental change
        const int MAX_SESSIONS = 8;
        std::println("max sessions: {}", MAX_SESSIONS);
        // MAX_SESSIONS = 9;   // ERROR — const protects it

        // Pattern 5: if-init statement (C++17) — scope condition variable
        if (int id = generateCallId(); id > 1000)
            std::println("new call id={} (only exists in this if)", id);

        std::cout << "\nScope best practices:\n";
        std::cout << "  Declare variables as close to use as possible\n";
        std::cout << "  Limit scope to the minimum needed\n";
        std::cout << "  Avoid globals — use parameters or class members\n";
        std::cout << "  Use namespaces to prevent name collisions\n";
        std::cout << "  Use RAII for resource management\n";
        std::cout << "  Use static local instead of global when possible\n";
    }

    std::cout << "\n=== Scope complete ===\n";
    return 0;
}
