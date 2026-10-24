# 26_NestedClasses - Organizing Related Classes

## 📚 Overview

This folder introduces **Nested Classes** (also called inner classes) - classes defined inside other classes. Nested classes provide logical grouping, encapsulation, and namespace management. They're perfect for iterators, implementation details, builder patterns, and tightly coupled helper classes.

**Nested Classes** = Classes defined inside other classes

This is **ESSENTIAL** for proper encapsulation and class organization.

---

## 🎯 What You'll Learn

- What nested classes are and why they exist
- Types of nested classes (member, local, anonymous)
- Access rules between nested and outer classes
- Public vs private nested classes
- Common patterns (Iterator, Node, Builder)
- Static members in nested classes
- Local classes in functions
- Forward declaration of nested classes
- When to use (and when not to use)
- Best practices and real-world examples

---

## 📖 Concept Explanation

### What are Nested Classes?

A **nested class** (or inner class) is a class defined inside another class. It's a member of the enclosing class and can have different access levels.

```cpp
class Outer {
public:
    // ✅ Nested class
    class Inner {
    private:
        int value;
        
    public:
        Inner(int v) : value(v) { }
        void display() const {
            cout << "Value: " << value << "\n";
        }
    };
};

// Usage - note the scope resolution
Outer::Inner inner(42);
inner.display();
```

### Why Use Nested Classes?

**1. Logical Grouping**
```cpp
class Container {
public:
    class Iterator { };  // Iterator belongs to Container
};
```

**2. Encapsulation**
```cpp
class LinkedList {
private:
    class Node { };  // Hide implementation detail
};
```

**3. Namespace Management**
```cpp
class Pizza {
public:
    class Builder { };  // Clear ownership: Pizza::Builder
};
```

**4. Access to Outer's Private Members**
```cpp
class Outer {
private:
    int secret;
    
public:
    class Inner {
    public:
        void access(const Outer& o) {
            // ✅ Can access private members!
            int x = o.secret;
        }
    };
};
```

### Real-World Analogy

```
Think of a car and its components:

CAR (Outer Class)
├─ Engine (Public Nested)
│  └─ Users can see it exists
├─ Fuel Pump (Private Nested)
│  └─ Hidden implementation detail
└─ Dashboard::Speedometer (Nested in Nested)
   └─ Clear hierarchy

NESTED CLASSES = Components that belong to the car
  • Engine is part of Car
  • Doesn't make sense without Car
  • Car controls Engine's visibility
  • Clear relationship and ownership

SEPARATE CLASSES = Independent vehicles
  • Bicycle is separate from Car
  • Makes sense independently
  • No special relationship
```

---

## 💻 Basic Syntax

### Simple Nested Class

```cpp
class Outer {
public:
    class Inner {
    private:
        int data;
        
    public:
        Inner(int d) : data(d) { }
        void method() { }
    };
};

// Creating nested class object
Outer::Inner inner(42);  // Note: Outer::Inner
inner.method();
```

### Private Nested Class

```cpp
class LinkedList {
private:
    // ✅ Private nested class - implementation detail
    class Node {
    public:
        int data;
        Node* next;
        Node(int d) : data(d), next(nullptr) { }
    };
    
    Node* head;
    
public:
    void add(int value) {
        Node* newNode = new Node(value);  // ✅ Outer can use it
        // ...
    }
};

// LinkedList::Node node(10);  // ❌ Error - private!
```

### Local Class (Inside Function)

```cpp
void processData() {
    // ✅ Local class - only visible in this function
    class Helper {
    private:
        int value;
        
    public:
        Helper(int v) : value(v) { }
        int process() { return value * 2; }
    };
    
    Helper h(10);
    int result = h.process();
}
```

---

## 📝 Common Use Cases

### 1. Iterator Pattern

**Perfect use case for public nested classes:**

```cpp
class Container {
private:
    vector<int> data;
    
public:
    Container(initializer_list<int> init) : data(init) { }
    
    // ✅ Public nested Iterator class
    class Iterator {
    private:
        const Container* container;
        size_t index;
        
    public:
        Iterator(const Container* c, size_t i) 
            : container(c), index(i) { }
        
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
    
    Iterator begin() const { return Iterator(this, 0); }
    Iterator end() const { return Iterator(this, data.size()); }
};

// Usage
Container c{1, 2, 3, 4, 5};
for (Container::Iterator it = c.begin(); it != c.end(); ++it) {
    cout << *it << " ";
}
```

