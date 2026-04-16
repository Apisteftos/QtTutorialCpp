#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// ============================================
// SINGLE RESPONSIBILITY PRINCIPLE (SRP)
// ============================================

/*
SINGLE RESPONSIBILITY PRINCIPLE:
    "A class should have one, and only one, reason to change."
    — Robert C. Martin

WHAT IT MEANS:
    Each class should have ONE responsibility.
    Each class should have ONE reason to change.

WHY IT MATTERS:
    ✅ Easier to understand
    ✅ Easier to test
    ✅ Easier to maintain
    ✅ Easier to reuse
    ✅ Lower coupling

VIOLATION SIGNS:
    🚨 Class name with "And", "Manager", "Handler"
    🚨 Methods that don't relate to each other
    🚨 Many reasons to change the class
    🚨 Large classes (> 500 lines)
    🚨 Hard to name the class clearly

HOW TO APPLY:
    1. Identify responsibilities
    2. Extract each into separate class
    3. Each class does ONE thing well
    4. Clear, focused names
*/

// ============================================
// EXAMPLE 1: EMPLOYEE CLASS (Classic Example)
// ============================================

void example1_employee_class() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: EMPLOYEE CLASS (SRP Violation)     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD: Employee class with multiple responsibilities
    cout << "\n❌ BAD CODE (Multiple Responsibilities):\n";
    cout << "```cpp\n";
    cout << "class Employee {\n";
    cout << "private:\n";
    cout << "    string name;\n";
    cout << "    double salary;\n";
    cout << "    string department;\n";
    cout << "\n";
    cout << "public:\n";
    cout << "    // Responsibility 1: Employee data management\n";
    cout << "    void setName(string n) { name = n; }\n";
    cout << "    string getName() const { return name; }\n";
    cout << "    \n";
    cout << "    // Responsibility 2: Salary calculation (Business logic)\n";
    cout << "    double calculatePay() {\n";
    cout << "        // Complex pay calculation\n";
    cout << "        return salary * 1.1;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Responsibility 3: Database operations\n";
    cout << "    void save() {\n";
    cout << "        // Save to database\n";
    cout << "        cout << \"Saving employee to DB...\" << endl;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Responsibility 4: Report generation\n";
    cout << "    string generateReport() {\n";
    cout << "        return \"Employee Report: \" + name;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • 4 different responsibilities in one class!\n";
    cout << "   • Change in DB schema → modify Employee\n";
    cout << "   • Change in pay calculation → modify Employee\n";
    cout << "   • Change in report format → modify Employee\n";
    cout << "   • Hard to test each responsibility separately\n";
    cout << "   • High coupling\n";

    // ✅ GOOD: Separated responsibilities
    cout << "\n✅ GOOD CODE (Separated Responsibilities):\n";
    cout << "```cpp\n";
    cout << "// Responsibility 1: Employee data (Entity)\n";
    cout << "class Employee {\n";
    cout << "private:\n";
    cout << "    string name;\n";
    cout << "    double salary;\n";
    cout << "    string department;\n";
    cout << "\n";
    cout << "public:\n";
    cout << "    void setName(string n) { name = n; }\n";
    cout << "    string getName() const { return name; }\n";
    cout << "    double getSalary() const { return salary; }\n";
    cout << "};\n\n";
    cout << "// Responsibility 2: Salary calculation (Business logic)\n";
    cout << "class PayrollCalculator {\n";
    cout << "public:\n";
    cout << "    double calculatePay(const Employee& emp) {\n";
    cout << "        // Complex pay calculation logic\n";
    cout << "        return emp.getSalary() * 1.1;\n";
    cout << "    }\n";
    cout << "};\n\n";
    cout << "// Responsibility 3: Database operations\n";
    cout << "class EmployeeRepository {\n";
    cout << "public:\n";
    cout << "    void save(const Employee& emp) {\n";
    cout << "        // Database operations\n";
    cout << "        cout << \"Saving \" << emp.getName() << \" to DB\" << endl;\n";
    cout << "    }\n";
    cout << "};\n\n";
    cout << "// Responsibility 4: Report generation\n";
    cout << "class EmployeeReportGenerator {\n";
    cout << "public:\n";
    cout << "    string generate(const Employee& emp) {\n";
    cout << "        return \"Employee Report: \" + emp.getName();\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Each class has ONE clear responsibility\n";
    cout << "   ✅ Change in DB → only modify EmployeeRepository\n";
    cout << "   ✅ Change in pay → only modify PayrollCalculator\n";
    cout << "   ✅ Easy to test each class independently\n";
    cout << "   ✅ Can reuse PayrollCalculator for other entities\n";
    cout << "   ✅ Low coupling, high cohesion\n";

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   \"Separate what changes for different reasons!\"\n";
}

