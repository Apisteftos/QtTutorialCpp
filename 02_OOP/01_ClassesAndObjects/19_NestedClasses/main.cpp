#include <print>
#include <string>
#include <vector>
#include <stdexcept>
#include <initializer_list>
using namespace std;

// ============================================
// WHAT ARE NESTED CLASSES?
// ============================================

/*
 * NESTED CLASSES:
 * - Class defined inside another class
 * - Also called "inner classes"
 * - Member of the enclosing class
 * - Has the same access as a member function of the enclosing class
 * - Helps organize related classes
 *
 * TYPES OF NESTED CLASSES:
 * 1. Member nested classes (inside class definition)
 * 2. Local classes (inside function)
 * 3. Anonymous classes (unnamed nested classes)
 *
 * WHY USE NESTED CLASSES?
 * - Logical grouping of classes
 * - Encapsulation and information hiding
 * - Implementation details hidden
 * - Namespace management
 * - Helper classes that only make sense with outer class
 *
 * ACCESS RULES:
 * - Nested class can access outer's private/protected members (needs an object for
 *   non-static ones; static members can be used directly)
 * - Outer class can NOT access nested's private members, unless the nested class
 *   says "friend class Outer;" (Example 8)
 * - The nested class is not "inside" the outer object: it has no outer object of its own
 * - Public/private/protected apply to nested class visibility
 *
 * COMMON USES:
 * - Iterator classes
 * - Node classes in data structures
 * - Builder patterns
 * - Implementation details
 * - Helper classes
 *
 * BEST PRACTICES:
 * - Use for classes tightly coupled to outer class
 * - Keep nested classes small
 * - Make private if implementation detail
 * - Use for better encapsulation
 * - Document relationship with outer class
 */

// ============================================
// EXAMPLE 1: BASIC NESTED CLASS
// ============================================

class OuterBasic {
private:
    int outerValue;
    string outerName;

public:
    OuterBasic(int v, const string& n) : outerValue(v), outerName(n) {
        std::println("  OuterBasic created: {}", outerName);
    }

    // ✅ Nested class definition
    class Inner {
    private:
        int innerValue;

    public:
        Inner(int v) : innerValue(v) {
            std::println("    Inner created with value: {}", v);
        }

        void display() const {
            std::println("    Inner value: {}", innerValue);
        }

        // Can access outer's private members if given object
        void showOuter(const OuterBasic& outer) const {
            std::println("    Outer value: {}", outer.outerValue);
            std::println("    Outer name: {}", outer.outerName);
        }
    };

    void display() const {
        std::println("  Outer: {}, value: {}", outerName, outerValue);
    }
};

void demonstrateBasic() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC NESTED CLASS    ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating outer object ---");
    OuterBasic outer(42, "MainOuter");
    outer.display();

    std::println("");
    std::println("--- Creating nested class object ---");
    OuterBasic::Inner inner(100);  // Note the scope: OuterBasic::Inner
    inner.display();

    std::println("");
    std::println("--- Inner accessing outer's private members ---");
    inner.showOuter(outer);
}

// ============================================
// EXAMPLE 2: PRIVATE NESTED CLASS
// ============================================

class Stack {
private:
    // ✅ Private nested class - implementation detail!
    class Node {
    public:
        int data;
        Node* next;

        Node(int d) : data(d), next(nullptr) {
            std::println("    Node created with data: {}", d);
        }
    };

    Node* top;
    int count;

public:
    Stack() : top(nullptr), count(0) {
        std::println("  Stack created");
    }

    ~Stack() {
        while (top) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
        std::println("  Stack destroyed");
    }

    // Raw pointer ownership: copying would double-delete the nodes
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        count++;
        std::println("  Pushed: {}", value);
    }

    int pop() {
        if (!top) {
            throw runtime_error("Stack is empty");
        }
        Node* temp = top;
        int value = temp->data;
        top = top->next;
        delete temp;
        count--;
        std::println("  Popped: {}", value);
        return value;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int size() const {
        return count;
    }
};

void demonstratePrivateNested() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 2: PRIVATE NESTED CLASS  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Using Stack (Node is hidden) ---");
    Stack s;

    s.push(10);
    s.push(20);
    s.push(30);

    std::println("");
    std::println("--- Stack size: {} ---", s.size());

    std::println("");
    std::println("--- Popping values ---");
    while (!s.isEmpty()) {
        s.pop();
    }

    std::println("");
    std::println("--- Popping from an empty stack throws ---");
    try {
        s.pop();
    } catch (const runtime_error& e) {
        std::println("  Caught: {}", e.what());
    }
}

