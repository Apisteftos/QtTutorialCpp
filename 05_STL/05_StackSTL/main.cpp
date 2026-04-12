#include <iostream>
#include <stack>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <print>

// ============================================================
// std::stack — LIFO Container Adapter in C++23
// ============================================================
// Topics covered:
//   1.  What is a stack (LIFO)
//   2.  Declaration and initialization
//   3.  push, pop, top, empty, size
//   4.  Stack with different underlying containers
//   5.  Practical: balanced parentheses checker
//   6.  Practical: undo/redo system
//   7.  Practical: expression evaluator (postfix)
//   8.  Practical: DFS (depth-first search)
//   9.  Stack of objects
//  10.  Stack vs deque vs vector comparison
// ============================================================

// ============================================================
// Example 5 — Balanced parentheses checker
// ============================================================
bool isBalanced(const std::string& expr)
{
    std::stack<char> st;

    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char top = st.top(); st.pop();
            if (c == ')' && top != '(') return false;
            if (c == ']' && top != '[') return false;
            if (c == '}' && top != '{') return false;
        }
    }
    return st.empty();
}


// ============================================================
// Example 6 — Undo/Redo system
// ============================================================
class TextEditor {
private:
    std::string           m_text;
    std::stack<std::string> m_undoStack;
    std::stack<std::string> m_redoStack;

public:
    void type(const std::string& addition) {
        m_undoStack.push(m_text);   // save current state
        while (!m_redoStack.empty()) m_redoStack.pop();  // clear redo
        m_text += addition;
        std::println("  typed '{}' -> text: '{}'", addition, m_text);
    }

    void undo() {
        if (m_undoStack.empty()) {
            std::cout << "  nothing to undo\n"; return;
        }
        m_redoStack.push(m_text);
        m_text = m_undoStack.top();
        m_undoStack.pop();
        std::println("  undo -> text: '{}'", m_text);
    }

    void redo() {
        if (m_redoStack.empty()) {
            std::cout << "  nothing to redo\n"; return;
        }
        m_undoStack.push(m_text);
        m_text = m_redoStack.top();
        m_redoStack.pop();
        std::println("  redo -> text: '{}'", m_text);
    }

    std::string getText() const { return m_text; }
};


// ============================================================
// Example 7 — Postfix expression evaluator
// ============================================================
int evaluatePostfix(const std::string& expr)
{
    std::stack<int> st;

    for (char c : expr) {
        if (c == ' ') continue;

        if (std::isdigit(c)) {
            st.push(c - '0');
        } else {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            switch (c) {
            case '+': st.push(a + b); break;
            case '-': st.push(a - b); break;
            case '*': st.push(a * b); break;
            case '/': st.push(a / b); break;
            }
        }
    }
    return st.top();
}


// ============================================================
// Example 8 — Depth-First Search (DFS) with stack
// ============================================================
void dfsIterative(int start, const std::vector<std::vector<int>>& graph)
{
    std::vector<bool> visited(graph.size(), false);
    std::stack<int>   st;

    st.push(start);
    std::cout << "DFS from node " << start << ": ";

    while (!st.empty()) {
        int node = st.top(); st.pop();

        if (visited[node]) continue;
        visited[node] = true;
        std::cout << node << ' ';

        // Push neighbors (in reverse to maintain order)
        for (int i = (int)graph[node].size() - 1; i >= 0; --i)
            if (!visited[graph[node][i]])
                st.push(graph[node][i]);
    }
    std::cout << '\n';
}


// ============================================================
// Example 9 — Stack of objects
// ============================================================
struct StackFrame {
    std::string functionName;
    int         lineNumber;
    int         localVar;

    void print() const {
        std::println("  {} (line {}) localVar={}", functionName, lineNumber, localVar);
    }
};

