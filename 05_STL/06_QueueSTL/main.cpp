#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <print>
#include <queue>
#include <list>

// ============================================================
// std::queue — FIFO Container Adapter in C++23
// ============================================================
// Topics covered:
//   1.  What is a queue (FIFO)
//   2.  Declaration and initialization
//   3.  push, pop, front, back, empty, size
//   4.  Queue with different underlying containers
//   5.  Practical: task processing system
//   6.  Practical: BFS (breadth-first search)
//   7.  Practical: print spooler simulation
//   8.  Queue of objects
//   9.  Queue vs stack vs deque
// ============================================================

// ============================================================
// Example 6 — BFS (Breadth-First Search)
// ============================================================
void bfs(int start, const std::vector<std::vector<int>>& graph)
{
    std::vector<bool> visited(graph.size(), false);
    std::queue<int>   q;

    q.push(start);
    visited[start] = true;

    std::cout << "BFS from node " << start << ": ";
    while (!q.empty()) {
        int node = q.front(); q.pop();
        std::cout << node << ' ';

        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    std::cout << '\n';
}


// ============================================================
// Example 7 — Print spooler simulation
// ============================================================
struct PrintJob {
    int         id;
    std::string document;
    int         pages;

    void print() const {
        std::println("  Job #{}: '{}' ({} pages)", id, document, pages);
    }
};

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What is a Queue (FIFO)\n";
    std::cout << "=====================================================\n";

    std::cout << "FIFO = First In, First Out\n";
    std::cout << "Think of a queue at a ticket office:\n";
    std::cout << "  push (enqueue) -> joins at the BACK\n";
    std::cout << "  pop  (dequeue) -> leaves from the FRONT\n";
    std::cout << "First person to arrive is first to be served\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    // Default underlying container is std::deque
    std::queue<int> q1;

    // Initialize from container
    std::deque<int> dq = {10, 20, 30, 40, 50};
    std::queue<int> q2(dq);

    std::cout << "q1 empty:  " << q1.empty() << '\n';
    std::cout << "q2 size:   " << q2.size()  << '\n';
    std::cout << "q2 front:  " << q2.front() << '\n';  // 10 — first in
    std::cout << "q2 back:   " << q2.back()  << '\n';  // 50 — last in


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: push, pop, front, back, empty, size\n";
    std::cout << "=====================================================\n";

    std::queue<int> q;

    // push — adds to BACK
    q.push(10);
    q.push(20);
    q.push(30);
    q.emplace(40);   // construct in place

    std::cout << "after push 10,20,30,40:\n";
    std::cout << "  size:  " << q.size()  << '\n';
    std::cout << "  front: " << q.front() << '\n';  // 10 — first in
    std::cout << "  back:  " << q.back()  << '\n';  // 40 — last in

    // pop — removes from FRONT (FIFO order)
    std::cout << "dequeuing: ";
    while (!q.empty()) {
        std::cout << q.front() << ' ';
        q.pop();
    }
    std::cout << "\n(10 came out first — first in, first out)\n";

    // Modify front and back
    q.push(100); q.push(200); q.push(300);
    q.front() = 999;   // modify first element
    q.back()  = 888;   // modify last element
    std::cout << "after modifying front/back: front=" << q.front()
              << " back=" << q.back() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Different Underlying Containers\n";
    std::cout << "=====================================================\n";

    // Default: deque
    std::queue<int, std::deque<int>> qDeque;
    qDeque.push(1); qDeque.push(2); qDeque.push(3);
    std::cout << "deque-backed front: " << qDeque.front()
              << " back: " << qDeque.back() << '\n';

    // List-backed (no vector — vector has no push_front)
    std::queue<int, std::list<int>> qList;
    qList.push(1); qList.push(2); qList.push(3);
    std::cout << "list-backed  front: " << qList.front()
              << " back: " << qList.back() << '\n';

    std::cout << "Note: std::vector cannot back a queue\n";
    std::cout << "      (queue needs push_front which vector lacks)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Task Processing System\n";
    std::cout << "=====================================================\n";

    std::queue<std::string> taskQueue;

    // Add tasks
    taskQueue.push("Download file");
    taskQueue.push("Parse XML");
    taskQueue.push("Validate data");
    taskQueue.push("Save to database");
    taskQueue.push("Send notification");

    std::cout << "Processing " << taskQueue.size() << " tasks in order:\n";
    int taskNum = 1;
    while (!taskQueue.empty()) {
        std::string task = taskQueue.front();
        taskQueue.pop();
        std::println("  [{}] Processing: '{}'", taskNum++, task);
    }
    std::cout << "All tasks done\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: BFS — Breadth-First Search\n";
    std::cout << "=====================================================\n";

    //   0 - 1 - 3
    //   |   |
    //   2 - 4 - 5
    std::vector<std::vector<int>> graph = {
        {1, 2},      // 0
        {0, 3, 4},   // 1
        {0, 4},      // 2
        {1},         // 3
        {1, 2, 5},   // 4
        {4}          // 5
    };

    bfs(0, graph);
    std::cout << "(BFS visits level-by-level — unlike DFS with stack)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Print Spooler Simulation\n";
    std::cout << "=====================================================\n";

    std::queue<PrintJob> spooler;

    // Jobs arrive
    spooler.push({1, "Annual Report",    45});
    spooler.push({2, "Invoice_2026.pdf", 2});
    spooler.push({3, "Project Plan",     12});
    spooler.push({4, "Meeting Notes",    3});

    std::cout << "Print queue (" << spooler.size() << " jobs):\n";
    {
        std::queue<PrintJob> temp = spooler;
        while (!temp.empty()) { temp.front().print(); temp.pop(); }
    }

    std::cout << "\nPrinting jobs:\n";
    while (!spooler.empty()) {
        PrintJob job = spooler.front(); spooler.pop();
        std::println("  [printing] '{}' ({} pages)", job.document, job.pages);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Queue of Objects — MCX Message Queue\n";
    std::cout << "=====================================================\n";

    struct McxMessage {
        int         id;
        std::string type;
        std::string payload;
    };

    std::queue<McxMessage> messageQueue;

    // Incoming MCX messages
    messageQueue.push({1, "GROUP_CALL_REQUEST",  "group_id=42"});
    messageQueue.push({2, "FLOOR_REQUEST",        "user_id=7"});
    messageQueue.push({3, "FLOOR_GRANTED",        "user_id=7"});
    messageQueue.push({4, "GROUP_CALL_RELEASE",   "group_id=42"});

    std::cout << "Processing MCX message queue:\n";
    while (!messageQueue.empty()) {
        auto& msg = messageQueue.front();
        std::println("  [{}] {} | {}", msg.id, msg.type, msg.payload);
        messageQueue.pop();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Queue characteristics\n";
    std::cout << "=====================================================\n";

    std::cout << "std::queue is a CONTAINER ADAPTER\n";
    std::cout << "  wraps deque (default) or list\n";
    std::cout << "  exposes only: push, pop, front, back, empty, size\n";
    std::cout << "  NO iterators, NO random access, NO search\n\n";

    std::cout << "Queue vs Stack vs Deque:\n";
    std::cout << "  queue  — FIFO (first in, first out)\n";
    std::cout << "  stack  — LIFO (last in, first out)\n";
    std::cout << "  deque  — both ends, random access, full interface\n\n";

    std::cout << "Real-world uses:\n";
    std::cout << "  - Task/job processing (process in order received)\n";
    std::cout << "  - BFS graph traversal\n";
    std::cout << "  - Message queues (MCX, network packets)\n";
    std::cout << "  - Print spooler\n";
    std::cout << "  - Producer/consumer buffer\n";
    std::cout << "  - Event queues in Qt (QCoreApplication event loop)\n";

    std::cout << "\n=== QueueSTL complete ===\n";
    return 0;
}
