#include <algorithm>
#include <cstddef>
#include <functional>
#include <print>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// ============================================================
// UNORDERED CONTAINERS — C++23
// ============================================================
// Topics covered:
//   1.  unordered_map  — key-value, O(1) average lookup
//   2.  unordered_set  — unique values, O(1) average lookup
//   3.  unordered_multimap — multiple values per key
//   4.  unordered_multiset — duplicate values allowed
//   5.  Custom hash function
//   6.  Bucket inspection
//   7.  Real use case — word frequency counter
//   8.  Real use case — device registry
// ============================================================

namespace
{
    void banner(std::string_view title)
    {
        std::println("=====================================================");
        std::println("{}", title);
        std::println("=====================================================");
    }
}

int main()
{
    // === EXAMPLE 1: unordered_map — key-value O(1) lookup ===
    banner("Example 1: unordered_map — key-value O(1) lookup");

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
    std::println("Bob:    {}", scores.contains("Bob") ? scores.at("Bob") : 0);    // contains(): C++20

    // Check existence
    std::println("");
    std::println("contains Alice: {}", scores.contains("Alice"));
    std::println("contains Zara:  {}", scores.contains("Zara"));

    // find()
    if (auto it = scores.find("Maria"); it != scores.end())
        std::println("found Maria: {}", it->second);

    // Iterate — order NOT guaranteed
    std::println("");
    std::println("All scores (unordered):");
    for (const auto& [name, score] : scores)
        std::println("  {} → {}", name, score);

    // Modify
    scores["Bob"] += 5;
    std::println("");
    std::println("Bob after +5: {}", scores["Bob"]);

    // Erase
    scores.erase("Andre");
    std::println("size after erase: {}", scores.size());
    std::println("");


    // === EXAMPLE 2: unordered_set — unique values O(1) lookup ===
    banner("Example 2: unordered_set — unique values O(1) lookup");

    std::unordered_set<std::string> tags;

    tags.insert("TAG-ALPHA");
    tags.insert("TAG-BETA");
    tags.insert("TAG-GAMMA");
    tags.insert("TAG-ALPHA");   // duplicate — ignored

    std::println("size (no duplicates): {}", tags.size());

    std::print("tags: ");
    for (const auto& t : tags) std::print("{} ", t);
    std::println("");

    // Membership
    std::println("");
    std::println("contains TAG-ALPHA: {}", tags.contains("TAG-ALPHA"));
    std::println("contains TAG-DELTA: {}", tags.contains("TAG-DELTA"));

    // Erase
    tags.erase("TAG-BETA");
    std::println("after erase TAG-BETA, size: {}", tags.size());

    // Build from vector (deduplicate)
    std::vector<int> nums = {3,1,4,1,5,9,2,6,5,3,5};
    std::unordered_set<int> unique(nums.begin(), nums.end());
    std::println("");
    std::print("deduped nums: ");
    for (int n : unique) std::print("{} ", n);
    std::println("");
    std::println("original size: {}  unique size: {}", nums.size(), unique.size());
    std::println("");


    // === EXAMPLE 3: unordered_multimap — multiple values per key ===
    banner("Example 3: unordered_multimap — multiple values per key");

    // Multiple values for same key — unlike unordered_map
    std::unordered_multimap<std::string, std::string> contacts;

    contacts.insert({"Kostas", "kostas@work.com"});
    contacts.insert({"Kostas", "kostas@personal.com"});
    contacts.insert({"Kostas", "+49-123-456"});
    contacts.insert({"Maria",  "maria@work.com"});
    contacts.insert({"Maria",  "+49-789-012"});

    std::println("total entries: {}", contacts.size());

    // Get all entries for a key
    std::println("");
    std::println("All contacts for Kostas:");
    auto [first, last] = contacts.equal_range("Kostas");
    for (auto it = first; it != last; ++it)
        std::println("  {}", it->second);

    std::println("count for Kostas: {}", contacts.count("Kostas"));
    std::println("");


    // === EXAMPLE 4: unordered_multiset — duplicates allowed ===
    banner("Example 4: unordered_multiset — duplicates allowed");

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
    std::println("");


    // === EXAMPLE 5: Custom hash function ===
    banner("Example 5: Custom hash function");

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
            return hx ^ (hy << 1);   // simple XOR-shift combine (works, but collides easily — see README)
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
    std::println("");


    // === EXAMPLE 6: Bucket inspection ===
    banner("Example 6: Bucket inspection");

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
    std::println("");


    // === EXAMPLE 7: Word frequency counter ===
    banner("Example 7: Word frequency counter");

    std::string text = "the quick brown fox jumps over the lazy dog "
                       "the fox was quick and the dog was lazy";

    std::unordered_map<std::string, int> freq;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word)
        freq[word]++;

    // Sort by frequency for display (ties: alphabetical, so the output is deterministic)
    std::vector<std::pair<std::string,int>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b){
                  if (a.second != b.second) return a.second > b.second;
                  return a.first < b.first;
              });

    std::println("Word frequencies (top 5):");
    for (int i = 0; i < std::min(5, static_cast<int>(sorted.size())); ++i)
        std::println("  {:10} → {}", sorted[i].first, sorted[i].second);
    std::println("");


    // === EXAMPLE 8: Device registry ===
    banner("Example 8: Device registry");

    struct Device {
        std::string zone;
        std::string host;
        int         port;
        bool        active;
    };

    std::unordered_map<std::string, Device> registry;

    // Register devices
    registry["DEV-001"] = {"ZONE-A", "192.168.1.100", 8080, true};
    registry["DEV-002"] = {"ZONE-B", "192.168.1.101", 8080, true};
    registry["DEV-003"] = {"ZONE-A", "192.168.1.102", 8081, false};

    // O(1) lookup by device ID
    if (registry.contains("DEV-001")) {
        const auto& d = registry.at("DEV-001");
        std::println("DEV-001: zone={} host={} active={}",
                     d.zone, d.host, d.active);
    }

    // Find all active devices
    std::println("");
    std::println("Active devices:");
    for (const auto& [id, d] : registry)
        if (d.active)
            std::println("  {} → {} ({}:{})", id, d.zone, d.host, d.port);

    // Deregister
    registry.erase("DEV-003");
    std::println("registry size after erase: {}", registry.size());

    std::println("");
    std::println("=== Unordered Containers complete ===");
    return 0;
}
