#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <functional>
#include <print>
#include <climits>

// ============================================================
// std::priority_queue — Heap-Based Queue in C++23
// ============================================================
// Topics covered:
//   1.  What is a priority queue (max-heap by default)
//   2.  Declaration and initialization
//   3.  push, pop, top, empty, size
//   4.  Min-heap (reverse order)
//   5.  Custom comparator with struct
//   6.  Practical: task scheduler by priority
//   7.  Practical: Dijkstra shortest path
//   8.  Practical: top-K elements
//   9.  Priority queue of objects
//  10.  Heap operations: make_heap, push_heap, pop_heap
// ============================================================

// ============================================================
// Example 6 — Task scheduler by priority
// ============================================================
struct Task {
    int         priority;   // lower = more urgent
    std::string name;

    // Comparator: min-heap (lowest priority number = highest urgency)
    bool operator>(const Task& other) const {
        return priority > other.priority;
    }
};

// ============================================================
// Example 7 — Dijkstra shortest path
// ============================================================
std::vector<int> dijkstra(int src, const std::vector<std::vector<std::pair<int,int>>>& graph)
{
    int n = (int)graph.size();
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

        for (auto [w, v] : graph[u]) {
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
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What is a Priority Queue\n";
    std::cout << "=====================================================\n";

    std::cout << "std::priority_queue is a MAX-HEAP by default\n";
    std::cout << "The element with the HIGHEST value is always at top\n";
    std::cout << "Push any order — always pops in sorted (high->low) order\n";
    std::cout << "Internal structure: binary heap (complete binary tree)\n";
    std::cout << "push: O(log n)  pop: O(log n)  top: O(1)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    // Default: max-heap
    std::priority_queue<int> maxHeap;

    // From vector
    std::vector<int> vals = {3, 1, 4, 1, 5, 9, 2, 6};
    std::priority_queue<int> fromVec(vals.begin(), vals.end());

    std::cout << "fromVec top (max): " << fromVec.top() << '\n';
    std::cout << "fromVec size:      " << fromVec.size() << '\n';

    // Min-heap
    std::priority_queue<int,
                        std::vector<int>,
                        std::greater<int>> minHeap(vals.begin(), vals.end());
    std::cout << "minHeap top (min): " << minHeap.top() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: push, pop, top, empty, size\n";
    std::cout << "=====================================================\n";

    std::priority_queue<int> pq;

    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    pq.push(40);

    std::cout << "pushed: 30 10 50 20 40\n";
    std::cout << "size:   " << pq.size()  << '\n';
    std::cout << "top:    " << pq.top()   << " (max)\n";

    std::cout << "popping in order: ";
    while (!pq.empty()) {
        std::cout << pq.top() << ' ';
        pq.pop();
    }
    std::cout << "\n(always highest first — regardless of push order)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Min-Heap\n";
    std::cout << "=====================================================\n";

    // Three-argument form: <type, container, comparator>
    std::priority_queue<int,
                        std::vector<int>,
                        std::greater<int>> minPq;

    minPq.push(30);
    minPq.push(10);
    minPq.push(50);
    minPq.push(20);

    std::cout << "min-heap top: " << minPq.top() << " (smallest)\n";
    std::cout << "popping: ";
    while (!minPq.empty()) {
        std::cout << minPq.top() << ' ';
        minPq.pop();
    }
    std::cout << "\n(always lowest first)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Custom Comparator with Lambda\n";
    std::cout << "=====================================================\n";

    // Sort strings by length (shortest first = min-heap by length)
    auto cmp = [](const std::string& a, const std::string& b) {
        return a.size() < b.size();   // longer strings have higher priority
    };

    std::priority_queue<std::string,
                        std::vector<std::string>,
                        decltype(cmp)> strPq(cmp);

    strPq.push("cat");
    strPq.push("elephant");
    strPq.push("dog");
    strPq.push("hippopotamus");
    strPq.push("ox");

    std::cout << "popping by length (longest first):\n";
    while (!strPq.empty()) {
        std::println("  '{}' (len {})", strPq.top(), strPq.top().size());
        strPq.pop();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Task Scheduler by Priority\n";
    std::cout << "=====================================================\n";

    // Min-heap: lowest priority NUMBER = highest urgency
    std::priority_queue<Task,
                        std::vector<Task>,
                        std::greater<Task>> scheduler;

    scheduler.push({3, "Write documentation"});
    scheduler.push({1, "CRITICAL: fix crash"});
    scheduler.push({2, "Review pull request"});
    scheduler.push({1, "CRITICAL: security patch"});
    scheduler.push({4, "Update README"});

    std::cout << "Executing tasks (priority 1 = most urgent):\n";
    while (!scheduler.empty()) {
        const Task& t = scheduler.top();
        std::println("  [pri {}] {}", t.priority, t.name);
        scheduler.pop();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Dijkstra Shortest Path\n";
    std::cout << "=====================================================\n";

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
    std::cout << "Shortest distances from node 0:\n";
    for (int i = 0; i < n; ++i)
        std::println("  0 -> {}: {}", i, dist[i]);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Top-K Elements\n";
    std::cout << "=====================================================\n";

    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 7};
    const int K = 4;

    // Find top-K largest using min-heap of size K
    std::priority_queue<int, std::vector<int>, std::greater<int>> topK;

    for (int x : data) {
        topK.push(x);
        if ((int)topK.size() > K)
            topK.pop();   // remove smallest — keep only K largest
    }

    std::cout << "Top " << K << " largest from {3,1,4,1,5,9,2,6,5,3,5,8,7}:\n";
    std::vector<int> result;
    while (!topK.empty()) { result.push_back(topK.top()); topK.pop(); }
    std::cout << "  ";
    for (int x : result) std::cout << x << ' ';
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Priority Queue of Objects (MCX calls)\n";
    std::cout << "=====================================================\n";

    struct McxCall {
        int         priority;   // 1=emergency, 2=group, 3=individual
        std::string callId;
        std::string type;

        bool operator<(const McxCall& other) const {
            return priority > other.priority;  // lower number = higher priority
        }
    };

    std::priority_queue<McxCall> callQueue;

    callQueue.push({2, "GRP-001", "GROUP_CALL"});
    callQueue.push({1, "EMG-042", "EMERGENCY_CALL"});
    callQueue.push({3, "IND-007", "INDIVIDUAL_CALL"});
    callQueue.push({1, "EMG-043", "EMERGENCY_CALL"});
    callQueue.push({2, "GRP-002", "GROUP_CALL"});

    std::cout << "MCX call queue (priority order):\n";
    while (!callQueue.empty()) {
        const auto& c = callQueue.top();
        std::println("  [pri {}] {} - {}", c.priority, c.callId, c.type);
        callQueue.pop();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: std::make_heap, push_heap, pop_heap\n";
    std::cout << "=====================================================\n";

    // Heap operations directly on a vector
    std::vector<int> heapVec = {3, 1, 4, 1, 5, 9, 2, 6};

    std::make_heap(heapVec.begin(), heapVec.end());
    std::cout << "after make_heap, max: " << heapVec.front() << '\n';

    heapVec.push_back(10);
    std::push_heap(heapVec.begin(), heapVec.end());
    std::cout << "after push_heap(10), max: " << heapVec.front() << '\n';

    std::pop_heap(heapVec.begin(), heapVec.end());
    int maxVal = heapVec.back();
    heapVec.pop_back();
    std::cout << "popped: " << maxVal << " new max: " << heapVec.front() << '\n';

    std::sort_heap(heapVec.begin(), heapVec.end());
    std::cout << "sort_heap result: ";
    for (int x : heapVec) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "\n=== PriorityQueueSTL complete ===\n";
    return 0;
}
