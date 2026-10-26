#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <print>
#include <string>
#include <string_view>

// ============================================================
// std::list — Doubly Linked List in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  Push/pop at both ends
//   3.  Element access (front, back — NO random access)
//   4.  Insert and erase (O(1) with iterator)
//   5.  List-specific algorithms: sort, reverse, unique
//   6.  Merge — combine two sorted lists
//   7.  Splice — move elements between lists
//   8.  Remove and remove_if
//   9.  List of objects
//  10.  List vs vector vs deque comparison
// ============================================================

struct Employee {
    int         id;
    std::string name;
    std::string department;

    void print() const {
        std::println("  Employee #{}: {} ({})", id, name, department);
    }
};

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

    std::list<int> empty;
    std::list<int> sized(5);              // 5 elements, value 0
    std::list<int> filled(5, 99);         // 5 elements, value 99
    std::list<int> fromList = {1,2,3,4,5};
    std::list<int> copy = fromList;

    std::println("empty size: {}", empty.size());
    printAll("sized:      ", sized);
    printAll("filled:     ", filled);
    printAll("fromList:   ", fromList);
    printAll("copy:       ", copy);


    // === EXAMPLE 2: Push and Pop at Both Ends ===
    banner("Example 2: Push and Pop at Both Ends");

    std::list<int> l;

    l.push_back(20);
    l.push_back(30);
    l.push_front(10);
    l.push_front(5);
    l.emplace_back(40);
    l.emplace_front(1);
    printAll("after pushes:  ", l);

    l.pop_back();
    printAll("after pop_back:  ", l);
    l.pop_front();
    printAll("after pop_front: ", l);


    // === EXAMPLE 3: Element Access (front and back only) ===
    banner("Example 3: Element Access (front and back only)");

    std::list<int> a = {10, 20, 30, 40, 50};

    std::println("front(): {}", a.front());   // O(1)
    std::println("back():  {}", a.back());   // O(1)

    // NO random access — a[2] does NOT exist!
    std::println("(list has no operator[] — use iterators to traverse)");

    // To access by index — must traverse (O(n))
    auto it = a.begin();
    std::advance(it, 2);   // move iterator 2 steps forward
    std::println("element at index 2 (via advance): {}", *it);


    // === EXAMPLE 4: Insert and Erase — O(1) with iterator ===
    banner("Example 4: Insert and Erase — O(1) with iterator");

    std::list<int> b = {10, 20, 30, 40, 50};
    printAll("original:           ", b);

    // Insert before iterator position — O(1)
    auto pos = b.begin();
    std::advance(pos, 2);      // point to 30
    b.insert(pos, 99);         // insert 99 before 30
    printAll("insert 99 before 30:", b);

    // Insert multiple
    b.insert(pos, {11, 12});
    printAll("insert {11,12}:     ", b);

    // Erase at position — O(1)
    pos = b.begin();
    std::advance(pos, 1);
    b.erase(pos);              // erase second element
    printAll("erase [1]:          ", b);

    // Erase range
    auto first = b.begin();
    auto last  = b.begin();
    std::advance(last, 3);
    b.erase(first, last);
    printAll("erase [0..3):       ", b);

    b.clear();
    std::println("after clear — empty: {}", b.empty());


    // === EXAMPLE 5: List-Specific Algorithms ===
    banner("Example 5: List-Specific Algorithms");

    std::list<int> c = {5, 3, 8, 1, 9, 2, 7};

    // sort — std::sort does NOT work on list (no random access)
    // Use list's own sort — O(n log n)
    c.sort();
    printAll("sorted (asc):  ", c);

    c.sort(std::greater<int>());
    printAll("sorted (desc): ", c);

    // reverse — O(n)
    c.reverse();
    printAll("reversed:      ", c);

    // unique — removes consecutive duplicates (list must be sorted first)
    std::list<int> d = {1, 1, 2, 3, 3, 3, 4, 5, 5};
    d.unique();
    printAll("unique:        ", d);

    // unique with predicate
    std::list<int> e = {1, 3, 2, 4, 6, 8, 5, 7};
    e.sort();
    e.unique([](int lhs, int rhs) { return lhs == rhs; });
    printAll("unique(pred):  ", e);


    // === EXAMPLE 6: Merge — combine two sorted lists ===
    banner("Example 6: Merge — combine two sorted lists");

    std::list<int> list1 = {1, 3, 5, 7, 9};
    std::list<int> list2 = {2, 4, 6, 8, 10};

    printAll("list1: ", list1);
    printAll("list2: ", list2);

    list1.merge(list2);   // list2 becomes empty, list1 contains all sorted
    printAll("merged: ", list1);
    std::println("list2 after merge: {}", list2.empty() ? "empty" : "not empty");

    // merge with custom comparator
    std::list<int> la = {9, 7, 5, 3, 1};
    std::list<int> lb = {10, 8, 6, 4, 2};
    la.merge(lb, std::greater<int>());
    printAll("merged desc: ", la);


    // === EXAMPLE 7: Splice — move elements between lists ===
    banner("Example 7: Splice — move elements between lists");

    std::list<int> src  = {100, 200, 300, 400, 500};
    std::list<int> dest = {1, 2, 3};

    printAll("src:  ", src);
    printAll("dest: ", dest);

    // splice entire src into dest at position 1
    auto destPos = dest.begin();
    std::advance(destPos, 1);   // point to '2'
    dest.splice(destPos, src);  // moves all of src into dest
    printAll("dest after splice: ", dest);
    std::println("src after splice:  {} elements (moved)", src.size());

    // splice a single element
    std::list<int> p = {10, 20, 30, 40};
    std::list<int> q = {1, 2, 3};
    auto pIt = p.begin();
    std::advance(pIt, 2);   // point to 30
    q.splice(q.end(), p, pIt);   // move 30 from p to end of q
    printAll("p after single splice: ", p);
    printAll("q after single splice: ", q);


    // === EXAMPLE 8: Remove and remove_if ===
    banner("Example 8: Remove and remove_if");

    std::list<int> f = {1, 2, 3, 2, 4, 2, 5};
    f.remove(2);   // removes ALL occurrences of 2 — O(n)
    printAll("after remove(2):       ", f);

    std::list<int> g = {1, 2, 3, 4, 5, 6, 7, 8};
    g.remove_if([](int x) { return x % 2 == 0; });   // remove all evens
    printAll("after remove_if(even): ", g);


    // === EXAMPLE 9: List of Objects ===
    banner("Example 9: List of Objects");

    std::list<Employee> employees;
    employees.push_back({1, "Kostas",  "Engineering"});
    employees.push_back({2, "Maria",   "Marketing"});
    employees.push_back({3, "Andre",   "Engineering"});
    employees.push_back({4, "Sophie",  "HR"});
    employees.push_back({5, "Nick",    "Engineering"});

    std::println("All employees:");
    for (const auto& emp : employees) {
        emp.print();
    }

    // Sort by name
    employees.sort([](const Employee& lhs, const Employee& rhs) {
        return lhs.name < rhs.name;
    });
    std::println("");
    std::println("Sorted by name:");
    for (const auto& emp : employees) {
        emp.print();
    }

    // Remove HR department
    employees.remove_if([](const Employee& emp) {
        return emp.department == "HR";
    });
    std::println("");
    std::println("After removing HR:");
    for (const auto& emp : employees) {
        emp.print();
    }

    // Find by id using iterator
    auto found = std::find_if(employees.begin(), employees.end(),
                              [](const Employee& emp) { return emp.id == 3; });
    if (found != employees.end()) {
        std::println("");
        std::print("Found employee: ");
        found->print();
    }


    // === EXAMPLE 10: List vs Vector vs Deque ===
    banner("Example 10: List vs Vector vs Deque");

    std::println("Operation           | vector  | deque   | list");
    std::println("push_back           | O(1)    | O(1)    | O(1)");
    std::println("push_front          | O(n)    | O(1)    | O(1)");
    std::println("pop_front           | O(n)    | O(1)    | O(1)");
    std::println("insert (with iter)  | O(n)    | O(n)    | O(1)  <- list wins");
    std::println("erase  (with iter)  | O(n)    | O(n)    | O(1)  <- list wins");
    std::println("random access []    | O(1)    | O(1)    | NONE  <- list loses");
    std::println("search              | O(n)    | O(n)    | O(n)");
    std::println("sort                | O(nlogn)| O(nlogn)| O(nlogn)");
    std::println("splice              | NO      | NO      | O(1)  <- list only");
    std::println("merge (sorted)      | NO      | NO      | O(n)  <- list only");
    std::println("memory              | contiguous|chunks | scattered");
    std::println("cache performance   | best    | medium  | worst");

    std::println("");
    std::println("=== ListSTL complete ===");
    return 0;
}
