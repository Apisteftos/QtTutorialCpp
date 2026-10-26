#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <print>
#include <queue>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ============================================================
// std::priority_queue — Heap-Based Queue in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization (max-heap by default)
//   2.  push, pop, top, empty, size
//   3.  Min-heap (reverse order)
//   4.  Custom comparator with lambda
//   5.  Practical: task scheduler by priority
//   6.  Practical: Dijkstra shortest path
//   7.  Practical: top-K elements
//   8.  Priority queue of objects
//   9.  Heap operations: make_heap, push_heap, pop_heap
// ============================================================

// Print the "=====" banner that introduces an example
void banner(std::string_view title)
{
    std::println("");
    std::println("=====================================================");
    std::println("{}", title);
    std::println("=====================================================");
}

// ============================================================
// Example 5 — Task scheduler by priority
// ============================================================
struct Task {
    int         priority;   // lower = more urgent
    std::string name;

    // Used by std::greater<Task>: min-heap (lowest priority number = highest urgency)
    bool operator>(const Task& other) const {
        return priority > other.priority;
    }
};

// ============================================================
// Example 6 — Dijkstra shortest path
// ============================================================
std::vector<int> dijkstra(int src, const std::vector<std::vector<std::pair<int,int>>>& graph)
{
    std::size_t n = graph.size();
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    dist[src] = 0;

    // Min-heap: {distance, node}
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<std::pair<int,int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;   // outdated entry

        for (auto [w, v] : graph[u]) {   // graph edges are {weight, destination}
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}


int main()
{
    // === EXAMPLE 1: Declaration and Initialization ===
    banner("Example 1: Declaration and Initialization");

    // Default: max-heap
    std::priority_queue<int> maxHeap;
    std::println("maxHeap empty: {}", maxHeap.empty());

    // From vector
    std::vector<int> vals = {3, 1, 4, 1, 5, 9, 2, 6};
    std::priority_queue<int> fromVec(vals.begin(), vals.end());

    std::println("fromVec top (max): {}", fromVec.top());
    std::println("fromVec size:      {}", fromVec.size());

    // Min-heap
    std::priority_queue<int,
                        std::vector<int>,
                        std::greater<int>> minHeap(vals.begin(), vals.end());
    std::println("minHeap top (min): {}", minHeap.top());


    // === EXAMPLE 2: push, pop, top, empty, size ===
    banner("Example 2: push, pop, top, empty, size");

    std::priority_queue<int> pq;

    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    pq.push(40);

    std::println("pushed: 30 10 50 20 40");
    std::println("size:   {}", pq.size());
    std::println("top:    {} (max)", pq.top());

    std::print("popping in order: ");
    while (!pq.empty()) {
        std::print("{} ", pq.top());
        pq.pop();
    }
    std::println("");
    std::println("(always highest first — regardless of push order)");


    // === EXAMPLE 3: Min-Heap ===
    banner("Example 3: Min-Heap");

    // Three-argument form: <type, container, comparator>
    std::priority_queue<int,
                        std::vector<int>,
                        std::greater<int>> minPq;

    minPq.push(30);
    minPq.push(10);
    minPq.push(50);
    minPq.push(20);

    std::println("min-heap top: {} (smallest)", minPq.top());
    std::print("popping: ");
    while (!minPq.empty()) {
        std::print("{} ", minPq.top());
        minPq.pop();
    }
    std::println("");
    std::println("(always lowest first)");


    // === EXAMPLE 4: Custom Comparator with Lambda ===
    banner("Example 4: Custom Comparator with Lambda");

    // comp(a, b) == true means: a has LOWER priority than b (b comes out first).
    // Here longer strings have higher priority -> longest string first.
    auto cmp = [](const std::string& lhs, const std::string& rhs) {
        return lhs.size() < rhs.size();
    };

    std::priority_queue<std::string,
                        std::vector<std::string>,
                        decltype(cmp)> strPq(cmp);

    strPq.push("cat");
    strPq.push("elephant");
    strPq.push("dog");
    strPq.push("hippopotamus");
    strPq.push("ox");

    std::println("popping by length (longest first):");
    while (!strPq.empty()) {
        std::println("  '{}' (len {})", strPq.top(), strPq.top().size());
        strPq.pop();
    }


    // === EXAMPLE 5: Task Scheduler by Priority ===
    banner("Example 5: Task Scheduler by Priority");

    // Min-heap: lowest priority NUMBER = highest urgency
    std::priority_queue<Task,
                        std::vector<Task>,
                        std::greater<Task>> scheduler;

    scheduler.push({3, "Write documentation"});
    scheduler.push({1, "CRITICAL: fix crash"});
    scheduler.push({2, "Review pull request"});
    scheduler.push({1, "CRITICAL: security patch"});
    scheduler.push({4, "Update README"});

    std::println("Executing tasks (priority 1 = most urgent):");
    while (!scheduler.empty()) {
        const Task& t = scheduler.top();
        std::println("  [pri {}] {}", t.priority, t.name);
        scheduler.pop();   // t must not be used after this line
    }


    // === EXAMPLE 6: Dijkstra Shortest Path ===
    banner("Example 6: Dijkstra Shortest Path");

    // Graph: {weight, destination}
    // 0 --1-- 1 --2-- 3
    // |       |
    // 4       1
    // |       |
    // 2 --1-- 4
    int n = 5;
    std::vector<std::vector<std::pair<int,int>>> graph(n);
    graph[0].push_back({1, 1}); graph[1].push_back({1, 0});
    graph[0].push_back({4, 2}); graph[2].push_back({4, 0});
    graph[1].push_back({2, 3}); graph[3].push_back({2, 1});
    graph[1].push_back({1, 4}); graph[4].push_back({1, 1});
    graph[2].push_back({1, 4}); graph[4].push_back({1, 2});

    auto dist = dijkstra(0, graph);
    std::println("Shortest distances from node 0:");
    for (int i = 0; i < n; ++i) {
        std::println("  0 -> {}: {}", i, dist[i]);
    }


    // === EXAMPLE 7: Top-K Elements ===
    banner("Example 7: Top-K Elements");

    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 7};
    constexpr std::size_t K = 4;

    // Find top-K largest using min-heap of size K
    std::priority_queue<int, std::vector<int>, std::greater<int>> topK;

    for (int x : data) {
        topK.push(x);
        if (topK.size() > K) {
            topK.pop();   // remove smallest — keep only K largest
        }
    }

    std::println("Top {} largest from {{3,1,4,1,5,9,2,6,5,3,5,8,7}}:", K);
    std::vector<int> result;
    while (!topK.empty()) {
        result.push_back(topK.top());
        topK.pop();
    }
    std::print("  ");
    for (int x : result) {
        std::print("{} ", x);
    }
    std::println("");


    // === EXAMPLE 8: Priority Queue of Objects ===
    banner("Example 8: Priority Queue of Objects (support tickets)");

    struct Ticket {
        int         priority;   // 1=urgent, 2=normal, 3=low
        std::string id;
        std::string type;

        // priority_queue puts the element that is "largest" according to operator< on top.
        // We want the LOWEST number on top, so "less" means "has the higher number".
        bool operator<(const Ticket& other) const {
            return priority > other.priority;
        }
    };

    std::priority_queue<Ticket> ticketQueue;

    ticketQueue.push({2, "TKT-001", "FEATURE_REQUEST"});
    ticketQueue.push({1, "TKT-042", "CRASH_REPORT"});
    ticketQueue.push({3, "TKT-007", "TYPO_FIX"});
    ticketQueue.push({1, "TKT-043", "SECURITY_ISSUE"});
    ticketQueue.push({2, "TKT-002", "DOCS_UPDATE"});

    std::println("Ticket queue (priority order):");
    while (!ticketQueue.empty()) {
        const auto& t = ticketQueue.top();
        std::println("  [pri {}] {} - {}", t.priority, t.id, t.type);
        ticketQueue.pop();   // t must not be used after this line
    }


    // === EXAMPLE 9: std::make_heap, push_heap, pop_heap ===
    banner("Example 9: std::make_heap, push_heap, pop_heap");

    // Heap operations directly on a vector
    std::vector<int> heapVec = {3, 1, 4, 1, 5, 9, 2, 6};

    std::make_heap(heapVec.begin(), heapVec.end());
    std::println("after make_heap, max: {}", heapVec.front());

    heapVec.push_back(10);
    std::push_heap(heapVec.begin(), heapVec.end());
    std::println("after push_heap(10), max: {}", heapVec.front());

    std::pop_heap(heapVec.begin(), heapVec.end());
    int maxVal = heapVec.back();
    heapVec.pop_back();
    std::println("popped: {} new max: {}", maxVal, heapVec.front());

    std::sort_heap(heapVec.begin(), heapVec.end());
    std::print("sort_heap result: ");
    for (int x : heapVec) {
        std::print("{} ", x);
    }
    std::println("");

    std::println("");
    std::println("=== PriorityQueueSTL complete ===");
    return 0;
}
