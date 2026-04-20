#include <iostream>
#include <string>
#include <vector>
#include <print>

// ============================================================
// Package Managers — C++23
// ============================================================
// This file demonstrates a project that uses packages managed
// by vcpkg and Conan — the two most popular C++ package managers.
//
// In a real project, these headers would come from installed packages:
// #include <nlohmann/json.hpp>    ← from vcpkg: nlohmann-json
// #include <fmt/format.h>         ← from vcpkg: fmt
// #include <spdlog/spdlog.h>      ← from vcpkg: spdlog
// #include <gtest/gtest.h>        ← from vcpkg: gtest
//
// This file simulates the same concepts without requiring
// the actual packages installed.
// ============================================================

// Simulated JSON-like structure (replaces nlohmann/json)
struct Config {
    std::string host     = "192.168.1.100";
    int         port     = 5060;
    std::string group    = "GRP-ALPHA";
    bool        encrypted = true;

    void print() const {
        std::println("{{");
        std::println("  \"host\":      \"{}\"", host);
        std::println("  \"port\":      {}", port);
        std::println("  \"group\":     \"{}\"", group);
        std::println("  \"encrypted\": {}", encrypted ? "true" : "false");
        std::println("}}");
    }
};

// Simulated logging (replaces spdlog)
enum class LogLevel { DEBUG, INFO, WARN, ERROR };

void log(LogLevel level, const std::string& msg) {
    const char* levelStr = "";
    switch (level) {
        case LogLevel::DEBUG: levelStr = "DEBUG"; break;
        case LogLevel::INFO:  levelStr = "INFO "; break;
        case LogLevel::WARN:  levelStr = "WARN "; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
    }
    std::println("[{}] {}", levelStr, msg);
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Package Managers in C++\n";
    std::cout << "=====================================================\n";

    std::cout << "\nPopular C++ package managers:\n";
    std::vector<std::pair<std::string,std::string>> managers = {
        {"vcpkg",  "Microsoft — integrates with CMake/MSBuild"},
        {"Conan",  "JFrog — Python-based, flexible"},
        {"apt",    "Ubuntu/Debian — system packages"},
        {"Hunter", "CMake-integrated package manager"},
        {"CPM",    "CMake Package Manager — lightweight"},
    };

    for (const auto& [name, desc] : managers)
        std::println("  {:8} — {}", name, desc);


    std::cout << "\n=====================================================\n";
    std::cout << "Simulated usage (what packages enable)\n";
    std::cout << "=====================================================\n";

    std::cout << "\n--- nlohmann/json simulation ---\n";
    Config cfg;
    std::println("Config as JSON:");
    cfg.print();

    std::cout << "\n--- spdlog simulation ---\n";
    log(LogLevel::INFO,  "Application started");
    log(LogLevel::INFO,  "Connecting to " + cfg.host);
    log(LogLevel::DEBUG, "Port: " + std::to_string(cfg.port));
    log(LogLevel::WARN,  "Retry attempt 1/3");
    log(LogLevel::ERROR, "Connection timeout");

    std::cout << "\n--- fmt simulation ---\n";
    std::println("std::print / std::println (C++23) is fmt built into stdlib!");
    std::println("Port formatted: {:>6}", cfg.port);
    std::println("Host padded:    {:>20}", cfg.host);

    std::cout << "\n=== Package Managers example complete ===\n";
    std::cout << "See README.md for vcpkg and Conan setup instructions.\n";
    return 0;
}
