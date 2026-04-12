#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <print>

// ============================================================
// std::map — Sorted Key-Value Store in C++23
// ============================================================
// Topics covered:
//   1.  What is a map
//   2.  Declaration and initialization
//   3.  insert, emplace, operator[], erase
//   4.  find, count, contains, at
//   5.  Iterators — always sorted by key
//   6.  lower_bound, upper_bound
//   7.  Custom key comparator
//   8.  std::unordered_map — O(1) average
//   9.  Practical: word frequency counter
//  10.  Practical: phone book, config store
// ============================================================

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What is a Map\n";
    std::cout << "=====================================================\n";

    std::cout << "std::map properties:\n";
    std::cout << "  - Stores KEY-VALUE pairs\n";
    std::cout << "  - Keys are UNIQUE\n";
    std::cout << "  - Always SORTED by key (ascending)\n";
    std::cout << "  - Backed by a balanced BST (red-black tree)\n";
    std::cout << "  - insert/find/erase: O(log n)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    std::map<std::string, int> empty;
    std::map<std::string, int> scores = {
        {"Alice",  95},
        {"Bob",    82},
        {"Charlie",78},
        {"Diana",  91}
    };

    std::cout << "scores (sorted by key):\n";
    for (const auto& [name, score] : scores)
        std::println("  {}: {}", name, score);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Insert, emplace, operator[], erase\n";
    std::cout << "=====================================================\n";

    std::map<std::string, int> m;

    // operator[] — inserts if key doesn't exist (default value = 0)
    m["alpha"] = 1;
    m["beta"]  = 2;
    m["gamma"] = 3;
    std::cout << "after [] inserts: ";
    for (const auto& [k, v] : m) std::cout << k << "=" << v << " "; std::cout << '\n';

    // operator[] on missing key creates it with default value!
    m["delta"];   // creates "delta" with value 0
    std::cout << "m[\"delta\"] (missing key): " << m["delta"] << " (created with 0)\n";

    // insert — safe, does NOT overwrite existing
    auto [it1, ok1] = m.insert({"epsilon", 5});
    auto [it2, ok2] = m.insert({"alpha",   99});  // alpha already exists!
    std::cout << "insert(alpha=99): inserted=" << ok2 << " (existing unchanged)\n";
    std::cout << "alpha is still: " << m["alpha"] << '\n';

    // insert_or_assign — always overwrites (C++17)
    m.insert_or_assign("alpha", 99);
    std::cout << "insert_or_assign(alpha=99): alpha=" << m["alpha"] << '\n';

    // emplace — construct in place
    m.emplace("zeta", 6);

    // try_emplace — only inserts if key not present (C++17)
    m.try_emplace("alpha", 999);   // alpha exists — does nothing
    m.try_emplace("eta",   7);     // new key — inserts
    std::cout << "after try_emplace: alpha=" << m["alpha"]
              << " eta=" << m["eta"] << '\n';

    // erase by key
    m.erase("delta");
    std::cout << "after erase(delta) size: " << m.size() << '\n';

    // erase by iterator
    auto itErase = m.find("epsilon");
    if (itErase != m.end()) m.erase(itErase);
    std::cout << "after erase(it->epsilon) size: " << m.size() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: find, count, contains, at\n";
    std::cout << "=====================================================\n";

    std::map<std::string, int> phonebook = {
        {"Alice",   555001},
        {"Bob",     555002},
        {"Charlie", 555003}
    };

    // find — returns iterator or end()
    auto found = phonebook.find("Bob");
    if (found != phonebook.end())
        std::println("find(Bob): {}", found->second);

    // contains — C++20
    std::cout << "contains(Alice):  " << phonebook.contains("Alice")  << '\n';
    std::cout << "contains(nobody): " << phonebook.contains("nobody") << '\n';

    // at — bounds checked, throws if key missing
    std::cout << "at(Charlie): " << phonebook.at("Charlie") << '\n';
    try {
        int val = phonebook.at("nobody");
        (void)val;
    } catch (const std::out_of_range& e) {
        std::cout << "at(nobody) threw: " << e.what() << '\n';
    }

    // count — 0 or 1 for map
    std::cout << "count(Alice): " << phonebook.count("Alice") << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Iterators — sorted by key\n";
    std::cout << "=====================================================\n";

    std::map<int, std::string> m5 = {{3,"c"},{1,"a"},{4,"d"},{2,"b"}};

    std::cout << "forward (sorted by key): ";
    for (const auto& [k, v] : m5)
        std::cout << k << ":" << v << " ";
    std::cout << '\n';

    std::cout << "reverse:                 ";
    for (auto it = m5.rbegin(); it != m5.rend(); ++it)
        std::cout << it->first << ":" << it->second << " ";
    std::cout << '\n';

    // Structured bindings (C++17)
    std::cout << "structured bindings:\n";
    for (const auto& [key, value] : m5)
        std::println("  key={} value={}", key, value);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: lower_bound, upper_bound\n";
    std::cout << "=====================================================\n";

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
    std::cout << "grades in [60, 80): ";
    for (auto it = grades.lower_bound(60); it != grades.lower_bound(80); ++it)
        std::cout << it->first << ":" << it->second << " ";
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Custom Key Comparator\n";
    std::cout << "=====================================================\n";

    // Case-insensitive string key
    auto caseInsensitive = [](const std::string& a, const std::string& b) {
        std::string la = a, lb = b;
        for (auto& c : la) c = std::tolower(c);
        for (auto& c : lb) c = std::tolower(c);
        return la < lb;
    };

    std::map<std::string, int, decltype(caseInsensitive)>
        ciMap(caseInsensitive);
    ciMap["Alice"] = 1;
    ciMap["BOB"]   = 2;
    ciMap["charlie"] = 3;

    std::cout << "case-insensitive map:\n";
    for (const auto& [k, v] : ciMap)
        std::println("  {}: {}", k, v);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: std::unordered_map — O(1) average\n";
    std::cout << "=====================================================\n";

    std::unordered_map<std::string, int> um = {
        {"one", 1}, {"two", 2}, {"three", 3}
    };

    um["four"] = 4;
    um.insert({"five", 5});

    std::cout << "unordered_map (order not guaranteed):\n";
    for (const auto& [k, v] : um)
        std::println("  {}: {}", k, v);

    std::cout << "contains(two):  " << um.contains("two")    << '\n';
    std::cout << "contains(nine): " << um.contains("nine")   << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Word Frequency Counter\n";
    std::cout << "=====================================================\n";

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
    if (!word.empty()) freq[word]++;

    std::cout << "word frequencies (sorted):\n";
    for (const auto& [w, count] : freq)
        std::println("  '{}': {}", w, count);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Config Store\n";
    std::cout << "=====================================================\n";

    std::map<std::string, std::string> config = {
        {"server.host",    "192.168.1.100"},
        {"server.port",    "5060"},
        {"mcx.group_id",   "42"},
        {"mcx.codec",      "AMR-NB"},
        {"log.level",      "INFO"},
        {"log.file",       "/var/log/mcx.log"}
    };

    std::cout << "Configuration:\n";
    for (const auto& [key, value] : config)
        std::println("  {} = {}", key, value);

    // Safe access with default value
    auto getConfig = [&](const std::string& key,
                         const std::string& defaultVal) -> std::string {
        auto it = config.find(key);
        return (it != config.end()) ? it->second : defaultVal;
    };

    std::cout << "\nserver.host:  " << getConfig("server.host",  "localhost") << '\n';
    std::cout << "server.proto: " << getConfig("server.proto", "SIP")       << '\n';

    std::cout << "\n=== MapSTL complete ===\n";
    return 0;
}