// ============================================
// EXAMPLE 2: USER REGISTRATION (Real-World)
// ============================================

namespace bad_example2 {
// ❌ BAD: UserRegistration with multiple responsibilities
class UserRegistration {
private:
    string username;
    string email;
    string password;

public:
    bool registerUser(string user, string mail, string pass) {
        username = user;
        email = mail;
        password = pass;

        // Responsibility 1: Validation
        if (username.empty() || email.empty() || password.empty()) {
            cout << "❌ Validation failed!" << endl;
            return false;
        }

        // Responsibility 2: Password hashing
        string hashedPassword = hashPassword(password);

        // Responsibility 3: Database storage
        saveToDatabase(username, email, hashedPassword);

        // Responsibility 4: Email notification
        sendWelcomeEmail(email);

        return true;
    }

private:
    string hashPassword(string pass) {
        // Hash logic
        return "hashed_" + pass;
    }

    void saveToDatabase(string user, string mail, string pass) {
        cout << "💾 Saving to database..." << endl;
    }

    void sendWelcomeEmail(string mail) {
        cout << "📧 Sending welcome email to " << mail << endl;
    }
};
}

namespace good_example2 {
// ✅ GOOD: Separated responsibilities

// 1. Data validation
class UserValidator {
public:
    bool validate(const string& username, const string& email, const string& password) {
        if (username.empty() || email.empty() || password.empty()) {
            return false;
        }
        // More validation rules...
        return true;
    }
};

// 2. Password security
class PasswordHasher {
public:
    string hash(const string& password) {
        return "hashed_" + password;
    }
};

// 3. Database operations
class UserRepository {
public:
    void save(const string& username, const string& email, const string& hashedPassword) {
        cout << "💾 Saving user to database..." << endl;
    }
};

// 4. Email notifications
class EmailService {
public:
    void sendWelcomeEmail(const string& email) {
        cout << "📧 Sending welcome email to " << email << endl;
    }
};

// 5. Orchestrator (coordinates the process)
class UserRegistrationService {
private:
    UserValidator validator;
    PasswordHasher hasher;
    UserRepository repository;
    EmailService emailService;

public:
    bool registerUser(const string& username, const string& email, const string& password) {
        // Coordinate the registration process
        if (!validator.validate(username, email, password)) {
            cout << "❌ Validation failed!" << endl;
            return false;
        }

        string hashedPassword = hasher.hash(password);
        repository.save(username, email, hashedPassword);
        emailService.sendWelcomeEmail(email);

        return true;
    }
};
}