// ============================================
// EXAMPLE 3: PUBLIC NESTED CLASS - ITERATOR
// ============================================

class Container {
private:
    vector<int> data;

public:
    Container(initializer_list<int> init) : data(init) {
        std::println("  Container created with {} elements", data.size());
    }

    // ✅ Public nested Iterator class
    class Iterator {
    private:
        const Container* container;
        size_t index;

    public:
        Iterator(const Container* c, size_t i)
            : container(c), index(i) {
        }

        int operator*() const {
            return container->data[index];
        }

        Iterator& operator++() {
            ++index;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return index != other.index;
        }
    };

    Iterator begin() const {
        return Iterator(this, 0);
    }

    Iterator end() const {
        return Iterator(this, data.size());
    }

    void display() const {
        std::print("  Container: ");
        for (int val : data) {
            std::print("{} ", val);
        }
        std::println("");
    }
};

void demonstrateIterator() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 3: ITERATOR PATTERN      ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating container ---");
    Container c{10, 20, 30, 40, 50};
    c.display();

    std::println("");
    std::println("--- Using nested Iterator ---");
    std::print("  Elements: ");
    for (Container::Iterator it = c.begin(); it != c.end(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");

    std::print("  Range-for works too (begin/end + !=, ++, *): ");
    for (int v : c) {
        std::print("{} ", v);
    }
    std::println("");
}

// ============================================
// EXAMPLE 4: BUILDER PATTERN
// ============================================

class Pizza {
private:
    string dough;
    string sauce;
    string cheese;
    vector<string> toppings;

    // Private constructor - only Builder can create Pizza
    Pizza() {
        std::println("  Pizza object created");
    }

public:
    // ✅ Public nested Builder class (defined below, after Pizza is complete)
    class Builder;

    void display() const {
        std::println("  Pizza Details:");
        std::println("    Dough: {}", dough);
        std::println("    Sauce: {}", sauce);
        std::println("    Cheese: {}", cheese);
        std::print("    Toppings: ");
        for (const auto& t : toppings) {
            std::print("{} ", t);
        }
        std::println("");
    }
};

// Defined AFTER Pizza: inside Pizza the type is still incomplete, so a member
// "Pizza pizza;" would not compile there. A nested class may still use Pizza's private constructor.
class Pizza::Builder {
private:
    Pizza pizza;  // ✅ a value member: no new/delete, no leak

public:
    Builder() {
        std::println("  Pizza Builder created");
    }

    Builder& setDough(const string& d) {
        pizza.dough = d;
        std::println("    Dough set: {}", d);
        return *this;
    }

    Builder& setSauce(const string& s) {
        pizza.sauce = s;
        std::println("    Sauce set: {}", s);
        return *this;
    }

    Builder& setCheese(const string& c) {
        pizza.cheese = c;
        std::println("    Cheese set: {}", c);
        return *this;
    }

    Builder& addTopping(const string& t) {
        pizza.toppings.push_back(t);
        std::println("    Topping added: {}", t);
        return *this;
    }

    Pizza build() {
        std::println("  Pizza built!");
        return pizza;
    }
};

void demonstrateBuilder() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 4: BUILDER PATTERN       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Building pizza with nested Builder ---");
    Pizza pizza = Pizza::Builder()
                      .setDough("Thin crust")
                      .setSauce("Marinara")
                      .setCheese("Mozzarella")
                      .addTopping("Pepperoni")
                      .addTopping("Mushrooms")
                      .addTopping("Olives")
                      .build();

    std::println("");
    std::println("--- Final pizza ---");
    pizza.display();
}

// ============================================
// EXAMPLE 5: NESTED CLASS WITH STATIC MEMBERS
// ============================================

class Outer {
private:
    int outerData;
    static int outerCount;

public:
    Outer(int d) : outerData(d) {
        outerCount++;
    }

    // Nested class with static members
    class Inner {
    private:
        static int innerCount;
        int innerData;

    public:
        Inner(int d) : innerData(d) {
            innerCount++;
            std::println("  Inner created (count: {})", innerCount);
        }

        static int getInnerCount() {
            return innerCount;
        }

        // Can access outer's static members directly
        static int getOuterCount() {
            return outerCount;  // ✅ Direct access to outer's static
        }

        void display() const {
            std::println("  Inner data: {}", innerData);
        }
    };

    static int getOuterCount() {
        return outerCount;
    }
};

// Initialize static members
int Outer::outerCount = 0;
int Outer::Inner::innerCount = 0;

