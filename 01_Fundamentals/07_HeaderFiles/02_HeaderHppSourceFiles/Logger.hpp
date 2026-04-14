#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <print>

// ============================================================
// Logger.hpp — Header-only utility class (C++ only)
// ============================================================
// Small utility classes are often kept entirely in .hpp —
// no separate .cpp needed.
// ============================================================

enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger
{
public:
    explicit Logger(const std::string& name)
        : m_name(name), m_level(LogLevel::DEBUG) {}

    void setLevel(LogLevel level) { m_level = level; }

    void debug  (const std::string& msg) { log(LogLevel::DEBUG,   msg); }
    void info   (const std::string& msg) { log(LogLevel::INFO,    msg); }
    void warning(const std::string& msg) { log(LogLevel::WARNING, msg); }
    void error  (const std::string& msg) { log(LogLevel::ERROR,   msg); }

private:
    void log(LogLevel level, const std::string& msg) const
    {
        if (level < m_level) return;

        const char* levelStr = "";
        switch (level) {
            case LogLevel::DEBUG:   levelStr = "DEBUG";   break;
            case LogLevel::INFO:    levelStr = "INFO";    break;
            case LogLevel::WARNING: levelStr = "WARN";    break;
            case LogLevel::ERROR:   levelStr = "ERROR";   break;
        }
        std::println("[{}][{}] {}", levelStr, m_name, msg);
    }

    std::string m_name;
    LogLevel    m_level;
};
