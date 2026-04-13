# Scope — C++23

**Scope** defines where a variable or function is visible and accessible.
A variable only exists within the block `{}` where it was declared.

---

## 1. Global scope

```cpp
int g_appVersion = 23;        // accessible from everywhere
std::string g_appName = "MCXApp";

void printAppInfo() {
    // g_appName and g_appVersion accessible here
}
```

⚠️ Avoid globals — they create hidden dependencies and are hard to test.
Prefer: pass as parameters or use class members.

---

## 2. Local (function) scope

```cpp
void demo() {
    int localVar = 42;   // exists only inside demo()
}   // localVar DESTROYED here

// localVar does not exist here
```

---

## 3. Block scope — if, for, while, {}

```cpp
// C++17 if-init — variable scoped to if block
if (int result = compute(); result > 0) {
    use(result);
}
// result does NOT exist here

// loop variable scoped to loop
for (int i = 0; i < 5; ++i) { }
// i does NOT exist here

// Manual block — limit lifetime deliberately
{
    std::string tempBuffer = "large data";
    use(tempBuffer);
}   // tempBuffer freed immediately — memory released
```

---

## 4. Nested scope — inner shadows outer

```cpp
int value = 100;

{
    int value = 200;   // shadows outer — separate variable!
    // value = 200 here
    {
        int value = 300;
        // value = 300 here
    }
    // value = 200 here
}

// value = 100 here — outer is UNCHANGED
```

⚠️ Compile with `-Wshadow` to catch accidental shadowing.

---

## 5. Scope resolution operator ::

```cpp
int counter = 0;   // global

void demo() {
    int counter = 99;          // local — shadows global
    counter;                   // 99 — local
    ::counter;                 // 0  — global via ::
    ::counter += 1;            // modify global
}
```

---

## 6. Class scope — private, public, static

```cpp
class McxSession {
private:
    int  m_sessionId;      // only inside class
    bool m_connected;
public:
    static int s_totalSessions;  // shared across ALL instances
    void connect();
};

McxSession s1(1), s2(2);
McxSession::s_totalSessions;   // accessible via class name
s1.m_sessionId;                // ERROR — private
```

---

## 7. Namespace scope

```cpp
namespace Audio   { int sampleRate = 44100; void play(); }
namespace Network { int sampleRate = 1000;  void play(); }

Audio::play();              // no conflict
Network::sampleRate;        // no conflict

using Audio::sampleRate;    // bring into local scope
```

---

## 8. RAII — scope-based resource lifetime

Constructor acquires, destructor releases — **always** called on scope exit,
even if an exception is thrown:

```cpp
class ScopedLock {
public:
    ScopedLock(const std::string& name) { acquire(name); }
    ~ScopedLock() { release(); }   // always runs!
};

{
    ScopedLock lock("mutex");   // acquired
    doWork();
}   // lock.~ScopedLock() → always released, even on exception
```

This is how `std::lock_guard`, `std::unique_ptr`, `std::ifstream` all work.

---

## 9. static local variable

Initialized **once** on first call, persists until program ends,
but only **accessible** inside its function:

```cpp
int generateId() {
    static int s_nextId = 1000;   // initialized once
    return s_nextId++;
}

generateId();   // 1000
generateId();   // 1001
generateId();   // 1002
```

Prefer `static local` over global — same persistence, but encapsulated.

---

## Best practices

```
✅ Declare variables as close to use as possible
✅ Limit scope to the minimum needed
✅ Use namespaces to prevent name collisions
✅ Use RAII for resource management (locks, files, sockets)
✅ Use static local instead of global when possible
✅ Prefix member variables with m_ to avoid shadowing
✅ Use if-init (C++17) to scope condition variables

❌ Avoid global variables — hidden dependencies
❌ Don't shadow outer names — use -Wshadow to detect
```
