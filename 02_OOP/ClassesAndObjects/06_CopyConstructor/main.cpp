#include <iostream>
#include <cstring>   // for strlen, strcpy
#include <string>

// =============================================================
// Example 1: Class WITHOUT copy constructor (shallow copy)
// Demonstrates the problem that copy constructors solve
// =============================================================
class ShallowBox {
public:
    int*  data;
    int   size;

    // Constructor
    ShallowBox(int size) : size(size) {
        data = new int[size];
        for (int i = 0; i < size; ++i)
            data[i] = i * 10;
        std::cout << "[ShallowBox] Constructor called, data @ " << data << '\n';
    }

    // Destructor
    ~ShallowBox() {
        std::cout << "[ShallowBox] Destructor called, data @ " << data << '\n';
        // delete[] data;   // PROBLEM: if two objects share the same pointer,
            // this deletes memory the other object still points to
    }

    void print() const {
        std::cout << "ShallowBox data: ";
        for (int i = 0; i < size; ++i)
            std::cout << data[i] << " ";
        std::cout << '\n';
    }
};


// =============================================================
// Example 2: Class WITH copy constructor (deep copy)
// =============================================================
class DeepBox {
public:
    int*  data;
    int   size;
    std::string name;

    // Constructor
    DeepBox(int size, const std::string& name) : size(size), name(name) {
        data = new int[size];
        for (int i = 0; i < size; ++i)
            data[i] = i * 10;
        std::cout << "[DeepBox] Constructor called for '" << name
                  << "', data @ " << data << '\n';
    }

    // Copy constructor — deep copy
    // Signature: ClassName(const ClassName& other)
    DeepBox(const DeepBox& other) : size(other.size), name(other.name + "_copy") {
        data = new int[size];              // allocate NEW memory
        for (int i = 0; i < size; ++i)
            data[i] = other.data[i];       // copy the VALUES, not the pointer
        std::cout << "[DeepBox] Copy constructor called for '" << name
                  << "', data @ " << data << '\n';
    }

    // Destructor
    ~DeepBox() {
        std::cout << "[DeepBox] Destructor called for '" << name
                  << "', data @ " << data << '\n';
        delete[] data;
    }

    void modify(int index, int value) {
        if (index >= 0 && index < size)
            data[index] = value;
    }

    void print() const {
        std::cout << "DeepBox '" << name << "' data: ";
        for (int i = 0; i < size; ++i)
            std::cout << data[i] << " ";
        std::cout << '\n';
    }
};


// =============================================================
// Example 3: Class with copy constructor using std::string
// (no raw pointers — std::string manages its own memory)
// =============================================================
class Player {
public:
    std::string name;
    int         score;
    int         level;

    // Constructor
    Player(const std::string& name, int score, int level)
        : name(name), score(score), level(level) {
        std::cout << "[Player] Constructor called for '" << name << "'\n";
    }

    // Copy constructor
    Player(const Player& other)
        : name(other.name + "_copy"), score(other.score), level(other.level) {
        std::cout << "[Player] Copy constructor called for '" << name << "'\n";
    }

    // Copy assignment operator
    Player& operator=(const Player& other) {
        if (this == &other) return *this;   // self-assignment guard
        name  = other.name + "_assigned";
        score = other.score;
        level = other.level;
        std::cout << "[Player] Copy assignment operator called for '" << name << "'\n";
        return *this;
    }

    ~Player() {
        std::cout << "[Player] Destructor called for '" << name << "'\n";
    }

    void print() const {
        std::cout << "Player: " << name
                  << " | score: " << score
                  << " | level: " << level << '\n';
    }
};


int main()
{
    // =================================================
    // Example 1: Shallow copy — THE PROBLEM
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Shallow Copy — The Problem\n";
    std::cout << "=====================================================\n";

    {
        ShallowBox box1(3);
        box1.print();

        ShallowBox box2 = box1;   // compiler-generated copy — copies the POINTER
            // both box1.data and box2.data point to SAME memory!

        std::cout << "box1.data address: " << box1.data << '\n';
        std::cout << "box2.data address: " << box2.data << '\n';
        std::cout << "Same address? " << (box1.data == box2.data ? "YES — PROBLEM!" : "NO") << '\n';

        // When this block ends, BOTH destructors run and delete[] the same pointer
        // → undefined behavior / crash
        std::cout << "Leaving scope — double delete incoming...\n";
    }
    // Note: this may crash or produce undefined behavior — intentional demonstration


    // =================================================
    // Example 2: Deep copy — THE SOLUTION
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Deep Copy — The Solution\n";
    std::cout << "=====================================================\n";

    {
        DeepBox original(4, "original");
        original.print();

        // Copy constructor called here
        DeepBox copy1(original);
        copy1.print();

        // Also triggers copy constructor (initialization)
        DeepBox copy2 = original;
        copy2.print();

        std::cout << "\noriginal.data address: " << original.data << '\n';
        std::cout << "copy1.data address:    " << copy1.data    << '\n';
        std::cout << "copy2.data address:    " << copy2.data    << '\n';
        std::cout << "All different? "
                  << (original.data != copy1.data && copy1.data != copy2.data
                          ? "YES — correct deep copy!" : "NO") << '\n';

        // Modify copy — should NOT affect original
        std::cout << "\nModifying copy1.data[0] to 999...\n";
        copy1.modify(0, 999);
        original.print();   // should be unchanged
        copy1.print();      // should show 999
    }


    // =================================================
    // Example 3: Copy constructor vs copy assignment
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Copy Constructor vs Copy Assignment\n";
    std::cout << "=====================================================\n";

    {
        Player p1("Kostas", 1500, 10);
        p1.print();

        // Copy constructor — called during INITIALIZATION
        Player p2(p1);
        p2.print();

        Player p3 = p1;    // also copy constructor — this is initialization, NOT assignment
        p3.print();

        // Copy assignment operator — called when assigning to EXISTING object
        Player p4("Andre", 800, 5);
        p4.print();

        std::cout << "\nAssigning p1 to p4...\n";
        p4 = p1;           // copy assignment operator called here
        p4.print();
    }


    // =================================================
    // Example 4: Passing and returning by value
    // (copy constructor called implicitly)
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Copy constructor called implicitly\n";
    std::cout << "=====================================================\n";

    {
        // Lambda to show copy on pass-by-value
        auto printPlayer = [](Player p) {   // copy constructor called here
            std::cout << "Inside function: ";
            p.print();
        };

        Player hero("Hero", 9999, 99);
        std::cout << "Calling printPlayer(hero)...\n";
        printPlayer(hero);  // hero is COPIED into the function parameter
        std::cout << "Back in main — original hero unchanged:\n";
        hero.print();
    }

    return 0;
}