void demonstrateStatic() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 5: STATIC MEMBERS        ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating objects ---");
    Outer o1(10);
    Outer o2(20);

    Outer::Inner i1(100);
    Outer::Inner i2(200);
    Outer::Inner i3(300);

    std::println("");
    std::println("--- Accessing static members ---");
    std::println("Outer count: {}", Outer::getOuterCount());
    std::println("Inner count: {}", Outer::Inner::getInnerCount());
    std::println("Outer count (from Inner): {}", Outer::Inner::getOuterCount());
}

// ============================================
// EXAMPLE 6: MULTIPLE NESTED CLASSES
// ============================================

class BinaryTree {
private:
    // Private nested Node class
    class Node {
    public:
        int data;
        Node* left;
        Node* right;

        Node(int d) : data(d), left(nullptr), right(nullptr) {
            std::println("    Node created: {}", d);
        }
    };

    Node* root;

    void inorderHelper(Node* node) const {
        if (node) {
            inorderHelper(node->left);
            std::print("{} ", node->data);
            inorderHelper(node->right);
        }
    }

    void deleteTree(Node* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    BinaryTree() : root(nullptr) {
        std::println("  BinaryTree created");
    }

    ~BinaryTree() {
        deleteTree(root);
        std::println("  BinaryTree destroyed");
    }

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    void insert(int value) {
        if (!root) {
            root = new Node(value);
        } else {
            insertHelper(root, value);
        }
    }

private:
    void insertHelper(Node* node, int value) {
        if (value < node->data) {
            if (!node->left) {
                node->left = new Node(value);
            } else {
                insertHelper(node->left, value);
            }
        } else {
            if (!node->right) {
                node->right = new Node(value);
            } else {
                insertHelper(node->right, value);
            }
        }
    }

public:
    void display() const {
        std::print("  Tree (inorder): ");
        inorderHelper(root);
        std::println("");
    }

    // Public nested Iterator class
    class Iterator {
    private:
        vector<Node*> stack;
        Node* current;

        void pushLeft(Node* node) {
            while (node) {
                stack.push_back(node);
                node = node->left;
            }
        }

    public:
        Iterator(Node* root) : current(nullptr) {
            pushLeft(root);
            if (!stack.empty()) {
                current = stack.back();
                stack.pop_back();
            }
        }

        int operator*() const {
            return current->data;
        }

        Iterator& operator++() {
            if (current->right) {
                pushLeft(current->right);
            }
            if (!stack.empty()) {
                current = stack.back();
                stack.pop_back();
            } else {
                current = nullptr;
            }
            return *this;
        }

        bool hasNext() const {
            return current != nullptr;
        }
    };

    Iterator getIterator() const {
        return Iterator(root);
    }
};

void demonstrateMultipleNested() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 6: MULTIPLE NESTED       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating binary tree ---");
    BinaryTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    std::println("");
    std::println("--- Displaying tree ---");
    tree.display();

    std::println("");
    std::println("--- Using iterator ---");
    std::print("  Iterator: ");
    BinaryTree::Iterator it = tree.getIterator();
    while (it.hasNext()) {
        std::print("{} ", *it);
        ++it;
    }
    std::println("");
}

// ============================================
// EXAMPLE 7: LOCAL CLASS (INSIDE FUNCTION)
// ============================================

void demonstrateLocalClass() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 7: LOCAL CLASS           ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    static int base = 1000;

    // ✅ Local class - defined inside function!
    // It may use types, constants and STATIC locals of the function, but not its
    // parameters or automatic variables; no static data members, no member templates
    class LocalHelper {
    private:
        int value;

    public:
        LocalHelper(int v) : value(v) {
            std::println("  LocalHelper created with value: {}", v);
        }

        void display() const {
            std::println("  LocalHelper value: {}", value);
        }

        int getValue() const {
            return value;
        }

        int shifted() const {
            return base + value;  // ✅ a static local of the function is allowed
        }
    };

    std::println("--- Using local class ---");
    LocalHelper helper1(100);
    LocalHelper helper2(200);

    helper1.display();
    helper2.display();
    std::println("  helper1.shifted() = {}", helper1.shifted());

    std::println("");
    std::println("--- Processing with local class ---");
    vector<LocalHelper> helpers;
    helpers.push_back(LocalHelper(10));
    helpers.push_back(LocalHelper(20));
    helpers.push_back(LocalHelper(30));

    std::print("  Sum: ");
    int sum = 0;
    for (const auto& h : helpers) {
        sum += h.getValue();
    }
    std::println("{}", sum);
}

// ============================================
// EXAMPLE 8: NESTED CLASS ACCESS RULES
// ============================================