### 2. Data Structure Nodes

**Private nested classes for implementation:**

```cpp
class Stack {
private:
    // ✅ Private Node - hidden from users
    class Node {
    public:
        int data;
        Node* next;
        Node(int d) : data(d), next(nullptr) { }
    };
    
    Node* top;
    
public:
    Stack() : top(nullptr) { }
    
    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }
    
    int pop() {
        if (!top) throw runtime_error("Empty stack");
        Node* temp = top;
        int value = temp->data;
        top = top->next;
        delete temp;
        return value;
    }
};

// Perfect encapsulation - users can't access Node!
```

This shortened snippet owns raw pointers, so a copy would delete the nodes twice. `main.cpp` (Example 2) therefore deletes the copy constructor and copy assignment (rule of three/five).

### 3. Builder Pattern

**Public nested builder for fluent construction:**

```cpp
class Pizza {
private:
    string dough;
    string sauce;
    vector<string> toppings;

    Pizza() { }  // Private constructor

public:
    // ✅ Public nested Builder - only DECLARED here
    class Builder;

    void display() const {
        cout << "Pizza: " << dough << ", " << sauce << "\n";
        cout << "Toppings: ";
        for (const auto& t : toppings) cout << t << " ";
        cout << "\n";
    }
};

// Defined after Pizza, because inside Pizza the type is still incomplete
// and a member "Pizza pizza;" would not compile there.
class Pizza::Builder {
private:
    Pizza pizza;  // ✅ nested class may use Pizza's private constructor

public:
    Builder& setDough(const string& d) {
        pizza.dough = d;
        return *this;
    }

    Builder& setSauce(const string& s) {
        pizza.sauce = s;
        return *this;
    }

    Builder& addTopping(const string& t) {
        pizza.toppings.push_back(t);
        return *this;
    }

    Pizza build() {
        return pizza;
    }
};

// Usage - fluent interface!
Pizza pizza = Pizza::Builder()
    .setDough("Thin crust")
    .setSauce("Marinara")
    .addTopping("Pepperoni")
    .addTopping("Mushrooms")
    .build();
```

❌ The earlier version of this snippet put `class Builder { Pizza pizza; ... };` directly inside `Pizza`. That does **not** compile (GCC: `field 'pizza' has incomplete type 'Pizza'`, clang: `field has incomplete type 'Pizza'`) ✅ tested. The alternatives are a `Pizza*` / `unique_ptr<Pizza>` member (then no leak: use `unique_ptr`, not a raw `new` that is never deleted) or the forward declaration shown above.

### 4. Exception Hierarchies

**Nested exception classes:**

```cpp
class Database {
public:
    // ✅ Nested exception hierarchy
    class Exception : public std::exception {
    protected:
        string message;
    public:
        Exception(const string& msg) : message(msg) { }
        const char* what() const noexcept override {
            return message.c_str();
        }
    };
    
    class ConnectionError : public Exception {
    public:
        ConnectionError(const string& msg) : Exception(msg) { }
    };
    
    class QueryError : public Exception {
    public:
        QueryError(const string& msg) : Exception(msg) { }
    };
};

// Usage
try {
    throw Database::ConnectionError("Cannot connect to server");
} catch (const Database::Exception& e) {
    cout << "Database error: " << e.what() << "\n";
}
```

### 5. Configuration and Settings

**Nested classes for related configuration:**

```cpp
class Application {
public:
    class Config {
    public:
        string appName;
        int port;
        bool debugMode;
        
        Config(const string& name, int p, bool debug)
            : appName(name), port(p), debugMode(debug) { }
    };
    
    class Logger {
    public:
        enum Level { DEBUG, INFO, WARNING, ERROR };
        Level level;
        
        Logger(Level l) : level(l) { }
    };
    
private:
    Config config;
    Logger logger;
    
public:
    Application(const Config& c, const Logger& l)
        : config(c), logger(l) { }
};

// Usage - clear grouping
Application::Config config("MyApp", 8080, true);
Application::Logger logger(Application::Logger::DEBUG);
Application app(config, logger);
```

