# Dependency — C++

## What is Dependency?

The **weakest** relationship. Class A depends on class B only **temporarily** —
during a single method call. B appears as a parameter, local variable, or
return type — never stored as a member of A.

---

## Class diagram

```mermaid
classDiagram
    class ReportGenerator {
        -string m_title
        -vector~string~ m_lines
        +addLine(string line)
        +printReport(Printer& printer)
        +printSummary(Printer& printer)
    }

    class Printer {
        -string m_name
        +print(string content)
        +printLine()
    }

    ReportGenerator ..> Printer : depends on (uses temporarily)
    note for ReportGenerator "Printer is NOT stored\nas a member — only used\nduring method calls"
```

---

## Sequence diagram — temporary use only

```mermaid
sequenceDiagram
    participant Main
    participant Report as ReportGenerator
    participant OfficePrinter
    participant PdfPrinter

    Main->>Report: create ReportGenerator("Q1 Report")
    Note over Report: No Printer stored inside

    Main->>OfficePrinter: create Printer("Office")
    Main->>PdfPrinter: create Printer("PDF")

    Main->>Report: printReport(officePrinter)
    activate Report
    Report->>OfficePrinter: print("=== Q1 Report ===")
    Report->>OfficePrinter: printLine()
    deactivate Report
    Note over Report: Printer reference gone after call

    Main->>Report: printReport(pdfPrinter)
    activate Report
    Report->>PdfPrinter: print("=== Q1 Report ===")
    Report->>PdfPrinter: printLine()
    deactivate Report
    Note over Report: Different printer — same report
```

---

## OrderProcessor dependency injection

```mermaid
classDiagram
    class OrderProcessor {
        -string m_orderId
        -double m_total
        +process(PaymentService& p, EmailService& e, card, email) bool
    }

    class PaymentService {
        +processPayment(string card, double amount) bool
    }

    class EmailService {
        +sendConfirmation(string email, string msg)
    }

    OrderProcessor ..> PaymentService : uses in process()
    OrderProcessor ..> EmailService : uses in process()
    note for OrderProcessor "Neither service stored\nas member — injected\nper call"
```

---

## Why dependency enables testability

```mermaid
flowchart TD
    subgraph Production
        FC1[FloorController] -. "real call" .-> AL1[AuditLogger\nwrites to disk]
    end

    subgraph Testing
        FC2[FloorController] -. "inject mock" .-> AL2[MockAuditLogger\nchecks calls in memory]
    end

    note["Same FloorController code\nDifferent logger injected\nNo code changes needed for testing!"]
    style note fill:#2a2a2a,color:#aaa
```

---

## Dependency vs Association

```mermaid
flowchart LR
    subgraph Dependency ["Dependency — temporary"]
        A1[ReportGenerator] -. "parameter only\nlasts ONE method call" .-> B1[Printer]
    end

    subgraph Association ["Association — persistent"]
        A2[McxSession] -- "stored m_group pointer\nlasts until changed" --> B2[McxGroup]
    end
```

| | Dependency | Association |
|--|-----------|------------|
| Duration | Method call only | Longer term |
| Storage | Parameter/local | Member pointer/reference |
| Connection | Temporary | Persistent |

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
        printer.print("=== " + m_title + " ===");
        for (const auto& line : m_lines)
            printer.print(line);
    }
};

// Different printer each time
report.printReport(officePrinter);   // use office printer
report.printReport(pdfPrinter);      // use pdf printer
```

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
