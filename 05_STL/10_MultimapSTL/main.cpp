#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <print>

// ============================================================
// std::multimap — Sorted Key-Value with Duplicate Keys in C++23
// ============================================================
// Topics covered:
//   1.  What is a multimap
//   2.  Declaration and initialization
//   3.  insert, emplace — no operator[]
//   4.  find, count, contains, equal_range
//   5.  Iterators — sorted by key
//   6.  Erase — single vs all duplicates
//   7.  lower_bound, upper_bound
//   8.  Practical: student grades by subject
//   9.  Practical: event log by timestamp
//  10.  multimap vs map vs unordered_multimap
// ============================================================

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What is a Multimap\n";
    std::cout << "=====================================================\n";

    std::cout << "std::multimap properties:\n";
    std::cout << "  - KEY-VALUE pairs like std::map\n";
    std::cout << "  - DUPLICATE KEYS are allowed\n";
    std::cout << "  - Always SORTED by key\n";
    std::cout << "  - NO operator[] (multiple values per key)\n";
    std::cout << "  - insert/find/erase: O(log n)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    std::multimap<std::string, int> empty;
    std::multimap<std::string, int> mm = {
        {"Alice",   95},
        {"Bob",     82},
        {"Alice",   88},   // duplicate key Alice!
        {"Charlie", 78},
        {"Bob",     91}    // duplicate key Bob!
    };

    std::cout << "multimap (sorted by key, duplicates allowed):\n";
    for (const auto& [k, v] : mm)
        std::println("  {}: {}", k, v);
    std::cout << "size: " << mm.size() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: insert and emplace (NO operator[])\n";
    std::cout << "=====================================================\n";

    std::multimap<int, std::string> events;

    // insert always succeeds — no uniqueness check
    events.insert({100, "user_login"});
    events.insert({200, "data_fetch"});
    events.insert({100, "auth_check"});   // duplicate key 100 — allowed!
    events.insert({300, "data_save"});
    events.insert({200, "cache_hit"});    // duplicate key 200 — allowed!
    events.emplace(400, "user_logout");

    std::cout << "events (multiple per timestamp):\n";
    for (const auto& [ts, event] : events)
        std::println("  [{}] {}", ts, event);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: find, count, contains, equal_range\n";
    std::cout << "=====================================================\n";

    std::multimap<std::string, int> grades = {
        {"Math",    90}, {"Math",    85}, {"Math",    78},
        {"Science", 92}, {"Science", 88},
        {"History", 75}
    };

    // find — returns iterator to FIRST occurrence
    auto it = grades.find("Math");
    if (it != grades.end())
        std::println("find(Math) first: {}", it->second);

    // count — returns number of entries with this key
    std::cout << "count(Math):    " << grades.count("Math")    << '\n';
    std::cout << "count(Science): " << grades.count("Science") << '\n';
    std::cout << "count(History): " << grades.count("History") << '\n';

    // contains — C++20
    std::cout << "contains(Math):   " << grades.contains("Math")   << '\n';
    std::cout << "contains(Art):    " << grades.contains("Art")    << '\n';

    // equal_range — returns [first, last) range for a key
    auto [lo, hi] = grades.equal_range("Math");
    std::cout << "all Math grades: ";
    for (auto itr = lo; itr != hi; ++itr)
        std::cout << itr->second << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Erase — single vs all\n";
    std::cout << "=====================================================\n";

    std::multimap<std::string, int> mm5 = {
        {"A", 1}, {"A", 2}, {"A", 3}, {"B", 4}, {"B", 5}
    };
    std::cout << "before erase: ";
    for (const auto& [k,v] : mm5) std::cout << k<<"="<<v<<" ";
    std::cout << '\n';

    // Erase ALL entries with key "A"
    mm5.erase("A");
    std::cout << "after erase(A): ";
    for (const auto& [k,v] : mm5) std::cout << k<<"="<<v<<" ";
    std::cout << '\n';

    // Erase only ONE entry (by iterator)
    auto itOne = mm5.find("B");
    if (itOne != mm5.end()) mm5.erase(itOne);   // removes first B
    std::cout << "after erase one B: ";
    for (const auto& [k,v] : mm5) std::cout << k<<"="<<v<<" ";
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: lower_bound, upper_bound\n";
    std::cout << "=====================================================\n";

    std::multimap<int, std::string> log = {
        {100, "event_A"}, {200, "event_B"}, {200, "event_C"},
        {300, "event_D"}, {400, "event_E"}, {400, "event_F"}
    };

    // All events between timestamps 150 and 350
    auto startIt = log.lower_bound(150);
    auto endIt   = log.upper_bound(350);

    std::cout << "events in (150, 350]:\n";
    for (auto itr = startIt; itr != endIt; ++itr)
        std::println("  [{}] {}", itr->first, itr->second);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Group values by key\n";
    std::cout << "=====================================================\n";

    std::multimap<std::string, std::string> departments = {
        {"Engineering", "Kostas"},
        {"Engineering", "Andre"},
        {"Engineering", "Sophie"},
        {"Marketing",   "Maria"},
        {"Marketing",   "Tom"},
        {"HR",          "Anna"}
    };

    // Print grouped by department
    std::cout << "Employees by department:\n";
    std::string lastDept;
    for (const auto& [dept, name] : departments) {
        if (dept != lastDept) {
            std::cout << "  " << dept << ":\n";
            lastDept = dept;
        }
        std::cout << "    - " << name << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Student grades per subject\n";
    std::cout << "=====================================================\n";

    std::multimap<std::string, int> studentGrades = {
        {"Math",    90}, {"Math",    75}, {"Math",    85},
        {"Science", 88}, {"Science", 92}, {"Science", 79},
        {"English", 95}, {"English", 72}
    };

    // Calculate average per subject using equal_range
    std::vector<std::string> subjects = {"Math", "Science", "English"};
    for (const auto& subj : subjects) {
        auto [lo2, hi2] = studentGrades.equal_range(subj);
        int sum = 0, cnt = 0;
        for (auto itr = lo2; itr != hi2; ++itr) { sum += itr->second; ++cnt; }
        if (cnt > 0)
            std::println("  {} avg: {:.1f} ({} students)",
                         subj, (double)sum/cnt, cnt);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Event log by timestamp\n";
    std::cout << "=====================================================\n";

    std::multimap<long long, std::string> eventLog;

    // Multiple events at same millisecond timestamp
    eventLog.insert({1000, "CALL_INITIATED"});
    eventLog.insert({1000, "FLOOR_REQUEST"});    // same timestamp
    eventLog.insert({1050, "FLOOR_GRANTED"});
    eventLog.insert({1050, "AUDIO_START"});      // same timestamp
    eventLog.insert({2000, "CALL_RELEASED"});

    std::cout << "MCX Event Log:\n";
    for (const auto& [ts, event] : eventLog)
        std::println("  [{}ms] {}", ts, event);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: multimap vs map vs vector<pair>\n";
    std::cout << "=====================================================\n";

    std::cout << "map:\n";
    std::cout << "  - unique keys, operator[] available\n";
    std::cout << "  - use when one value per key\n\n";

    std::cout << "multimap:\n";
    std::cout << "  - duplicate keys allowed, NO operator[]\n";
    std::cout << "  - use when multiple values per key, sorted order needed\n\n";

    std::cout << "vector<pair<K,V>>:\n";
    std::cout << "  - allows duplicates, NOT automatically sorted\n";
    std::cout << "  - use when order doesn't matter or insertion order needed\n\n";

    std::cout << "unordered_multimap:\n";
    std::cout << "  - duplicate keys, O(1) avg, NOT sorted\n";
    std::cout << "  - use when speed matters and sorted order not needed\n";

    std::cout << "\n=== MultimapSTL complete ===\n";
    return 0;
}
