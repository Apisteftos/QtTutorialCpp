# Dependency — C++

## What is Dependency?

The **weakest** relationship. Class A depends on class B only **temporarily** —
during a single method call. B appears as a parameter, local variable, or
return type — never stored as a member of A.

---

## Key characteristics

- No persistent connection — relationship lasts only during a method call
- B is passed in from outside (parameter) — not created or stored by A
- A does not know about B between calls
- Different B objects can be passed on different calls
- Makes code highly testable — easy to inject mocks

---

## Code pattern

```cpp
class ReportGenerator {
    // No Printer member — dependency only

public:
    // Printer appears ONLY as a parameter — not stored
    void printReport(Printer& printer) const {
        printer.print("=== Report ===");
        for (const auto& line : m_lines)
            printer.print(line);
    }
};

// Different printer each time — generator doesn't care
Printer office("Office Printer");
Printer pdf("PDF Printer");

report.printReport(office);   // use office printer
report.printReport(pdf);      // use pdf printer — no changes to ReportGenerator
```

---

## Examples in this file

| # | Dependent | Dependency | Used as |
|---|-----------|-----------|---------|
| 1 | ReportGenerator | Printer | method parameter |
| 2 | OrderProcessor | PaymentService, EmailService | method parameters |
| 3 | DataSerializer | FileWriter | method parameter |
| 4 | FloorController | AuditLogger | method parameter |

---

## Dependency vs Association

Both are "uses-a" but differ in how long the connection lasts:

```cpp
// DEPENDENCY — B only in method signature, not stored
class ReportGenerator {
    // no Printer member
    void print(Printer& p) { p.print(...); }   // temporary
};

// ASSOCIATION — B stored as pointer/reference (longer lasting)
class McxSession {
    McxGroup* m_group;          // stored — association
    void joinGroup(McxGroup* g) { m_group = g; }
};
```

| | Dependency | Association |
|--|-----------|------------|
| Duration | Method call only | Longer term |
| Storage | Parameter/local | Member pointer/reference |
| Connection | Temporary | Persistent |

---

## Why Dependency enables testability

```cpp
// Production code
FloorController floor("GRP-ALPHA");
AuditLogger     realLogger;
floor.requestFloor("SES-001", realLogger);

// Test code — inject a mock logger instead
class MockLogger : public AuditLogger {
    std::vector<std::string> m_logged;
public:
    void log(const std::string& e, const std::string& d) override {
        m_logged.push_back(e + ": " + d);
    }
    bool wasLogged(const std::string& event) { /* check m_logged */ }
};

MockLogger mock;
floor.requestFloor("SES-001", mock);   // inject mock — no real logging
assert(mock.wasLogged("FLOOR_GRANTED"));
```

This is **Dependency Injection** — one of the most important patterns in
testable code design, and it starts here at the simplest level.

---

## When to use Dependency

✅ The external class is only needed temporarily for one operation
✅ You want to swap implementations (testing, mocking, strategy)
✅ The class doesn't need to remember the dependency between calls
✅ Reducing coupling — A should know as little about B as possible

❌ If A needs to remember B between calls → use **Association**
❌ If A contains B as a part → use **Aggregation** or **Composition**

---

## Summary

| Property | Value |
|----------|-------|
| Type | uses-a (temporary) |
| Ownership | None |
| Duration | Method call only |
| Storage | Parameter or local variable — never a member |
| Key benefit | Swappable, mockable, loosely coupled |
| UML | `A - - - - -> B` dashed line, open arrow |
