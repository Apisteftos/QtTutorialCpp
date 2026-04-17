#include <iostream>
#include <string>
#include <vector>
#include <print>

// ============================================================
// DEPENDENCY — uses-a relationship (temporary)
// ============================================================
// The weakest relationship. Class A depends on class B only
// during a method call — as a parameter, local variable,
// or return type. No lasting connection between A and B.
//
// Key signal: B appears ONLY in method signatures or as
//             a local variable — NOT stored as a member.
// ============================================================


// ============================================================
// Example 1 — ReportGenerator depends on Printer
// ============================================================
class Printer {
private:
    std::string m_name;

public:
    explicit Printer(const std::string& name) : m_name(name) {}

    void print(const std::string& content) const {
        std::println("  [{}] Printing: {}", m_name, content);
    }

    void printLine() const {
        std::println("  [{}] ----------------------------------------", m_name);
    }
};

class ReportGenerator {
private:
    std::string m_title;
    std::vector<std::string> m_lines;

public:
    explicit ReportGenerator(const std::string& title) : m_title(title) {}

    void addLine(const std::string& line) { m_lines.push_back(line); }

    // Dependency: Printer used only during this method — not stored
    void printReport(Printer& printer) const {
        printer.print("=== " + m_title + " ===");
        printer.printLine();
        for (const auto& line : m_lines)
            printer.print(line);
        printer.printLine();
    }

    // Dependency: different Printer can be passed each time
    void printSummary(Printer& printer) const {
        printer.print("Summary: " + m_title + " (" +
                      std::to_string(m_lines.size()) + " lines)");
    }
};


// ============================================================
// Example 2 — OrderProcessor depends on PaymentService
// ============================================================
class PaymentService {
public:
    bool processPayment(const std::string& card, double amount) const {
        std::println("  [PaymentService] charging {} EUR to card {}", amount, card);
        return amount < 10000.0;   // simulate success
    }
};

class EmailService {
public:
    void sendConfirmation(const std::string& email, const std::string& msg) const {
        std::println("  [EmailService] sending to {}: {}", email, msg);
    }
};

class OrderProcessor {
private:
    std::string m_orderId;
    double      m_total;

public:
    OrderProcessor(const std::string& id, double total)
        : m_orderId(id), m_total(total) {}

    // Dependency: PaymentService and EmailService used only here
    bool process(PaymentService& payment, EmailService& email,
                 const std::string& card, const std::string& customerEmail) const
    {
        std::println("  Processing order {} (EUR {:.2f}):", m_orderId, m_total);

        bool success = payment.processPayment(card, m_total);
        if (success) {
            email.sendConfirmation(customerEmail,
                                   "Order " + m_orderId + " confirmed — EUR " +
                                       std::to_string(m_total));
        }
        return success;
    }
};


// ============================================================
// Example 3 — Serializer depends on FileWriter
// ============================================================
class FileWriter {
private:
    std::string m_path;

public:
    explicit FileWriter(const std::string& path) : m_path(path) {}

    void write(const std::string& data) const {
        std::println("  [FileWriter] writing to '{}': {}", m_path, data);
    }

    void writeLine(const std::string& line) const {
        std::println("  [FileWriter '{}'] {}", m_path, line);
    }
};

class DataSerializer {
private:
    std::string m_format;

public:
    explicit DataSerializer(const std::string& format) : m_format(format) {}

    // Dependency: FileWriter passed in — not stored
    void serialize(const std::vector<std::string>& data, FileWriter& writer) const {
        writer.writeLine("BEGIN " + m_format);
        for (const auto& item : data)
            writer.writeLine("  " + item);
        writer.writeLine("END " + m_format);
    }
};


// ============================================================
// Example 4 — MCX context: FloorController depends on Logger
// Logger is used for audit but not stored in FloorController
// ============================================================
class AuditLogger {
public:
    void log(const std::string& event, const std::string& details) const {
        std::println("  [AUDIT] {} — {}", event, details);
    }
};

enum class FloorResult { Granted, Denied, Released };