---

## 📋 Examples in This Folder

### Example 1: Basic Nested Class
Simple nested class with access to outer's private members.
- Created as `OuterBasic::Inner`; an `Inner` object does not contain an `OuterBasic`, it only sees one if you pass it in

### Example 2: Private Nested Class
Stack with hidden Node implementation.
- Users cannot name `Stack::Node`; added: popping an empty stack throws (caught), and the copy operations are deleted (raw pointer ownership)

### Example 3: Iterator Pattern
Container with public Iterator nested class.
- Controlled access to the container's data; range-for also works because `begin()`/`end()` return something with `!=`, `++` and `*` ✅ tested

### Example 4: Builder Pattern
Pizza with fluent Builder interface.
- Fluent interface; `Pizza` has a private constructor that only the nested `Builder` may call
- Fixed: the old `Builder` did `new Pizza()` and never deleted it (LeakSanitizer: 120 bytes leaked). It now holds a `Pizza` value and is defined after `Pizza` (see Common Use Cases, Builder)

### Example 5: Static Members
Nested classes with static member variables.
- Independent static storage; the nested class reads the outer's `outerCount` directly, no object needed ✅ tested

### Example 6: Multiple Nested Classes
Binary tree with Node and Iterator.
- `Node` private (implementation), `Iterator` public (interface); `insertHelper` is now private, copy operations deleted

### Example 7: Local Class
Helper class defined inside a function.
- Only visible in that function; added a `static` local used by the class (allowed) - see the restriction list under Local Classes

### Example 8: Access Rules
Demonstrating access between nested and outer.
- Nested → outer: private, protected and public members through an object, and a static member without one
- Added `Locker`/`Key`: outer → nested private access works only with `friend class Locker;`

### Example 9: Forward Declaration
Forward declaring nested classes.
- Declare `class Inner;` inside the outer class, use `Inner*` in a signature, define `Inner` later (this is also how the Builder fix works)

### Example 10: Best Practices
When to use (and when not to use) nested classes.
- Was only printed text; now a real demo: a nested exception hierarchy (`Database::Exception`) and `sizeof` showing that a nested class adds nothing to the outer object

---

## 🎓 Key Concepts

### Access Rules

**CRITICAL: a nested class needs an object to use outer's NON-static members.** It has the same access as a member function of the outer class (private and protected included), but it has no outer object of its own. Static members, types and enumerators of the outer class can be used directly ✅ tested.

```cpp
class Outer {
private:
    int privateData = 10;
    
public:
    class Inner {
    public:
        void wrongAccess() {
            // int x = privateData;  // ❌ Error! No implicit access
        }
        
        void correctAccess(const Outer& outer) {
            int x = outer.privateData;  // ✅ OK with object reference
        }
    };
};
```

**The other direction: the outer class has NO special access to the nested class's private members.** The nesting works in one direction only:

```cpp
class Outer {
public:
    class Inner {
    private:
        int secret = 42;
        friend class Outer;  // ✅ needed - without it the line below is an error
    };

    void access() {
        Inner inner;
        int x = inner.secret;  // ✅ OK only because of the friend declaration
    }
};
```

Without the `friend` line GCC reports `'int Outer::Inner::secret' is private within this context` ✅ tested. (The old heading "Outer can access nested's private", the takeaway list and the diagram in this README, and the comments in `main.cpp`, said this works without `friend` - that is wrong.)

### Visibility Control

```cpp
class Outer {
public:
    class PublicNested { };    // ✅ Anyone can use Outer::PublicNested
    
protected:
    class ProtectedNested { }; // ✅ Only Outer and derived classes (tested)
    
private:
    class PrivateNested { };   // ✅ Only Outer can use
};

// Outside Outer:
Outer::PublicNested pn;        // ✅ OK
// Outer::ProtectedNested pn;  // ❌ Error
// Outer::PrivateNested pn;    // ❌ Error
```

### Static Members in Nested Classes

