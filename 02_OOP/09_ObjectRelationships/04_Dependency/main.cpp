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
// Example 4 — MeetingRoom depends on ActivityLogger
// Logger is used for an activity log but not stored in MeetingRoom
// ============================================================
class ActivityLogger {
public:
    void log(const std::string& event, const std::string& details) const {
        std::println("  [LOG] {} — {}", event, details);
    }
};

enum class BookingResult { Booked, Denied, Released };

class MeetingRoom {
private:
    std::string m_roomId;
    std::string m_currentUser;

public:
    explicit MeetingRoom(const std::string& roomId)
        : m_roomId(roomId), m_currentUser("") {}

    // Dependency: ActivityLogger passed in — not stored as member
    BookingResult book(const std::string& user, ActivityLogger& logger) {
        if (m_currentUser.empty()) {
            m_currentUser = user;
            std::println("  Room {} BOOKED by {}", m_roomId, user);
            logger.log("ROOM_BOOKED", "user=" + user + " room=" + m_roomId);
            return BookingResult::Booked;
        } else {
            std::println("  Room {} DENIED for {} — booked by {}", m_roomId, user, m_currentUser);
            logger.log("BOOKING_DENIED", "user=" + user + " holder=" + m_currentUser);
            return BookingResult::Denied;
        }
    }

    BookingResult release(const std::string& user, ActivityLogger& logger) {
        if (m_currentUser == user) {
            m_currentUser = "";
            std::println("  Room {} RELEASED by {}", m_roomId, user);
            logger.log("ROOM_RELEASED", "user=" + user);
            return BookingResult::Released;
        }
        return BookingResult::Denied;
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::println("=====================================================");
    std::println("Example 1: ReportGenerator — Printer (dependency)");
    std::println("=====================================================");

    {
        ReportGenerator report("Q1 Performance Report");
        report.addLine("Revenue: EUR 125,000");
        report.addLine("Costs:   EUR  89,000");
        report.addLine("Profit:  EUR  36,000");

        Printer officePrinter("Office Printer");
        Printer pdfPrinter("PDF Printer");

        // Different printer each time — report stores neither
        report.printReport(officePrinter);
        std::println("");
        report.printSummary(pdfPrinter);
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 2: OrderProcessor — PaymentService, EmailService");
    std::println("=====================================================");

    {
        OrderProcessor order("ORD-2026-001", 249.99);

        PaymentService payment;
        EmailService   email;

        bool ok = order.process(payment, email,
                                "4111-1111-1111-1111",
                                "kostas@example.com");
        std::println("  Order result: {}", ok ? "SUCCESS" : "FAILED");
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 3: DataSerializer — FileWriter");
    std::println("=====================================================");

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
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 4: MeetingRoom — ActivityLogger");
    std::println("=====================================================");

    {
        MeetingRoom     room("ROOM-A");
        ActivityLogger  logger;

        room.book("alice", logger);      // booked
        room.book("bob",   logger);      // denied
        room.release("alice", logger);   // released
        room.book("bob",   logger);      // now booked
    }

    return 0;
}
