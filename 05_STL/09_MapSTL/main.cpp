#include <cctype>
#include <map>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

// ============================================================
// std::map — Sorted Key-Value Store in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  insert, emplace, operator[], erase
//   3.  find, count, contains, at
//   4.  Iterators — always sorted by key
//   5.  lower_bound, upper_bound
//   6.  Custom key comparator
//   7.  std::unordered_map — O(1) average
//   8.  Practical: word frequency counter
//   9.  Practical: config store
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

    std::map<std::string, int> empty;
    std::map<std::string, int> scores = {
        {"Alice",  95},
        {"Bob",    82},
        {"Charlie",78},
        {"Diana",  91}
    };

    std::println("empty size: {}", empty.size());
    std::println("scores (sorted by key):");
    for (const auto& [name, score] : scores) {
        std::println("  {}: {}", name, score);
    }


    // === EXAMPLE 2: Insert, emplace, operator[], erase ===
    banner("Example 2: Insert, emplace, operator[], erase");

    std::map<std::string, int> m;

    // operator[] — inserts if key doesn't exist (default value = 0)
    m["alpha"] = 1;
    m["beta"]  = 2;
    m["gamma"] = 3;
    printMap("after [] inserts: ", m);

    // operator[] on missing key creates it with default value!
    m["delta"];   // creates "delta" with value 0
    std::println("m[\"delta\"] (missing key): {} (created with 0)", m["delta"]);

    // insert — safe, does NOT overwrite existing
    m.insert({"epsilon", 5});
    auto [it2, ok2] = m.insert({"alpha", 99});  // alpha already exists!
    std::println("insert(alpha=99): inserted={} (existing unchanged)", ok2);
    std::println("alpha is still: {}", m["alpha"]);

    // insert_or_assign — always overwrites (C++17)
    m.insert_or_assign("alpha", 99);
    std::println("insert_or_assign(alpha=99): alpha={}", m["alpha"]);

    // emplace — construct in place
    m.emplace("zeta", 6);

    // try_emplace — only inserts if key not present (C++17)
    m.try_emplace("alpha", 999);   // alpha exists — does nothing
    m.try_emplace("eta",   7);     // new key — inserts
    std::println("after try_emplace: alpha={} eta={}", m["alpha"], m["eta"]);

    // erase by key
    m.erase("delta");
    std::println("after erase(delta) size: {}", m.size());

    // erase by iterator
    auto itErase = m.find("epsilon");
    if (itErase != m.end()) {
        m.erase(itErase);
    }
    std::println("after erase(it->epsilon) size: {}", m.size());


    // === EXAMPLE 3: find, count, contains, at ===
    banner("Example 3: find, count, contains, at");

    std::map<std::string, int> phonebook = {
        {"Alice",   555001},
        {"Bob",     555002},
        {"Charlie", 555003}
    };

    // find — returns iterator or end()
    auto found = phonebook.find("Bob");
    if (found != phonebook.end()) {
        std::println("find(Bob): {}", found->second);
    }

    // contains — C++20
    std::println("contains(Alice):  {}", phonebook.contains("Alice"));
    std::println("contains(nobody): {}", phonebook.contains("nobody"));

    // at — bounds checked, throws if key missing
    std::println("at(Charlie): {}", phonebook.at("Charlie"));
    try {
        int val = phonebook.at("nobody");
        (void)val;
    } catch (const std::out_of_range& e) {
        std::println("at(nobody) threw: {}", e.what());
    }

    // count — 0 or 1 for map
    std::println("count(Alice): {}", phonebook.count("Alice"));


    // === EXAMPLE 4: Iterators — sorted by key ===
    banner("Example 4: Iterators — sorted by key");

    std::map<int, std::string> m5 = {{3,"c"},{1,"a"},{4,"d"},{2,"b"}};

    std::print("forward (sorted by key): ");
    for (const auto& [k, v] : m5) {
        std::print("{}:{} ", k, v);
    }
    std::println("");

    std::print("reverse:                 ");
    for (auto it = m5.rbegin(); it != m5.rend(); ++it) {
        std::print("{}:{} ", it->first, it->second);
    }
    std::println("");

    // Structured bindings (C++17)
    std::println("structured bindings:");
    for (const auto& [key, value] : m5) {
        std::println("  key={} value={}", key, value);
    }


    // === EXAMPLE 5: lower_bound, upper_bound ===
    banner("Example 5: lower_bound, upper_bound");

    std::map<int, std::string> grades = {
        {50, "F"}, {60, "D"}, {70, "C"}, {80, "B"}, {90, "A"}
    };

    // lower_bound — first key >= value
    auto lb = grades.lower_bound(65);
    std::println("lower_bound(65): key={} val={}", lb->first, lb->second);

    // upper_bound — first key > value
    auto ub = grades.upper_bound(70);
    std::println("upper_bound(70): key={} val={}", ub->first, ub->second);

    // Range [60, 80)
    std::print("grades in [60, 80): ");
    for (auto it = grades.lower_bound(60); it != grades.lower_bound(80); ++it) {
        std::print("{}:{} ", it->first, it->second);
    }
    std::println("");


    // === EXAMPLE 6: Custom Key Comparator ===
    banner("Example 6: Custom Key Comparator");

    // Case-insensitive string key
    auto lower = [](std::string s) {
        for (auto& c : s) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return s;
    };
    auto caseInsensitive = [&lower](const std::string& lhs, const std::string& rhs) {
        return lower(lhs) < lower(rhs);
    };

    std::map<std::string, int, decltype(caseInsensitive)>
        ciMap(caseInsensitive);
    ciMap["Alice"] = 1;
    ciMap["BOB"]   = 2;
    ciMap["charlie"] = 3;
    ciMap["ALICE"] = 10;   // same key as "Alice" -> overwrites the value

    std::println("case-insensitive map:");
    for (const auto& [k, v] : ciMap) {
        std::println("  {}: {}", k, v);
    }


    // === EXAMPLE 7: std::unordered_map — O(1) average ===
    banner("Example 7: std::unordered_map — O(1) average");

    std::unordered_map<std::string, int> um = {
        {"one", 1}, {"two", 2}, {"three", 3}
    };

    um["four"] = 4;
    um.insert({"five", 5});

    std::println("unordered_map (order not guaranteed):");
    for (const auto& [k, v] : um) {
        std::println("  {}: {}", k, v);
    }

    std::println("contains(two):  {}", um.contains("two"));
    std::println("contains(nine): {}", um.contains("nine"));


    // === EXAMPLE 8: Word Frequency Counter ===
    banner("Example 8: Word Frequency Counter");

    std::string text = "the quick brown fox jumps over the lazy dog the fox";
    std::map<std::string, int> freq;

    std::string word;
    for (char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                freq[word]++;   // operator[] creates with 0, then increments
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        freq[word]++;
    }

    std::println("word frequencies (sorted):");
    for (const auto& [w, count] : freq) {
        std::println("  '{}': {}", w, count);
    }


    // === EXAMPLE 9: Config Store ===
    banner("Example 9: Config Store");

    std::map<std::string, std::string> config = {
        {"server.host",    "192.168.1.100"},
        {"server.port",    "8080"},
        {"db.name",        "inventory"},
        {"db.timeout",     "30"},
        {"log.level",      "INFO"},
        {"log.file",       "/var/log/app.log"}
    };

    std::println("Configuration:");
    for (const auto& [key, value] : config) {
        std::println("  {} = {}", key, value);
    }

    // Safe access with default value
    auto getConfig = [&](const std::string& key,
                         const std::string& defaultVal) -> std::string {
        auto it = config.find(key);
        return (it != config.end()) ? it->second : defaultVal;
    };

    std::println("");
    std::println("server.host:  {}", getConfig("server.host",  "localhost"));
    std::println("server.proto: {}", getConfig("server.proto", "HTTP"));

    std::println("");
    std::println("=== MapSTL complete ===");
    return 0;
}
