#include <iostream>
#include <list>
#include <algorithm>
#include <numeric>
#include <string>
#include <print>

// ============================================================
// std::list — Doubly Linked List in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  Push/pop at both ends
//   3.  Element access (front, back — NO random access)
//   4.  Insert and erase (O(1) with iterator)
//   5.  List-specific algorithms: sort, reverse, unique, merge
//   6.  Splice — move elements between lists
//   7.  Remove and remove_if
//   8.  Iterators
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

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    std::list<int> empty;
    std::list<int> sized(5);              // 5 elements, value 0
    std::list<int> filled(5, 99);         // 5 elements, value 99
    std::list<int> fromList = {1,2,3,4,5};
    std::list<int> copy = fromList;

    auto printList = [](const std::string& label, const std::list<int>& l) {
        std::cout << label;
        for (int x : l) std::cout << x << ' ';
        std::cout << '\n';
    };

    std::cout << "empty size: " << empty.size() << '\n';
    printList("sized:      ", sized);
    printList("filled:     ", filled);
    printList("fromList:   ", fromList);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Push and Pop at Both Ends\n";
    std::cout << "=====================================================\n";

    std::list<int> l;

    l.push_back(20);
    l.push_back(30);
    l.push_front(10);
    l.push_front(5);
    l.emplace_back(40);
    l.emplace_front(1);
    printList("after pushes:  ", l);

    l.pop_back();
    printList("after pop_back:  ", l);
    l.pop_front();
    printList("after pop_front: ", l);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Element Access (front and back only)\n";
    std::cout << "=====================================================\n";

    std::list<int> a = {10, 20, 30, 40, 50};

    std::cout << "front(): " << a.front() << '\n';   // O(1)
    std::cout << "back():  " << a.back()  << '\n';   // O(1)

    // NO random access — a[2] does NOT exist!
    std::cout << "(list has no operator[] — use iterators to traverse)\n";

    // To access by index — must traverse (O(n))
    auto it = a.begin();
    std::advance(it, 2);   // move iterator 2 steps forward
    std::cout << "element at index 2 (via advance): " << *it << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Insert and Erase — O(1) with iterator\n";
    std::cout << "=====================================================\n";

    std::list<int> b = {10, 20, 30, 40, 50};
    printList("original:           ", b);

    // Insert before iterator position — O(1)
    auto pos = b.begin();
    std::advance(pos, 2);      // point to 30
    b.insert(pos, 99);         // insert 99 before 30
    printList("insert 99 before 30:", b);

    // Insert multiple
    b.insert(pos, {11, 12});
    printList("insert {11,12}:     ", b);

    // Erase at position — O(1)
    pos = b.begin();
    std::advance(pos, 1);
    b.erase(pos);              // erase second element
    printList("erase [1]:          ", b);

    // Erase range
    auto first = b.begin();
    auto last  = b.begin();
    std::advance(last, 3);
    b.erase(first, last);
    printList("erase [0..3):       ", b);

    b.clear();
    std::cout << "after clear — empty: " << b.empty() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: List-Specific Algorithms\n";
    std::cout << "=====================================================\n";

    std::list<int> c = {5, 3, 8, 1, 9, 2, 7};

    // sort — std::sort does NOT work on list (no random access)
    // Use list's own sort — O(n log n)
    c.sort();
    printList("sorted (asc):  ", c);

    c.sort(std::greater<int>());
    printList("sorted (desc): ", c);

    // reverse — O(n)
    c.reverse();
    printList("reversed:      ", c);

    // unique — removes consecutive duplicates (list must be sorted first)
    std::list<int> d = {1, 1, 2, 3, 3, 3, 4, 5, 5};
    d.unique();
    printList("unique:        ", d);

    // unique with predicate
    std::list<int> e = {1, 3, 2, 4, 6, 8, 5, 7};
    e.sort();
    e.unique([](int a, int b) { return a == b; });
    printList("unique(pred):  ", e);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Merge — combine two sorted lists\n";
    std::cout << "=====================================================\n";

    std::list<int> list1 = {1, 3, 5, 7, 9};
    std::list<int> list2 = {2, 4, 6, 8, 10};

    printList("list1: ", list1);
    printList("list2: ", list2);

    list1.merge(list2);   // list2 becomes empty, list1 contains all sorted
    printList("merged: ", list1);
    std::cout << "list2 after merge: " << (list2.empty() ? "empty" : "not empty") << '\n';

    // merge with custom comparator
    std::list<int> la = {9, 7, 5, 3, 1};
    std::list<int> lb = {10, 8, 6, 4, 2};
    la.merge(lb, std::greater<int>());
    printList("merged desc: ", la);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Splice — move elements between lists\n";
    std::cout << "=====================================================\n";

    std::list<int> src  = {100, 200, 300, 400, 500};
    std::list<int> dest = {1, 2, 3};

    printList("src:  ", src);
    printList("dest: ", dest);

    // splice entire src into dest at position 1
    auto destPos = dest.begin();
    std::advance(destPos, 1);   // point to '2'
    dest.splice(destPos, src);  // moves all of src into dest
    printList("dest after splice: ", dest);
    std::cout << "src after splice:  " << src.size() << " elements (moved)\n";

    // splice a single element
    std::list<int> p = {10, 20, 30, 40};
    std::list<int> q = {1, 2, 3};
    auto pIt = p.begin();
    std::advance(pIt, 2);   // point to 30
    q.splice(q.end(), p, pIt);   // move 30 from p to end of q
    printList("p after single splice: ", p);
    printList("q after single splice: ", q);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Remove and remove_if\n";
    std::cout << "=====================================================\n";

    std::list<int> f = {1, 2, 3, 2, 4, 2, 5};
    f.remove(2);   // removes ALL occurrences of 2 — O(n)
    printList("after remove(2):       ", f);

    std::list<int> g = {1, 2, 3, 4, 5, 6, 7, 8};
    g.remove_if([](int x) { return x % 2 == 0; });   // remove all evens
    printList("after remove_if(even): ", g);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: List of Objects\n";
    std::cout << "=====================================================\n";

    std::list<Employee> employees;
    employees.push_back({1, "Kostas",  "Engineering"});
    employees.push_back({2, "Maria",   "Marketing"});
    employees.push_back({3, "Andre",   "Engineering"});
    employees.push_back({4, "Sophie",  "HR"});
    employees.push_back({5, "Nick",    "Engineering"});

    std::cout << "All employees:\n";
    for (const auto& e : employees) e.print();

    // Sort by name
    employees.sort([](const Employee& a, const Employee& b) {
        return a.name < b.name;
    });
    std::cout << "\nSorted by name:\n";
    for (const auto& e : employees) e.print();

    // Remove HR department
    employees.remove_if([](const Employee& e) {
        return e.department == "HR";
    });
    std::cout << "\nAfter removing HR:\n";
    for (const auto& e : employees) e.print();

    // Find by id using iterator
    auto found = std::find_if(employees.begin(), employees.end(),
                              [](const Employee& e) { return e.id == 3; });
    if (found != employees.end()) {
        std::cout << "\nFound employee: ";
        found->print();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: List vs Vector vs Deque\n";
    std::cout << "=====================================================\n";

    std::cout << "Operation           | vector  | deque   | list\n";
    std::cout << "push_back           | O(1)    | O(1)    | O(1)\n";
    std::cout << "push_front          | O(n)    | O(1)    | O(1)\n";
    std::cout << "pop_front           | O(n)    | O(1)    | O(1)\n";
    std::cout << "insert (with iter)  | O(n)    | O(n)    | O(1)  <- list wins\n";
    std::cout << "erase  (with iter)  | O(n)    | O(n)    | O(1)  <- list wins\n";
    std::cout << "random access []    | O(1)    | O(1)    | NONE  <- list loses\n";
    std::cout << "search              | O(n)    | O(n)    | O(n)\n";
    std::cout << "sort                | O(nlogn)| O(nlogn)| O(nlogn)\n";
    std::cout << "splice              | NO      | NO      | O(1)  <- list only\n";
    std::cout << "merge (sorted)      | NO      | NO      | O(n)  <- list only\n";
    std::cout << "memory              | contiguous|chunks | scattered\n";
    std::cout << "cache performance   | best    | medium  | worst\n";

    std::cout << "\n=== ListSTL complete ===\n";
    return 0;
}
