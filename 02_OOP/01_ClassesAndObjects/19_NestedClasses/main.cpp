#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// WHAT ARE NESTED CLASSES?
// ============================================

/*
 * NESTED CLASSES:
 * - Class defined inside another class
 * - Also called "inner classes"
 * - Member of the enclosing class
 * - Can access enclosing class's private members (with object)
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
 * - Nested class can access outer's private members (with object)
 * - Outer class can access nested's private members
 * - Nested class doesn't have special access without object
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
        cout << "  OuterBasic created: " << outerName << "\n";
    }

    // ✅ Nested class definition
    class Inner {
    private:
        int innerValue;

    public:
        Inner(int v) : innerValue(v) {
            cout << "    Inner created with value: " << v << "\n";
        }

        void display() const {
            cout << "    Inner value: " << innerValue << "\n";
        }

        // Can access outer's private members if given object
        void showOuter(const OuterBasic& outer) const {
            cout << "    Outer value: " << outer.outerValue << "\n";
            cout << "    Outer name: " << outer.outerName << "\n";
        }
    };

    void display() const {
        cout << "  Outer: " << outerName << ", value: " << outerValue << "\n";
    }
};

void demonstrateBasic() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC NESTED CLASS    ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating outer object ---\n";
    OuterBasic outer(42, "MainOuter");
    outer.display();

    cout << "\n--- Creating nested class object ---\n";
    OuterBasic::Inner inner(100);  // Note the scope: OuterBasic::Inner
    inner.display();

    cout << "\n--- Inner accessing outer's private members ---\n";
    inner.showOuter(outer);

    cout << "\n💡 KEY POINTS:\n";
    cout << "   • Nested class defined inside outer class\n";
    cout << "   • Access with OuterBasic::Inner\n";
    cout << "   • Can access outer's private members (with object)\n";
    cout << "   • Independent objects (nested doesn't contain outer)\n";
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
            cout << "    Node created with data: " << d << "\n";
        }
    };

    Node* top;
    int count;

public:
    Stack() : top(nullptr), count(0) {
        cout << "  Stack created\n";
    }

    ~Stack() {
        while (top) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
        cout << "  Stack destroyed\n";
    }

    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        count++;
        cout << "  Pushed: " << value << "\n";
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
        cout << "  Popped: " << value << "\n";
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
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: PRIVATE NESTED CLASS  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Using Stack (Node is hidden) ---\n";
    Stack s;

    s.push(10);
    s.push(20);
    s.push(30);

    cout << "\n--- Stack size: " << s.size() << " ---\n";

    cout << "\n--- Popping values ---\n";
    while (!s.isEmpty()) {
        s.pop();
    }

    cout << "\n💡 PRIVATE NESTED CLASS:\n";
    cout << "   • Node is implementation detail\n";
    cout << "   • Hidden from users\n";
    cout << "   • Perfect encapsulation\n";
    cout << "   • Users can't access Stack::Node\n";
}

// ============================================
// EXAMPLE 3: PUBLIC NESTED CLASS - ITERATOR
// ============================================

class Container {
private:
    vector<int> data;

public:
    Container(initializer_list<int> init) : data(init) {
        cout << "  Container created with " << data.size() << " elements\n";
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
        cout << "  Container: ";
        for (int val : data) {
            cout << val << " ";
        }
        cout << "\n";
    }
};

void demonstrateIterator() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: ITERATOR PATTERN      ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating container ---\n";
    Container c{10, 20, 30, 40, 50};
    c.display();

    cout << "\n--- Using nested Iterator ---\n";
    cout << "  Elements: ";
    for (Container::Iterator it = c.begin(); it != c.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\n";

    cout << "\n💡 ITERATOR PATTERN:\n";
    cout << "   • Iterator is public nested class\n";
    cout << "   • Provides controlled access to container\n";
    cout << "   • Follows standard iterator pattern\n";
    cout << "   • Encapsulates iteration logic\n";
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
        cout << "  Pizza object created\n";
    }

public:
    // ✅ Public nested Builder class
    class Builder {
    private:
        Pizza* pizza;

    public:
        Builder() {
            pizza = new Pizza();
            cout << "  Pizza Builder created\n";
        }

        Builder& setDough(const string& d) {
            pizza->dough = d;
            cout << "    Dough set: " << d << "\n";
            return *this;
        }

        Builder& setSauce(const string& s) {
            pizza->sauce = s;
            cout << "    Sauce set: " << s << "\n";
            return *this;
        }

        Builder& setCheese(const string& c) {
            pizza->cheese = c;
            cout << "    Cheese set: " << c << "\n";
            return *this;
        }

        Builder& addTopping(const string& t) {
            pizza->toppings.push_back(t);
            cout << "    Topping added: " << t << "\n";
            return *this;
        }

        Pizza build() {
            cout << "  Pizza built!\n";
            return *pizza;
        }
    };

    void display() const {
        cout << "  Pizza Details:\n";
        cout << "    Dough: " << dough << "\n";
        cout << "    Sauce: " << sauce << "\n";
        cout << "    Cheese: " << cheese << "\n";
        cout << "    Toppings: ";
        for (const auto& t : toppings) {
            cout << t << " ";
        }
        cout << "\n";
    }
};

void demonstrateBuilder() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: BUILDER PATTERN       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Building pizza with nested Builder ---\n";
    Pizza pizza = Pizza::Builder()
                      .setDough("Thin crust")
                      .setSauce("Marinara")
                      .setCheese("Mozzarella")
                      .addTopping("Pepperoni")
                      .addTopping("Mushrooms")
                      .addTopping("Olives")
                      .build();

    cout << "\n--- Final pizza ---\n";
    pizza.display();

    cout << "\n💡 BUILDER PATTERN:\n";
    cout << "   • Builder is public nested class\n";
    cout << "   • Provides fluent interface\n";
    cout << "   • Simplifies complex object creation\n";
    cout << "   • Main class has private constructor\n";
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
            cout << "  Inner created (count: " << innerCount << ")\n";
        }

        static int getInnerCount() {
            return innerCount;
        }

        // Can access outer's static members directly
        static int getOuterCount() {
            return outerCount;  // ✅ Direct access to outer's static
        }

        void display() const {
            cout << "  Inner data: " << innerData << "\n";
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
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: STATIC MEMBERS        ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating objects ---\n";
    Outer o1(10);
    Outer o2(20);

    Outer::Inner i1(100);
    Outer::Inner i2(200);
    Outer::Inner i3(300);

    cout << "\n--- Accessing static members ---\n";
    cout << "Outer count: " << Outer::getOuterCount() << "\n";
    cout << "Inner count: " << Outer::Inner::getInnerCount() << "\n";
    cout << "Outer count (from Inner): " << Outer::Inner::getOuterCount() << "\n";

    cout << "\n💡 STATIC MEMBERS:\n";
    cout << "   • Nested class can have static members\n";
    cout << "   • Can access outer's static members directly\n";
    cout << "   • Independent static storage\n";
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
            cout << "    Node created: " << d << "\n";
        }
    };

    Node* root;

    void inorderHelper(Node* node) const {
        if (node) {
            inorderHelper(node->left);
            cout << node->data << " ";
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
        cout << "  BinaryTree created\n";
    }

    ~BinaryTree() {
        deleteTree(root);
        cout << "  BinaryTree destroyed\n";
    }

    void insert(int value) {
        if (!root) {
            root = new Node(value);
        } else {
            insertHelper(root, value);
        }
    }

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

    void display() const {
        cout << "  Tree (inorder): ";
        inorderHelper(root);
        cout << "\n";
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
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: MULTIPLE NESTED       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating binary tree ---\n";
    BinaryTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    cout << "\n--- Displaying tree ---\n";
    tree.display();

    cout << "\n--- Using iterator ---\n";
    cout << "  Iterator: ";
    BinaryTree::Iterator it = tree.getIterator();
    while (it.hasNext()) {
        cout << *it << " ";
        ++it;
    }
    cout << "\n";

    cout << "\n💡 MULTIPLE NESTED CLASSES:\n";
    cout << "   • Node is private (implementation)\n";
    cout << "   • Iterator is public (interface)\n";
    cout << "   • Both nested in BinaryTree\n";
    cout << "   • Perfect encapsulation\n";
}

// ============================================
// EXAMPLE 7: LOCAL CLASS (INSIDE FUNCTION)
// ============================================

void demonstrateLocalClass() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: LOCAL CLASS           ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    // ✅ Local class - defined inside function!
    class LocalHelper {
    private:
        int value;

    public:
        LocalHelper(int v) : value(v) {
            cout << "  LocalHelper created with value: " << v << "\n";
        }

        void display() const {
            cout << "  LocalHelper value: " << value << "\n";
        }

        int getValue() const {
            return value;
        }
    };

    cout << "--- Using local class ---\n";
    LocalHelper helper1(100);
    LocalHelper helper2(200);

    helper1.display();
    helper2.display();

    cout << "\n--- Processing with local class ---\n";
    vector<LocalHelper> helpers;
    helpers.push_back(LocalHelper(10));
    helpers.push_back(LocalHelper(20));
    helpers.push_back(LocalHelper(30));

    cout << "  Sum: ";
    int sum = 0;
    for (const auto& h : helpers) {
        sum += h.getValue();
    }
    cout << sum << "\n";

    cout << "\n💡 LOCAL CLASS:\n";
    cout << "   • Defined inside function\n";
    cout << "   • Only visible in that function\n";
    cout << "   • Can't have static members\n";
    cout << "   • Useful for function-specific helpers\n";
}

// ============================================
// EXAMPLE 8: NESTED CLASS ACCESS RULES
// ============================================

class AccessDemo {
private:
    int privateData = 10;

protected:
    int protectedData = 20;

public:
    int publicData = 30;

    // Public nested class
    class PublicNested {
    public:
        void accessOuter(const AccessDemo& outer) {
            // ✅ Can access ALL members with object reference
            cout << "  Private: " << outer.privateData << "\n";
            cout << "  Protected: " << outer.protectedData << "\n";
            cout << "  Public: " << outer.publicData << "\n";
        }
    };

    // Private nested class
private:
    class PrivateNested {
    public:
        void accessOuter(const AccessDemo& outer) {
            // ✅ Can still access all outer members
            cout << "  Private: " << outer.privateData << "\n";
            cout << "  Protected: " << outer.protectedData << "\n";
            cout << "  Public: " << outer.publicData << "\n";
        }
    };

public:
    void usePrivateNested() {
        PrivateNested nested;
        cout << "--- Private nested accessing outer ---\n";
        nested.accessOuter(*this);
    }
};

void demonstrateAccess() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: ACCESS RULES          ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    AccessDemo outer;

    cout << "--- Public nested accessing outer ---\n";
    AccessDemo::PublicNested publicNested;
    publicNested.accessOuter(outer);

    cout << "\n";
    outer.usePrivateNested();

    cout << "\n💡 ACCESS RULES:\n";
    cout << "   • Nested can access outer's private (with object)\n";
    cout << "   • Outer can access nested's private\n";
    cout << "   • Public/private controls nested visibility\n";
    cout << "   • Nested needs object reference to access members\n";
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
            cout << "  Inner value: " << value << "\n";
        }

        int getValue() const {
            return value;
        }
    };
};

void OuterForward::useInner(Inner* inner) {
    cout << "  Using inner object: ";
    inner->display();
}

void demonstrateForward() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: FORWARD DECLARATION   ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Using forward-declared nested class ---\n";
    OuterForward outer;
    OuterForward::Inner inner(42);

    outer.useInner(&inner);

    cout << "\n💡 FORWARD DECLARATION:\n";
    cout << "   • Can forward declare nested classes\n";
    cout << "   • Useful for breaking circular dependencies\n";
    cout << "   • Define later in same class\n";
}

// ============================================
// EXAMPLE 10: WHEN (NOT) TO USE NESTED CLASSES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: BEST PRACTICES       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "✅ USE NESTED CLASSES WHEN:\n";
    cout << "   • Class is tightly coupled to outer class\n";
    cout << "   • Implementation detail (private nested)\n";
    cout << "   • Iterator for container\n";
    cout << "   • Builder pattern\n";
    cout << "   • Node classes in data structures\n";
    cout << "   • Helper classes only used by outer\n";
    cout << "   • Logical grouping improves organization\n";
    cout << "\n";
    cout << "❌ DON'T USE NESTED CLASSES WHEN:\n";
    cout << "   • Class is independent and reusable\n";
    cout << "   • Nesting makes code harder to read\n";
    cout << "   • Class is used by multiple classes\n";
    cout << "   • Deep nesting (nested in nested in nested...)\n";
    cout << "   • Class is complex and large\n";
    cout << "   • Just to group unrelated classes\n";
    cout << "\n";
    cout << "📋 NAMING CONVENTION:\n";
    cout << "   • OuterClass::InnerClass\n";
    cout << "   • Use descriptive names\n";
    cout << "   • Make relationship clear\n";
    cout << "\n";
    cout << "💡 DESIGN PRINCIPLES:\n";
    cout << "   • High cohesion - nested class belongs to outer\n";
    cout << "   • Low coupling - limit dependencies\n";
    cout << "   • Encapsulation - hide implementation details\n";
    cout << "   • Keep nested classes small and focused\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              NESTED CLASSES IN C++                        ║\n";
    cout << "║           Organizing Related Classes                      ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

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

    // ============================================
    // COMPREHENSIVE EXPLANATION
    // ============================================

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              COMPREHENSIVE EXPLANATION                    ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n========================================\n";
    cout << "WHAT ARE NESTED CLASSES?\n";
    cout << "========================================\n";
    cout << "Definition:\n";
    cout << "  A nested class (or inner class) is a class defined\n";
    cout << "  within another class. It's a member of the enclosing class.\n";
    cout << "\n";
    cout << "BASIC SYNTAX:\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    class Inner {  // Nested class\n";
    cout << "    public:\n";
    cout << "        void method() { }\n";
    cout << "    };\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage:\n";
    cout << "Outer::Inner inner;  // Note the scope resolution\n";
    cout << "\n";
    cout << "TYPES:\n";
    cout << "  • Member nested classes (in class definition)\n";
    cout << "  • Local classes (in function)\n";
    cout << "  • Anonymous classes (unnamed)\n";

    cout << "\n========================================\n";
    cout << "WHY USE NESTED CLASSES?\n";
    cout << "========================================\n";
    cout << "BENEFITS:\n";
    cout << "1. Logical Grouping\n";
    cout << "   • Classes belong together conceptually\n";
    cout << "   • Clear relationship in code\n";
    cout << "   • Example: Container::Iterator\n";
    cout << "\n";
    cout << "2. Encapsulation\n";
    cout << "   • Hide implementation details\n";
    cout << "   • Private nested class invisible outside\n";
    cout << "   • Example: LinkedList::Node\n";
    cout << "\n";
    cout << "3. Namespace Management\n";
    cout << "   • Avoid name pollution\n";
    cout << "   • Clear ownership\n";
    cout << "   • Example: Pizza::Builder\n";
    cout << "\n";
    cout << "4. Access to Outer Members\n";
    cout << "   • Can access outer's private members\n";
    cout << "   • Tight coupling when needed\n";
    cout << "   • Clean design\n";

    cout << "\n========================================\n";
    cout << "ACCESS RULES\n";
    cout << "========================================\n";
    cout << "NESTED TO OUTER ACCESS:\n";
    cout << "class Outer {\n";
    cout << "private:\n";
    cout << "    int privateData;\n";
    cout << "    \n";
    cout << "    void access() {\n";
    cout << "        Inner inner;\n";
    cout << "        // ✅ Can access Inner's private!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "VISIBILITY CONTROL:\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    class PublicNested { };   // Anyone can use\n";
    cout << "    \n";
    cout << "private:\n";
    cout << "    class PrivateNested { };  // Only Outer can use\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Iterator\n";
    cout << "class Container {\n";
    cout << "public:\n";
    cout << "    class Iterator {\n";
    cout << "    private:\n";
    cout << "        Container* container;\n";
    cout << "        size_t index;\n";
    cout << "    public:\n";
    cout << "        // Iterator methods...\n";
    cout << "    };\n";
    cout << "    \n";
    cout << "    Iterator begin() { return Iterator(this, 0); }\n";
    cout << "    Iterator end() { return Iterator(this, size()); }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Private Implementation (Node)\n";
    cout << "class LinkedList {\n";
    cout << "private:\n";
    cout << "    class Node {  // Hidden from users!\n";
    cout << "    public:\n";
    cout << "        int data;\n";
    cout << "        Node* next;\n";
    cout << "    };\n";
    cout << "    \n";
    cout << "    Node* head;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: Builder\n";
    cout << "class Product {\n";
    cout << "private:\n";
    cout << "    Product() { }  // Private constructor\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    class Builder {\n";
    cout << "    private:\n";
    cout << "        Product product;\n";
    cout << "    public:\n";
    cout << "        Builder& setProp(int val) {\n";
    cout << "            product.prop = val;\n";
    cout << "            return *this;\n";
    cout << "        }\n";
    cout << "        Product build() { return product; }\n";
    cout << "    };\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 4: Exception Hierarchy\n";
    cout << "class Database {\n";
    cout << "public:\n";
    cout << "    class Exception : public std::exception { };\n";
    cout << "    class ConnectionError : public Exception { };\n";
    cout << "    class QueryError : public Exception { };\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "LOCAL CLASSES\n";
    cout << "========================================\n";
    cout << "Definition:\n";
    cout << "  A local class is defined inside a function.\n";
    cout << "\n";
    cout << "EXAMPLE:\n";
    cout << "void function() {\n";
    cout << "    class LocalClass {  // Only visible in this function\n";
    cout << "    public:\n";
    cout << "        void method() { }\n";
    cout << "    };\n";
    cout << "    \n";
    cout << "    LocalClass obj;\n";
    cout << "    obj.method();\n";
    cout << "}\n";
    cout << "\n";
    cout << "RESTRICTIONS:\n";
    cout << "  • Can't have static members\n";
    cout << "  • Can only access function's parameters and local variables\n";
    cout << "  • Only visible within function\n";
    cout << "  • Can't be used as template arguments (pre-C++11)\n";
    cout << "\n";
    cout << "USE CASES:\n";
    cout << "  • Function-specific helper classes\n";
    cout << "  • One-time use classes\n";
    cout << "  • Callbacks or functors\n";

    cout << "\n========================================\n";
    cout << "NESTED CLASS VS INHERITANCE\n";
    cout << "========================================\n";
    cout << "NESTED CLASS:\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    class Inner { };  // Inner is MEMBER of Outer\n";
    cout << "};\n";
    cout << "\n";
    cout << "Outer o;\n";
    cout << "Outer::Inner i;  // Separate objects!\n";
    cout << "\n";
    cout << "INHERITANCE:\n";
    cout << "class Base { };\n";
    cout << "class Derived : public Base { };  // Derived IS-A Base\n";
    cout << "\n";
    cout << "Derived d;  // Contains Base part\n";
    cout << "\n";
    cout << "KEY DIFFERENCES:\n";
    cout << "  • Nested: HAS-A relationship (containment)\n";
    cout << "  • Inheritance: IS-A relationship\n";
    cout << "  • Nested: Independent objects\n";
    cout << "  • Inheritance: Derived contains base\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use for tightly coupled classes\n";
    cout << "  • Hide implementation details with private nested\n";
    cout << "  • Use for iterators\n";
    cout << "  • Use for builder patterns\n";
    cout << "  • Keep nested classes small\n";
    cout << "  • Document relationship clearly\n";
    cout << "  • Use descriptive names\n";
    cout << "  • Consider forward declaration if needed\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Nest just to group unrelated classes\n";
    cout << "  • Create deep nesting hierarchies\n";
    cout << "  • Use for independent, reusable classes\n";
    cout << "  • Make large, complex nested classes\n";
    cout << "  • Forget about access requirements\n";
    cout << "  • Overuse - keep it simple\n";

    cout << "\n========================================\n";
    cout << "STATIC MEMBERS IN NESTED CLASSES\n";
    cout << "========================================\n";
    cout << "Nested classes CAN have static members:\n";
    cout << "\n";
    cout << "class Outer {\n";
    cout << "private:\n";
    cout << "    static int outerStatic;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    class Inner {\n";
    cout << "    private:\n";
    cout << "        static int innerStatic;  // ✅ Allowed!\n";
    cout << "        \n";
    cout << "    public:\n";
    cout << "        static void method() {\n";
    cout << "            // Can access outer's static directly\n";
    cout << "            int x = outerStatic;  // ✅ OK\n";
    cout << "        }\n";
    cout << "    };\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Define static members\n";
    cout << "int Outer::outerStatic = 0;\n";
    cout << "int Outer::Inner::innerStatic = 0;\n";
    cout << "\n";
    cout << "NOTE: Local classes CAN'T have static members!\n";

    cout << "\n========================================\n";
    cout << "FORWARD DECLARATION\n";
    cout << "========================================\n";
    cout << "Can forward declare nested classes:\n";
    cout << "\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    class Inner;  // Forward declaration\n";
    cout << "    \n";
    cout << "    void useInner(Inner* ptr);  // Can use pointer\n";
    cout << "    \n";
    cout << "    class Inner {  // Definition\n";
    cout << "    public:\n";
    cout << "        void method();\n";
    cout << "    };\n";
    cout << "};\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • Break circular dependencies\n";
    cout << "  • Organize class definition\n";
    cout << "  • Defer implementation\n";

    cout << "\n========================================\n";
    cout << "TEMPLATES AND NESTED CLASSES\n";
    cout << "========================================\n";
    cout << "Nested classes in template classes:\n";
    cout << "\n";
    cout << "template<typename T>\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    class Inner {\n";
    cout << "        T data;  // Can use template parameter\n";
    cout << "    public:\n";
    cout << "        Inner(T d) : data(d) { }\n";
    cout << "    };\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage:\n";
    cout << "Outer<int>::Inner inner(42);\n";
    cout << "\n";
    cout << "Template nested classes:\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    template<typename T>\n";
    cout << "    class Inner {\n";
    cout << "        T data;\n";
    cout << "    public:\n";
    cout << "        Inner(T d) : data(d) { }\n";
    cout << "    };\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage:\n";
    cout << "Outer::Inner<int> inner(42);\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "EXAMPLE 1: std::string::iterator\n";
    cout << "  • Iterator is nested in string class\n";
    cout << "  • Provides iteration over string\n";
    cout << "  • Hidden implementation details\n";
    cout << "\n";
    cout << "EXAMPLE 2: std::vector::iterator\n";
    cout << "  • Vector's iterator is nested class\n";
    cout << "  • Standard pattern for containers\n";
    cout << "  • Type-safe iteration\n";
    cout << "\n";
    cout << "EXAMPLE 3: Tree::Node\n";
    cout << "  • Node is private implementation\n";
    cout << "  • Only tree can manipulate nodes\n";
    cout << "  • Perfect encapsulation\n";
    cout << "\n";
    cout << "EXAMPLE 4: Builder patterns\n";
    cout << "  • StringBuilder, HttpRequestBuilder, etc.\n";
    cout << "  • Builder is public nested class\n";
    cout << "  • Fluent interface for construction\n";

    cout << "\n========================================\n";
    cout << "COMPARISON WITH ALTERNATIVES\n";
    cout << "========================================\n";
    cout << "NESTED CLASS:\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    class Inner { };\n";
    cout << "};\n";
    cout << "Pros: Logical grouping, access to outer's private\n";
    cout << "Cons: Tight coupling, harder to reuse\n";
    cout << "\n";
    cout << "SEPARATE CLASS:\n";
    cout << "class Outer { };\n";
    cout << "class Inner { };\n";
    cout << "Pros: Independent, reusable, simpler\n";
    cout << "Cons: No special relationship, separate namespaces\n";
    cout << "\n";
    cout << "NAMESPACE:\n";
    cout << "namespace MyLib {\n";
    cout << "    class Outer { };\n";
    cout << "    class Inner { };\n";
    cout << "}\n";
    cout << "Pros: Grouping, no coupling\n";
    cout << "Cons: No special access privileges\n";
    cout << "\n";
    cout << "CHOOSE BASED ON:\n";
    cout << "  • Coupling: How tightly related?\n";
    cout << "  • Reusability: Used elsewhere?\n";
    cout << "  • Encapsulation: Hide implementation?\n";
    cout << "  • Clarity: What's clearest?\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic nested class\n";
    cout << "class Outer {\n";
    cout << "public:\n";
    cout << "    class Inner {\n";
    cout << "        int data;\n";
    cout << "    public:\n";
    cout << "        Inner(int d) : data(d) { }\n";
    cout << "        void method() { }\n";
    cout << "    };\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage\n";
    cout << "Outer::Inner inner(42);\n";
    cout << "inner.method();\n";
    cout << "\n";
    cout << "// Private nested class\n";
    cout << "class Outer {\n";
    cout << "private:\n";
    cout << "    class Inner { };  // Only Outer can use\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Local class\n";
    cout << "void func() {\n";
    cout << "    class Local { };  // Only in this function\n";
    cout << "    Local obj;\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Accessing outer's members\n";
    cout << "class Outer {\n";
    cout << "    int data;\n";
    cout << "public:\n";
    cout << "    class Inner {\n";
    cout << "        void access(Outer& o) {\n";
    cout << "            o.data;  // ✅ Can access private\n";
    cout << "        }\n";
    cout << "    };\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Nested classes are defined inside another class\n";
    cout << "2. Use Outer::Inner to access nested class\n";
    cout << "3. Nested can access outer's private (with object)\n";
    cout << "4. Outer can access nested's private\n";
    cout << "5. Perfect for iterators and implementation details\n";
    cout << "6. Private nested classes hide implementation\n";
    cout << "7. Public nested classes provide interfaces\n";
    cout << "8. Local classes defined in functions\n";
    cout << "9. Use for tightly coupled classes only\n";
    cout << "10. Keep nested classes small and focused\n";

    cout << "\n========================================\n";
    cout << "DECISION TREE\n";
    cout << "========================================\n";
    cout << "Is class tightly coupled to another class?\n";
    cout << "  NO  → Use separate class\n";
    cout << "  YES → Continue...\n";
    cout << "\n";
    cout << "Is it an implementation detail?\n";
    cout << "  YES → Private nested class ✅\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Is it part of the public interface?\n";
    cout << "  YES → Public nested class ✅\n";
    cout << "        (e.g., Iterator, Builder)\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Will it be reused elsewhere?\n";
    cout << "  YES → Separate class\n";
    cout << "  NO  → Nested class ✅\n";
    cout << "\n";
    cout << "Is it only used in one function?\n";
    cout << "  YES → Local class ✅\n";
    cout << "  NO  → Nested class ✅\n";

    cout << "\n========================================\n";
    cout << "COMPARISON TABLE\n";
    cout << "========================================\n";
    cout << "┌──────────────────┬─────────┬──────────┬───────────┐\n";
    cout << "│ Feature          │ Nested  │ Separate │ Local     │\n";
    cout << "├──────────────────┼─────────┼──────────┼───────────┤\n";
    cout << "│ Scope            │ Outer   │ Global   │ Function  │\n";
    cout << "│ Access outer     │ Yes*    │ No       │ Limited   │\n";
    cout << "│ Reusability      │ Low     │ High     │ None      │\n";
    cout << "│ Encapsulation    │ High    │ Low      │ High      │\n";
    cout << "│ Coupling         │ High    │ Low      │ High      │\n";
    cout << "│ Visibility       │ Control │ Public   │ Private   │\n";
    cout << "│ Static members   │ Yes     │ Yes      │ No        │\n";
    cout << "│ Use case         │ Related │ General  │ Helper    │\n";
    cout << "└──────────────────┴─────────┴──────────┴───────────┘\n";
    cout << "* With object reference\n";

    return 0;
}
