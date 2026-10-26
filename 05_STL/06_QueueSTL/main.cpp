#include <deque>
#include <list>
#include <print>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

// ============================================================
// std::queue — FIFO Container Adapter in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  push, pop, front, back, empty, size
//   3.  Queue with different underlying containers
//   4.  Practical: task processing system
//   5.  Practical: BFS (breadth-first search)
//   6.  Practical: print spooler simulation
//   7.  Queue of objects
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
// Example 5 — BFS (Breadth-First Search)
// ============================================================
void bfs(int start, const std::vector<std::vector<int>>& graph)
{
    std::vector<bool> visited(graph.size(), false);
    std::queue<int>   q;

    q.push(start);
    visited[start] = true;

    std::print("BFS from node {}: ", start);
    while (!q.empty()) {
        int node = q.front(); q.pop();
        std::print("{} ", node);

        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    std::println("");
}


// ============================================================
// Example 6 — Print spooler simulation
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
    // === EXAMPLE 1: Declaration and Initialization ===
    banner("Example 1: Declaration and Initialization");

    // Default underlying container is std::deque
    std::queue<int> q1;

    // Initialize from container
    std::deque<int> dq = {10, 20, 30, 40, 50};
    std::queue<int> q2(dq);

    std::println("q1 empty:  {}", q1.empty());
    std::println("q2 size:   {}", q2.size());
    std::println("q2 front:  {}", q2.front());  // 10 — first in
    std::println("q2 back:   {}", q2.back());   // 50 — last in


    // === EXAMPLE 2: push, pop, front, back, empty, size ===
    banner("Example 2: push, pop, front, back, empty, size");

    std::queue<int> q;

    // push — adds to BACK
    q.push(10);
    q.push(20);
    q.push(30);
    q.emplace(40);   // construct in place

    std::println("after push 10,20,30,40:");
    std::println("  size:  {}", q.size());
    std::println("  front: {}", q.front());  // 10 — first in
    std::println("  back:  {}", q.back());   // 40 — last in

    // pop — removes from FRONT (FIFO order)
    std::print("dequeuing: ");
    while (!q.empty()) {
        std::print("{} ", q.front());
        q.pop();
    }
    std::println("");
    std::println("(10 came out first — first in, first out)");

    // Modify front and back
    q.push(100); q.push(200); q.push(300);
    q.front() = 999;   // modify first element
    q.back()  = 888;   // modify last element
    std::println("after modifying front/back: front={} back={}", q.front(), q.back());


    // === EXAMPLE 3: Different Underlying Containers ===
    banner("Example 3: Different Underlying Containers");

    // Default: deque
    std::queue<int, std::deque<int>> qDeque;
    qDeque.push(1); qDeque.push(2); qDeque.push(3);
    std::println("deque-backed front: {} back: {}", qDeque.front(), qDeque.back());

    // List-backed (no vector — vector has no pop_front)
    std::queue<int, std::list<int>> qList;
    qList.push(1); qList.push(2); qList.push(3);
    std::println("list-backed  front: {} back: {}", qList.front(), qList.back());


    // === EXAMPLE 4: Task Processing System ===
    banner("Example 4: Task Processing System");

    std::queue<std::string> taskQueue;

    // Add tasks
    taskQueue.push("Download file");
    taskQueue.push("Parse XML");
    taskQueue.push("Validate data");
    taskQueue.push("Save to database");
    taskQueue.push("Send notification");

    std::println("Processing {} tasks in order:", taskQueue.size());
    int taskNum = 1;
    while (!taskQueue.empty()) {
        std::string task = std::move(taskQueue.front());
        taskQueue.pop();
        std::println("  [{}] Processing: '{}'", taskNum++, task);
    }
    std::println("All tasks done");


    // === EXAMPLE 5: BFS — Breadth-First Search ===
    banner("Example 5: BFS — Breadth-First Search");

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


    // === EXAMPLE 6: Print Spooler Simulation ===
    banner("Example 6: Print Spooler Simulation");

    std::queue<PrintJob> spooler;

    // Jobs arrive
    spooler.push({1, "Annual Report",    45});
    spooler.push({2, "Invoice_2026.pdf", 2});
    spooler.push({3, "Project Plan",     12});
    spooler.push({4, "Meeting Notes",    3});

    std::println("Print queue ({} jobs):", spooler.size());
    {
        std::queue<PrintJob> temp = spooler;   // copy, so we can drain it
        while (!temp.empty()) {
            temp.front().print();
            temp.pop();
        }
    }

    std::println("");
    std::println("Printing jobs:");
    while (!spooler.empty()) {
        PrintJob job = spooler.front(); spooler.pop();
        std::println("  [printing] '{}' ({} pages)", job.document, job.pages);
    }


    // === EXAMPLE 7: Queue of Objects — Message Queue ===
    banner("Example 7: Queue of Objects — Message Queue");

    struct Message {
        int         id;
        std::string type;
        std::string payload;
    };

    std::queue<Message> messageQueue;

    // Incoming messages
    messageQueue.push({1, "LOGIN_REQUEST", "user=alice"});
    messageQueue.push({2, "DATA_UPLOAD",   "size=2048"});
    messageQueue.push({3, "UPLOAD_DONE",   "size=2048"});
    messageQueue.push({4, "LOGOUT",        "user=alice"});

    std::println("Processing message queue:");
    while (!messageQueue.empty()) {
        const auto& msg = messageQueue.front();
        std::println("  [{}] {} | {}", msg.id, msg.type, msg.payload);
        messageQueue.pop();   // msg must not be used after this line
    }

    std::println("");
    std::println("=== QueueSTL complete ===");
    return 0;
}