```cpp
class Outer {
private:
    static int outerStatic;
    
public:
    class Inner {
    private:
        static int innerStatic;  // ✅ Nested class can have static
        
    public:
        static void method() {
            // ✅ Can access outer's static directly
            int x = outerStatic;
        }
    };
};

// Define static members
int Outer::outerStatic = 0;
int Outer::Inner::innerStatic = 0;
```

### Local Classes

```cpp
void function() {
    // ✅ Local class - only visible here
    class LocalHelper {
    private:
        int value;
        
    public:
        LocalHelper(int v) : value(v) { }
        int getValue() const { return value; }
    };
    
    LocalHelper helper(42);
    cout << helper.getValue() << "\n";
}

// LocalHelper obj;  // ❌ Error - not visible outside function
```

**Restrictions on local classes** (all ✅ tested with GCC 13, `-std=c++17`):
- ❌ No static data members (`local class shall not have static data member`)
- ❌ Cannot use the function's **parameters** or **automatic local variables** (`use of parameter from containing function`, `use of local variable with automatic storage from containing function`) - this is still true in C++17; it was not changed by C++11
- ✅ Can use the function's `static` locals, types and enumerators; static member *functions* are fine
- ❌ No member templates; member functions must be defined inside the class (an out-of-line definition is an error)
- ✅ Can be used as a template argument since C++11 (`vector<LocalHelper>` works); with `-std=c++03` GCC rejects it

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Assuming Automatic Access to Outer's Non-static Members

**Bad:**
```cpp
class Outer {
private:
    int data;
    
public:
    class Inner {
    public:
        void access() {
            // int x = data;  // ❌ Error! No implicit access
        }
    };
};
```

**Fix:**
```cpp
class Outer {
private:
    int data;
    
public:
    class Inner {
    public:
        void access(const Outer& outer) {
            int x = outer.data;  // ✅ OK with object reference
        }
    };
};
```

### ❌ Mistake 2: Making Everything Nested

**Bad:**
```cpp
class BadDesign {
public:
    class UnrelatedHelper1 { };  // ⚠️ Not really related!
    class UnrelatedHelper2 { };  // ⚠️ Could be separate
    class UnrelatedHelper3 { };  // ⚠️ Overuse of nesting
};
```

**Fix:** Only nest when there's a logical relationship.

### ❌ Mistake 3: Deep Nesting

**Bad:**
```cpp
class Level1 {
public:
    class Level2 {
    public:
        class Level3 {
        public:
            class Level4 { };  // ⚠️ Too deep!
        };
    };
};
```

**Fix:** Keep nesting shallow (1-2 levels max).

### ❌ Mistake 4: Static Members in Local Classes

**Bad:**
```cpp
void func() {
    class Local {
        static int count;  // ❌ Error! Not allowed in local classes
    };
}
```

Other things a local class cannot do: use the function's parameters / automatic variables, declare member templates (see "Restrictions on local classes" above).

---

## 🎯 Practice Exercises

### Exercise 1: Implement Stack
Create a Stack class with private nested Node class.

### Exercise 2: Create Iterator
Implement a custom container with public Iterator nested class.

### Exercise 3: Builder Pattern
Create a ComplexObject class with Builder pattern.

### Exercise 4: Binary Tree
Implement binary tree with nested Node and Iterator.

### Exercise 5: Refactor Code
Given code with separate helper classes, refactor to use nested classes.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o nested_classes
```

(`<print>` / `std::println` needs GCC 14 or newer.)

### Run:
```bash
./nested_classes
```

### Expected Output:
The program demonstrates:
1. Basic nested class usage
2. Private nested class for encapsulation
3. Iterator pattern with nested class
4. Builder pattern
5. Static members in nested classes
6. Multiple nested classes in one outer class
7. Local classes in functions
8. Access rules and visibility
9. Forward declaration
10. Best practices guide

---

## 📊 Visual Concepts

### Nested vs Separate Classes

```
NESTED CLASSES
┌─────────────────────────────────┐
│ Outer Class                     │
│ ┌─────────────────────────────┐ │
│ │ Inner Class (Nested)        │ │
│ │ • Part of Outer             │ │
│ │ • Access Outer's private    │ │
│ │ • Scoped: Outer::Inner      │ │
│ └─────────────────────────────┘ │
└─────────────────────────────────┘

