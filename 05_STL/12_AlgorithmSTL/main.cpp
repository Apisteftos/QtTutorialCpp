#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <array>
#include <list>
#include <print>
#include <ranges>
#include <random>

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

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Sorting\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

        // sort — O(n log n), not stable
        std::sort(v.begin(), v.end());
        std::cout << "sort (asc):    ";
        for (int x : v) std::cout << x << ' '; std::cout << '\n';

        std::sort(v.begin(), v.end(), std::greater<int>());
        std::cout << "sort (desc):   ";
        for (int x : v) std::cout << x << ' '; std::cout << '\n';

        // stable_sort — preserves relative order of equal elements
        struct Item { int val; char id; };
        std::vector<Item> items = {{3,'a'},{1,'b'},{3,'c'},{2,'d'},{1,'e'}};
        std::stable_sort(items.begin(), items.end(),
                         [](const Item& a, const Item& b){ return a.val < b.val; });
        std::cout << "stable_sort:   ";
        for (const auto& i : items) std::cout << i.val << i.id << ' '; std::cout << '\n';

        // partial_sort — only sort first K elements
        std::vector<int> p = {5,3,8,1,9,2,7};
        std::partial_sort(p.begin(), p.begin()+3, p.end());
        std::cout << "partial_sort(3): ";
        for (int x : p) std::cout << x << ' '; std::cout << "(only first 3 sorted)\n";

        // nth_element — element at position n as if sorted, rest unordered
        std::vector<int> n = {5,3,8,1,9,2,7,4,6};
        std::nth_element(n.begin(), n.begin()+4, n.end());
        std::cout << "nth_element(4): " << n[4] << " (5th smallest)\n";
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Searching\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        // find — linear O(n)
        auto it = std::find(v.begin(), v.end(), 5);
        if (it != v.end())
            std::println("find(5) at index: {}", (int)std::distance(v.begin(), it));

        // find_if
        auto even = std::find_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
        std::println("first even: {}", *even);

        // find_if_not
        auto notSmall = std::find_if_not(v.begin(), v.end(), [](int x){ return x < 5; });
        std::println("first >= 5: {}", *notSmall);

        // binary_search — O(log n), must be sorted
        std::cout << "binary_search(7): " << std::binary_search(v.begin(), v.end(), 7) << '\n';
        std::cout << "binary_search(10):" << std::binary_search(v.begin(), v.end(), 10) << '\n';

        // lower_bound, upper_bound
        auto lb = std::lower_bound(v.begin(), v.end(), 5);
        auto ub = std::upper_bound(v.begin(), v.end(), 5);
        std::println("lower_bound(5): index {}", (int)std::distance(v.begin(), lb));
        std::println("upper_bound(5): index {}", (int)std::distance(v.begin(), ub));

        // search — find subsequence
        std::vector<int> sub = {4, 5, 6};
        auto sit = std::search(v.begin(), v.end(), sub.begin(), sub.end());
        if (sit != v.end())
            std::println("subsequence {{4,5,6}} at index: {}", (int)std::distance(v.begin(), sit));

        // all_of, any_of, none_of
        std::cout << "all positive:  " << std::all_of(v.begin(),v.end(),[](int x){return x>0;}) << '\n';
        std::cout << "any > 8:       " << std::any_of(v.begin(),v.end(),[](int x){return x>8;}) << '\n';
        std::cout << "none negative: " << std::none_of(v.begin(),v.end(),[](int x){return x<0;}) << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Counting\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {1,2,3,2,4,2,5,2,6};

        std::cout << "count(2):         " << std::count(v.begin(), v.end(), 2) << '\n';
        std::cout << "count_if(even):   "
                  << std::count_if(v.begin(), v.end(), [](int x){ return x%2==0; }) << '\n';
        std::cout << "count_if(>3):     "
                  << std::count_if(v.begin(), v.end(), [](int x){ return x>3; }) << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Modifying Algorithms\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {1,2,3,4,5};

        // transform — apply function, write to output
        std::vector<int> squared(v.size());
        std::transform(v.begin(), v.end(), squared.begin(),
                       [](int x){ return x*x; });
        std::cout << "squared:   ";
        for (int x : squared) std::cout << x << ' '; std::cout << '\n';

        // transform with two ranges
        std::vector<int> sums(v.size());
        std::transform(v.begin(), v.end(), squared.begin(), sums.begin(),
                       [](int a, int b){ return a+b; });
        std::cout << "v+squared: ";
        for (int x : sums) std::cout << x << ' '; std::cout << '\n';

        // replace
        std::vector<int> r = {1,2,3,2,4,2,5};
        std::replace(r.begin(), r.end(), 2, 99);
        std::cout << "replace(2->99): ";
        for (int x : r) std::cout << x << ' '; std::cout << '\n';

        // replace_if
        std::vector<int> ri = {1,2,3,4,5,6,7,8};
        std::replace_if(ri.begin(), ri.end(), [](int x){ return x%2==0; }, 0);
        std::cout << "replace_if(even->0): ";
        for (int x : ri) std::cout << x << ' '; std::cout << '\n';

        // fill
        std::vector<int> f(6);
        std::fill(f.begin(), f.end(), 7);
        std::cout << "fill(7):   ";
        for (int x : f) std::cout << x << ' '; std::cout << '\n';

        // generate
        int counter = 0;
        std::vector<int> g(5);
        std::generate(g.begin(), g.end(), [&counter](){ return counter++ * 10; });
        std::cout << "generate:  ";
        for (int x : g) std::cout << x << ' '; std::cout << '\n';

        // iota — sequential values
        std::vector<int> seq(8);
        std::iota(seq.begin(), seq.end(), 1);
        std::cout << "iota(1):   ";
        for (int x : seq) std::cout << x << ' '; std::cout << '\n';

        // copy_if
        std::vector<int> odds;
        std::copy_if(seq.begin(), seq.end(), std::back_inserter(odds),
                     [](int x){ return x%2!=0; });
        std::cout << "copy_if(odd): ";
        for (int x : odds) std::cout << x << ' '; std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Removing Elements\n";
    std::cout << "=====================================================\n";

    {
        // Erase-remove idiom
        std::vector<int> v = {1,2,3,2,4,2,5};
        v.erase(std::remove(v.begin(), v.end(), 2), v.end());
        std::cout << "remove(2):     ";
        for (int x : v) std::cout << x << ' '; std::cout << '\n';

        // remove_if
        std::vector<int> v2 = {1,2,3,4,5,6,7,8,9,10};
        v2.erase(std::remove_if(v2.begin(), v2.end(),
                                [](int x){ return x%2==0; }), v2.end());
        std::cout << "remove_if(even): ";
        for (int x : v2) std::cout << x << ' '; std::cout << '\n';

        // unique — removes consecutive duplicates (sort first)
        std::vector<int> v3 = {1,1,2,3,3,3,4,5,5};
        v3.erase(std::unique(v3.begin(), v3.end()), v3.end());
        std::cout << "unique:        ";
        for (int x : v3) std::cout << x << ' '; std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Reordering\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {1,2,3,4,5};

        // reverse
        std::reverse(v.begin(), v.end());
        std::cout << "reversed: ";
        for (int x : v) std::cout << x << ' '; std::cout << '\n';

        // rotate — rotate left by 2
        std::rotate(v.begin(), v.begin()+2, v.end());
        std::cout << "rotate(2):" ;
        for (int x : v) std::cout << x << ' '; std::cout << '\n';

        // next_permutation
        std::vector<int> perm = {1,2,3};
        std::cout << "permutations of {1,2,3}:\n  ";
        do {
            for (int x : perm) std::cout << x;
            std::cout << ' ';
        } while (std::next_permutation(perm.begin(), perm.end()));
        std::cout << '\n';

        // shuffle (random)
        std::vector<int> deck = {1,2,3,4,5,6,7,8};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(deck.begin(), deck.end(), gen);
        std::cout << "shuffled: ";
        for (int x : deck) std::cout << x << ' '; std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Min / Max\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {5,3,8,1,9,2,7};

        std::cout << "min_element: " << *std::min_element(v.begin(), v.end()) << '\n';
        std::cout << "max_element: " << *std::max_element(v.begin(), v.end()) << '\n';

        auto [mn, mx] = std::minmax_element(v.begin(), v.end());
        std::println("minmax: {} {}", *mn, *mx);

        std::cout << "min(3,7):    " << std::min(3,7) << '\n';
        std::cout << "max(3,7):    " << std::max(3,7) << '\n';

        // clamp — keep value within [lo, hi]
        std::cout << "clamp(15,0,10): " << std::clamp(15,0,10) << '\n';
        std::cout << "clamp(-5,0,10): " << std::clamp(-5,0,10) << '\n';
        std::cout << "clamp(5,0,10):  " << std::clamp(5,0,10)  << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Numeric Algorithms\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {1,2,3,4,5,6,7,8,9,10};

        // accumulate — sum, product, custom op
        int sum     = std::accumulate(v.begin(), v.end(), 0);
        int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
        std::println("accumulate sum:     {}", sum);
        std::println("accumulate product: {}", product);

        // reduce (C++17) — like accumulate but parallelizable
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
        std::cout << "partial_sum: ";
        for (int x : psum) std::cout << x << ' '; std::cout << '\n';

        // adjacent_difference
        std::vector<int> diff(v.size());
        std::adjacent_difference(v.begin(), v.end(), diff.begin());
        std::cout << "adj_diff:    ";
        for (int x : diff) std::cout << x << ' '; std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Set Operations (on sorted ranges)\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> a = {1,2,3,4,5};
        std::vector<int> b = {3,4,5,6,7};
        std::vector<int> result;

        // union
        std::set_union(a.begin(),a.end(), b.begin(),b.end(),
                       std::back_inserter(result));
        std::cout << "union:        ";
        for (int x : result) std::cout << x << ' '; std::cout << '\n';

        result.clear();
        std::set_intersection(a.begin(),a.end(), b.begin(),b.end(),
                              std::back_inserter(result));
        std::cout << "intersection: ";
        for (int x : result) std::cout << x << ' '; std::cout << '\n';

        result.clear();
        std::set_difference(a.begin(),a.end(), b.begin(),b.end(),
                            std::back_inserter(result));
        std::cout << "difference (A-B): ";
        for (int x : result) std::cout << x << ' '; std::cout << '\n';

        result.clear();
        std::set_symmetric_difference(a.begin(),a.end(), b.begin(),b.end(),
                                      std::back_inserter(result));
        std::cout << "symmetric diff:   ";
        for (int x : result) std::cout << x << ' '; std::cout << '\n';

        std::cout << "includes(a,{2,3,4}): "
                  << std::includes(a.begin(),a.end(),
                                   std::vector<int>{2,3,4}.begin(),
                                   std::vector<int>{2,3,4}.end()) << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: C++20 Ranges\n";
    std::cout << "=====================================================\n";

    {
        std::vector<int> v = {5,3,8,1,9,2,7,4,6};

        // ranges::sort — no begin/end needed
        std::ranges::sort(v);
        std::cout << "ranges::sort: ";
        for (int x : v) std::cout << x << ' '; std::cout << '\n';

        // ranges::find
        auto it = std::ranges::find(v, 7);
        std::println("ranges::find(7) at index: {}", (int)std::distance(v.begin(), it));

        // ranges::count_if
        std::println("ranges::count_if(>5): {}",
                     std::ranges::count_if(v, [](int x){ return x>5; }));

        // views pipeline — lazy evaluation
        auto pipeline = v
                        | std::views::filter([](int x){ return x%2==0; })   // keep evens
                        | std::views::transform([](int x){ return x*x; });  // square them

        std::cout << "evens squared: ";
        for (int x : pipeline) std::cout << x << ' '; std::cout << '\n';

        // views::take, views::drop
        std::cout << "first 3:       ";
        for (int x : v | std::views::take(3)) std::cout << x << ' '; std::cout << '\n';

        std::cout << "skip first 5:  ";
        for (int x : v | std::views::drop(5)) std::cout << x << ' '; std::cout << '\n';

        // ranges::sort with projection
        struct Person { std::string name; int age; };
        std::vector<Person> people = {{"Charlie",30},{"Alice",25},{"Bob",35}};
        std::ranges::sort(people, {}, &Person::age);   // sort by age field
        std::cout << "sorted by age: ";
        for (const auto& p : people)
            std::println("{} ({})", p.name, p.age);
    }

    std::cout << "\n=== AlgorithmSTL complete ===\n";
    return 0;
}
