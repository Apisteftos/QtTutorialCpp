#include <iostream>
#include <deque>
#include <algorithm>
#include <numeric>
#include <string>
#include <print>
#include <vector>

// ============================================================
// std::deque — Double-Ended Queue in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  Push/pop at both ends
//   3.  Element access
//   4.  Insert and erase in the middle
//   5.  Iterators
//   6.  Sorting and searching
//   7.  Deque as a sliding window
//   8.  Deque vs vector comparison
//   9.  Deque of objects
//  10.  C++20 ranges with deque
// ============================================================

struct Task {
    int         priority;
    std::string name;

    void print() const {
        std::println("  Task [pri:{}] '{}'", priority, name);
    }
};

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    std::deque<int> empty;
    std::deque<int> sized(5);              // 5 elements, value 0
    std::deque<int> filled(5, 42);         // 5 elements, value 42
    std::deque<int> fromList = {1,2,3,4,5};
    std::deque<int> copy = fromList;

    std::cout << "empty size:   " << empty.size() << '\n';
    std::cout << "sized:        ";
    for (int x : sized)    std::cout << x << ' '; std::cout << '\n';
    std::cout << "filled:       ";
    for (int x : filled)   std::cout << x << ' '; std::cout << '\n';
    std::cout << "fromList:     ";
    for (int x : fromList) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Push and Pop at Both Ends\n";
    std::cout << "=====================================================\n";

    std::deque<int> dq;

    // Push to back
    dq.push_back(10);
    dq.push_back(20);
    dq.push_back(30);
    std::cout << "after push_back(10,20,30):  ";
    for (int x : dq) std::cout << x << ' '; std::cout << '\n';

    // Push to FRONT (key feature — O(1), unlike vector)
    dq.push_front(5);
    dq.push_front(1);
    std::cout << "after push_front(5,1):      ";
    for (int x : dq) std::cout << x << ' '; std::cout << '\n';

    // emplace versions (construct in place)
    dq.emplace_back(40);
    dq.emplace_front(0);
    std::cout << "after emplace front/back:   ";
    for (int x : dq) std::cout << x << ' '; std::cout << '\n';

    // Pop from back
    dq.pop_back();
    std::cout << "after pop_back():           ";
    for (int x : dq) std::cout << x << ' '; std::cout << '\n';

    // Pop from FRONT (O(1), unlike vector)
    dq.pop_front();
    std::cout << "after pop_front():          ";
    for (int x : dq) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Element Access\n";
    std::cout << "=====================================================\n";

    std::deque<int> a = {10, 20, 30, 40, 50};

    std::cout << "a[2]:       " << a[2]      << '\n';   // no bounds check
    std::cout << "a.at(2):    " << a.at(2)   << '\n';   // bounds checked
    std::cout << "a.front():  " << a.front() << '\n';   // first
    std::cout << "a.back():   " << a.back()  << '\n';   // last

    // Modify
    a.front() = 999;
    a.back()  = 888;
    std::cout << "after modify front/back: ";
    for (int x : a) std::cout << x << ' '; std::cout << '\n';

    // Bounds check exception
    try {
        int bad = a.at(100);
        (void)bad;
    } catch (const std::out_of_range& e) {
        std::cout << "at(100) threw: " << e.what() << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Insert and Erase in the Middle\n";
    std::cout << "=====================================================\n";

    std::deque<int> b = {10, 20, 30, 40, 50};

    // Insert at position
    b.insert(b.begin() + 2, 99);
    std::cout << "after insert at [2]:   ";
    for (int x : b) std::cout << x << ' '; std::cout << '\n';

    // Insert multiple
    b.insert(b.begin() + 1, {11, 12, 13});
    std::cout << "after insert 3 at [1]: ";
    for (int x : b) std::cout << x << ' '; std::cout << '\n';

    // Erase at position
    b.erase(b.begin() + 3);
    std::cout << "after erase [3]:       ";
    for (int x : b) std::cout << x << ' '; std::cout << '\n';

    // Erase range
    b.erase(b.begin(), b.begin() + 2);
    std::cout << "after erase [0..2):    ";
    for (int x : b) std::cout << x << ' '; std::cout << '\n';

    // Clear
    b.clear();
    std::cout << "after clear — empty: " << b.empty() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Size and Capacity\n";
    std::cout << "=====================================================\n";

    std::deque<int> c = {1,2,3,4,5};
    std::cout << "size():     " << c.size()     << '\n';
    std::cout << "max_size(): " << c.max_size() << '\n';
    std::cout << "empty():    " << c.empty()    << '\n';

    c.resize(8, -1);   // extend with -1
    std::cout << "after resize(8,-1): ";
    for (int x : c) std::cout << x << ' '; std::cout << '\n';

    c.resize(3);        // shrink
    std::cout << "after resize(3):    ";
    for (int x : c) std::cout << x << ' '; std::cout << '\n';

    // Note: deque has NO reserve() or capacity() — different internal structure


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Iterators\n";
    std::cout << "=====================================================\n";

    std::deque<int> d = {5, 3, 8, 1, 9};

    std::cout << "range-based: ";
    for (const auto& x : d) std::cout << x << ' '; std::cout << '\n';

    std::cout << "forward:     ";
    for (auto it = d.begin(); it != d.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    std::cout << "reverse:     ";
    for (auto it = d.rbegin(); it != d.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Sorting and Searching\n";
    std::cout << "=====================================================\n";

    std::deque<int> e = {5, 3, 8, 1, 9, 2, 7};

    std::sort(e.begin(), e.end());
    std::cout << "sorted:   ";
    for (int x : e) std::cout << x << ' '; std::cout << '\n';

    auto it = std::find(e.begin(), e.end(), 7);
    if (it != e.end())
        std::println("find(7) at index: {}", std::distance(e.begin(), it));

    std::cout << "sum: " << std::accumulate(e.begin(), e.end(), 0) << '\n';
    std::cout << "min: " << *std::min_element(e.begin(), e.end()) << '\n';
    std::cout << "max: " << *std::max_element(e.begin(), e.end()) << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Sliding Window — practical deque use case\n";
    std::cout << "=====================================================\n";

    // Process a stream of values, keep only the last N
    std::vector<int> stream = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    const int WINDOW = 4;
    std::deque<int> window;

    std::cout << "Stream with window=" << WINDOW << ":\n";
    for (int val : stream) {
        window.push_back(val);          // add new value to back
        if ((int)window.size() > WINDOW)
            window.pop_front();         // remove oldest from front

        int sum = std::accumulate(window.begin(), window.end(), 0);
        std::cout << "  added " << val << " -> window: [";
        for (int i = 0; i < (int)window.size(); ++i) {
            std::cout << window[i];
            if (i < (int)window.size()-1) std::cout << ',';
        }
        std::cout << "] sum=" << sum << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Deque of Objects (Task Queue)\n";
    std::cout << "=====================================================\n";

    std::deque<Task> taskQueue;

    // Add normal priority tasks to back
    taskQueue.push_back({2, "Process data"});
    taskQueue.push_back({2, "Save results"});
    taskQueue.push_back({2, "Send report"});

    // Add high priority tasks to front
    taskQueue.push_front({1, "URGENT: fix crash"});
    taskQueue.push_front({1, "URGENT: security patch"});

    std::cout << "Task queue (front=highest priority):\n";
    for (const auto& t : taskQueue) t.print();

    std::cout << "\nProcessing tasks:\n";
    while (!taskQueue.empty()) {
        Task t = taskQueue.front();
        taskQueue.pop_front();
        std::println("  [processing] '{}'", t.name);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Deque vs Vector\n";
    std::cout << "=====================================================\n";

    std::cout << "Operation          | vector    | deque\n";
    std::cout << "push_back          | O(1) amort| O(1) amort\n";
    std::cout << "push_front         | O(n)      | O(1) amort  <- deque wins\n";
    std::cout << "pop_front          | O(n)      | O(1)        <- deque wins\n";
    std::cout << "random access []   | O(1)      | O(1)\n";
    std::cout << "insert middle      | O(n)      | O(n)\n";
    std::cout << "cache friendliness | High      | Medium      <- vector wins\n";
    std::cout << "reserve/capacity   | YES       | NO\n";
    std::cout << "contiguous memory  | YES       | NO          <- vector wins\n";
    std::cout << "data() raw pointer | YES       | NO\n";

    std::cout << "\n=== DequeSTL complete ===\n";
    return 0;
}
