#include <functional>
#include <iterator>
#include <print>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

// ============================================================
// std::set — Sorted Unique Elements in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  insert, emplace, erase, clear
//   3.  find, count, contains (C++20)
//   4.  Iterators (always sorted)
//   5.  lower_bound, upper_bound, equal_range
//   6.  Custom comparator
//   7.  std::multiset (allows duplicates)
//   8.  std::unordered_set (hash-based, O(1) avg)
//   9.  Practical: removing duplicates, set operations
// ============================================================

// Print a label followed by all elements of a range
template<typename Range>
void printAll(std::string_view label, const Range& r)
{
    std::print("{}", label);
    for (const auto& x : r) {
        std::print("{} ", x);
    }
    std::println("");
}

// Print the "=====" banner that introduces an example
void banner(std::string_view title)
{
    std::println("");
    std::println("=====================================================");
    std::println("{}", title);
    std::println("=====================================================");
}

int main()
{
    // === EXAMPLE 1: Declaration and Initialization ===
    banner("Example 1: Declaration and Initialization");

    std::set<int> empty;
    std::set<int> fromList = {5, 3, 8, 1, 9, 2, 3, 5};  // duplicates removed!
    std::set<int> copy = fromList;
    std::set<int> fromVec;
    std::vector<int> v = {4, 2, 7, 1, 4, 2};
    fromVec.insert(v.begin(), v.end());

    std::println("empty size: {}", empty.size());
    printAll("fromList {5,3,8,1,9,2,3,5}: ", fromList);
    printAll("copy of fromList:           ", copy);
    printAll("fromVec  {4,2,7,1,4,2}:     ", fromVec);


    // === EXAMPLE 2: insert, emplace, erase, clear ===
    banner("Example 2: insert, emplace, erase, clear");

    std::set<int> s;

    // insert returns {iterator, bool} — bool=false if already exists
    auto [it1, inserted1] = s.insert(10);
    auto [it2, inserted2] = s.insert(20);
    auto [it3, inserted3] = s.insert(10);   // duplicate!

    std::println("insert(10): inserted={}", inserted1);
    std::println("insert(20): inserted={}", inserted2);
    std::println("insert(10): inserted={} (duplicate ignored)", inserted3);

    s.insert({30, 40, 50, 15, 25});   // insert multiple
    printAll("after inserts: ", s);

    s.emplace(5);   // construct in place (same as insert for int)
    printAll("after emplace(5): ", s);

    // erase by value
    s.erase(15);
    printAll("after erase(15): ", s);

    // erase by iterator
    auto itErase = s.find(25);
    if (itErase != s.end()) {
        s.erase(itErase);
    }
    printAll("after erase(it->25): ", s);

    // erase range
    auto first = s.find(20);
    auto last  = s.find(40);
    if (first != s.end() && last != s.end()) {
        s.erase(first, last);   // erases [20, 40)
    }
    printAll("after erase range [20,40): ", s);


    // === EXAMPLE 3: find, count, contains ===
    banner("Example 3: find, count, contains");

    std::set<int> nums = {10, 20, 30, 40, 50};

    // find — returns iterator or end()
    auto found = nums.find(30);
    if (found != nums.end()) {
        std::println("find(30): found = {}", *found);
    } else {
        std::println("find(30): not found");
    }

    auto notFound = nums.find(99);
    std::println("find(99): {}", notFound == nums.end() ? "not found" : "found");

    // count — returns 0 or 1 (set has unique elements)
    std::println("count(30): {}", nums.count(30));
    std::println("count(99): {}", nums.count(99));

    // contains — C++20, cleaner than count()
    std::println("contains(30): {}", nums.contains(30));
    std::println("contains(99): {}", nums.contains(99));


    // === EXAMPLE 4: Iterators — always in sorted order ===
    banner("Example 4: Iterators — always in sorted order");

    std::set<std::string> words = {"banana", "apple", "cherry", "date", "avocado"};

    printAll("forward (sorted): ", words);

    std::print("reverse:          ");
    for (auto it = words.rbegin(); it != words.rend(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");


    // === EXAMPLE 5: lower_bound, upper_bound, equal_range ===
    banner("Example 5: lower_bound, upper_bound, equal_range");

    std::set<int> rangeSet = {10, 20, 30, 40, 50, 60};

    // lower_bound — first element >= value
    auto lb = rangeSet.lower_bound(25);
    std::println("lower_bound(25): {}", *lb);   // 30

    // upper_bound — first element > value
    auto ub = rangeSet.upper_bound(30);
    std::println("upper_bound(30): {}", *ub);   // 40

    // equal_range — returns [lower, upper) pair
    auto [lo, hi] = rangeSet.equal_range(30);
    std::print("equal_range(30): [");
    for (auto it = lo; it != hi; ++it) {
        std::print("{}", *it);
    }
    std::println("]");

    // Elements in range [20, 50)
    auto start = rangeSet.lower_bound(20);
    auto stop  = rangeSet.lower_bound(50);
    std::print("elements in [20,50): ");
    for (auto it = start; it != stop; ++it) {
        std::print("{} ", *it);
    }
    std::println("");


    // === EXAMPLE 6: Custom Comparator ===
    banner("Example 6: Custom Comparator");

    // Sort strings by length, then alphabetically.
    // The alphabetical tie-break is REQUIRED: two strings that compare
    // "equal" (same length) would otherwise count as duplicates.
    auto byLength = [](const std::string& lhs, const std::string& rhs) {
        if (lhs.size() != rhs.size()) return lhs.size() < rhs.size();
        return lhs < rhs;
    };

    std::set<std::string, decltype(byLength)> byLen(byLength);
    byLen.insert({"banana", "cat", "apple", "fig", "kiwi", "date"});

    printAll("sorted by length: ", byLen);

    // Descending order
    std::set<int, std::greater<int>> descSet = {5, 3, 8, 1, 9, 2};
    printAll("descending:       ", descSet);


    // === EXAMPLE 7: std::multiset — allows duplicates ===
    banner("Example 7: std::multiset — allows duplicates");

    std::multiset<int> ms = {5, 3, 8, 3, 1, 5, 5, 2};
    printAll("multiset {5,3,8,3,1,5,5,2}: ", ms);

    std::println("count(5): {}", ms.count(5));   // 3

    // erase ALL occurrences of a value
    ms.erase(5);
    printAll("after erase(5): ", ms);

    // erase only ONE occurrence
    auto itMs = ms.find(3);
    if (itMs != ms.end()) {
        ms.erase(itMs);   // erase single element by iterator
    }
    printAll("after erase one 3: ", ms);


    // === EXAMPLE 8: std::unordered_set — O(1) average ===
    banner("Example 8: std::unordered_set — O(1) average");

    std::unordered_set<int> us = {5, 3, 8, 1, 9, 2};
    printAll("unordered_set: ", us);
    std::println("(order not guaranteed)");

    us.insert(42);
    std::println("contains(42): {}", us.contains(42));
    std::println("contains(99): {}", us.contains(99));

    us.erase(3);
    printAll("after erase(3): ", us);


    // === EXAMPLE 9: Practical — remove duplicates, set ops ===
    banner("Example 9: Practical — remove duplicates, set ops");

    // Remove duplicates from vector
    std::vector<int> withDupes = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::set<int> unique(withDupes.begin(), withDupes.end());
    printAll("original: ", withDupes);
    printAll("unique:   ", unique);

    // Set operations
    std::set<int> setA = {1, 2, 3, 4, 5};
    std::set<int> setB = {3, 4, 5, 6, 7};
    printAll("A: ", setA);
    printAll("B: ", setB);

    std::set<int> intersection, unionSet, difference;
    for (int x : setA) {
        if (setB.contains(x)) intersection.insert(x);
    }
    for (int x : setA) unionSet.insert(x);
    for (int x : setB) unionSet.insert(x);
    for (int x : setA) {
        if (!setB.contains(x)) difference.insert(x);
    }

    printAll("A∩B: ", intersection);
    printAll("A∪B: ", unionSet);
    printAll("A\\B: ", difference);

    std::println("");
    std::println("=== SetSTL complete ===");
    return 0;
}