void example2_user_registration() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: USER REGISTRATION (Real-World)     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD: One class does everything\n";
    cout << "   • Validation + Hashing + Database + Email\n";
    cout << "   • Hard to test\n";
    cout << "   • Hard to change one part\n";

    cout << "\n✅ GOOD: Each class has one job\n";
    cout << "   • UserValidator → validates input\n";
    cout << "   • PasswordHasher → handles security\n";
    cout << "   • UserRepository → manages database\n";
    cout << "   • EmailService → sends emails\n";
    cout << "   • UserRegistrationService → coordinates\n";

    cout << "\n🔧 DEMONSTRATION:\n";

    good_example2::UserRegistrationService registrationService;
    bool result = registrationService.registerUser("john_doe", "john@example.com", "secret123");

    cout << "\n✅ Registration " << (result ? "successful" : "failed") << "!\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Can test validation independently\n";
    cout << "   • Can change email service without touching validation\n";
    cout << "   • Can reuse EmailService in other contexts\n";
    cout << "   • Each class is small and focused\n";
}

// ============================================
// EXAMPLE 3: LOGGER CLASS (Common Mistake)
// ============================================

namespace bad_example3 {
// ❌ BAD: Logger with multiple responsibilities
class Logger {
private:
    vector<string> logs;

public:
    void log(const string& message) {
        logs.push_back(message);
    }

    // Responsibility 2: Formatting
    string formatLog(const string& message) {
        return "[LOG] " + message;
    }

    // Responsibility 3: File I/O
    void saveToFile(const string& filename) {
        ofstream file(filename);
        for (const auto& log : logs) {
            file << log << endl;
        }
        file.close();
    }

    // Responsibility 4: Network transmission
    void sendToServer(const string& serverUrl) {
        cout << "📡 Sending logs to " << serverUrl << endl;
    }
};
}

namespace good_example3 {
// ✅ GOOD: Separated responsibilities

// 1. Log storage
class LogStorage {
private:
    vector<string> logs;

public:
    void add(const string& message) {
        logs.push_back(message);
    }

    const vector<string>& getLogs() const {
        return logs;
    }
};

// 2. Log formatting
class LogFormatter {
public:
    string format(const string& message) {
        return "[LOG] " + message;
    }
};

// 3. File writer
class LogFileWriter {
public:
    void write(const string& filename, const vector<string>& logs) {
        ofstream file(filename);
        for (const auto& log : logs) {
            file << log << endl;
        }
        file.close();
    }
};

// 4. Network sender
class LogNetworkSender {
public:
    void send(const string& serverUrl, const vector<string>& logs) {
        cout << "📡 Sending " << logs.size() << " logs to " << serverUrl << endl;
    }
};

// 5. Logger (coordinates)
class Logger {
private:
    LogStorage storage;
    LogFormatter formatter;

public:
    void log(const string& message) {
        string formatted = formatter.format(message);
        storage.add(formatted);
    }

    void saveToFile(const string& filename) {
        LogFileWriter writer;
        writer.write(filename, storage.getLogs());
    }

    void sendToServer(const string& serverUrl) {
        LogNetworkSender sender;
        sender.send(serverUrl, storage.getLogs());
    }
};
}

void example3_logger_class() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: LOGGER CLASS (Common Mistake)      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD: Logger does storage + formatting + file I/O + network\n";
    cout << "   • Change file format → modify Logger\n";
    cout << "   • Change network protocol → modify Logger\n";
    cout << "   • Can't test formatting without file I/O\n";

    cout << "\n✅ GOOD: Separated concerns\n";
    cout << "   • LogStorage → stores logs in memory\n";
    cout << "   • LogFormatter → formats messages\n";
    cout << "   • LogFileWriter → writes to file\n";
    cout << "   • LogNetworkSender → sends over network\n";

    cout << "\n🔧 DEMONSTRATION:\n";

    good_example3::Logger logger;
    logger.log("Application started");
    logger.log("User logged in");
    logger.log("Processing completed");

    logger.saveToFile("app.log");
    logger.sendToServer("https://logs.example.com");

    cout << "\n💡 NOW YOU CAN:\n";
    cout << "   • Change file format without touching network code\n";
    cout << "   • Test formatting independently\n";
    cout << "   • Reuse LogFileWriter for other purposes\n";
    cout << "   • Switch storage implementation easily\n";
}

// ============================================
// EXAMPLE 4: QUICK COMPARISON
// ============================================

