#include <iostream>
#include <string>
#include <memory>

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
        std::cout << "[CONSOLE] " << message << std::endl;
    }
};

// File Logger
class FileLogger : public Logger {
public:
    FileLogger(LogLevel level) : Logger(level) {}

protected:
    void write(const std::string& message) override {
        std::cout << "[FILE] Writing to file: " << message << std::endl;
    }
};

// Email Logger
class EmailLogger : public Logger {
public:
    EmailLogger(LogLevel level) : Logger(level) {}

protected:
    void write(const std::string& message) override {
        std::cout << "[EMAIL] Sending email alert: " << message << std::endl;
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

    std::cout << "=== Logging System with Chain of Responsibility ===" << std::endl << std::endl;

    std::cout << "INFO Message:" << std::endl;
    consoleLogger->logMessage(INFO, "Application started successfully");
    std::cout << std::endl;

    std::cout << "WARNING Message:" << std::endl;
    consoleLogger->logMessage(WARNING, "High memory usage detected");
    std::cout << std::endl;

    std::cout << "ERROR Message:" << std::endl;
    consoleLogger->logMessage(ERROR, "Critical system failure!");

    return 0;
}
