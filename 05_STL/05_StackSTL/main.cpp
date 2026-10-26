#include <cctype>
#include <deque>
#include <list>
#include <print>
#include <stack>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ============================================================
// std::stack — LIFO Container Adapter in C++23
// ============================================================
// Topics covered:
//   1.  Declaration and initialization
//   2.  push, pop, top, empty, size
//   3.  Stack with different underlying containers
//   4.  Practical: balanced parentheses checker
//   5.  Practical: undo/redo system
//   6.  Practical: expression evaluator (postfix)
//   7.  Practical: DFS (depth-first search)
//   8.  Stack of objects
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
// Example 4 — Balanced parentheses checker
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
// Example 5 — Undo/Redo system
// ============================================================
class TextEditor {
private:
    std::string             m_text;
    std::stack<std::string> m_undoStack;
    std::stack<std::string> m_redoStack;

public:
    void type(const std::string& addition) {
        m_undoStack.push(m_text);   // save current state
        m_redoStack = {};           // a new action clears redo
        m_text += addition;
        std::println("  typed '{}' -> text: '{}'", addition, m_text);
    }

    void undo() {
        if (m_undoStack.empty()) {
            std::println("  nothing to undo");
            return;
        }
        m_redoStack.push(m_text);
        m_text = m_undoStack.top();
        m_undoStack.pop();
        std::println("  undo -> text: '{}'", m_text);
    }

    void redo() {
        if (m_redoStack.empty()) {
            std::println("  nothing to redo");
            return;
        }
        m_undoStack.push(m_text);
        m_text = m_redoStack.top();
        m_redoStack.pop();
        std::println("  redo -> text: '{}'", m_text);
    }

    std::string getText() const { return m_text; }
};


// ============================================================
// Example 6 — Postfix expression evaluator
// (single-digit operands only, input is assumed to be valid)
// ============================================================
int evaluatePostfix(const std::string& expr)
{
    std::stack<int> st;

    for (char c : expr) {
        if (c == ' ') continue;

        if (std::isdigit(static_cast<unsigned char>(c))) {
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
// Example 7 — Depth-First Search (DFS) with stack
// ============================================================
void dfsIterative(int start, const std::vector<std::vector<int>>& graph)
{
    std::vector<bool> visited(graph.size(), false);
    std::stack<int>   st;

    st.push(start);
    std::print("DFS from node {}: ", start);

    while (!st.empty()) {
        int node = st.top(); st.pop();

        if (visited[node]) continue;
        visited[node] = true;
        std::print("{} ", node);

        // Push neighbors in reverse so the first neighbor is visited first
        for (int i = static_cast<int>(graph[node].size()) - 1; i >= 0; --i) {
            if (!visited[graph[node][i]]) {
                st.push(graph[node][i]);
            }
        }
    }
    std::println("");
}


// ============================================================
// Example 8 — Stack of objects
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
    // === EXAMPLE 1: Declaration and Initialization ===
    banner("Example 1: Declaration and Initialization");

    // Default underlying container is std::deque
    std::stack<int> s1;

    // Initialize from container
    std::deque<int> dq = {1, 2, 3, 4, 5};
    std::stack<int> s2(dq);   // top = 5 (last element of deque)

    std::println("s1 empty: {}", s1.empty());
    std::println("s2 size:  {}", s2.size());
    std::println("s2 top:   {}", s2.top());


    // === EXAMPLE 2: push, pop, top, empty, size ===
    banner("Example 2: push, pop, top, empty, size");

    std::stack<int> st;

    std::println("empty: {} size: {}", st.empty(), st.size());

    st.push(10);
    st.push(20);
    st.push(30);
    st.emplace(40);   // construct in place (same as push for int)

    std::println("after pushing 10,20,30,40:");
    std::println("  size:  {}", st.size());
    std::println("  top:   {}", st.top());     // 40 — last pushed
    std::println("  empty: {}", st.empty());

    // Drain the stack (LIFO order)
    std::print("popping all: ");
    while (!st.empty()) {
        std::print("{} ", st.top());
        st.pop();
    }
    std::println("");
    std::println("now empty: {}", st.empty());

    // Modify top
    st.push(100);
    st.top() = 999;   // top() returns a reference
    std::println("after push(100) and top()=999: {}", st.top());


    // === EXAMPLE 3: Different Underlying Containers ===
    banner("Example 3: Different Underlying Containers");

    // Stack over deque (default)
    std::stack<int, std::deque<int>> stackDeque;
    stackDeque.push(1); stackDeque.push(2); stackDeque.push(3);
    std::println("deque-backed top: {}", stackDeque.top());

    // Stack over vector (more cache-friendly)
    std::stack<int, std::vector<int>> stackVector;
    stackVector.push(1); stackVector.push(2); stackVector.push(3);
    std::println("vector-backed top: {}", stackVector.top());

    // Stack over list
    std::stack<int, std::list<int>> stackList;
    stackList.push(1); stackList.push(2); stackList.push(3);
    std::println("list-backed top: {}", stackList.top());


    // === EXAMPLE 4: Balanced Parentheses Checker ===
    banner("Example 4: Balanced Parentheses Checker");

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


    // === EXAMPLE 5: Undo/Redo System ===
    banner("Example 5: Undo/Redo System");

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
    editor.undo();   // back to the empty text
    editor.undo();   // nothing to undo beyond this


    // === EXAMPLE 6: Postfix Expression Evaluator ===
    banner("Example 6: Postfix Expression Evaluator");

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


    // === EXAMPLE 7: DFS with Stack ===
    banner("Example 7: DFS with Stack");

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


    // === EXAMPLE 8: Stack of Objects (call stack simulation) ===
    banner("Example 8: Stack of Objects (call stack simulation)");

    std::stack<StackFrame> callStack;

    // Simulate function calls
    callStack.push({"main",          10, 0});
    callStack.push({"processData",   25, 42});
    callStack.push({"parseBuffer",   8,  100});
    callStack.push({"validateBytes", 3,  255});

    std::println("Call stack (top = current function):");
    std::stack<StackFrame> temp = callStack;   // copy, so we can drain it
    while (!temp.empty()) {
        temp.top().print();
        temp.pop();
    }

    // Return from functions
    std::println("");
    std::println("Unwinding call stack:");
    while (!callStack.empty()) {
        std::println("  returning from: {}", callStack.top().functionName);
        callStack.pop();
    }

    std::println("");
    std::println("=== StackSTL complete ===");
    return 0;
}
