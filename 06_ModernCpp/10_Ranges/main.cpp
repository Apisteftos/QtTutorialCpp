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
//  10.  Real use case — task filtering
// ============================================================

namespace views = std::views;
namespace ranges = std::ranges;


int main()
{
    // -------------------------------------------------------
    std::println("=====================================================");
    std::println("Example 1: What are Ranges?");
    std::println("=====================================================");

    std::println("Ranges = begin + end iterator pair (any iterable)");
    std::println("");

    std::println("OLD WAY — manual loops:");
    std::vector<int> nums = {1,2,3,4,5,6,7,8,9,10};
    std::vector<int> result;
    for (int n : nums)
        if (n % 2 == 0)
            result.push_back(n * n);
    std::print("  even squares: ");
    for (int n : result) std::print("{} ", n);
    std::println("");

    std::println("");
    std::println("NEW WAY — ranges pipeline:");
    auto pipeline = nums
                    | views::filter([](int n){ return n % 2 == 0; })
                    | views::transform([](int n){ return n * n; });

    std::print("  even squares: ");
    for (int n : pipeline) std::print("{} ", n);
    std::println("");
    std::println("  (lazy — no intermediate vector created!)");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: views::filter");
    std::println("=====================================================");

    std::vector<int> v = {1,2,3,4,5,6,7,8,9,10};

    // Filter even numbers
    std::print("even:    ");
    for (int n : v | views::filter([](int n){ return n % 2 == 0; }))
        std::print("{} ", n);
    std::println("");

    // Filter > 5
    std::print("> 5:     ");
    for (int n : v | views::filter([](int n){ return n > 5; }))
        std::print("{} ", n);
    std::println("");

    // Filter strings by length
    std::vector<std::string> words = {"hi","hello","hey","howdy","h","happy"};
    std::print("len > 3: ");
    for (const auto& w : words | views::filter([](const std::string& s){ return s.size() > 3; }))
        std::print("{} ", w);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: views::transform");
    std::println("=====================================================");

    std::vector<int> nums2 = {1,2,3,4,5};

    std::print("squared:  ");
    for (int n : nums2 | views::transform([](int n){ return n * n; }))
        std::print("{} ", n);
    std::println("");

    std::print("doubled:  ");
    for (int n : nums2 | views::transform([](int n){ return n * 2; }))
        std::print("{} ", n);
    std::println("");

    // Transform strings to uppercase
    std::vector<std::string> names = {"kostas","alice","bob"};
    auto toUpper = [](std::string s) {
        for (char& c : s) c = std::toupper(c);
        return s;
    };
    std::print("uppercase:");
    for (const auto& n : names | views::transform(toUpper))
        std::print(" {}", n);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: views::take and views::drop");
    std::println("=====================================================");

    std::vector<int> data = {10,20,30,40,50,60,70,80,90,100};

    std::print("take(3):    ");
    for (int n : data | views::take(3)) std::print("{} ", n);
    std::println("");

    std::print("drop(7):    ");
    for (int n : data | views::drop(7)) std::print("{} ", n);
    std::println("");

    std::print("take(3)+drop(2): ");
    for (int n : data | views::drop(2) | views::take(3)) std::print("{} ", n);
    std::println("");

    // take_while / drop_while
    std::print("take_while(<50): ");
    for (int n : data | views::take_while([](int n){ return n < 50; }))
        std::print("{} ", n);
    std::println("");

    std::print("drop_while(<50): ");
    for (int n : data | views::drop_while([](int n){ return n < 50; }))
        std::print("{} ", n);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Chaining views — the pipe operator |");
    std::println("=====================================================");

    std::vector<int> numbers = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    // Chain: filter even → multiply by 3 → take first 4
    auto chain = numbers
                 | views::filter([](int n){ return n % 2 == 0; })
                 | views::transform([](int n){ return n * 3; })
                 | views::take(4);

    std::print("even → *3 → take(4): ");
    for (int n : chain) std::print("{} ", n);
    std::println("");

    // More complex chain
    auto complex = numbers
                   | views::filter([](int n){ return n > 5; })
                   | views::transform([](int n){ return n * n; })
                   | views::drop(2)
                   | views::take(3);

    std::print(">5 → squared → drop(2) → take(3): ");
    for (int n : complex) std::print("{} ", n);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: views::iota — generate sequences");
    std::println("=====================================================");

    // Infinite range — must use take
    std::print("iota(1) take(10):    ");
    for (int n : views::iota(1) | views::take(10)) std::print("{} ", n);
    std::println("");

    // Bounded range
    std::print("iota(1, 11):         ");
    for (int n : views::iota(1, 11)) std::print("{} ", n);
    std::println("");

    // Even numbers 1-20
    std::print("even 1-20:           ");
    for (int n : views::iota(1, 21) | views::filter([](int n){ return n%2==0; }))
        std::print("{} ", n);
    std::println("");

    // First 5 squares
    std::print("first 5 squares:     ");
    for (int n : views::iota(1) | views::transform([](int n){ return n*n; }) | views::take(5))
        std::print("{} ", n);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: views::reverse and views::keys/values");
    std::println("=====================================================");

    std::vector<int> rev = {1,2,3,4,5};
    std::print("reverse:  ");
    for (int n : rev | views::reverse) std::print("{} ", n);
    std::println("");

    // keys and values from map-like pairs
    std::vector<std::pair<std::string,int>> pairs = {
        {"alpha",1}, {"beta",2}, {"gamma",3}
    };
    std::print("keys:   ");
    for (const auto& k : pairs | views::keys)   std::print("{} ", k);
    std::println("");
    std::print("values: ");
    for (const auto& v : pairs | views::values) std::print("{} ", v);
    std::println("");

    // enumerate (C++23)
    std::vector<std::string> fruits = {"apple","banana","cherry"};
    std::println("enumerate:");
    for (const auto& [i, f] : fruits | views::enumerate)
        std::println("  [{}] {}", i, f);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: Range algorithms — std::ranges::");
    std::println("=====================================================");

    std::vector<int> alg = {5,2,8,1,9,3,7,4,6};

    // sort — works directly on range
    std::vector<int> sorted = alg;
    ranges::sort(sorted);
    std::print("sorted:  ");
    for (int n : sorted) std::print("{} ", n);
    std::println("");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: Lazy evaluation");
    std::println("=====================================================");

    std::println("Views are LAZY — they don't process data until iterated:");
    std::println("");

    std::vector<int> big = {1,2,3,4,5,6,7,8,9,10};

    // This creates NO intermediate vector — purely lazy
    auto lazy = big
                | views::filter([](int n){
                      std::println("  filter({})", n);
                      return n % 2 == 0;
                  })
                | views::transform([](int n){
                      std::println("  transform({})", n);
                      return n * 10;
                  })
                | views::take(2);

    std::println("View created — nothing computed yet!");
    std::println("Now iterating:");
    for (int n : lazy) std::println("  result: {}", n);

    std::println("");
    std::println("Only processed elements until take(2) was satisfied");
    std::println("No copy of filtered data — elements processed one by one");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 10: Real use case — task filtering");
    std::println("=====================================================");

    struct Task {
        std::string id;
        std::string team;
        int         priority;
        bool        active;
        int         estimate;
    };

    std::vector<Task> tasks = {
                                        {"T-001", "backend", 1, true,  5},
                                        {"T-002", "frontend",  2, false, 3},
                                        {"T-003", "backend", 2, true,  8},
                                        {"T-004", "ops", 1, true,  2},
                                        {"T-005", "frontend",  3, false, 6},
                                        {"T-006", "backend", 1, true,  4},
                                        };

    // Active tasks only
    std::println("Active tasks:");
    for (const auto& s : tasks
                             | views::filter([](const Task& s){ return s.active; }))
        std::println("  {} ({}) priority={}", s.id, s.team, s.priority);

    // backend active tasks sorted by priority
    std::println("");
    std::println("backend active tasks:");
    std::vector<Task> alpha;
    for (const auto& s : tasks
                             | views::filter([](const Task& s){ return s.active && s.team == "backend"; }))
        alpha.push_back(s);
    ranges::sort(alpha, {}, &Task::priority);
    for (const auto& s : alpha)
        std::println("  {} priority={} estimate={}", s.id, s.priority, s.estimate);

    // Total participants in active tasks
    int total = 0;
    for (const auto& s : tasks | views::filter([](const Task& s){ return s.active; }))
        total += s.estimate;
    std::println("");
    std::println("Total estimate (active): {}", total);

    // Task IDs only
    std::print("Active task IDs: ");
    for (const auto& id : tasks
                              | views::filter([](const Task& s){ return s.active; })
                              | views::transform([](const Task& s){ return s.id; }))
        std::print("{} ", id);
    std::println("");

    std::println("");
    std::println("=== Ranges complete ===");
    return 0;
}
