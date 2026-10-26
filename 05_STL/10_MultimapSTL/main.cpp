#include <map>
#include <print>
#include <string>
#include <string_view>
#include <vector>

// ============================================================
// std::multimap — Sorted Key-Value with Duplicate Keys in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  insert, emplace — no operator[]
//   3.  find, count, contains, equal_range
//   4.  Erase — single vs all duplicates
//   5.  lower_bound, upper_bound
//   6.  Group values by key
//   7.  Practical: student grades by subject
//   8.  Practical: event log by timestamp
// ============================================================

// Print the "=====" banner that introduces an example
void banner(std::string_view title)
{
    std::println("");
    std::println("=====================================================");
    std::println("{}", title);
    std::println("=====================================================");
}

// Print a label followed by all key=value pairs of a map
template<typename Map>
void printMap(std::string_view label, const Map& m)
{
    std::print("{}", label);
    for (const auto& [k, v] : m) {
        std::print("{}={} ", k, v);
    }
    std::println("");
}

int main()
{
    // === EXAMPLE 1: Declaration and Initialization ===
    banner("Example 1: Declaration and Initialization");

    std::multimap<std::string, int> empty;
    std::multimap<std::string, int> mm = {
        {"Alice",   95},
        {"Bob",     82},
        {"Alice",   88},   // duplicate key Alice!
        {"Charlie", 78},
        {"Bob",     91}    // duplicate key Bob!
    };

    std::println("empty size: {}", empty.size());
    std::println("multimap (sorted by key, duplicates allowed):");
    for (const auto& [k, v] : mm) {
        std::println("  {}: {}", k, v);
    }
    std::println("size: {}", mm.size());


    // === EXAMPLE 2: insert and emplace (NO operator[]) ===
    banner("Example 2: insert and emplace (NO operator[])");

    std::multimap<int, std::string> events;

    // insert always succeeds — no uniqueness check
    events.insert({100, "user_login"});
    events.insert({200, "data_fetch"});
    events.insert({100, "auth_check"});   // duplicate key 100 — allowed!
    events.insert({300, "data_save"});
    events.insert({200, "cache_hit"});    // duplicate key 200 — allowed!
    events.emplace(400, "user_logout");

    // Entries with equal keys stay in INSERTION order
    std::println("events (multiple per timestamp):");
    for (const auto& [ts, event] : events) {
        std::println("  [{}] {}", ts, event);
    }


    // === EXAMPLE 3: find, count, contains, equal_range ===
    banner("Example 3: find, count, contains, equal_range");

    std::multimap<std::string, int> grades = {
        {"Math",    90}, {"Math",    85}, {"Math",    78},
        {"Science", 92}, {"Science", 88},
        {"History", 75}
    };

    // find — returns iterator to FIRST occurrence
    auto it = grades.find("Math");
    if (it != grades.end()) {
        std::println("find(Math) first: {}", it->second);
    }

    // count — returns number of entries with this key
    std::println("count(Math):    {}", grades.count("Math"));
    std::println("count(Science): {}", grades.count("Science"));
    std::println("count(History): {}", grades.count("History"));

    // contains — C++20
    std::println("contains(Math):   {}", grades.contains("Math"));
    std::println("contains(Art):    {}", grades.contains("Art"));

    // equal_range — returns [first, last) range for a key
    auto [lo, hi] = grades.equal_range("Math");
    std::print("all Math grades: ");
    for (auto itr = lo; itr != hi; ++itr) {
        std::print("{} ", itr->second);
    }
    std::println("");


    // === EXAMPLE 4: Erase — single vs all ===
    banner("Example 4: Erase — single vs all");

    std::multimap<std::string, int> mm5 = {
        {"A", 1}, {"A", 2}, {"A", 3}, {"B", 4}, {"B", 5}
    };
    printMap("before erase: ", mm5);

    // Erase ALL entries with key "A"
    mm5.erase("A");
    printMap("after erase(A): ", mm5);

    // Erase only ONE entry (by iterator)
    auto itOne = mm5.find("B");
    if (itOne != mm5.end()) {
        mm5.erase(itOne);   // removes first B
    }
    printMap("after erase one B: ", mm5);


    // === EXAMPLE 5: lower_bound, upper_bound ===
    banner("Example 5: lower_bound, upper_bound");

    std::multimap<int, std::string> timeline = {
        {100, "event_A"}, {200, "event_B"}, {200, "event_C"},
        {300, "event_D"}, {400, "event_E"}, {400, "event_F"}
    };

    // All events with timestamps between 150 and 350
    auto startIt = timeline.lower_bound(150);   // first key >= 150
    auto endIt   = timeline.upper_bound(350);   // first key >  350

    std::println("events in [150, 350]:");
    for (auto itr = startIt; itr != endIt; ++itr) {
        std::println("  [{}] {}", itr->first, itr->second);
    }


    // === EXAMPLE 6: Group values by key ===
    banner("Example 6: Group values by key");

    std::multimap<std::string, std::string> departments = {
        {"Engineering", "Kostas"},
        {"Engineering", "Andre"},
        {"Engineering", "Sophie"},
        {"Marketing",   "Maria"},
        {"Marketing",   "Tom"},
        {"HR",          "Anna"}
    };

    // Print grouped by department
    std::println("Employees by department:");
    std::string lastDept;
    for (const auto& [dept, name] : departments) {
        if (dept != lastDept) {
            std::println("  {}:", dept);
            lastDept = dept;
        }
        std::println("    - {}", name);
    }


    // === EXAMPLE 7: Student grades per subject ===
    banner("Example 7: Student grades per subject");

    std::multimap<std::string, int> studentGrades = {
        {"Math",    90}, {"Math",    75}, {"Math",    85},
        {"Science", 88}, {"Science", 92}, {"Science", 79},
        {"English", 95}, {"English", 72}
    };

    // Calculate average per subject using equal_range
    std::vector<std::string> subjects = {"Math", "Science", "English"};
    for (const auto& subj : subjects) {
        auto [first, last] = studentGrades.equal_range(subj);
        int sum = 0;
        int cnt = 0;
        for (auto itr = first; itr != last; ++itr) {
            sum += itr->second;
            ++cnt;
        }
        if (cnt > 0) {
            std::println("  {} avg: {:.1f} ({} students)",
                         subj, static_cast<double>(sum) / cnt, cnt);
        }
    }


    // === EXAMPLE 8: Event log by timestamp ===
    banner("Example 8: Event log by timestamp");

    std::multimap<long long, std::string> eventLog;

    // Multiple events at the same millisecond timestamp
    eventLog.insert({1000, "FILE_OPENED"});
    eventLog.insert({1000, "READ_STARTED"});    // same timestamp
    eventLog.insert({1050, "PARSE_DONE"});
    eventLog.insert({1050, "CACHE_UPDATED"});   // same timestamp
    eventLog.insert({2000, "FILE_CLOSED"});

    std::println("Event log:");
    for (const auto& [ts, event] : eventLog) {
        std::println("  [{}ms] {}", ts, event);
    }

    std::println("");
    std::println("=== MultimapSTL complete ===");
    return 0;
}
