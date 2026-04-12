#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include <string>
#include <print>

// ============================================================
// std::set — Sorted Unique Elements in C++23
// ============================================================
// Topics covered:
//   1.  What is a set
//   2.  Declaration and initialization
//   3.  insert, emplace, erase, clear
//   4.  find, count, contains (C++20)
//   5.  Iterators (always sorted)
//   6.  lower_bound, upper_bound, equal_range
//   7.  Custom comparator
//   8.  std::multiset (allows duplicates)
//   9.  std::unordered_set (hash-based, O(1) avg)
//  10.  Practical: removing duplicates, set operations
// ============================================================

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What is a Set\n";
    std::cout << "=====================================================\n";

    std::cout << "std::set properties:\n";
    std::cout << "  - All elements are UNIQUE (duplicates ignored)\n";
    std::cout << "  - Elements always SORTED (ascending by default)\n";
    std::cout << "  - Backed by a balanced BST (red-black tree)\n";
    std::cout << "  - insert/erase/find: O(log n)\n";
    std::cout << "  - No random access (no operator[])\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    std::set<int> empty;
    std::set<int> fromList = {5, 3, 8, 1, 9, 2, 3, 5};  // duplicates removed!
    std::set<int> copy = fromList;
    std::set<int> fromVec;
    std::vector<int> v = {4, 2, 7, 1, 4, 2};
    fromVec.insert(v.begin(), v.end());

    std::cout << "fromList {5,3,8,1,9,2,3,5}: ";
    for (int x : fromList) std::cout << x << ' ';
    std::cout << "(sorted, no duplicates)\n";

    std::cout << "fromVec  {4,2,7,1,4,2}:     ";
    for (int x : fromVec) std::cout << x << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: insert, emplace, erase, clear\n";
    std::cout << "=====================================================\n";

    std::set<int> s;

    // insert returns {iterator, bool} — bool=false if already exists
    auto [it1, inserted1] = s.insert(10);
    auto [it2, inserted2] = s.insert(20);
    auto [it3, inserted3] = s.insert(10);   // duplicate!

    std::cout << "insert(10): inserted=" << inserted1 << '\n';
    std::cout << "insert(20): inserted=" << inserted2 << '\n';
    std::cout << "insert(10): inserted=" << inserted3 << " (duplicate ignored)\n";

    s.insert({30, 40, 50, 15, 25});   // insert multiple
    std::cout << "after inserts: ";
    for (int x : s) std::cout << x << ' '; std::cout << '\n';

    s.emplace(5);   // construct in place (same as insert for int)
    std::cout << "after emplace(5): ";
    for (int x : s) std::cout << x << ' '; std::cout << '\n';

    // erase by value
    s.erase(15);
    std::cout << "after erase(15): ";
    for (int x : s) std::cout << x << ' '; std::cout << '\n';

    // erase by iterator
    auto itErase = s.find(25);
    if (itErase != s.end()) s.erase(itErase);
    std::cout << "after erase(it->25): ";
    for (int x : s) std::cout << x << ' '; std::cout << '\n';

    // erase range
    auto first = s.find(20);
    auto last  = s.find(40);
    if (first != s.end() && last != s.end())
        s.erase(first, last);   // erases [20, 40)
    std::cout << "after erase range [20,40): ";
    for (int x : s) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: find, count, contains\n";
    std::cout << "=====================================================\n";

    std::set<int> nums = {10, 20, 30, 40, 50};

    // find — returns iterator or end()
    auto found = nums.find(30);
    if (found != nums.end())
        std::println("find(30): found = {}", *found);
    else
        std::cout << "find(30): not found\n";

    auto notFound = nums.find(99);
    std::cout << "find(99): " << (notFound == nums.end() ? "not found" : "found") << '\n';

    // count — returns 0 or 1 (set has unique elements)
    std::cout << "count(30): " << nums.count(30) << '\n';
    std::cout << "count(99): " << nums.count(99) << '\n';

    // contains — C++20, cleaner than count()
    std::cout << "contains(30): " << nums.contains(30) << '\n';
    std::cout << "contains(99): " << nums.contains(99) << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Iterators — always in sorted order\n";
    std::cout << "=====================================================\n";

    std::set<std::string> words = {"banana", "apple", "cherry", "date", "avocado"};

    std::cout << "forward (sorted): ";
    for (const auto& w : words) std::cout << w << ' '; std::cout << '\n';

    std::cout << "reverse:          ";
    for (auto it = words.rbegin(); it != words.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: lower_bound, upper_bound, equal_range\n";
    std::cout << "=====================================================\n";

    std::set<int> rangeSet = {10, 20, 30, 40, 50, 60};

    // lower_bound — first element >= value
    auto lb = rangeSet.lower_bound(25);
    std::println("lower_bound(25): {}", *lb);   // 30

    // upper_bound — first element > value
    auto ub = rangeSet.upper_bound(30);
    std::println("upper_bound(30): {}", *ub);   // 40

    // equal_range — returns [lower, upper) pair
    auto [lo, hi] = rangeSet.equal_range(30);
    std::cout << "equal_range(30): [";
    for (auto it = lo; it != hi; ++it) std::cout << *it;
    std::cout << "]\n";

    // Count elements in range [20, 50)
    auto start = rangeSet.lower_bound(20);
    auto stop  = rangeSet.lower_bound(50);
    std::cout << "elements in [20,50): ";
    for (auto it = start; it != stop; ++it) std::cout << *it << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Custom Comparator\n";
    std::cout << "=====================================================\n";

    // Sort strings by length, then alphabetically
    auto byLength = [](const std::string& a, const std::string& b) {
        if (a.size() != b.size()) return a.size() < b.size();
        return a < b;
    };

    std::set<std::string, decltype(byLength)> byLen(byLength);
    byLen.insert({"banana", "cat", "apple", "fig", "kiwi", "date"});

    std::cout << "sorted by length: ";
    for (const auto& w : byLen) std::cout << w << ' '; std::cout << '\n';

    // Descending order
    std::set<int, std::greater<int>> descSet = {5, 3, 8, 1, 9, 2};
    std::cout << "descending:       ";
    for (int x : descSet) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: std::multiset — allows duplicates\n";
    std::cout << "=====================================================\n";

    std::multiset<int> ms = {5, 3, 8, 3, 1, 5, 5, 2};
    std::cout << "multiset {5,3,8,3,1,5,5,2}: ";
    for (int x : ms) std::cout << x << ' '; std::cout << '\n';

    std::cout << "count(5): " << ms.count(5) << '\n';   // 3

    // erase ALL occurrences of a value
    ms.erase(5);
    std::cout << "after erase(5): ";
    for (int x : ms) std::cout << x << ' '; std::cout << '\n';

    // erase only ONE occurrence
    auto itMs = ms.find(3);
    if (itMs != ms.end()) ms.erase(itMs);   // erase single element by iterator
    std::cout << "after erase one 3: ";
    for (int x : ms) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: std::unordered_set — O(1) average\n";
    std::cout << "=====================================================\n";

    std::unordered_set<int> us = {5, 3, 8, 1, 9, 2};
    std::cout << "unordered_set: ";
    for (int x : us) std::cout << x << ' ';
    std::cout << "(order not guaranteed)\n";

    us.insert(42);
    std::cout << "contains(42): " << us.contains(42) << '\n';
    std::cout << "contains(99): " << us.contains(99) << '\n';

    us.erase(3);
    std::cout << "after erase(3): ";
    for (int x : us) std::cout << x << ' '; std::cout << '\n';

    std::cout << "\nset vs unordered_set:\n";
    std::cout << "  std::set          — sorted, O(log n), ordered iteration\n";
    std::cout << "  std::unordered_set — unsorted, O(1) avg, hash-based\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Practical — remove duplicates, set ops\n";
    std::cout << "=====================================================\n";

    // Remove duplicates from vector
    std::vector<int> withDupes = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::set<int> unique(withDupes.begin(), withDupes.end());
    std::cout << "original: ";
    for (int x : withDupes) std::cout << x << ' '; std::cout << '\n';
    std::cout << "unique:   ";
    for (int x : unique) std::cout << x << ' '; std::cout << '\n';

    // Set intersection
    std::set<int> setA = {1, 2, 3, 4, 5};
    std::set<int> setB = {3, 4, 5, 6, 7};
    std::cout << "A: "; for (int x : setA) std::cout << x << ' '; std::cout << '\n';
    std::cout << "B: "; for (int x : setB) std::cout << x << ' '; std::cout << '\n';

    std::set<int> intersection, unionSet, difference;
    for (int x : setA) if (setB.contains(x))  intersection.insert(x);
    for (int x : setA) unionSet.insert(x);
    for (int x : setB) unionSet.insert(x);
    for (int x : setA) if (!setB.contains(x)) difference.insert(x);

    std::cout << "A∩B: "; for (int x : intersection) std::cout << x << ' '; std::cout << '\n';
    std::cout << "A∪B: "; for (int x : unionSet)      std::cout << x << ' '; std::cout << '\n';
    std::cout << "A\\B: "; for (int x : difference)    std::cout << x << ' '; std::cout << '\n';

    std::cout << "\n=== SetSTL complete ===\n";
    return 0;
}
