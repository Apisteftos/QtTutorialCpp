#include <string>
#include <memory>
#include <print>

enum LogLevel {
    INFO = 1,
    DEBUG = 2,
    WARNING = 3,
    ERROR = 4
};

// Abstract Logger
class Logger {
protected:
    LogLevel level;
    std::shared_ptr<Logger> nextLogger;

public:
    Logger(LogLevel lvl) : level(lvl), nextLogger(nullptr) {}
    virtual ~Logger() {}

    void setNext(std::shared_ptr<Logger> logger) {
        nextLogger = logger;
    }

    void logMessage(LogLevel msgLevel, const std::string& message) {
        if (msgLevel >= level) {
            write(message);
        }
        if (nextLogger) {
            nextLogger->logMessage(msgLevel, message);
        }
    }

protected:
    virtual void write(const std::string& message) = 0;
};

// Console Logger
class ConsoleLogger : public Logger {
public:
    ConsoleLogger(LogLevel level) : Logger(level) {}

protected:
    void write(const std::string& message) override {
        std::println("[CONSOLE] {}", message);
    }
};

// File Logger
class FileLogger : public Logger {
public:
    FileLogger(LogLevel level) : Logger(level) {}

protected:
    void write(const std::string& message) override {
        std::println("[FILE] Writing to file: {}", message);
    }
};

// Email Logger
class EmailLogger : public Logger {
public:
    EmailLogger(LogLevel level) : Logger(level) {}

protected:
    void write(const std::string& message) override {
        std::println("[EMAIL] Sending email alert: {}", message);
    }
};

int main() {
    // Create loggers
    auto consoleLogger = std::make_shared<ConsoleLogger>(INFO);
    auto fileLogger = std::make_shared<FileLogger>(WARNING);
    auto emailLogger = std::make_shared<EmailLogger>(ERROR);

    // Chain them: Console -> File -> Email
    consoleLogger->setNext(fileLogger);
    fileLogger->setNext(emailLogger);

    std::println("=== Logging System with Chain of Responsibility ===");
    std::println("");

    std::println("INFO Message:");
    consoleLogger->logMessage(INFO, "Application started successfully");
    std::println("");

    std::println("WARNING Message:");
    consoleLogger->logMessage(WARNING, "High memory usage detected");
    std::println("");

    std::println("ERROR Message:");
    consoleLogger->logMessage(ERROR, "Critical system failure!");

    return 0;
}
