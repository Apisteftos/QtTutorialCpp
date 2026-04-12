#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <print>

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

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    std::vector<int> empty;                          // empty vector
    std::vector<int> withSize(5);                    // 5 elements, all 0
    std::vector<int> withValue(5, 42);               // 5 elements, all 42
    std::vector<int> fromList = {10, 20, 30, 40, 50}; // initializer list
    std::vector<int> copy = fromList;                // copy
    std::vector<int> range(fromList.begin() + 1,
                           fromList.end() - 1);     // from iterators {20,30,40}

    std::cout << "empty size:      " << empty.size()    << '\n';
    std::cout << "withSize:        ";
    for (int v : withSize) std::cout << v << ' '; std::cout << '\n';
    std::cout << "withValue:       ";
    for (int v : withValue) std::cout << v << ' '; std::cout << '\n';
    std::cout << "fromList:        ";
    for (int v : fromList) std::cout << v << ' '; std::cout << '\n';
    std::cout << "range [1..-1]:   ";
    for (int v : range) std::cout << v << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Adding and Removing Elements\n";
    std::cout << "=====================================================\n";

    std::vector<int> v;

    // Add to end
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    std::cout << "After push_back: ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // emplace_back — constructs in place (preferred over push_back for objects)
    v.emplace_back(40);
    v.emplace_back(50);
    std::cout << "After emplace_back: ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // Insert at position
    v.insert(v.begin() + 2, 99);
    std::cout << "After insert at [2]: ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // Insert multiple
    v.insert(v.begin(), {1, 2, 3});
    std::cout << "After insert {1,2,3} at front: ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // Remove last
    v.pop_back();
    std::cout << "After pop_back: ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // Erase at index
    v.erase(v.begin() + 3);
    std::cout << "After erase [3]: ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // Erase range
    v.erase(v.begin(), v.begin() + 2);
    std::cout << "After erase [0..2): ";
    for (int x : v) std::cout << x << ' '; std::cout << '\n';

    // Clear all
    v.clear();
    std::cout << "After clear — empty: " << v.empty() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Element Access\n";
    std::cout << "=====================================================\n";

    std::vector<int> a = {10, 20, 30, 40, 50};

    std::cout << "a[2]:        " << a[2]        << '\n';   // no bounds check
    std::cout << "a.at(2):     " << a.at(2)     << '\n';   // bounds checked — throws
    std::cout << "a.front():   " << a.front()   << '\n';   // first element
    std::cout << "a.back():    " << a.back()    << '\n';   // last element
    std::cout << "a.data()[1]: " << a.data()[1] << '\n';   // raw pointer access

    // at() throws std::out_of_range
    try {
        int bad = a.at(100);
        (void)bad;
    } catch (const std::out_of_range& e) {
        std::cout << "at(100) threw: " << e.what() << '\n';
    }

    // Modify through reference
    a[0] = 999;
    std::cout << "After a[0]=999: front=" << a.front() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Iterators\n";
    std::cout << "=====================================================\n";

    std::vector<int> b = {5, 3, 8, 1, 9, 2, 7};

    // Range-based for (most common)
    std::cout << "range-based: ";
    for (const auto& x : b) std::cout << x << ' '; std::cout << '\n';

    // Iterator loop
    std::cout << "iterator:    ";
    for (auto it = b.begin(); it != b.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // Reverse iterator
    std::cout << "reverse:     ";
    for (auto it = b.rbegin(); it != b.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // Const iterator (read-only)
    std::cout << "const iter:  ";
    for (auto it = b.cbegin(); it != b.cend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Size, Capacity, Reserve\n";
    std::cout << "=====================================================\n";

    std::vector<int> c;
    std::cout << "empty — size=" << c.size() << " capacity=" << c.capacity() << '\n';

    // Reserve avoids reallocations
    c.reserve(10);
    std::cout << "after reserve(10) — size=" << c.size()
              << " capacity=" << c.capacity() << '\n';

    for (int i = 0; i < 10; ++i) c.push_back(i);
    std::cout << "after 10 push_backs — size=" << c.size()
              << " capacity=" << c.capacity() << '\n';

    c.push_back(99);   // triggers reallocation — capacity grows
    std::cout << "after 11th push — size=" << c.size()
              << " capacity=" << c.capacity() << " (grew!)\n";

    // shrink_to_fit — release excess capacity
    c.shrink_to_fit();
    std::cout << "after shrink_to_fit — capacity=" << c.capacity() << '\n';

    // resize — change size
    c.resize(5);
    std::cout << "after resize(5) — size=" << c.size() << '\n';
    c.resize(10, -1);   // new elements filled with -1
    std::cout << "after resize(10,-1): ";
    for (int x : c) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Searching and Sorting\n";
    std::cout << "=====================================================\n";

    std::vector<int> d = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    // Sort ascending
    std::sort(d.begin(), d.end());
    std::cout << "sorted asc:  ";
    for (int x : d) std::cout << x << ' '; std::cout << '\n';

    // Sort descending
    std::sort(d.begin(), d.end(), std::greater<int>());
    std::cout << "sorted desc: ";
    for (int x : d) std::cout << x << ' '; std::cout << '\n';

    // Sort back for searching
    std::sort(d.begin(), d.end());

    // Linear search (unsorted)
    auto it = std::find(d.begin(), d.end(), 7);
    if (it != d.end())
        std::cout << "find(7): found at index " << std::distance(d.begin(), it) << '\n';

    // Binary search (sorted)
    bool found = std::binary_search(d.begin(), d.end(), 7);
    std::cout << "binary_search(7): " << found << '\n';

    // lower_bound / upper_bound
    auto lb = std::lower_bound(d.begin(), d.end(), 5);
    auto ub = std::upper_bound(d.begin(), d.end(), 5);
    std::cout << "lower_bound(5) index: " << std::distance(d.begin(), lb) << '\n';
    std::cout << "upper_bound(5) index: " << std::distance(d.begin(), ub) << '\n';

    // Min/Max
    std::cout << "min: " << *std::min_element(d.begin(), d.end()) << '\n';
    std::cout << "max: " << *std::max_element(d.begin(), d.end()) << '\n';
    std::cout << "sum: " << std::accumulate(d.begin(), d.end(), 0) << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Modifying with Algorithms\n";
    std::cout << "=====================================================\n";

    std::vector<int> e = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // transform — apply function to each element
    std::vector<int> squared(e.size());
    std::transform(e.begin(), e.end(), squared.begin(),
                   [](int x) { return x * x; });
    std::cout << "squared: ";
    for (int x : squared) std::cout << x << ' '; std::cout << '\n';

    // remove_if + erase (erase-remove idiom)
    e.erase(std::remove_if(e.begin(), e.end(),
                           [](int x) { return x % 2 == 0; }),
            e.end());
    std::cout << "odd only: ";
    for (int x : e) std::cout << x << ' '; std::cout << '\n';

    // fill
    std::vector<int> filled(5);
    std::fill(filled.begin(), filled.end(), 7);
    std::cout << "filled:   ";
    for (int x : filled) std::cout << x << ' '; std::cout << '\n';

    // iota — fill with sequential values
    std::vector<int> seq(8);
    std::iota(seq.begin(), seq.end(), 1);   // 1,2,3,4,5,6,7,8
    std::cout << "iota(1):  ";
    for (int x : seq) std::cout << x << ' '; std::cout << '\n';

    // count_if
    int evens = std::count_if(seq.begin(), seq.end(),
                              [](int x) { return x % 2 == 0; });
    std::cout << "even count: " << evens << '\n';

    // reverse
    std::reverse(seq.begin(), seq.end());
    std::cout << "reversed: ";
    for (int x : seq) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: 2D Vector\n";
    std::cout << "=====================================================\n";

    // 3x4 matrix initialized to 0
    int rows = 3, cols = 4;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

    // Fill with values
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            matrix[r][c] = r * cols + c + 1;

    std::cout << "3x4 matrix:\n";
    for (const auto& row : matrix) {
        std::cout << "  ";
        for (int val : row) std::cout << val << "\t";
        std::cout << '\n';
    }

    // Add a row
    matrix.push_back({100, 200, 300, 400});
    std::cout << "rows after push_back: " << matrix.size() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Vector of Objects\n";
    std::cout << "=====================================================\n";

    std::vector<Player> players;
    players.reserve(4);   // avoid reallocations

    // emplace_back — constructs directly in vector (no copy)
    players.emplace_back("Kostas", 1500, 10);
    players.emplace_back("Andre",  800,  5);
    players.emplace_back("Maria",  2200, 15);
    players.emplace_back("Nick",   650,  3);

    std::cout << "All players:\n";
    for (const auto& p : players) p.print();

    // Sort by score descending
    std::sort(players.begin(), players.end(),
              [](const Player& a, const Player& b) {
                  return a.score > b.score;
              });
    std::cout << "\nSorted by score (desc):\n";
    for (const auto& p : players) p.print();

    // Find player by name
    auto playerFound = std::find_if(players.begin(), players.end(),
                                    [](const Player& p) { return p.name == "Andre"; });
    if (playerFound != players.end()) {
        std::cout << "\nFound: "; playerFound->print();
    }

    // Remove players with score < 700
    players.erase(
        std::remove_if(players.begin(), players.end(),
                       [](const Player& p) { return p.score < 700; }),
        players.end());
    std::cout << "\nAfter removing score < 700:\n";
    for (const auto& p : players) p.print();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: C++23 Features\n";
    std::cout << "=====================================================\n";

    std::vector<int> f = {3, 1, 4, 1, 5, 9, 2, 6};

    // std::ranges::sort (C++20) — cleaner syntax
    std::ranges::sort(f);
    std::cout << "ranges::sort: ";
    for (int x : f) std::cout << x << ' '; std::cout << '\n';

    // std::ranges::find
    auto r = std::ranges::find(f, 5);
    if (r != f.end())
        std::println("ranges::find(5) at index: {}", std::distance(f.begin(), r));

    // std::ranges::count
    std::vector<int> g = {1,2,1,3,1,4,1};
    std::println("count of 1: {}", std::ranges::count(g, 1));

    // Projection in ranges
    std::ranges::sort(players, {}, &Player::score);   // sort by score field
    std::cout << "players sorted by score (asc):\n";
    for (const auto& p : players) p.print();

    std::cout << "\n=== VectorSTL complete ===\n";
    return 0;
}
