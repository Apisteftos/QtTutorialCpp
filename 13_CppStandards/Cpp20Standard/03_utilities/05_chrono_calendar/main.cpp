/*
 * <chrono> calendar and clocks: year_month_day, weekday, sys_days, days,
 * duration_cast, std::format with chrono types (C++20)
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <chrono>
#include <format>
#include <iostream>

int main() {
    namespace chrono = std::chrono;

    // === EXAMPLE 1: calendar dates ===
    std::cout << "=== Example 1: year_month_day ===\n";
    chrono::year_month_day date = chrono::year(2024) / chrono::February / 28;
    std::cout << "date: " << date << ", valid: " << date.ok() << "\n";
    std::cout << "leap year: " << date.year().is_leap() << "\n";
    chrono::year_month_day invalid = chrono::year(2023) / chrono::February / 29;
    std::cout << "2023-02-29 valid: " << invalid.ok() << "\n";

    // === EXAMPLE 2: day arithmetic through sys_days ===
    std::cout << "\n=== Example 2: arithmetic ===\n";
    chrono::sys_days start = chrono::sys_days{date};
    chrono::year_month_day next = start + chrono::days{2};
    std::cout << "28 Feb 2024 + 2 days = " << next << "\n";
    std::cout << "weekday: " << chrono::weekday{start} << "\n";
    chrono::sys_days christmas = chrono::sys_days{chrono::year(2024) / chrono::December / 25};
    std::cout << "days until 25 Dec: " << (christmas - start).count() << "\n";
    chrono::year_month_day lastDay = chrono::year(2024) / chrono::February / chrono::last;
    std::cout << "last day of Feb 2024: " << lastDay << "\n";

    // === EXAMPLE 3: durations and format ===
    std::cout << "\n=== Example 3: durations ===\n";
    auto total = chrono::hours(2) + chrono::minutes(30) + chrono::seconds(15);
    std::cout << "total seconds: " << chrono::duration_cast<chrono::seconds>(total).count() << "\n";
    std::cout << "as minutes (floor): " << chrono::floor<chrono::minutes>(total).count() << "\n";
    std::cout << std::format("formatted: {}\n", total);
    std::cout << std::format("date formatted: {:%d.%m.%Y}\n", date);
    std::cout << std::format("hh:mm:ss: {:%T}\n", chrono::hh_mm_ss{total});

    // === EXAMPLE 4: clocks ===
    std::cout << "\n=== Example 4: clocks ===\n";
    auto t0 = chrono::steady_clock::now();
    auto t1 = chrono::steady_clock::now();
    std::cout << std::boolalpha;
    std::cout << "steady_clock is monotonic: " << (t1 >= t0) << "\n";
    std::cout << "system_clock is after 2020: "
              << (chrono::system_clock::now() > chrono::sys_days{chrono::year(2020) / chrono::January / 1}) << "\n";
    return 0;
}