class AccessDemo {
private:
    int privateData = 10;

protected:
    int protectedData = 20;
    static inline int instances = 3;

public:
    int publicData = 30;

    // Public nested class
    class PublicNested {
    public:
        void accessOuter(const AccessDemo& outer) {
            // ✅ Can access ALL members with object reference
            std::println("  Private: {}", outer.privateData);
            std::println("  Protected: {}", outer.protectedData);
            std::println("  Public: {}", outer.publicData);
        }

        void accessStatic() const {
            // ✅ A static member needs no object
            std::println("  Static (no object needed): {}", instances);
        }
    };

    // Private nested class
private:
    class PrivateNested {
    public:
        void accessOuter(const AccessDemo& outer) {
            // ✅ Can still access all outer members
            std::println("  Private: {}", outer.privateData);
            std::println("  Protected: {}", outer.protectedData);
            std::println("  Public: {}", outer.publicData);
        }
    };

public:
    void usePrivateNested() {
        PrivateNested nested;
        std::println("--- Private nested accessing outer ---");
        nested.accessOuter(*this);
    }
};

// The other direction: the OUTER class has no special access to the nested class
class Locker {
public:
    class Key {
    private:
        int code;
        friend class Locker;  // ✅ without this line Locker::open() does not compile

    public:
        explicit Key(int c) : code(c) {}
    };

    void open(const Key& key) const {
        std::println("  Locker reads Key::code = {} (allowed by friend)", key.code);
    }
};

void demonstrateAccess() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 8: ACCESS RULES          ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    AccessDemo outer;

    std::println("--- Public nested accessing outer ---");
    AccessDemo::PublicNested publicNested;
    publicNested.accessOuter(outer);
    publicNested.accessStatic();

    std::println("");
    outer.usePrivateNested();

    std::println("");
    std::println("--- Outer reading the nested class's private member: needs friend ---");
    Locker locker;
    locker.open(Locker::Key(1234));
}

// ============================================
// EXAMPLE 9: FORWARD DECLARATION
// ============================================

class OuterForward {
public:
    class Inner;  // ✅ Forward declaration

    void useInner(Inner* inner);

    // Inner defined later
    class Inner {
    private:
        int value;

    public:
        Inner(int v) : value(v) {}

        void display() const {
            std::println("  Inner value: {}", value);
        }

        int getValue() const {
            return value;
        }
    };
};

void OuterForward::useInner(Inner* inner) {
    std::print("  Using inner object: ");
    inner->display();
}

void demonstrateForward() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 9: FORWARD DECLARATION   ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Using forward-declared nested class ---");
    OuterForward outer;
    OuterForward::Inner inner(42);

    outer.useInner(&inner);
}

// ============================================
// EXAMPLE 10: WHEN (NOT) TO USE NESTED CLASSES
// ============================================

// Good use case 1: an exception hierarchy that belongs to one class
class Database {
public:
    class Exception : public runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class ConnectionError : public Exception {
    public:
        using Exception::Exception;
    };

    class QueryError : public Exception {
    public:
        using Exception::Exception;
    };

    void connect(bool ok) const {
        if (!ok) throw ConnectionError("cannot connect to server");
    }

    void query(const string& sql) const {
        if (sql.empty()) throw QueryError("empty query");
    }
};

// A nested CLASS TYPE is not a data member: it adds nothing to the outer object
struct WithoutNested { int x; };
struct WithNested {
    class Helper { int a, b, c; };
    int x;
};
static_assert(sizeof(WithNested) == sizeof(WithoutNested));

void demonstrateBestPractices() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 10: BEST PRACTICES       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Nested exception hierarchy: Database::Exception ---");
    Database db;
    try {
        db.connect(false);
    } catch (const Database::Exception& e) {
        std::println("  Caught as Database::Exception: {}", e.what());
    }

    try {
        db.query("");
    } catch (const Database::QueryError& e) {
        std::println("  Caught as Database::QueryError: {}", e.what());
    }

    std::println("");
    std::println("--- A nested class does not make the outer object bigger ---");
    std::println("  sizeof(WithoutNested) = {}, sizeof(WithNested) = {}", sizeof(WithoutNested), sizeof(WithNested));
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║              NESTED CLASSES IN C++                        ║");
    std::println("║           Organizing Related Classes                      ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

    demonstrateBasic();
    demonstratePrivateNested();
    demonstrateIterator();
    demonstrateBuilder();
    demonstrateStatic();
    demonstrateMultipleNested();
    demonstrateLocalClass();
    demonstrateAccess();
    demonstrateForward();
    demonstrateBestPractices();

    return 0;
}
