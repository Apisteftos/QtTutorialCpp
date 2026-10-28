#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <print>

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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: EMPLOYEE CLASS (SRP Violation)      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD: Employee class with multiple responsibilities
    std::println("");
    std::println("    ");
    std::println("    ");
    std::println("    ");

}

// ============================================
// EXAMPLE 2: USER REGISTRATION (Real-World)
// ============================================

namespace bad_example2 {
// ❌ BAD: UserRegistration with multiple responsibilities
class UserRegistration {
private:
    std::string username;
    std::string email;
    std::string password;

public:
    bool registerUser(std::string user, std::string mail, std::string pass) {
        username = user;
        email = mail;
        password = pass;

        // Responsibility 1: Validation
        if (username.empty() || email.empty() || password.empty()) {
            std::println("❌ Validation failed!");
            return false;
        }

        // Responsibility 2: Password hashing
        std::string hashedPassword = hashPassword(password);

        // Responsibility 3: Database storage
        saveToDatabase(username, email, hashedPassword);

        // Responsibility 4: Email notification
        sendWelcomeEmail(email);

        return true;
    }

private:
    std::string hashPassword(std::string pass) {
        // Hash logic
        return "hashed_" + pass;
    }

    void saveToDatabase(std::string user, std::string mail, std::string pass) {
        std::println("💾 Saving to database...");
    }

    void sendWelcomeEmail(std::string mail) {
        std::println("📧 Sending welcome email to {}", mail);
    }
};
}

namespace good_example2 {
// ✅ GOOD: Separated responsibilities

// 1. Data validation
class UserValidator {
public:
    bool validate(const std::string& username, const std::string& email, const std::string& password) {
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
    std::string hash(const std::string& password) {
        return "hashed_" + password;
    }
};

// 3. Database operations
class UserRepository {
public:
    void save(const std::string& username, const std::string& email, const std::string& hashedPassword) {
        std::println("💾 Saving user to database...");
    }
};

// 4. Email notifications
class EmailService {
public:
    void sendWelcomeEmail(const std::string& email) {
        std::println("📧 Sending welcome email to {}", email);
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
    bool registerUser(const std::string& username, const std::string& email, const std::string& password) {
        // Coordinate the registration process
        if (!validator.validate(username, email, password)) {
            std::println("❌ Validation failed!");
            return false;
        }

        std::string hashedPassword = hasher.hash(password);
        repository.save(username, email, hashedPassword);
        emailService.sendWelcomeEmail(email);

        return true;
    }
};
}

void example2_user_registration() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: USER REGISTRATION (Real-World)      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example2::UserRegistrationService registrationService;
    bool result = registrationService.registerUser("john_doe", "john@example.com", "secret123");

    std::println("");
    std::println("✅ Registration {}!", (result ? "successful" : "failed"));

}

// ============================================
// EXAMPLE 3: LOGGER CLASS (Common Mistake)
// ============================================

namespace bad_example3 {
// ❌ BAD: Logger with multiple responsibilities
class Logger {
private:
    std::vector<std::string> logs;

public:
    void log(const std::string& message) {
        logs.push_back(message);
    }

    // Responsibility 2: Formatting
    std::string formatLog(const std::string& message) {
        return "[LOG] " + message;
    }

    // Responsibility 3: File I/O
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& log : logs) {
            file << log << std::endl;
        }
        file.close();
    }

    // Responsibility 4: Network transmission
    void sendToServer(const std::string& serverUrl) {
        std::println("📡 Sending logs to {}", serverUrl);
    }
};
}

namespace good_example3 {
// ✅ GOOD: Separated responsibilities

// 1. Log storage
class LogStorage {
private:
    std::vector<std::string> logs;

public:
    void add(const std::string& message) {
        logs.push_back(message);
    }

    const std::vector<std::string>& getLogs() const {
        return logs;
    }
};

// 2. Log formatting
class LogFormatter {
public:
    std::string format(const std::string& message) {
        return "[LOG] " + message;
    }
};

// 3. File writer
class LogFileWriter {
public:
    void write(const std::string& filename, const std::vector<std::string>& logs) {
        std::ofstream file(filename);
        for (const auto& log : logs) {
            file << log << std::endl;
        }
        file.close();
    }
};

// 4. Network sender
class LogNetworkSender {
public:
    void send(const std::string& serverUrl, const std::vector<std::string>& logs) {
        std::println("📡 Sending {} logs to {}", logs.size(), serverUrl);
    }
};

// 5. Logger (coordinates)
class Logger {
private:
    LogStorage storage;
    LogFormatter formatter;

public:
    void log(const std::string& message) {
        std::string formatted = formatter.format(message);
        storage.add(formatted);
    }

    void saveToFile(const std::string& filename) {
        LogFileWriter writer;
        writer.write(filename, storage.getLogs());
    }

    void sendToServer(const std::string& serverUrl) {
        LogNetworkSender sender;
        sender.send(serverUrl, storage.getLogs());
    }
};
}

void example3_logger_class() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: LOGGER CLASS (Common Mistake)       ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example3::Logger logger;
    logger.log("Application started");
    logger.log("User logged in");
    logger.log("Processing completed");

    logger.saveToFile("app.log");
    logger.sendToServer("https://logs.example.com");

}

// ============================================
// EXAMPLE 4: QUICK COMPARISON
// ============================================

void example4_comparison() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: SRP QUICK COMPARISON                ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 5: BENEFITS SUMMARY
// ============================================

void example5_benefits() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: SRP BENEFITS SUMMARY                ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║      SINGLE RESPONSIBILITY PRINCIPLE (SRP)            ║");
    std::println("║                                                       ║");
    std::println("║   \"A class should have one reason to change\"        ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_employee_class();
    example2_user_registration();
    example3_logger_class();
    example4_comparison();
    example5_benefits();

    return 0;
}