class FloorController {
private:
    std::string m_groupId;
    std::string m_currentHolder;

public:
    explicit FloorController(const std::string& groupId)
        : m_groupId(groupId), m_currentHolder("") {}

    // Dependency: AuditLogger passed in — not stored as member
    FloorResult requestFloor(const std::string& sessionId, AuditLogger& logger) {
        if (m_currentHolder.empty()) {
            m_currentHolder = sessionId;
            std::println("  Floor GRANTED to {} in group {}", sessionId, m_groupId);
            logger.log("FLOOR_GRANTED", "session=" + sessionId + " group=" + m_groupId);
            return FloorResult::Granted;
        } else {
            std::println("  Floor DENIED for {} — held by {}", sessionId, m_currentHolder);
            logger.log("FLOOR_DENIED", "session=" + sessionId + " holder=" + m_currentHolder);
            return FloorResult::Denied;
        }
    }

    FloorResult releaseFloor(const std::string& sessionId, AuditLogger& logger) {
        if (m_currentHolder == sessionId) {
            m_currentHolder = "";
            std::println("  Floor RELEASED by {} in group {}", sessionId, m_groupId);
            logger.log("FLOOR_RELEASED", "session=" + sessionId);
            return FloorResult::Released;
        }
        return FloorResult::Denied;
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: ReportGenerator — Printer (dependency)\n";
    std::cout << "=====================================================\n";

    {
        ReportGenerator report("Q1 Performance Report");
        report.addLine("Revenue: EUR 125,000");
        report.addLine("Costs:   EUR  89,000");
        report.addLine("Profit:  EUR  36,000");

        Printer officePrinter("Office Printer");
        Printer pdfPrinter("PDF Printer");

        // Different printer each time — report stores neither
        report.printReport(officePrinter);
        std::cout << '\n';
        report.printSummary(pdfPrinter);

        std::cout << "\nReportGenerator stores no Printer as member.\n";
        std::cout << "Any Printer can be passed at any time.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: OrderProcessor — PaymentService, EmailService\n";
    std::cout << "=====================================================\n";

    {
        OrderProcessor order("ORD-2026-001", 249.99);

        PaymentService payment;
        EmailService   email;

        bool ok = order.process(payment, email,
                                "4111-1111-1111-1111",
                                "kostas@example.com");
        std::println("  Order result: {}", ok ? "SUCCESS" : "FAILED");

        std::cout << "\nOrderProcessor stores neither service as member.\n";
        std::cout << "Different services can be injected — testable!\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: DataSerializer — FileWriter\n";
    std::cout << "=====================================================\n";

    {
        DataSerializer serializer("JSON");
        std::vector<std::string> data = {
            "name: Kostas",
            "role: Senior C++ Dev",
            "city: Hannover"
        };

        FileWriter writer("output.json");
        serializer.serialize(data, writer);   // writer passed in, not stored

        FileWriter backup("backup.json");
        serializer.serialize(data, backup);   // different writer, same serializer

        std::cout << "\nSerializer can write to any FileWriter passed to it.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: FloorController — AuditLogger (MCX)\n";
    std::cout << "=====================================================\n";

    {
        FloorController floor("GRP-ALPHA");
        AuditLogger     logger;

        floor.requestFloor("SES-001", logger);   // granted
        floor.requestFloor("SES-002", logger);   // denied
        floor.releaseFloor("SES-001", logger);   // released
        floor.requestFloor("SES-002", logger);   // now granted

        std::cout << "\nFloorController stores no AuditLogger as member.\n";
        std::cout << "Logger injected per call — easy to swap for testing.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Summary: Dependency\n";
    std::cout << "=====================================================\n";
    std::cout << "  Relationship:  uses-a (temporary)\n";
    std::cout << "  Ownership:     none\n";
    std::cout << "  Duration:      only during the method call\n";
    std::cout << "  Storage:       method parameter or local variable only\n";
    std::cout << "  Key benefit:   easy to swap — enables testing/mocking\n";
    std::cout << "  UML:           A - - - - -> B (dashed line, open arrow)\n";

    return 0;
}