SEPARATE CLASSES
┌─────────────────┐  ┌─────────────────┐
│ Class A         │  │ Class B         │
│ • Independent   │  │ • Independent   │
│ • No special    │  │ • No special    │
│   relationship  │  │   relationship  │
└─────────────────┘  └─────────────────┘
```

### Access Rules Diagram

```
ACCESS BETWEEN NESTED AND OUTER

┌──────────────────────────────────────┐
│ Outer Class                          │
│ ├─ private: int secret               │
│ ├─ public: void method()             │
│                                      │
│ ┌────────────────────────────────┐   │
│ │ Nested Class                   │   │
│ │                                │   │
│ │ access(Outer& o) {             │   │
│ │   o.secret ✅  // Can access!   │   │
│ │ }                              │   │
│ └────────────────────────────────┘   │
│         ↓                            │
│  Needs object reference!             │
└──────────────────────────────────────┘

┌──────────────────────────────────────┐
│ Outer Class                          │
│                                      │
│ useNested() {                        │
│   Nested n;                          │
│   n.privateData ❌ unless Nested has │
│         friend class Outer;          │
│ }                                    │
│ ┌────────────────────────────────┐   │
│ │ Nested Class                   │   │
│ │ ├─ private: int privateData    │   │
│ └────────────────────────────────┘   │
│         ↑                            │
│  No special access in this direction │
└──────────────────────────────────────┘
```

### Common Patterns

```
PATTERN COMPARISON

Iterator Pattern
┌─────────────────────────────┐
│ Container                   │
│ ┌─────────────────────────┐ │
│ │ class Iterator          │ │  Public nested
│ │ • Iterates container    │ │  class for
│ │ • Public interface      │ │  controlled
│ └─────────────────────────┘ │  access
└─────────────────────────────┘

Node Pattern
┌─────────────────────────────┐
│ LinkedList                  │
│ ┌─────────────────────────┐ │
│ │ class Node              │ │  Private nested
│ │ • Implementation detail │ │  class hides
│ │ • Hidden from users     │ │  implementation
│ └─────────────────────────┘ │
└─────────────────────────────┘

Builder Pattern
┌─────────────────────────────┐
│ ComplexObject               │
│ ┌─────────────────────────┐ │
│ │ class Builder           │ │  Public nested
│ │ • Fluent interface      │ │  class for
│ │ • Simplifies creation   │ │  construction
│ └─────────────────────────┘ │
└─────────────────────────────┘
```

---

## 📚 Related Topics

### Prerequisites:
- **05_Encapsulation** - Access control
- **06_GettersSetters** - Member access

### Coming Next:
- **27_FriendClasses** - Friend relationships (needed for outer → nested private access)

### Related Concepts:
- **Encapsulation** - Information hiding
- **Composition** - Has-a relationships
- **Namespaces** - Name management

---

## 🎓 Summary

### What We Learned:

✅ **Nested classes** defined inside other classes  
✅ **Access with** `Outer::Inner` syntax  
✅ **Can access** outer's private (with object reference)  
✅ **Private nested** for implementation details  
✅ **Public nested** for interfaces (Iterator, Builder)  
✅ **Local classes** in functions  
✅ **Static members** allowed in nested classes  
✅ **Perfect for** iterators, nodes, builders  
✅ **Visibility control** with access modifiers  
✅ **Use sparingly** - only for tight coupling

### The Complete Pattern:

```cpp
// Comprehensive example with multiple nested classes
class DataStructure {
private:
    // ✅ Private nested Node - implementation detail
    class Node {
    public:
        int data;
        Node* next;
        Node(int d) : data(d), next(nullptr) { }
    };
    
    Node* head;
    int count;
    
public:
    DataStructure() : head(nullptr), count(0) { }
    
    // ✅ Public nested Iterator - interface
    class Iterator {
    private:
        Node* current;
        
    public:
        Iterator(Node* node) : current(node) { }
        
        int operator*() const {
            return current->data;
        }
        
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    
    void add(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        count++;
    }
    
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
    
    int size() const { return count; }
    
