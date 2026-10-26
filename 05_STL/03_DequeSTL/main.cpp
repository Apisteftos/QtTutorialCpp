#include <algorithm>
#include <cstddef>
#include <deque>
#include <iterator>
#include <numeric>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
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

    std::deque<int> empty;
    std::deque<int> sized(5);              // 5 elements, value 0
    std::deque<int> filled(5, 42);         // 5 elements, value 42
    std::deque<int> fromList = {1, 2, 3, 4, 5};
    std::deque<int> copy = fromList;

    std::println("empty size:   {}", empty.size());
    printAll("sized:        ", sized);
    printAll("filled:       ", filled);
    printAll("fromList:     ", fromList);
    printAll("copy:         ", copy);


    // === EXAMPLE 2: Push and Pop at Both Ends ===
    banner("Example 2: Push and Pop at Both Ends");

    std::deque<int> dq;

    // Push to back
    dq.push_back(10);
    dq.push_back(20);
    dq.push_back(30);
    printAll("after push_back(10,20,30):  ", dq);

    // Push to FRONT (key feature — O(1), unlike vector)
    dq.push_front(5);
    dq.push_front(1);
    printAll("after push_front(5,1):      ", dq);

    // emplace versions (construct in place)
    dq.emplace_back(40);
    dq.emplace_front(0);
    printAll("after emplace front/back:   ", dq);

    // Pop from back
    dq.pop_back();
    printAll("after pop_back():           ", dq);

    // Pop from FRONT (O(1), unlike vector)
    dq.pop_front();
    printAll("after pop_front():          ", dq);


    // === EXAMPLE 3: Element Access ===
    banner("Example 3: Element Access");

    std::deque<int> a = {10, 20, 30, 40, 50};

    std::println("a[2]:       {}", a[2]);       // no bounds check
    std::println("a.at(2):    {}", a.at(2));    // bounds checked
    std::println("a.front():  {}", a.front());  // first
    std::println("a.back():   {}", a.back());   // last

    // Modify
    a.front() = 999;
    a.back()  = 888;
    printAll("after modify front/back: ", a);

    // Bounds check exception
    try {
        int bad = a.at(100);
        (void)bad;
    } catch (const std::out_of_range& e) {
        std::println("at(100) threw: {}", e.what());
    }


    // === EXAMPLE 4: Insert and Erase in the Middle ===
    banner("Example 4: Insert and Erase in the Middle");

    std::deque<int> b = {10, 20, 30, 40, 50};

    // Insert at position
    b.insert(b.begin() + 2, 99);
    printAll("after insert at [2]:   ", b);

    // Insert multiple
    b.insert(b.begin() + 1, {11, 12, 13});
    printAll("after insert 3 at [1]: ", b);

    // Erase at position
    b.erase(b.begin() + 3);
    printAll("after erase [3]:       ", b);

    // Erase range
    b.erase(b.begin(), b.begin() + 2);
    printAll("after erase [0..2):    ", b);

    // Clear
    b.clear();
    std::println("after clear — empty: {}", b.empty());


    // === EXAMPLE 5: Size and Capacity ===
    banner("Example 5: Size and Capacity");

    std::deque<int> c = {1, 2, 3, 4, 5};
    std::println("size():     {}", c.size());
    std::println("max_size(): {}", c.max_size());
    std::println("empty():    {}", c.empty());

    c.resize(8, -1);   // extend with -1
    printAll("after resize(8,-1): ", c);

    c.resize(3);        // shrink
    printAll("after resize(3):    ", c);

    // Note: deque has NO reserve() or capacity() — different internal structure


    // === EXAMPLE 6: Iterators ===
    banner("Example 6: Iterators");

    std::deque<int> d = {5, 3, 8, 1, 9};

    printAll("range-based: ", d);

    std::print("forward:     ");
    for (auto it = d.begin(); it != d.end(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");

    std::print("reverse:     ");
    for (auto it = d.rbegin(); it != d.rend(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");


    // === EXAMPLE 7: Sorting and Searching ===
    banner("Example 7: Sorting and Searching");

    std::deque<int> e = {5, 3, 8, 1, 9, 2, 7};

    std::sort(e.begin(), e.end());
    printAll("sorted:   ", e);

    auto found = std::find(e.begin(), e.end(), 7);
    if (found != e.end()) {
        std::println("find(7) at index: {}", std::distance(e.begin(), found));
    }

    std::println("sum: {}", std::accumulate(e.begin(), e.end(), 0));
    std::println("min: {}", *std::min_element(e.begin(), e.end()));
    std::println("max: {}", *std::max_element(e.begin(), e.end()));


    // === EXAMPLE 8: Sliding Window — practical deque use case ===
    banner("Example 8: Sliding Window — practical deque use case");

    // Process a stream of values, keep only the last N
    std::vector<int> stream = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    constexpr std::size_t WINDOW = 4;
    std::deque<int> window;

    std::println("Stream with window={}:", WINDOW);
    for (int val : stream) {
        window.push_back(val);          // add new value to back
        if (window.size() > WINDOW) {
            window.pop_front();         // remove oldest from front
        }

        int sum = std::accumulate(window.begin(), window.end(), 0);
        std::print("  added {} -> window: [", val);
        for (std::size_t i = 0; i < window.size(); ++i) {
            std::print("{}", window[i]);
            if (i + 1 < window.size()) {
                std::print(",");
            }
        }
        std::println("] sum={}", sum);
    }


    // === EXAMPLE 9: Deque of Objects (Task Queue) ===
    banner("Example 9: Deque of Objects (Task Queue)");

    std::deque<Task> taskQueue;

    // Add normal priority tasks to back
    taskQueue.push_back({2, "Process data"});
    taskQueue.push_back({2, "Save results"});
    taskQueue.push_back({2, "Send report"});

    // Add high priority tasks to front
    taskQueue.push_front({1, "URGENT: fix crash"});
    taskQueue.push_front({1, "URGENT: security patch"});

    std::println("Task queue (front=highest priority):");
    for (const auto& t : taskQueue) {
        t.print();
    }

    std::println("");
    std::println("Processing tasks:");
    while (!taskQueue.empty()) {
        Task t = std::move(taskQueue.front());
        taskQueue.pop_front();
        std::println("  [processing] '{}'", t.name);
    }


    // === EXAMPLE 10: Deque vs Vector ===
    banner("Example 10: Deque vs Vector");

    std::println("Operation          | vector    | deque");
    std::println("push_back          | O(1) amort| O(1)");
    std::println("push_front         | O(n)      | O(1)        <- deque wins");
    std::println("pop_front          | O(n)      | O(1)        <- deque wins");
    std::println("random access []   | O(1)      | O(1)");
    std::println("insert middle      | O(n)      | O(n)");
    std::println("cache friendliness | High      | Medium      <- vector wins");
    std::println("reserve/capacity   | YES       | NO");
    std::println("contiguous memory  | YES       | NO          <- vector wins");
    std::println("data() raw pointer | YES       | NO");

    std::println("");
    std::println("=== DequeSTL complete ===");
    return 0;
}