void example4_comparison() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: SRP QUICK COMPARISON               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 VIOLATION vs COMPLIANCE:\n";

    cout << "\n❌ VIOLATIONS (God Classes):\n";
    cout << "   • UserManager: Creates, updates, validates, emails, logs\n";
    cout << "   • DataHandler: Reads files, parses, validates, saves\n";
    cout << "   • OrderProcessor: Validates, calculates, sends emails, logs\n";

    cout << "\n✅ COMPLIANCE (Focused Classes):\n";
    cout << "   • UserCreator: Creates users\n";
    cout << "   • UserValidator: Validates user data\n";
    cout << "   • EmailService: Sends emails\n";
    cout << "   • FileReader: Reads files\n";
    cout << "   • DataParser: Parses data\n";

    cout << "\n🎯 HOW TO IDENTIFY SRP VIOLATIONS:\n";
    cout << "   1. Count responsibilities (> 1 = violation)\n";
    cout << "   2. Count reasons to change (> 1 = violation)\n";
    cout << "   3. Try to describe class in one sentence\n";
    cout << "   4. Check if methods relate to each other\n";
    cout << "   5. Look for 'And' in class names\n";

    cout << "\n🔧 HOW TO FIX:\n";
    cout << "   1. List all responsibilities\n";
    cout << "   2. Create new class for each\n";
    cout << "   3. Move related methods\n";
    cout << "   4. Keep original as coordinator (optional)\n";
    cout << "   5. Update dependencies\n";
}

// ============================================
// EXAMPLE 5: BENEFITS SUMMARY
// ============================================

void example5_benefits() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: SRP BENEFITS SUMMARY              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF SRP:\n";

    cout << "\n1. EASIER TO UNDERSTAND:\n";
    cout << "   • Small, focused classes\n";
    cout << "   • Clear purpose\n";
    cout << "   • Less cognitive load\n";

    cout << "\n2. EASIER TO TEST:\n";
    cout << "   • Test one responsibility at a time\n";
    cout << "   • Mock dependencies easily\n";
    cout << "   • Faster test execution\n";

    cout << "\n3. EASIER TO MAINTAIN:\n";
    cout << "   • Changes isolated to one class\n";
    cout << "   • Less risk of breaking other features\n";
    cout << "   • Easier to find bugs\n";

    cout << "\n4. EASIER TO REUSE:\n";
    cout << "   • Small classes = more reusable\n";
    cout << "   • Can use EmailService anywhere\n";
    cout << "   • No baggage from unrelated code\n";

    cout << "\n5. BETTER COLLABORATION:\n";
    cout << "   • Team members can work on different classes\n";
    cout << "   • Less merge conflicts\n";
    cout << "   • Clear ownership\n";

    cout << "\n⚠️  COMMON MISTAKES:\n";
    cout << "   • Over-engineering (too many tiny classes)\n";
    cout << "   • Splitting at wrong boundaries\n";
    cout << "   • Creating artificial separations\n";

    cout << "\n💡 GOLDEN RULE:\n";
    cout << "   \"Gather together things that change for the same reason.\n";
    cout << "    Separate things that change for different reasons.\"\n";
    cout << "    — Robert C. Martin\n";

    cout << "\n🎯 REMEMBER:\n";
    cout << "   • One class = One responsibility\n";
    cout << "   • One class = One reason to change\n";
    cout << "   • If you use 'AND' to describe it, split it!\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║      SINGLE RESPONSIBILITY PRINCIPLE (SRP)            ║\n";
    cout << "║                                                       ║\n";
    cout << "║   \"A class should have one reason to change\"          ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_employee_class();
    example2_user_registration();
    example3_logger_class();
    example4_comparison();
    example5_benefits();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 SINGLE RESPONSIBILITY COMPLETE! 🎉         ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: One class, one job, one reason to     ║\n";
    cout << "║  change. Keep it focused and simple!                 ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