    ~DataStructure() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Usage - clean and intuitive
DataStructure ds;
ds.add(10);
ds.add(20);
ds.add(30);

for (DataStructure::Iterator it = ds.begin(); it != ds.end(); ++it) {
    cout << *it << " ";
}
```

---

## ✨ Quick Reference Card

```cpp
// === BASIC NESTED CLASS ===
class Outer {
public:
    class Inner {
        int data;
    public:
        Inner(int d) : data(d) { }
        void method() { }
    };
};

Outer::Inner inner(42);  // Usage

// === PRIVATE NESTED CLASS ===
class Outer {
private:
    class Inner { };  // Only Outer can use
};

// === ACCESS OUTER'S MEMBERS ===
class Outer {
    int data;
public:
    class Inner {
        void access(Outer& o) {
            o.data;  // ✅ With object reference
        }
    };
};

// === LOCAL CLASS ===
void func() {
    class Local {
        int value;
    public:
        Local(int v) : value(v) { }
    };
    
    Local obj(42);
}

// === STATIC MEMBERS ===
class Outer {
public:
    class Inner {
        static int count;  // ✅ Allowed
    };
};
int Outer::Inner::count = 0;
```

---

## 📝 Best Practices

### ✅ DO:

- **Use for tightly coupled** classes
- **Hide implementation** with private nested
- **Use for iterators** and accessors
- **Use for builder** patterns
- **Keep nested classes** small
- **Document relationship** clearly
- **Use descriptive names**
- **Consider forward** declaration if needed
- **Use for exception** hierarchies
- **Make private** if implementation detail

### ❌ DON'T:

- **Don't nest** just to group unrelated classes
- **Don't create** deep nesting hierarchies
- **Don't use** for independent, reusable classes
- **Don't make** large, complex nested classes
- **Don't forget** about access requirements
- **Don't overuse** - keep it simple
- **Don't assume** automatic access to outer
- **Don't use** when separate class is clearer
- **Don't nest** for the sake of nesting
- **Don't forget** visibility control

---

## 🌟 Real-World Applications

### Standard Library Examples
```cpp
// std::string::iterator (a member TYPE of string - but not necessarily a nested class)
string s = "hello";
for (string::iterator it = s.begin(); it != s.end(); ++it) {
    // string::iterator is a typedef in libstdc++
}

// std::vector::iterator
vector<int> v{1, 2, 3};
for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    // vector<int>::iterator is a typedef for __gnu_cxx::__normal_iterator<int*, vector<int>>
}
```

✅ tested (GCC 13 / libstdc++): `std::vector<int>::iterator` and `std::string::iterator` are **typedefs** for `__gnu_cxx::__normal_iterator<...>`, a class template outside of `vector`/`string`. The standard only requires that the member type names exist; it does not say they are nested classes. They are *used* like the nested `Iterator` of this lesson, but they are not examples of it.

### Custom Container
```cpp
class MyContainer {
private:
    class Node { };  // Private implementation
    
public:
    class Iterator { };  // Public interface
};
```

### Configuration System
```cpp
class Application {
public:
    class Config { };
    class Logger { };
    class Database { };
};
```

---

## 📖 Further Reading

- [cppreference.com - Nested types](https://en.cppreference.com/cpp/language/nested_types) (found via search, page not opened)
- [learncpp.com - 15.3 Nested types (member types)](https://www.learncpp.com/cpp-tutorial/nested-types-in-classes/) (found via search, page not opened)

---

## 🎯 Key Takeaways

1. **Nested classes** defined inside other classes
2. **Access with** `Outer::Inner` syntax
3. **Can access** outer's private with object reference
4. **Outer can NOT access** nested's private (unless `friend class Outer;`)
5. **Private nested** for implementation details
6. **Public nested** for interfaces
7. **Perfect for** iterators, nodes, builders
8. **Local classes** in functions (limited use)
9. **Use for** tightly coupled classes only
10. **Keep simple** - don't over-nest

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Use for tight coupling only
   Only nest classes that truly belong together
   
   ✅ Container::Iterator
   ✅ LinkedList::Node
   ❌ Unrelated helper classes

⚠️  RULE 2: Nested needs an object for non-static members
   Can't use outer's data members as plain names!
   
   ❌ int x = outerData;           // Error!
   ✅ int x = outer.outerData;     // OK with object

⚠️  RULE 3: Control visibility
   Private for implementation, public for interface
   
   private: class Node { };        // Hidden
   public: class Iterator { };     // Exposed

⚠️  RULE 4: Keep it simple
   Avoid deep nesting hierarchies
   
   ✅ Outer::Inner                 // Good
   ⚠️ Outer::Inner::Nested::Deep  // Too much!

⚠️  RULE 5: Use for specific patterns
   Iterator, Node, Builder, Exceptions
   
   Perfect patterns for nested classes
   Don't force nesting elsewhere

   NEST WISELY - LOGICAL GROUPING ONLY!
```

---

**Previous Topic:** [25_InheritingConstructors](../25_InheritingConstructors/) - Inheriting Constructors  
**Next Topic:** [27_FriendClasses](../27_FriendClasses/) - Friend Classes and Functions

---

*Part of the C++ Classes and Objects series*  
*Difficulty: Intermediate*  
*Prerequisites: Classes, access modifiers, pointers*  
*ESSENTIAL for Proper Encapsulation*
---

## 🧾 Notes moved from main.cpp

- **Access rules, corrected** (all ✅ tested with GCC 13):

| Question | Answer |
|---|---|
| Nested class uses outer's private/protected non-static member | Yes, through an object (`outer.privateData`); the plain name `privateData` is an error |
| Nested class uses outer's static member | Yes, directly, no object needed |
| Outer class uses nested class's private member | **No**, unless the nested class declares `friend class Outer;` |
| Outside code uses a private/protected nested *type* | No (`'class Outer::PrivNested' is private within this context`) |
| Derived class of outer uses a protected nested type | Yes |

- **Nested vs inheritance:** a nested class is *scoping/containment of a type* (`Outer::Inner i;` is an independent object); inheritance is IS-A (`Derived` contains a `Base` part). Nesting a type does not add a data member: `sizeof` of a struct with a nested class is the same as without ✅ tested (Example 10).
- **Templates and nested classes** ✅ compiled:

```cpp
template<typename T>
class Outer {                      // nested class in a class template
public:
    class Inner { T data; public: Inner(T d) : data(d) {} };
};
Outer<int>::Inner a(42);

class Plain {                      // member template: the NESTED class is the template
public:
    template<typename T>
    class Inner { T data; public: Inner(T d) : data(d) {} };
};
Plain::Inner<int> b(42);
```

- **Naming and design principles** (from Example 10): write `OuterClass::InnerClass`, use descriptive names that make the relationship clear; aim for high cohesion (the nested class really belongs to the outer one) and low coupling. Do not nest a class that several other classes use. (The remaining DO/DON'T lists are already in "Best Practices" above.)
- **Decision tree:** tightly coupled to another class? No → separate class. Implementation detail? → private nested class. Part of the public interface (Iterator, Builder)? → public nested class. Reused elsewhere? → separate class. Used in one function only? → local class.
- **Corrected comparison table:**

| Feature | Nested | Separate | Local |
|---|---|---|---|
| Scope | Outer class | Namespace / global | Function |
| Access to outer's members | Yes (like a member function) | No | Only the function's statics, types and enumerators - not its parameters or automatic variables |
| Reusability | Low | High | None |
| Encapsulation | High | Low | High |
| Coupling | High | Low | High |
| Visibility | Controlled by public/protected/private | Public | Function only |
| Static data members | Yes | Yes | **No** |
| Member templates | Yes | Yes | **No** |

  The old table said "Access outer: Limited" for local classes and did not mention the member-template restriction.
- **Code changes in main.cpp:**
  - Top comment ("Outer class can access nested's private members") and the Example 8 lecture text corrected; the friend case is a real demo now (`Locker`).
  - Example 4: `Builder` no longer leaks; it holds a `Pizza` value and is defined after `Pizza`.
  - Examples 2 and 6: copy operations deleted (raw pointers); `insertHelper` private.
  - Example 3: range-for demo; Example 7: static local; Example 8: static member without object; Example 10: real demo.
  - `#include <stdexcept>` / `<initializer_list>` added (they were used but not included); unused `<memory>` removed.
  - ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer (the original printed a leak report).
