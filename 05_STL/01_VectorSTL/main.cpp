#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// ============================================================
// std::vector — Dynamic Array in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  Adding and removing elements
//   3.  Access: [], at(), front(), back()
//   4.  Iterators
//   5.  Size, capacity, reserve, shrink_to_fit
//   6.  Searching and sorting
//   7.  Modifying with algorithms
//   8.  2D vector (vector of vectors)
//   9.  Vector of objects
//  10.  C++23 features
// ============================================================

struct Player {
    std::string name;
    int         score;
    int         level;

    void print() const {
        std::println("  Player: {} | score: {} | level: {}", name, score, level);
    }
};

// Print a label followed by all elements of a vector
template<typename T>
void printVec(std::string_view label, const std::vector<T>& vec)
{
    std::print("{}", label);
    for (const auto& x : vec) {
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

    std::vector<int> empty;                          // empty vector
    std::vector<int> withSize(5);                    // 5 elements, all 0
    std::vector<int> withValue(5, 42);               // 5 elements, all 42
    std::vector<int> fromList = {10, 20, 30, 40, 50}; // initializer list
    std::vector<int> copy = fromList;                // copy
    std::vector<int> range(fromList.begin() + 1,
                           fromList.end() - 1);     // from iterators {20,30,40}

    std::println("empty size:      {}", empty.size());
    printVec("withSize:        ", withSize);
    printVec("withValue:       ", withValue);
    printVec("fromList:        ", fromList);
    printVec("range [1..-1]:   ", range);


    // === EXAMPLE 2: Adding and Removing Elements ===
    banner("Example 2: Adding and Removing Elements");

    std::vector<int> v;

    // Add to end
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    printVec("After push_back: ", v);

    // emplace_back — constructs in place (preferred over push_back for objects)
    v.emplace_back(40);
    v.emplace_back(50);
    printVec("After emplace_back: ", v);

    // Insert at position
    v.insert(v.begin() + 2, 99);
    printVec("After insert at [2]: ", v);

    // Insert multiple
    v.insert(v.begin(), {1, 2, 3});
    printVec("After insert {1,2,3} at front: ", v);

    // Remove last
    v.pop_back();
    printVec("After pop_back: ", v);

    // Erase at index
    v.erase(v.begin() + 3);
    printVec("After erase [3]: ", v);

    // Erase range
    v.erase(v.begin(), v.begin() + 2);
    printVec("After erase [0..2): ", v);

    // Clear all
    v.clear();
    std::println("After clear — empty: {}", v.empty());


    // === EXAMPLE 3: Element Access ===
    banner("Example 3: Element Access");

    std::vector<int> a = {10, 20, 30, 40, 50};

    std::println("a[2]:        {}", a[2]);        // no bounds check
    std::println("a.at(2):     {}", a.at(2));     // bounds checked — throws
    std::println("a.front():   {}", a.front());   // first element
    std::println("a.back():    {}", a.back());    // last element
    std::println("a.data()[1]: {}", a.data()[1]); // raw pointer access

    // at() throws std::out_of_range
    try {
        int bad = a.at(100);
        (void)bad;
    } catch (const std::out_of_range& e) {
        std::println("at(100) threw: {}", e.what());
    }

    // Modify through reference
    a[0] = 999;
    std::println("After a[0]=999: front={}", a.front());


    // === EXAMPLE 4: Iterators ===
    banner("Example 4: Iterators");

    std::vector<int> b = {5, 3, 8, 1, 9, 2, 7};

    // Range-based for (most common)
    std::print("range-based: ");
    for (const auto& x : b) std::print("{} ", x);
    std::println("");

    // Iterator loop
    std::print("iterator:    ");
    for (auto it = b.begin(); it != b.end(); ++it)
        std::print("{} ", *it);
    std::println("");

    // Reverse iterator
    std::print("reverse:     ");
    for (auto it = b.rbegin(); it != b.rend(); ++it)
        std::print("{} ", *it);
    std::println("");

    // Const iterator (read-only)
    std::print("const iter:  ");
    for (auto it = b.cbegin(); it != b.cend(); ++it)
        std::print("{} ", *it);
    std::println("");


    // === EXAMPLE 5: Size, Capacity, Reserve ===
    banner("Example 5: Size, Capacity, Reserve");

    std::vector<int> c;
    std::println("empty — size={} capacity={}", c.size(), c.capacity());

    // Reserve avoids reallocations
    c.reserve(10);
    std::println("after reserve(10) — size={} capacity={}", c.size(), c.capacity());

    for (int i = 0; i < 10; ++i) c.push_back(i);
    std::println("after 10 push_backs — size={} capacity={}", c.size(), c.capacity());

    c.push_back(99);   // triggers reallocation — capacity grows
    std::println("after 11th push — size={} capacity={} (grew!)", c.size(), c.capacity());

    // shrink_to_fit — release excess capacity
    c.shrink_to_fit();
    std::println("after shrink_to_fit — capacity={}", c.capacity());

    // resize — change size
    c.resize(5);
    std::println("after resize(5) — size={}", c.size());
    c.resize(10, -1);   // new elements filled with -1
    printVec("after resize(10,-1): ", c);


    // === EXAMPLE 6: Searching and Sorting ===
    banner("Example 6: Searching and Sorting");

    std::vector<int> d = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    // Sort ascending
    std::sort(d.begin(), d.end());
    printVec("sorted asc:  ", d);

    // Sort descending
    std::sort(d.begin(), d.end(), std::greater<int>());
    printVec("sorted desc: ", d);

    // Sort back for searching
    std::sort(d.begin(), d.end());

    // Linear search (unsorted)
    auto it = std::find(d.begin(), d.end(), 7);
    if (it != d.end())
        std::println("find(7): found at index {}", std::distance(d.begin(), it));

    // Binary search (sorted)
    bool found = std::binary_search(d.begin(), d.end(), 7);
    std::println("binary_search(7): {}", found);

    // lower_bound / upper_bound
    auto lb = std::lower_bound(d.begin(), d.end(), 5);
    auto ub = std::upper_bound(d.begin(), d.end(), 5);
    std::println("lower_bound(5) index: {}", std::distance(d.begin(), lb));
    std::println("upper_bound(5) index: {}", std::distance(d.begin(), ub));

    // Min/Max
    std::println("min: {}", *std::min_element(d.begin(), d.end()));
    std::println("max: {}", *std::max_element(d.begin(), d.end()));
    std::println("sum: {}", std::accumulate(d.begin(), d.end(), 0));


    // === EXAMPLE 7: Modifying with Algorithms ===
    banner("Example 7: Modifying with Algorithms");

    std::vector<int> e = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // transform — apply function to each element
    std::vector<int> squared(e.size());
    std::transform(e.begin(), e.end(), squared.begin(),
                   [](int x) { return x * x; });
    printVec("squared: ", squared);

    // remove_if + erase (erase-remove idiom)
    e.erase(std::remove_if(e.begin(), e.end(),
                           [](int x) { return x % 2 == 0; }),
            e.end());
    printVec("odd only: ", e);

    // fill
    std::vector<int> filled(5);
    std::fill(filled.begin(), filled.end(), 7);
    printVec("filled:   ", filled);

    // iota — fill with sequential values
    std::vector<int> seq(8);
    std::iota(seq.begin(), seq.end(), 1);   // 1,2,3,4,5,6,7,8
    printVec("iota(1):  ", seq);

    // count_if
    auto evens = std::count_if(seq.begin(), seq.end(),
                               [](int x) { return x % 2 == 0; });
    std::println("even count: {}", evens);

    // reverse
    std::reverse(seq.begin(), seq.end());
    printVec("reversed: ", seq);


    // === EXAMPLE 8: 2D Vector ===
    banner("Example 8: 2D Vector");

    // 3x4 matrix initialized to 0
    int rows = 3, cols = 4;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

    // Fill with values
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            matrix[i][j] = i * cols + j + 1;

    std::println("3x4 matrix:");
    for (const auto& row : matrix) {
        std::print("  ");
        for (int val : row) std::print("{}\t", val);
        std::println("");
    }

    // Add a row
    matrix.push_back({100, 200, 300, 400});
    std::println("rows after push_back: {}", matrix.size());


    // === EXAMPLE 9: Vector of Objects ===
    banner("Example 9: Vector of Objects");

    std::vector<Player> players;
    players.reserve(4);   // avoid reallocations

    // emplace_back — constructs directly in vector (no copy)
    players.emplace_back("Kostas", 1500, 10);
    players.emplace_back("Andre",  800,  5);
    players.emplace_back("Maria",  2200, 15);
    players.emplace_back("Nick",   650,  3);

    std::println("All players:");
    for (const auto& p : players) p.print();

    // Sort by score descending
    std::sort(players.begin(), players.end(),
              [](const Player& lhs, const Player& rhs) {
                  return lhs.score > rhs.score;
              });
    std::println("");
    std::println("Sorted by score (desc):");
    for (const auto& p : players) p.print();

    // Find player by name
    auto playerFound = std::find_if(players.begin(), players.end(),
                                    [](const Player& p) { return p.name == "Andre"; });
    if (playerFound != players.end()) {
        std::println("");
        std::print("Found: ");
        playerFound->print();
    }

    // Remove players with score < 700
    players.erase(
        std::remove_if(players.begin(), players.end(),
                       [](const Player& p) { return p.score < 700; }),
        players.end());
    std::println("");
    std::println("After removing score < 700:");
    for (const auto& p : players) p.print();


    // === EXAMPLE 10: Ranges (C++20) and C++23 additions ===
    banner("Example 10: Ranges (C++20) and C++23 additions");

    std::vector<int> f = {3, 1, 4, 1, 5, 9, 2, 6};

    // std::ranges::sort (C++20) — cleaner syntax
    std::ranges::sort(f);
    printVec("ranges::sort: ", f);

    // std::ranges::find
    auto found5 = std::ranges::find(f, 5);
    if (found5 != f.end())
        std::println("ranges::find(5) at index: {}", std::distance(f.begin(), found5));

    // std::ranges::count
    std::vector<int> g = {1,2,1,3,1,4,1};
    std::println("count of 1: {}", std::ranges::count(g, 1));

    // std::ranges::contains (C++23)
    std::println("contains 9: {}", std::ranges::contains(f, 9));
    std::println("contains 8: {}", std::ranges::contains(f, 8));

    // Projection in ranges
    std::ranges::sort(players, {}, &Player::score);   // sort by score field
    std::println("players sorted by score (asc):");
    for (const auto& p : players) p.print();

    std::println("");
    std::println("=== VectorSTL complete ===");
    return 0;
}
