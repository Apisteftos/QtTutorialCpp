#include <iostream>
#include <ranges>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <numeric>
#include <print>

// ============================================================
// C++20/23 RANGES
// ============================================================
// Topics covered:
//   1.  What are ranges — views pipeline
//   2.  views::filter
//   3.  views::transform
//   4.  views::take / drop
//   5.  Chaining views — the pipe operator |
//   6.  views::iota — generate sequences
//   7.  views::reverse / sort / zip (C++23)
//   8.  Range algorithms — std::ranges::sort, find etc.
//   9.  Lazy evaluation — views don't copy data
//  10.  Real use case — MCX session filtering
// ============================================================

namespace views = std::views;
namespace ranges = std::ranges;


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What are Ranges?\n";
    std::cout << "=====================================================\n";

    std::cout << "Ranges = begin + end iterator pair (any iterable)\n\n";

    std::cout << "OLD WAY — manual loops:\n";
    std::vector<int> nums = {1,2,3,4,5,6,7,8,9,10};
    std::vector<int> result;
    for (int n : nums)
        if (n % 2 == 0)
            result.push_back(n * n);
    std::cout << "  even squares: ";
    for (int n : result) std::print("{} ", n);
    std::cout << '\n';

    std::cout << "\nNEW WAY — ranges pipeline:\n";
    auto pipeline = nums
                    | views::filter([](int n){ return n % 2 == 0; })
                    | views::transform([](int n){ return n * n; });

    std::cout << "  even squares: ";
    for (int n : pipeline) std::print("{} ", n);
    std::cout << "\n  (lazy — no intermediate vector created!)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: views::filter\n";
    std::cout << "=====================================================\n";

    std::vector<int> v = {1,2,3,4,5,6,7,8,9,10};

    // Filter even numbers
    std::cout << "even:    ";
    for (int n : v | views::filter([](int n){ return n % 2 == 0; }))
        std::print("{} ", n);
    std::cout << '\n';

    // Filter > 5
    std::cout << "> 5:     ";
    for (int n : v | views::filter([](int n){ return n > 5; }))
        std::print("{} ", n);
    std::cout << '\n';

    // Filter strings by length
    std::vector<std::string> words = {"hi","hello","hey","howdy","h","happy"};
    std::cout << "len > 3: ";
    for (const auto& w : words | views::filter([](const std::string& s){ return s.size() > 3; }))
        std::print("{} ", w);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: views::transform\n";
    std::cout << "=====================================================\n";

    std::vector<int> nums2 = {1,2,3,4,5};

    std::cout << "squared:  ";
    for (int n : nums2 | views::transform([](int n){ return n * n; }))
        std::print("{} ", n);
    std::cout << '\n';

    std::cout << "doubled:  ";
    for (int n : nums2 | views::transform([](int n){ return n * 2; }))
        std::print("{} ", n);
    std::cout << '\n';

    // Transform strings to uppercase
    std::vector<std::string> names = {"kostas","alice","bob"};
    auto toUpper = [](std::string s) {
        for (char& c : s) c = std::toupper(c);
        return s;
    };
    std::cout << "uppercase:";
    for (const auto& n : names | views::transform(toUpper))
        std::print(" {}", n);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: views::take and views::drop\n";
    std::cout << "=====================================================\n";

    std::vector<int> data = {10,20,30,40,50,60,70,80,90,100};

    std::cout << "take(3):    ";
    for (int n : data | views::take(3)) std::print("{} ", n);
    std::cout << '\n';

    std::cout << "drop(7):    ";
    for (int n : data | views::drop(7)) std::print("{} ", n);
    std::cout << '\n';

    std::cout << "take(3)+drop(2): ";
    for (int n : data | views::drop(2) | views::take(3)) std::print("{} ", n);
    std::cout << '\n';

    // take_while / drop_while
    std::cout << "take_while(<50): ";
    for (int n : data | views::take_while([](int n){ return n < 50; }))
        std::print("{} ", n);
    std::cout << '\n';

    std::cout << "drop_while(<50): ";
    for (int n : data | views::drop_while([](int n){ return n < 50; }))
        std::print("{} ", n);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Chaining views — the pipe operator |\n";
    std::cout << "=====================================================\n";

    std::vector<int> numbers = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    // Chain: filter even → multiply by 3 → take first 4
    auto chain = numbers
                 | views::filter([](int n){ return n % 2 == 0; })
                 | views::transform([](int n){ return n * 3; })
                 | views::take(4);

    std::cout << "even → *3 → take(4): ";
    for (int n : chain) std::print("{} ", n);
    std::cout << '\n';

    // More complex chain
    auto complex = numbers
                   | views::filter([](int n){ return n > 5; })
                   | views::transform([](int n){ return n * n; })
                   | views::drop(2)
                   | views::take(3);

    std::cout << ">5 → squared → drop(2) → take(3): ";
    for (int n : complex) std::print("{} ", n);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: views::iota — generate sequences\n";
    std::cout << "=====================================================\n";

    // Infinite range — must use take
    std::cout << "iota(1) take(10):    ";
    for (int n : views::iota(1) | views::take(10)) std::print("{} ", n);
    std::cout << '\n';

    // Bounded range
    std::cout << "iota(1, 11):         ";
    for (int n : views::iota(1, 11)) std::print("{} ", n);
    std::cout << '\n';

    // Even numbers 1-20
    std::cout << "even 1-20:           ";
    for (int n : views::iota(1, 21) | views::filter([](int n){ return n%2==0; }))
        std::print("{} ", n);
    std::cout << '\n';

    // First 5 squares
    std::cout << "first 5 squares:     ";
    for (int n : views::iota(1) | views::transform([](int n){ return n*n; }) | views::take(5))
        std::print("{} ", n);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: views::reverse and views::keys/values\n";
    std::cout << "=====================================================\n";

    std::vector<int> rev = {1,2,3,4,5};
    std::cout << "reverse:  ";
    for (int n : rev | views::reverse) std::print("{} ", n);
    std::cout << '\n';

    // keys and values from map-like pairs
    std::vector<std::pair<std::string,int>> pairs = {
        {"alpha",1}, {"beta",2}, {"gamma",3}
    };
    std::cout << "keys:   ";
    for (const auto& k : pairs | views::keys)   std::print("{} ", k);
    std::cout << '\n';
    std::cout << "values: ";
    for (const auto& v : pairs | views::values) std::print("{} ", v);
    std::cout << '\n';

    // enumerate (C++23)
    std::vector<std::string> fruits = {"apple","banana","cherry"};
    std::cout << "enumerate:\n";
    for (const auto& [i, f] : fruits | views::enumerate)
        std::println("  [{}] {}", i, f);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Range algorithms — std::ranges::\n";
    std::cout << "=====================================================\n";

    std::vector<int> alg = {5,2,8,1,9,3,7,4,6};

    // sort — works directly on range
    std::vector<int> sorted = alg;
    ranges::sort(sorted);
    std::cout << "sorted:  ";
    for (int n : sorted) std::print("{} ", n);
    std::cout << '\n';

    // find
    auto it = ranges::find(alg, 8);
    if (it != alg.end())
        std::println("found 8 at index {}", std::distance(alg.begin(), it));

    // count_if
    int evens = ranges::count_if(alg, [](int n){ return n%2==0; });
    std::println("even count: {}", evens);

    // min/max element
    std::println("min: {}", *ranges::min_element(alg));
    std::println("max: {}", *ranges::max_element(alg));

    // all_of / any_of / none_of
    std::println("all > 0:  {}", ranges::all_of(alg,  [](int n){ return n>0; }));
    std::println("any > 8:  {}", ranges::any_of(alg,  [](int n){ return n>8; }));
    std::println("none < 0: {}", ranges::none_of(alg, [](int n){ return n<0; }));

    // contains (C++23)
    std::println("contains 7: {}", ranges::contains(alg, 7));
    std::println("contains 99: {}", ranges::contains(alg, 99));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Lazy evaluation\n";
    std::cout << "=====================================================\n";

    std::cout << "Views are LAZY — they don't process data until iterated:\n\n";

    std::vector<int> big = {1,2,3,4,5,6,7,8,9,10};

    // This creates NO intermediate vector — purely lazy
    auto lazy = big
                | views::filter([](int n){
                      std::cout << "  filter(" << n << ")\n";
                      return n % 2 == 0;
                  })
                | views::transform([](int n){
                      std::cout << "  transform(" << n << ")\n";
                      return n * 10;
                  })
                | views::take(2);

    std::cout << "View created — nothing computed yet!\n";
    std::cout << "Now iterating:\n";
    for (int n : lazy) std::print("  result: {}\n", n);

    std::cout << "\nOnly processed elements until take(2) was satisfied\n";
    std::cout << "No copy of filtered data — elements processed one by one\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Real use case — MCX session filtering\n";
    std::cout << "=====================================================\n";

    struct McxSession {
        std::string id;
        std::string group;
        int         priority;
        bool        active;
        int         participants;
    };

    std::vector<McxSession> sessions = {
                                        {"SES-001", "GRP-ALPHA", 1, true,  5},
                                        {"SES-002", "GRP-BETA",  2, false, 3},
                                        {"SES-003", "GRP-ALPHA", 2, true,  8},
                                        {"SES-004", "GRP-GAMMA", 1, true,  2},
                                        {"SES-005", "GRP-BETA",  3, false, 6},
                                        {"SES-006", "GRP-ALPHA", 1, true,  4},
                                        };

    // Active sessions only
    std::cout << "Active sessions:\n";
    for (const auto& s : sessions
                             | views::filter([](const McxSession& s){ return s.active; }))
        std::println("  {} ({}) priority={}", s.id, s.group, s.priority);

    // GRP-ALPHA active sessions sorted by priority
    std::cout << "\nGRP-ALPHA active sessions:\n";
    std::vector<McxSession> alpha;
    for (const auto& s : sessions
                             | views::filter([](const McxSession& s){ return s.active && s.group == "GRP-ALPHA"; }))
        alpha.push_back(s);
    ranges::sort(alpha, {}, &McxSession::priority);
    for (const auto& s : alpha)
        std::println("  {} priority={} participants={}", s.id, s.priority, s.participants);

    // Total participants in active sessions
    int total = 0;
    for (const auto& s : sessions | views::filter([](const McxSession& s){ return s.active; }))
        total += s.participants;
    std::println("\nTotal participants (active): {}", total);

    // Session IDs only
    std::cout << "Active session IDs: ";
    for (const auto& id : sessions
                              | views::filter([](const McxSession& s){ return s.active; })
                              | views::transform([](const McxSession& s){ return s.id; }))
        std::print("{} ", id);
    std::cout << '\n';

    std::cout << "\n=== Ranges complete ===\n";
    return 0;
}
