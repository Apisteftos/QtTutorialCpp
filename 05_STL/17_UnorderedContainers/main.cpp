#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <print>
#include <sstream>
#include <algorithm>

// ============================================================
// UNORDERED CONTAINERS — C++23
// ============================================================
// Topics covered:
//   1.  unordered_map  — key-value, O(1) average lookup
//   2.  unordered_set  — unique values, O(1) average lookup
//   3.  unordered_multimap — multiple values per key
//   4.  unordered_multiset — duplicate values allowed
//   5.  Custom hash function
//   6.  Performance — ordered vs unordered comparison
//   7.  Bucket inspection
//   8.  Real use case — word frequency counter
//   9.  Real use case — session registry (MCX)
//  10.  When to use ordered vs unordered
// ============================================================


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: unordered_map — key-value O(1) lookup\n";
    std::cout << "=====================================================\n";

    std::unordered_map<std::string, int> scores;

    // Insert
    scores["Alice"]  = 95;
    scores["Bob"]    = 87;
    scores["Kostas"] = 92;
    scores["Maria"]  = 88;
    scores.insert({"Andre", 75});
    scores.emplace("Sara", 91);

    // Access
    std::println("Alice:  {}", scores["Alice"]);
    std::println("Kostas: {}", scores.at("Kostas"));
    std::println("Bob:    {}", scores.contains("Bob") ? scores.at("Bob") : 0);    // C++23!

    // Check existence
    std::println("\ncontains Alice: {}", scores.contains("Alice"));
    std::println("contains Zara:  {}", scores.contains("Zara"));

    // find()
    if (auto it = scores.find("Maria"); it != scores.end())
        std::println("found Maria: {}", it->second);

    // Iterate — order NOT guaranteed
    std::cout << "\nAll scores (unordered):\n";
    for (const auto& [name, score] : scores)
        std::println("  {} → {}", name, score);

    // Modify
    scores["Bob"] += 5;
    std::println("\nBob after +5: {}", scores["Bob"]);

    // Erase
    scores.erase("Andre");
    std::println("size after erase: {}", scores.size());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: unordered_set — unique values O(1) lookup\n";
    std::cout << "=====================================================\n";

    std::unordered_set<std::string> groups;

    groups.insert("GRP-ALPHA");
    groups.insert("GRP-BETA");
    groups.insert("GRP-GAMMA");
    groups.insert("GRP-ALPHA");   // duplicate — ignored

    std::println("size (no duplicates): {}", groups.size());

    std::cout << "groups: ";
    for (const auto& g : groups) std::print("{} ", g);
    std::cout << '\n';

    // Set operations
    std::println("\ncontains GRP-ALPHA: {}", groups.contains("GRP-ALPHA"));
    std::println("contains GRP-DELTA: {}", groups.contains("GRP-DELTA"));

    // Erase
    groups.erase("GRP-BETA");
    std::println("after erase GRP-BETA, size: {}", groups.size());

    // Build from vector (deduplicate)
    std::vector<int> nums = {3,1,4,1,5,9,2,6,5,3,5};
    std::unordered_set<int> unique(nums.begin(), nums.end());
    std::cout << "\ndeduped nums: ";
    for (int n : unique) std::print("{} ", n);
    std::cout << '\n';
    std::println("original size: {}  unique size: {}", nums.size(), unique.size());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: unordered_multimap — multiple values per key\n";
    std::cout << "=====================================================\n";

    // Multiple values for same key — unlike unordered_map
    std::unordered_multimap<std::string, std::string> contacts;

    contacts.insert({"Kostas", "kostas@work.com"});
    contacts.insert({"Kostas", "kostas@personal.com"});
    contacts.insert({"Kostas", "+49-123-456"});
    contacts.insert({"Maria",  "maria@work.com"});
    contacts.insert({"Maria",  "+49-789-012"});

    std::println("total entries: {}", contacts.size());

    // Get all entries for a key
    std::cout << "\nAll contacts for Kostas:\n";
    auto [first, last] = contacts.equal_range("Kostas");
    for (auto it = first; it != last; ++it)
        std::println("  {}", it->second);

    std::println("count for Kostas: {}", contacts.count("Kostas"));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: unordered_multiset — duplicates allowed\n";
    std::cout << "=====================================================\n";

    std::unordered_multiset<int> bag = {1, 2, 2, 3, 3, 3, 4};

    std::println("bag size: {}", bag.size());
    std::println("count(3): {}", bag.count(3));
    std::println("count(2): {}", bag.count(2));

    bag.insert(2);
    std::println("after insert(2), count(2): {}", bag.count(2));

    // Erase ONE occurrence
    bag.erase(bag.find(3));
    std::println("after erase one 3, count(3): {}", bag.count(3));

    // Erase ALL occurrences
    bag.erase(3);
    std::println("after erase all 3, count(3): {}", bag.count(3));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Custom hash function\n";
    std::cout << "=====================================================\n";

    // Custom hash for a struct
    struct Point {
        int x, y;
        bool operator==(const Point& o) const {
            return x == o.x && y == o.y;
        }
    };

    struct PointHash {
        std::size_t operator()(const Point& p) const {
            // Combine hashes of x and y
            std::size_t hx = std::hash<int>{}(p.x);
            std::size_t hy = std::hash<int>{}(p.y);
            return hx ^ (hy << 1);   // XOR shift combine
        }
    };

    std::unordered_map<Point, std::string, PointHash> grid;
    grid[{0, 0}] = "origin";
    grid[{1, 0}] = "right";
    grid[{0, 1}] = "up";
    grid[{1, 1}] = "diagonal";

    std::println("grid size: {}", grid.size());
    for (const auto& [pt, label] : grid)
        std::println("  ({},{}) → {}", pt.x, pt.y, label);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Bucket inspection\n";
    std::cout << "=====================================================\n";

    std::unordered_map<std::string, int> umap = {
        {"a",1},{"b",2},{"c",3},{"d",4},{"e",5}
    };

    std::println("size:           {}", umap.size());
    std::println("bucket_count:   {}", umap.bucket_count());
    std::println("load_factor:    {:.2f}", umap.load_factor());
    std::println("max_load_factor:{:.2f}", umap.max_load_factor());

    // Which bucket does "a" live in?
    std::println("bucket for 'a': {}", umap.bucket("a"));
    std::println("bucket for 'b': {}", umap.bucket("b"));

    // Reserve buckets to avoid rehashing
    umap.reserve(100);
    std::println("after reserve(100), bucket_count: {}", umap.bucket_count());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Performance — ordered vs unordered\n";
    std::cout << "=====================================================\n";

    std::cout << "std::map (ordered):\n";
    std::cout << "  Lookup:  O(log n) — binary search in red-black tree\n";
    std::cout << "  Insert:  O(log n)\n";
    std::cout << "  Order:   keys always sorted\n";
    std::cout << "  Memory:  more — tree node per element\n\n";

    std::cout << "std::unordered_map:\n";
    std::cout << "  Lookup:  O(1) average — hash table\n";
    std::cout << "  Insert:  O(1) average\n";
    std::cout << "  Order:   NOT guaranteed\n";
    std::cout << "  Memory:  less — contiguous buckets\n\n";

    std::cout << "Worst case unordered: O(n) — all keys hash to same bucket\n";
    std::cout << "Use ordered when: you need sorted iteration or range queries\n";
    std::cout << "Use unordered when: you only need fast lookup by key\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Word frequency counter\n";
    std::cout << "=====================================================\n";

    std::string text = "the quick brown fox jumps over the lazy dog "
                       "the fox was quick and the dog was lazy";

    std::unordered_map<std::string, int> freq;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word)
        freq[word]++;

    // Sort by frequency for display
    std::vector<std::pair<std::string,int>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    std::cout << "Word frequencies (top 5):\n";
    for (int i = 0; i < std::min(5, (int)sorted.size()); ++i)
        std::println("  {:10} → {}", sorted[i].first, sorted[i].second);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Session registry — MCX use case\n";
    std::cout << "=====================================================\n";

    struct Session {
        std::string groupId;
        std::string host;
        int         port;
        bool        active;
    };

    std::unordered_map<std::string, Session> registry;

    // Register sessions
    registry["SES-001"] = {"GRP-ALPHA", "192.168.1.100", 5060, true};
    registry["SES-002"] = {"GRP-BETA",  "192.168.1.101", 5060, true};
    registry["SES-003"] = {"GRP-ALPHA", "192.168.1.102", 5061, false};

    // O(1) lookup by session ID
    if (registry.contains("SES-001")) {
        const auto& s = registry.at("SES-001");
        std::println("SES-001: group={} host={} active={}",
                     s.groupId, s.host, s.active);
    }

    // Find all active sessions
    std::cout << "\nActive sessions:\n";
    for (const auto& [id, s] : registry)
        if (s.active)
            std::println("  {} → {} ({}:{})", id, s.groupId, s.host, s.port);

    // Deregister
    registry.erase("SES-003");
    std::println("registry size after erase: {}", registry.size());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: When to use ordered vs unordered\n";
    std::cout << "=====================================================\n";

    std::cout << "Use std::map / std::set when:\n";
    std::cout << "  ✅ You need sorted iteration\n";
    std::cout << "  ✅ You need range queries (lower_bound, upper_bound)\n";
    std::cout << "  ✅ Keys don't have a good hash function\n";
    std::cout << "  ✅ Worst-case O(log n) is required\n\n";

    std::cout << "Use std::unordered_map / std::unordered_set when:\n";
    std::cout << "  ✅ You only need fast lookup, insert, delete\n";
    std::cout << "  ✅ Key order doesn't matter\n";
    std::cout << "  ✅ Keys have good hash (string, int)\n";
    std::cout << "  ✅ Average O(1) performance matters\n\n";

    std::cout << "Use multimap / multiset when:\n";
    std::cout << "  ✅ Multiple values per key needed\n";
    std::cout << "  ✅ Duplicate values in set needed\n";

    std::cout << "\n=== Unordered Containers complete ===\n";
    return 0;
}
