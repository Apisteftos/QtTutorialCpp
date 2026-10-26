#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

// ============================================================
// std::algorithm — STL Algorithms in C++23
// ============================================================
// Topics covered:
//   1.  Sorting: sort, stable_sort, partial_sort, nth_element
//   2.  Searching: find, find_if, binary_search, lower/upper_bound
//   3.  Counting: count, count_if
//   4.  Modifying: transform, replace, fill, generate, iota
//   5.  Removing: remove, remove_if, unique
//   6.  Reordering: reverse, rotate, shuffle, next_permutation
//   7.  Min/Max: min, max, min_element, max_element, clamp
//   8.  Numeric: accumulate, reduce, inner_product, partial_sum
//   9.  Set operations: set_union, set_intersection, set_difference
//  10.  C++20 Ranges: ranges::sort, views::filter, views::transform
// ============================================================

// Print a label followed by all elements of a range
// (takes a forwarding reference: a filter view cannot be iterated through a const reference)
template<typename Range>
void printAll(std::string_view label, Range&& r)
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
    // === EXAMPLE 1: Sorting ===
    banner("Example 1: Sorting");

    {
        std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

        // sort — O(n log n), not stable
        std::sort(v.begin(), v.end());
        printAll("sort (asc):    ", v);

        std::sort(v.begin(), v.end(), std::greater<int>());
        printAll("sort (desc):   ", v);

        // stable_sort — preserves relative order of equal elements
        struct Item { int val; char id; };
        std::vector<Item> items = {{3,'a'},{1,'b'},{3,'c'},{2,'d'},{1,'e'}};
        std::stable_sort(items.begin(), items.end(),
                         [](const Item& lhs, const Item& rhs){ return lhs.val < rhs.val; });
        std::print("stable_sort:   ");
        for (const auto& i : items) {
            std::print("{}{} ", i.val, i.id);
        }
        std::println("");

        // partial_sort — only sort first K elements
        std::vector<int> p = {5,3,8,1,9,2,7};
        std::partial_sort(p.begin(), p.begin()+3, p.end());
        printAll("partial_sort(3): ", p);
        std::println("(only first 3 sorted)");

        // nth_element — element at position n as if sorted, rest unordered
        std::vector<int> n = {5,3,8,1,9,2,7,4,6};
        std::nth_element(n.begin(), n.begin()+4, n.end());
        std::println("nth_element(4): {} (5th smallest)", n[4]);
    }


    // === EXAMPLE 2: Searching ===
    banner("Example 2: Searching");

    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        // find — linear O(n)
        auto it = std::find(v.begin(), v.end(), 5);
        if (it != v.end()) {
            std::println("find(5) at index: {}", std::distance(v.begin(), it));
        }

        // find_if
        auto even = std::find_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
        std::println("first even: {}", *even);

        // find_if_not
        auto notSmall = std::find_if_not(v.begin(), v.end(), [](int x){ return x < 5; });
        std::println("first >= 5: {}", *notSmall);

        // binary_search — O(log n), must be sorted
        std::println("binary_search(7): {}", std::binary_search(v.begin(), v.end(), 7));
        std::println("binary_search(10):{}", std::binary_search(v.begin(), v.end(), 10));

        // lower_bound, upper_bound
        auto lb = std::lower_bound(v.begin(), v.end(), 5);
        auto ub = std::upper_bound(v.begin(), v.end(), 5);
        std::println("lower_bound(5): index {}", std::distance(v.begin(), lb));
        std::println("upper_bound(5): index {}", std::distance(v.begin(), ub));

        // search — find subsequence
        std::vector<int> sub = {4, 5, 6};
        auto sit = std::search(v.begin(), v.end(), sub.begin(), sub.end());
        if (sit != v.end()) {
            std::println("subsequence {{4,5,6}} at index: {}", std::distance(v.begin(), sit));
        }

        // all_of, any_of, none_of
        std::println("all positive:  {}", std::all_of(v.begin(), v.end(), [](int x){ return x > 0; }));
        std::println("any > 8:       {}", std::any_of(v.begin(), v.end(), [](int x){ return x > 8; }));
        std::println("none negative: {}", std::none_of(v.begin(), v.end(), [](int x){ return x < 0; }));
    }


    // === EXAMPLE 3: Counting ===
    banner("Example 3: Counting");

    {
        std::vector<int> v = {1,2,3,2,4,2,5,2,6};

        std::println("count(2):         {}", std::count(v.begin(), v.end(), 2));
        std::println("count_if(even):   {}",
                     std::count_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; }));
        std::println("count_if(>3):     {}",
                     std::count_if(v.begin(), v.end(), [](int x){ return x > 3; }));
    }


    // === EXAMPLE 4: Modifying Algorithms ===
    banner("Example 4: Modifying Algorithms");

    {
        std::vector<int> v = {1,2,3,4,5};

        // transform — apply function, write to output
        std::vector<int> squared(v.size());   // the output must already have room!
        std::transform(v.begin(), v.end(), squared.begin(),
                       [](int x){ return x*x; });
        printAll("squared:   ", squared);

        // transform with two ranges
        std::vector<int> sums(v.size());
        std::transform(v.begin(), v.end(), squared.begin(), sums.begin(),
                       [](int lhs, int rhs){ return lhs + rhs; });
        printAll("v+squared: ", sums);

        // replace
        std::vector<int> r = {1,2,3,2,4,2,5};
        std::replace(r.begin(), r.end(), 2, 99);
        printAll("replace(2->99): ", r);

        // replace_if
        std::vector<int> ri = {1,2,3,4,5,6,7,8};
        std::replace_if(ri.begin(), ri.end(), [](int x){ return x % 2 == 0; }, 0);
        printAll("replace_if(even->0): ", ri);

        // fill
        std::vector<int> f(6);
        std::fill(f.begin(), f.end(), 7);
        printAll("fill(7):   ", f);

        // generate
        int counter = 0;
        std::vector<int> g(5);
        std::generate(g.begin(), g.end(), [&counter](){ return counter++ * 10; });
        printAll("generate:  ", g);

        // iota — sequential values
        std::vector<int> seq(8);
        std::iota(seq.begin(), seq.end(), 1);
        printAll("iota(1):   ", seq);

        // copy_if
        std::vector<int> odds;
        std::copy_if(seq.begin(), seq.end(), std::back_inserter(odds),
                     [](int x){ return x % 2 != 0; });
        printAll("copy_if(odd): ", odds);
    }


    // === EXAMPLE 5: Removing Elements ===
    banner("Example 5: Removing Elements");

    {
        // Erase-remove idiom
        std::vector<int> v = {1,2,3,2,4,2,5};
        v.erase(std::remove(v.begin(), v.end(), 2), v.end());
        printAll("remove(2):     ", v);

        // remove_if
        std::vector<int> v2 = {1,2,3,4,5,6,7,8,9,10};
        v2.erase(std::remove_if(v2.begin(), v2.end(),
                                [](int x){ return x % 2 == 0; }), v2.end());
        printAll("remove_if(even): ", v2);

        // unique — removes consecutive duplicates (sort first)
        std::vector<int> v3 = {1,1,2,3,3,3,4,5,5};
        v3.erase(std::unique(v3.begin(), v3.end()), v3.end());
        printAll("unique:        ", v3);
    }


    // === EXAMPLE 6: Reordering ===
    banner("Example 6: Reordering");

    {
        std::vector<int> v = {1,2,3,4,5};

        // reverse
        std::reverse(v.begin(), v.end());
        printAll("reversed: ", v);

        // rotate — rotate left by 2
        std::rotate(v.begin(), v.begin()+2, v.end());
        printAll("rotate(2): ", v);

        // next_permutation
        std::vector<int> perm = {1,2,3};
        std::println("permutations of {{1,2,3}}:");
        std::print("  ");
        do {
            for (int x : perm) {
                std::print("{}", x);
            }
            std::print(" ");
        } while (std::next_permutation(perm.begin(), perm.end()));
        std::println("");

        // shuffle (random — differs on every run)
        std::vector<int> deck = {1,2,3,4,5,6,7,8};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(deck.begin(), deck.end(), gen);
        printAll("shuffled: ", deck);
    }


    // === EXAMPLE 7: Min / Max ===
    banner("Example 7: Min / Max");

    {
        std::vector<int> v = {5,3,8,1,9,2,7};

        std::println("min_element: {}", *std::min_element(v.begin(), v.end()));
        std::println("max_element: {}", *std::max_element(v.begin(), v.end()));

        auto [mn, mx] = std::minmax_element(v.begin(), v.end());
        std::println("minmax: {} {}", *mn, *mx);

        std::println("min(3,7):    {}", std::min(3, 7));
        std::println("max(3,7):    {}", std::max(3, 7));

        // clamp — keep value within [lo, hi]
        std::println("clamp(15,0,10): {}", std::clamp(15, 0, 10));
        std::println("clamp(-5,0,10): {}", std::clamp(-5, 0, 10));
        std::println("clamp(5,0,10):  {}", std::clamp(5, 0, 10));
    }


    // === EXAMPLE 8: Numeric Algorithms ===
    banner("Example 8: Numeric Algorithms");

    {
        std::vector<int> v = {1,2,3,4,5,6,7,8,9,10};

        // accumulate — sum, product, custom op
        int sum     = std::accumulate(v.begin(), v.end(), 0);
        int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
        std::println("accumulate sum:     {}", sum);
        std::println("accumulate product: {}", product);

        // reduce (C++17) — like accumulate, but the order of operations is unspecified
        // (and it can run in parallel with an execution policy)
        int rsum = std::reduce(v.begin(), v.end(), 0);
        std::println("reduce sum:         {}", rsum);

        // inner_product — dot product
        std::vector<int> a = {1,2,3};
        std::vector<int> b = {4,5,6};
        int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);
        std::println("inner_product(dot): {}", dot);  // 1*4+2*5+3*6 = 32

        // partial_sum — prefix sums
        std::vector<int> psum(v.size());
        std::partial_sum(v.begin(), v.end(), psum.begin());
        printAll("partial_sum: ", psum);

        // adjacent_difference
        std::vector<int> diff(v.size());
        std::adjacent_difference(v.begin(), v.end(), diff.begin());
        printAll("adj_diff:    ", diff);
    }


    // === EXAMPLE 9: Set Operations (on sorted ranges) ===
    banner("Example 9: Set Operations (on sorted ranges)");

    {
        std::vector<int> a = {1,2,3,4,5};
        std::vector<int> b = {3,4,5,6,7};
        std::vector<int> result;

        // union
        std::set_union(a.begin(),a.end(), b.begin(),b.end(),
                       std::back_inserter(result));
        printAll("union:        ", result);

        result.clear();
        std::set_intersection(a.begin(),a.end(), b.begin(),b.end(),
                              std::back_inserter(result));
        printAll("intersection: ", result);

        result.clear();
        std::set_difference(a.begin(),a.end(), b.begin(),b.end(),
                            std::back_inserter(result));
        printAll("difference (A-B): ", result);

        result.clear();
        std::set_symmetric_difference(a.begin(),a.end(), b.begin(),b.end(),
                                      std::back_inserter(result));
        printAll("symmetric diff:   ", result);

        // begin() and end() must come from the SAME vector, so give it a name
        std::vector<int> part = {2,3,4};
        std::println("includes(a,{{2,3,4}}): {}",
                     std::includes(a.begin(), a.end(), part.begin(), part.end()));
    }


    // === EXAMPLE 10: C++20 Ranges ===
    banner("Example 10: C++20 Ranges");

    {
        std::vector<int> v = {5,3,8,1,9,2,7,4,6};

        // ranges::sort — no begin/end needed
        std::ranges::sort(v);
        printAll("ranges::sort: ", v);

        // ranges::find
        auto it = std::ranges::find(v, 7);
        std::println("ranges::find(7) at index: {}", std::distance(v.begin(), it));

        // ranges::count_if
        std::println("ranges::count_if(>5): {}",
                     std::ranges::count_if(v, [](int x){ return x > 5; }));

        // views pipeline — lazy evaluation
        auto pipeline = v
                        | std::views::filter([](int x){ return x % 2 == 0; })   // keep evens
                        | std::views::transform([](int x){ return x * x; });    // square them

        printAll("evens squared: ", pipeline);

        // views::take, views::drop
        printAll("first 3:       ", v | std::views::take(3));
        printAll("skip first 5:  ", v | std::views::drop(5));

        // ranges::sort with projection
        struct Person { std::string name; int age; };
        std::vector<Person> people = {{"Charlie",30},{"Alice",25},{"Bob",35}};
        std::ranges::sort(people, {}, &Person::age);   // sort by age field
        std::println("sorted by age:");
        for (const auto& p : people) {
            std::println("  {} ({})", p.name, p.age);
        }
    }

    std::println("");
    std::println("=== AlgorithmSTL complete ===");
    return 0;
}