int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What is a Stack (LIFO)\n";
    std::cout << "=====================================================\n";

    std::cout << "LIFO = Last In, First Out\n";
    std::cout << "Think of a stack of plates:\n";
    std::cout << "  push plate -> goes on TOP\n";
    std::cout << "  pop plate  -> comes from TOP (last added, first removed)\n";
    std::cout << "Operations: push (add), pop (remove), top (peek)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Declaration and Initialization\n";
    std::cout << "=====================================================\n";

    // Default underlying container is std::deque
    std::stack<int> s1;

    // Initialize from container
    std::deque<int> dq = {1, 2, 3, 4, 5};
    std::stack<int> s2(dq);   // top = 5 (last element of deque)

    std::cout << "s1 empty: " << s1.empty() << '\n';
    std::cout << "s2 size:  " << s2.size()  << '\n';
    std::cout << "s2 top:   " << s2.top()   << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: push, pop, top, empty, size\n";
    std::cout << "=====================================================\n";

    std::stack<int> st;

    std::cout << "empty: " << st.empty() << " size: " << st.size() << '\n';

    st.push(10);
    st.push(20);
    st.push(30);
    st.emplace(40);   // construct in place (same as push for int)

    std::cout << "after pushing 10,20,30,40:\n";
    std::cout << "  size:  " << st.size()  << '\n';
    std::cout << "  top:   " << st.top()   << '\n';  // 40 — last pushed
    std::cout << "  empty: " << st.empty() << '\n';

    // Drain the stack (LIFO order)
    std::cout << "popping all: ";
    while (!st.empty()) {
        std::cout << st.top() << ' ';
        st.pop();
    }
    std::cout << '\n';
    std::cout << "now empty: " << st.empty() << '\n';

    // Modify top
    st.push(100);
    st.top() = 999;   // top() returns a reference
    std::cout << "after push(100) and top()=999: " << st.top() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Different Underlying Containers\n";
    std::cout << "=====================================================\n";

    // Stack over deque (default)
    std::stack<int, std::deque<int>> stackDeque;
    stackDeque.push(1); stackDeque.push(2); stackDeque.push(3);
    std::cout << "deque-backed top: " << stackDeque.top() << '\n';

    // Stack over vector (more cache-friendly)
    std::stack<int, std::vector<int>> stackVector;
    stackVector.push(1); stackVector.push(2); stackVector.push(3);
    std::cout << "vector-backed top: " << stackVector.top() << '\n';

    // Stack over list
    std::stack<int, std::list<int>> stackList;
    stackList.push(1); stackList.push(2); stackList.push(3);
    std::cout << "list-backed top: " << stackList.top() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Balanced Parentheses Checker\n";
    std::cout << "=====================================================\n";

    std::vector<std::string> exprs = {
        "(())",
        "({[]})",
        "(()",
        "([)]",
        "{[()]}",
        ""
    };

    for (const auto& expr : exprs) {
        bool balanced = isBalanced(expr);
        std::println("  '{}' -> {}", expr.empty() ? "<empty>" : expr,
                     balanced ? "BALANCED" : "NOT balanced");
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Undo/Redo System\n";
    std::cout << "=====================================================\n";

    TextEditor editor;
    editor.type("Hello");
    editor.type(", World");
    editor.type("!");
    editor.undo();
    editor.undo();
    editor.redo();
    editor.type(" Everyone");
    editor.undo();
    editor.undo();
    editor.undo();   // nothing to undo beyond this


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Postfix Expression Evaluator\n";
    std::cout << "=====================================================\n";

    // "3 4 +" = 3 + 4 = 7
    // "5 3 2 * +" = 5 + (3*2) = 11
    std::vector<std::pair<std::string, int>> postfixExprs = {
        {"3 4 +", 7},
        {"5 3 2 * +", 11},
        {"4 2 3 * +", 10},
        {"8 2 /", 4}
    };

    for (const auto& [expr, expected] : postfixExprs) {
        int result = evaluatePostfix(expr);
        std::println("  '{}' = {} (expected {})", expr, result, expected);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: DFS with Stack\n";
    std::cout << "=====================================================\n";

    // Graph as adjacency list (6 nodes)
    std::vector<std::vector<int>> graph = {
        {1, 2},      // 0 -> 1, 2
        {0, 3, 4},   // 1 -> 0, 3, 4
        {0, 5},      // 2 -> 0, 5
        {1},         // 3 -> 1
        {1},         // 4 -> 1
        {2}          // 5 -> 2
    };

    dfsIterative(0, graph);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Stack of Objects (call stack simulation)\n";
    std::cout << "=====================================================\n";

    std::stack<StackFrame> callStack;

    // Simulate function calls
    callStack.push({"main",          10, 0});
    callStack.push({"processData",   25, 42});
    callStack.push({"parseBuffer",   8,  100});
    callStack.push({"validateBytes", 3,  255});

    std::cout << "Call stack (top = current function):\n";
    std::stack<StackFrame> temp = callStack;
    while (!temp.empty()) {
        temp.top().print();
        temp.pop();
    }

    // Return from functions
    std::cout << "\nUnwinding call stack:\n";
    while (!callStack.empty()) {
        std::cout << "  returning from: " << callStack.top().functionName << '\n';
        callStack.pop();
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Stack characteristics\n";
    std::cout << "=====================================================\n";

    std::cout << "std::stack is a CONTAINER ADAPTER\n";
    std::cout << "  wraps deque/vector/list\n";
    std::cout << "  exposes only: push, pop, top, empty, size\n";
    std::cout << "  NO iterators, NO random access, NO find\n";
    std::cout << '\n';
    std::cout << "Underlying container choices:\n";
    std::cout << "  deque  (default) — good general choice\n";
    std::cout << "  vector — better cache performance, no push_front overhead\n";
    std::cout << "  list   — O(1) guaranteed push/pop, worst cache\n";
    std::cout << '\n';
    std::cout << "Real-world uses:\n";
    std::cout << "  - Undo/redo history\n";
    std::cout << "  - Expression parsing/evaluation\n";
    std::cout << "  - DFS graph traversal\n";
    std::cout << "  - Call stack simulation\n";
    std::cout << "  - Balanced bracket checking\n";
    std::cout << "  - Browser back/forward history\n";

    std::cout << "\n=== StackSTL complete ===\n";
    return 0;
}
