# C++ Callbacks - Complete Guide

## Table of Contents
- [What is a Callback?](#what-is-a-callback)
- [Where Callbacks Belong in C++](#where-callbacks-belong-in-c)
- [Callbacks vs Function Pointers](#callbacks-vs-function-pointers)
- [Why Use Callbacks?](#why-use-callbacks)
- [Implementation Methods](#implementation-methods)
- [Examples Explained](#examples-explained)
- [Real-World Use Cases](#real-world-use-cases)
- [Best Practices](#best-practices)

---

## What is a Callback?

A **callback** is a programming technique where you pass executable code (a function) as a parameter to another function. The receiving function can then "call back" to execute that code at an appropriate time.

### Conceptual Definition

```
Callback = "Call me back later with this function"

Regular function call:
    You → Function → Result
    
Callback pattern:
    You → Function (with callback) → Function calls your callback → Result
```

### Simple Analogy

Think of it like leaving your phone number at a restaurant:
- You give them your number (callback function)
- They take your order (main function executes)
- When food is ready, they call you back (execute callback)

```cpp
// Instead of waiting at counter:
waitForFood();  // Blocking

// You provide callback:
orderFood(callMeWhenReady);  // Non-blocking, they'll call you
```

---

## Where Callbacks Belong in C++

### Important Classification

**Callbacks are NOT a C++ language feature** - they are a **programming technique** that uses existing C++ features.

```
┌─────────────────────────────────────────────────────────┐
│  C++ LANGUAGE FEATURES (Built into the language)        │
│  ┌───────────────────────────────────────────────────┐ │
│  │  • Pointers                                       │ │
│  │  • Functions                                      │ │
│  │  • Function pointers                              │ │
│  │  • std::function (standard library)               │ │
│  │  • Lambda expressions                             │ │
│  │  • Templates                                      │ │
│  └───────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────┘
                          ↓
                    These enable
                          ↓
┌─────────────────────────────────────────────────────────┐
│  PROGRAMMING TECHNIQUES (How you use the features)      │
│  ┌───────────────────────────────────────────────────┐ │
│  │  • Callbacks ← YOU ARE HERE!                      │ │
│  │  • RAII                                           │ │
│  │  • Dependency Injection                           │ │
│  │  • Template Metaprogramming                       │ │
│  └───────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────┘
```

### Multiple Categories

Callbacks belong to several overlapping categories:

#### 1. **Programming Technique/Idiom**

Most accurate description - a recognized way of solving problems:

```cpp
// A technique for customizable behavior
void processData(Data d, function<void(Result)> callback) {
    Result r = compute(d);
    callback(r);  // Technique: pass control to user code
}
```

#### 2. **Functional Programming Concept**

Functions as first-class citizens:

```cpp
// Treating functions like data
function<int(int)> operation = multiplyByTwo;
int result = operation(5);  // Function stored in variable

// Passing functions as parameters
void applyToAll(vector<int>& v, function<void(int)> func) {
    for (int x : v) {
        func(x);  // Function as parameter
    }
}
```

#### 3. **Inversion of Control (IoC) Mechanism**

Control flow is inverted - library calls your code:

```cpp
// Traditional: You control everything
void traditionalFlow() {
    Data d = fetchData();
    Result r = process(d);
    display(r);            // You decide when to display
}

// Callback: Library controls when YOUR code runs
void callbackFlow() {
    fetchDataAsync(url, [](Data d) {
        Result r = process(d);
        display(r);        // Library decides WHEN to call this
    });
}
```

#### 4. **Design Pattern Component**

Callbacks are used to implement formal design patterns:

```cpp
// Observer Pattern (uses callbacks)
class Subject {
    vector<function<void(Event)>> observers;  // Callbacks
    void notify(Event e) {
        for (auto& callback : observers) {
            callback(e);  // Notify through callbacks
        }
    }
};

// Strategy Pattern (uses callbacks)
class Sorter {
    function<bool(int, int)> compareStrategy;  // Callback as strategy
    void sort(vector<int>& v) {
        // Use callback for comparison logic
    }
};
```

### Terminology Clarification

```
┌────────────────────────────────────────────────────────┐
│  WHAT PEOPLE CALL IT         │  TECHNICAL ACCURACY     │
├──────────────────────────────┼─────────────────────────┤
│  "Callback pattern"          │  ⚠️  Informal, acceptable│
│  "Callback technique"        │  ✅  Accurate            │
│  "Callback mechanism"        │  ✅  Very accurate       │
│  "Callback idiom"            │  ✅  Most precise        │
│  "Callback design pattern"   │  ❌  Technically wrong   │
│  "Callback language feature" │  ❌  Wrong               │
└──────────────────────────────┴─────────────────────────┘
```

**Note:** Callback is NOT a formal Gang of Four (GoF) design pattern, but it's a technique used BY design patterns.

---

## Callbacks vs Function Pointers

### The Relationship

**Key Point**: A callback is a *technique/concept*, while a function pointer is one *implementation method*.

```
┌─────────────────────────────────────────┐
│           CALLBACK (Technique)          │
│   "Passing executable code as a param"  │
│                                         │
│  Implementation Methods in C++:         │
│  ┌───────────────────────────────────┐ │
│  │  1. Function Pointers (C-style)   │ │
│  │  2. std::function (Modern C++)    │ │
│  │  3. Lambda Expressions            │ │
│  │  4. Function Objects (Functors)   │ │
│  │  5. Member Function Pointers      │ │
│  └───────────────────────────────────┘ │
└─────────────────────────────────────────┘
```

### Characterizing Callbacks

**Callback is:**
- ✅ A programming technique/idiom
- ✅ A mechanism used by design patterns
- ✅ A way to achieve inversion of control
- ✅ Often implemented using function pointers
- ✅ A functional programming concept
- ✅ More general than just function pointers

**Callback is NOT:**
- ❌ A C++ language feature
- ❌ Limited to only function pointers
- ❌ A formal GoF design pattern
- ❌ A specific C++ type
- ❌ Only used for asynchronous operations

### Comparison Table

| Aspect | Function Pointer | Callback |
|--------|-----------------|----------|
| **Nature** | Language feature | Programming technique |
| **Category** | C++ built-in capability | Implementation pattern |
| **Scope** | Specific C++ type | General concept (language-agnostic) |
| **Usage** | Stores function address | Provides custom behavior via functions |
| **Flexibility** | Limited (just function addresses) | Flexible (functions, lambdas, functors, etc.) |
| **Level** | Low-level mechanism | High-level technique |

---

## Why Use Callbacks?

### 1. **Customization and Flexibility**

Allow users of your function to customize its behavior:

```cpp
// Generic sorting with custom comparison
void sort(int arr[], int size, bool (*compare)(int, int)) {
    // Sort logic using compare callback
}

// Users decide sorting order
bool ascending(int a, int b) { return a < b; }
bool descending(int a, int b) { return a > b; }

sort(arr, 5, ascending);   // Custom behavior 1
sort(arr, 5, descending);  // Custom behavior 2
```

### 2. **Separation of Concerns**

Separate "what to do" from "how to do it":

```cpp
// Library code: defines WHEN to execute
void processData(Data d, void (*callback)(Data)) {
    // Complex processing...
    callback(d);  // Library doesn't know HOW to handle result
}

// User code: defines HOW to handle
void saveToFile(Data d) { /* save logic */ }
void sendToServer(Data d) { /* network logic */ }

processData(data, saveToFile);    // User decides what happens
processData(data, sendToServer);  // Different handling
```

### 3. **Event-Driven Programming**

Respond to events without tight coupling:

```cpp
// GUI framework
button.onClick(handleClick);        // Register callback
button.onHover(handleHover);        // Different event, different callback

// Framework calls your callbacks when events occur
```

### 4. **Asynchronous Operations**

Handle results that come later:

```cpp
// Start async operation with callback
fetchDataFromNetwork(url, [](Data result) {
    // This executes when data arrives
    processResult(result);
});

// Continue doing other work...
```

### 5. **Inversion of Control**

Library/framework controls when your code runs:

```cpp
// Without IoC: You control flow
void myCode() {
    step1();
    step2();  // You decide when
    step3();
}

// With IoC (callbacks): Library controls flow
void libraryCode(function<void()> userStep) {
    setupEnvironment();
    userStep();  // Library decides when
    cleanup();
}
```

---

## Implementation Methods

### Method 1: Function Pointers (C-Style)

**What it is:** Storing the address of a function

**Language feature used:** Function pointers (built-in to C/C++)

```cpp
// Syntax: return_type (*pointer_name)(parameter_types)
void (*callback)(int);  // Pointer to function taking int, returning void

// Usage
void myFunction(int x) {
    cout << x << endl;
}

callback = myFunction;  // Store address
callback(42);           // Call through pointer
```

**Pros:**
- ✅ Simple and lightweight
- ✅ Compatible with C
- ✅ Zero overhead
- ✅ Part of core language

**Cons:**
- ❌ Cannot capture state (no closures)
- ❌ Syntax can be confusing
- ❌ No type safety with templates
- ❌ Cannot use with lambdas that capture

**Category:** Core C/C++ language feature

### Method 2: std::function (Modern C++)

**What it is:** Type-erased function wrapper

**Language feature used:** std::function (C++11 standard library)

```cpp
#include <functional>

// Flexible type that can hold anything callable
function<void(int)> callback;

// Can hold function pointer
callback = myFunction;

// Can hold lambda
callback = [](int x) { cout << x; };

// Can hold functor
struct Functor {
    void operator()(int x) { cout << x; }
};
callback = Functor();
```

**Pros:**
- ✅ Very flexible (lambdas, functors, function pointers)
- ✅ Can capture state
- ✅ Type-safe
- ✅ Easy to read
- ✅ Standard library support

**Cons:**
- ❌ Small performance overhead (type erasure)
- ❌ Requires `<functional>` header
- ❌ Slight memory overhead

**Category:** C++ Standard Library feature

### Method 3: Lambda Expressions

**What it is:** Inline anonymous functions

**Language feature used:** Lambda expressions (C++11 core language feature)

```cpp
// Lambda can be passed directly as callback
processVector(vec, [](int x) {
    cout << x * 2 << " ";
});

// Lambda with capture (stores state)
int multiplier = 10;
processVector(vec, [multiplier](int x) {
    cout << x * multiplier << " ";  // Uses captured value
});
```

**Pros:**
- ✅ Concise inline syntax
- ✅ Can capture variables (closures)
- ✅ Modern and readable
- ✅ Powerful with captures
- ✅ Type-safe

**Cons:**
- ❌ Can be less clear for complex logic
- ❌ Capturing lambdas need std::function (can't use raw function pointers)
- ❌ C++11 or later required

**Category:** C++ Core language feature (C++11+)

### Method 4: Function Objects (Functors)

**What it is:** Objects that act like functions

**Language feature used:** Operator overloading + classes

```cpp
struct Multiplier {
    int factor;
    
    Multiplier(int f) : factor(f) {}
    
    void operator()(int x) {  // Makes object callable
        cout << x * factor << " ";
    }
};

// Usage
Multiplier timesTwo(2);
processVector(vec, timesTwo);  // Pass functor as callback
```

**Pros:**
- ✅ Can store state
- ✅ Can have multiple member functions
- ✅ Reusable
- ✅ Works with older C++ standards

**Cons:**
- ❌ More verbose
- ❌ Requires class definition
- ❌ More boilerplate code

**Category:** Object-Oriented Programming technique using C++ classes

### Method 5: Member Function Pointers

**What it is:** Pointers to class member functions

**Language feature used:** Member function pointers (built-in)

```cpp
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
};

// Pointer to member function
int (Calculator::*operation)(int, int);

Calculator calc;
operation = &Calculator::add;
int result = (calc.*operation)(5, 3);  // Call through pointer
```

**Pros:**
- ✅ Works with class methods
- ✅ Part of core language
- ✅ No overhead

**Cons:**
- ❌ Complex syntax
- ❌ Requires object instance
- ❌ Not as commonly used

**Category:** Core C++ language feature (object-oriented)

---

## Examples Explained

### Example 1: Function Pointer Callback

```cpp
void processArray(int arr[], int size, void (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);  // Execute callback for each element
    }
}
```

**Breakdown:**
```
void (*callback)(int)
 │    │    │      │
 │    │    │      └─ Parameter type: takes one int
 │    │    └──────── Name of the pointer variable
 │    └───────────── This is a POINTER
 └────────────────── Returns void
```

**Implementation method used:** Function pointers (C-style)

**How it works:**

```
Step 1: Define callback functions
┌──────────────────┐
│ void printDouble │
│ void printSquare │
└──────────────────┘

Step 2: Pass function as parameter
processArray(numbers, 5, printDouble)
                         │
                         └─ Address of printDouble function

Step 3: processArray calls the callback
for each element:
    callback(arr[0])  → printDouble(1) → prints 2
    callback(arr[1])  → printDouble(2) → prints 4
    callback(arr[2])  → printDouble(3) → prints 6
    ...
```

**Visual Flow:**

```
main()                          processArray()
  │                                   │
  ├─ numbers[] = {1,2,3,4,5}         │
  │                                   │
  ├─ calls processArray(..., printDouble)
  │                                   │
  │                             ┌─────▼──────┐
  │                             │ for loop   │
  │                             │ i = 0      │
  │                             └─────┬──────┘
  │                                   │
  │                             callback(arr[0])
  │                                   │
  │  ┌────────────────────────────────┘
  │  │
  │  ▼
  printDouble(1)
  │  │
  │  └─ prints: 2
  │
  └─ [loop continues...]
```

### Example 2: std::function with Lambda

```cpp
void processVector(const vector<int>& vec, function<void(int)> callback) {
    for (int num : vec) {
        callback(num);
    }
}

// Called with lambda
processVector(vec, [](int x) {
    cout << (x * x + x) << " ";
});
```

**Implementation method used:** std::function (modern C++) + Lambda expressions

**Why std::function is better here:**

```cpp
// function<void(int)> can accept:

1. Function pointer:
   processVector(vec, printDouble);

2. Lambda without capture:
   processVector(vec, [](int x) { cout << x; });

3. Lambda WITH capture:
   int offset = 10;
   processVector(vec, [offset](int x) { cout << x + offset; });
   // ❌ Would NOT work with raw function pointer!

4. Functor:
   processVector(vec, Multiplier(2));

5. Member function (with std::bind):
   processVector(vec, std::bind(&MyClass::method, &obj, _1));
```

---

## Real-World Use Cases

### 1. **Event Handlers (GUI Programming)**

**Technique:** Callbacks for event-driven architecture

```cpp
class Button {
    function<void()> onClick;
    
public:
    void setOnClickHandler(function<void()> callback) {
        onClick = callback;
    }
    
    void click() {
        if (onClick) {
            onClick();  // Execute callback
        }
    }
};

// Usage
Button btn;
btn.setOnClickHandler([]() {
    cout << "Button clicked!" << endl;
});

btn.click();  // Triggers callback
```

### 2. **Sorting with Custom Comparison**

**Technique:** Strategy pattern using callbacks

```cpp
struct Person {
    string name;
    int age;
};

vector<Person> people = {
    {"Alice", 30},
    {"Bob", 25},
    {"Carol", 35}
};

// Sort by age (callback defines strategy)
sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    return a.age < b.age;
});

// Sort by name (different callback, different strategy)
sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    return a.name < b.name;
});
```

### 3. **Asynchronous File I/O**

**Technique:** Inversion of control for async operations

```cpp
void readFileAsync(const string& filename, function<void(string)> onComplete) {
    // Start reading in background thread
    thread([filename, onComplete]() {
        string content = readFile(filename);
        onComplete(content);  // Call callback when done
    }).detach();
}

// Usage
readFileAsync("data.txt", [](string content) {
    cout << "File loaded: " << content.length() << " bytes" << endl;
});

// Program continues without waiting...
```

### 4. **Observer Pattern**

**Technique:** Design pattern implementation using callbacks

```cpp
class Subject {
    vector<function<void(int)>> observers;
    
public:
    void attach(function<void(int)> observer) {
        observers.push_back(observer);
    }
    
    void notify(int value) {
        for (auto& observer : observers) {
            observer(value);  // Notify all observers
        }
    }
};

// Usage
Subject subject;

// Register multiple observers (callbacks)
subject.attach([](int val) { cout << "Observer 1: " << val << endl; });
subject.attach([](int val) { cout << "Observer 2: " << val * 2 << endl; });

subject.notify(42);  // All observers called
```

### 5. **Filter/Map/Reduce Operations**

**Technique:** Functional programming with callbacks

```cpp
template<typename T>
vector<T> filter(const vector<T>& vec, function<bool(T)> predicate) {
    vector<T> result;
    for (const T& item : vec) {
        if (predicate(item)) {  // Use callback to test
            result.push_back(item);
        }
    }
    return result;
}

// Usage
vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Get even numbers
auto evens = filter(numbers, [](int x) { return x % 2 == 0; });

// Get numbers > 5
auto large = filter(numbers, [](int x) { return x > 5; });
```

---

## Callbacks Updating Classes

One of the most powerful uses of callbacks is to update class objects. Callbacks can modify class state in several ways:

### Method 1: Callback Receives Class by Reference

The callback receives the class object as a parameter and modifies it directly:

```cpp
class Counter {
public:
    int count = 0;
    
    void increment() { count++; }
    void reset() { count = 0; }
};

// Function accepts callback that can modify Counter
void processWithCallback(Counter& counter, function<void(Counter&)> callback) {
    cout << "Before callback: count = " << counter.count << endl;
    callback(counter);  // Callback modifies counter
    cout << "After callback: count = " << counter.count << endl;
}

int main() {
    Counter myCounter;
    myCounter.count = 5;
    
    // Lambda callback modifies the class
    processWithCallback(myCounter, [](Counter& c) {
        c.increment();
        c.increment();
        c.count += 10;
    });
    
    cout << "Final count: " << myCounter.count << endl;  // 17
    return 0;
}
```

**Output:**
```
Before callback: count = 5
After callback: count = 17
Final count: 17
```

**Use case:** Passing objects to functions that need to modify them through callbacks.

### Method 2: Member Function as Callback (Updates Own Class)

A class member function acts as a callback and updates the class it belongs to:

```cpp
class DataProcessor {
private:
    vector<int> results;
    int processedCount = 0;
    
public:
    // Member function that acts as callback
    void collectResult(int value) {
        results.push_back(value);      // Updates member variable
        processedCount++;              // Updates counter
        cout << "Collected: " << value << endl;
    }
    
    void process(const vector<int>& data) {
        // Using member function as callback
        for (int x : data) {
            // Bind member function to this object
            auto callback = bind(&DataProcessor::collectResult, this, placeholders::_1);
            callback(x * 2);  // Calls member function, updates class
        }
    }
    
    void showResults() {
        cout << "Processed " << processedCount << " items" << endl;
        cout << "Results: ";
        for (int r : results) cout << r << " ";
        cout << endl;
    }
};

int main() {
    DataProcessor processor;
    processor.process({1, 2, 3, 4, 5});
    processor.showResults();
    return 0;
}
```

**Output:**
```
Collected: 2
Collected: 4
Collected: 6
Collected: 8
Collected: 10
Processed 5 items
Results: 2 4 6 8 10
```

**Use case:** Object-oriented design where methods serve as callbacks for their own class.

### Method 3: Lambda Captures Class and Updates It

Lambda expressions can capture class objects and modify them:

```cpp
class Logger {
public:
    vector<string> logs;
    int errorCount = 0;
    int infoCount = 0;
    
    void addLog(const string& level, const string& message) {
        logs.push_back("[" + level + "] " + message);
        
        if (level == "ERROR") errorCount++;
        else if (level == "INFO") infoCount++;
    }
    
    void printStats() {
        cout << "Total logs: " << logs.size() 
             << " (Errors: " << errorCount 
             << ", Info: " << infoCount << ")" << endl;
    }
};

void doWork(function<void(string, string)> logCallback) {
    logCallback("INFO", "Starting work...");
    
    // Simulate some work with potential error
    bool hasError = true;
    if (hasError) {
        logCallback("ERROR", "Something went wrong!");
    }
    
    logCallback("INFO", "Work completed!");
}

int main() {
    Logger logger;
    
    // Lambda captures logger by reference and updates it
    doWork([&logger](string level, string msg) {
        logger.addLog(level, msg);  // Updates logger class
        cout << "Logged: [" << level << "] " << msg << endl;
    });
    
    cout << "\n";
    logger.printStats();
    
    return 0;
}
```

**Output:**
```
Logged: [INFO] Starting work...
Logged: [ERROR] Something went wrong!
Logged: [INFO] Work completed!

Total logs: 3 (Errors: 1, Info: 2)
```

**Use case:** Closures that maintain and update state across multiple invocations.

### Method 4: Callback Passed Class Pointer/Reference

The callback is given a pointer or reference to update the class:

```cpp
class GameScore {
private:
    int score = 0;
    int lives = 3;
    string playerName;
    
public:
    GameScore(string name) : playerName(name) {}
    
    void addPoints(int points) {
        score += points;
        cout << playerName << " earned " << points << " points! Total: " << score << endl;
    }
    
    void loseLife() {
        if (lives > 0) {
            lives--;
            cout << playerName << " lost a life! Remaining: " << lives << endl;
        }
    }
    
    void showStats() {
        cout << "\n=== " << playerName << " Stats ===" << endl;
        cout << "Score: " << score << endl;
        cout << "Lives: " << lives << endl;
    }
};

// Game event system that updates score through callbacks
void triggerEvent(string eventType, GameScore* score, 
                  function<void(GameScore*, string)> callback) {
    cout << "Event triggered: " << eventType << endl;
    callback(score, eventType);  // Callback updates GameScore
}

int main() {
    GameScore playerScore("Alice");
    
    // Define callback that updates GameScore based on events
    auto eventHandler = [](GameScore* score, string eventType) {
        if (eventType == "COLLECT_COIN") {
            score->addPoints(10);
        } else if (eventType == "COLLECT_GEM") {
            score->addPoints(50);
        } else if (eventType == "HIT_ENEMY") {
            score->loseLife();
        }
    };
    
    // Trigger various events
    triggerEvent("COLLECT_COIN", &playerScore, eventHandler);
    triggerEvent("COLLECT_GEM", &playerScore, eventHandler);
    triggerEvent("COLLECT_COIN", &playerScore, eventHandler);
    triggerEvent("HIT_ENEMY", &playerScore, eventHandler);
    
    playerScore.showStats();
    
    return 0;
}
```

**Output:**
```
Event triggered: COLLECT_COIN
Alice earned 10 points! Total: 10
Event triggered: COLLECT_GEM
Alice earned 50 points! Total: 60
Event triggered: COLLECT_COIN
Alice earned 10 points! Total: 70
Event triggered: HIT_ENEMY
Alice lost a life! Remaining: 2

=== Alice Stats ===
Score: 70
Lives: 2
```

**Use case:** Event-driven systems, game loops, state machines.

### Method 5: Observer Pattern - Multiple Classes Updated

Multiple class objects (observers) are updated through callbacks when a subject changes:

```cpp
class Display {
private:
    string name;
    int displayedValue = 0;
    
public:
    Display(string n) : name(n) {}
    
    void update(int value) {
        displayedValue = value;
        cout << name << " updated to: " << value << endl;
    }
    
    int getValue() const { return displayedValue; }
    string getName() const { return name; }
};

class Sensor {
private:
    vector<function<void(int)>> observers;  // Callbacks to update observers
    int currentValue = 0;
    string sensorName;
    
public:
    Sensor(string name) : sensorName(name) {}
    
    // Register callback that will update an observer
    void attach(function<void(int)> observer) {
        observers.push_back(observer);
        cout << "Observer attached to " << sensorName << endl;
    }
    
    void setValue(int value) {
        cout << "\n" << sensorName << " value changed: " 
             << currentValue << " -> " << value << endl;
        currentValue = value;
        notifyAll();  // Trigger all callbacks
    }
    
private:
    void notifyAll() {
        cout << "Notifying " << observers.size() << " observers..." << endl;
        for (auto& callback : observers) {
            callback(currentValue);  // Each callback updates its class
        }
    }
};

int main() {
    Sensor tempSensor("Temperature Sensor");
    
    Display display1("LCD Display");
    Display display2("LED Display");
    Display display3("Web Dashboard");
    
    // Register callbacks that update display objects
    tempSensor.attach([&display1](int value) {
        display1.update(value);  // Callback updates Display1
    });
    
    tempSensor.attach([&display2](int value) {
        display2.update(value);  // Callback updates Display2
    });
    
    tempSensor.attach([&display3](int value) {
        display3.update(value);  // Callback updates Display3
    });
    
    // Change sensor value - all displays get updated
    tempSensor.setValue(22);
    tempSensor.setValue(25);
    
    // Verify all displays were updated
    cout << "\n=== Final Display States ===" << endl;
    cout << display1.getName() << ": " << display1.getValue() << "°C" << endl;
    cout << display2.getName() << ": " << display2.getValue() << "°C" << endl;
    cout << display3.getName() << ": " << display3.getValue() << "°C" << endl;
    
    return 0;
}
```

**Output:**
```
Observer attached to Temperature Sensor
Observer attached to Temperature Sensor
Observer attached to Temperature Sensor

Temperature Sensor value changed: 0 -> 22
Notifying 3 observers...
LCD Display updated to: 22
LED Display updated to: 22
Web Dashboard updated to: 22

Temperature Sensor value changed: 22 -> 25
Notifying 3 observers...
LCD Display updated to: 25
LED Display updated to: 25
Web Dashboard updated to: 25

=== Final Display States ===
LCD Display: 25°C
LED Display: 25°C
Web Dashboard: 25°C
```

**Use case:** Pub-sub systems, MVC architecture, real-time dashboards, reactive programming.

### Method 6: Async Callback Updates Class State

Callbacks from asynchronous operations update class state when they complete:

```cpp
class DataFetcher {
private:
    string data;
    bool isLoading = false;
    bool hasError = false;
    string errorMessage;
    
public:
    void fetchData(string url, function<void()> onComplete) {
        isLoading = true;
        hasError = false;
        cout << "Fetching data from: " << url << endl;
        
        // Simulate async operation with thread
        thread([this, url, onComplete]() {
            this_thread::sleep_for(chrono::seconds(2));  // Simulate delay
            
            // Simulate success/failure
            if (url.find("valid") != string::npos) {
                data = "Data from " + url;
                isLoading = false;
                cout << "Data fetched successfully!" << endl;
            } else {
                hasError = true;
                errorMessage = "Failed to fetch from " + url;
                isLoading = false;
                cout << "Error occurred!" << endl;
            }
            
            onComplete();  // Trigger callback after async work
        }).detach();
    }
    
    void displayStatus() {
        cout << "\n=== Status ===" << endl;
        cout << "Loading: " << (isLoading ? "Yes" : "No") << endl;
        cout << "Has Error: " << (hasError ? "Yes" : "No") << endl;
        if (hasError) {
            cout << "Error: " << errorMessage << endl;
        } else if (!data.empty()) {
            cout << "Data: " << data << endl;
        }
    }
};

int main() {
    DataFetcher fetcher;
    
    cout << "Starting async fetch..." << endl;
    
    // Callback updates DataFetcher state when async operation completes
    fetcher.fetchData("https://api.valid-server.com/data", [&fetcher]() {
        cout << "\nCallback executed - data fetch complete!" << endl;
        fetcher.displayStatus();  // Show updated state
    });
    
    cout << "Main thread continues..." << endl;
    
    // Wait for async operation
    this_thread::sleep_for(chrono::seconds(3));
    
    return 0;
}
```

**Output:**
```
Starting async fetch...
Fetching data from: https://api.valid-server.com/data
Main thread continues...
Data fetched successfully!

Callback executed - data fetch complete!

=== Status ===
Loading: No
Has Error: No
Data: Data from https://api.valid-server.com/data
```

**Use case:** Network requests, file I/O, database queries, any async operation.

---

## Important Considerations When Updating Classes

### ⚠️ Lifetime and Scope Issues

**Problem: Dangling References**

```cpp
// ❌ DANGER: Class destroyed but callback still references it
function<void()> dangerousCallback;

void setupCallback() {
    MyClass obj;  // Stack-allocated object
    
    dangerousCallback = [&obj]() {
        obj.updateState();  // obj captured by reference
    };
}  // obj destroyed here!

int main() {
    setupCallback();
    dangerousCallback();  // ❌ UNDEFINED BEHAVIOR - obj is gone!
    return 0;
}
```

**Solution 1: Capture by Value**

```cpp
// ✅ SAFE: Capture by value (makes a copy)
void setupCallback() {
    MyClass obj;
    
    dangerousCallback = [obj]() mutable {
        obj.updateState();  // Works on copy
    };
}  // Original obj destroyed, but copy in lambda is safe
```

**Solution 2: Use shared_ptr**

```cpp
// ✅ SAFE: Use shared ownership
auto obj = make_shared<MyClass>();

auto safeCallback = [obj]() {
    obj->updateState();  // obj kept alive by shared_ptr
};

// obj stays alive as long as callback exists
```

**Solution 3: Ensure Object Outlives Callback**

```cpp
class Application {
    MyClass obj;  // Member variable, lives as long as Application
    
    void setupCallback() {
        auto callback = [this]() {
            obj.updateState();  // Safe: obj has same lifetime as Application
        };
        // Use callback...
    }
};
```

### ⚠️ Thread Safety

When callbacks update classes from different threads:

```cpp
class ThreadSafeCounter {
private:
    int count = 0;
    mutex mtx;  // Protect count with mutex
    
public:
    void increment() {
        lock_guard<mutex> lock(mtx);  // Thread-safe
        count++;
    }
    
    int getCount() {
        lock_guard<mutex> lock(mtx);
        return count;
    }
};

void parallelProcess(ThreadSafeCounter& counter) {
    vector<thread> threads;
    
    // Multiple threads updating same object via callbacks
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; j++) {
                counter.increment();  // Thread-safe update
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}
```

### ⚠️ Callback Order and Dependencies

When multiple callbacks update the same class:

```cpp
class StateMachine {
    string state = "IDLE";
    
public:
    void setState(string newState) { state = newState; }
    string getState() { return state; }
};

void processWithCallbacks(StateMachine& sm) {
    vector<function<void(StateMachine&)>> callbacks;
    
    // Callback 1: Expects IDLE state
    callbacks.push_back([](StateMachine& s) {
        if (s.getState() == "IDLE") {
            s.setState("PROCESSING");
        }
    });
    
    // Callback 2: Expects PROCESSING state
    callbacks.push_back([](StateMachine& s) {
        if (s.getState() == "PROCESSING") {
            s.setState("COMPLETE");
        }
    });
    
    // Execute in order - order matters!
    for (auto& callback : callbacks) {
        callback(sm);
    }
}
```

---

## Common Patterns for Updating Classes

### Pattern 1: UI Event Handlers

```cpp
class Button {
    string label;
    function<void(Button*)> clickHandler;
    
public:
    Button(string lbl) : label(lbl) {}
    
    void setOnClick(function<void(Button*)> handler) {
        clickHandler = handler;
    }
    
    void click() {
        if (clickHandler) {
            clickHandler(this);  // Pass self to callback
        }
    }
};

class Application {
    int clickCount = 0;
    
public:
    void run() {
        Button btn("Click Me");
        
        btn.setOnClick([this](Button* b) {
            clickCount++;  // Update application state
            cout << "Button clicked " << clickCount << " times" << endl;
        });
        
        btn.click();
        btn.click();
        btn.click();
    }
};
```

### Pattern 2: Data Pipeline

```cpp
class DataPipeline {
    vector<string> processedData;
    
public:
    void process(vector<string>& rawData, 
                 function<string(string)> transformer,
                 function<void(DataPipeline&, string)> collector) {
        for (auto& item : rawData) {
            string transformed = transformer(item);
            collector(*this, transformed);  // Callback updates pipeline
        }
    }
    
    void addProcessed(string data) {
        processedData.push_back(data);
    }
    
    void showResults() {
        for (auto& d : processedData) {
            cout << d << endl;
        }
    }
};
```

### Pattern 3: Configuration Builder

```cpp
class Config {
    map<string, string> settings;
    
public:
    void apply(function<void(Config&)> configurator) {
        configurator(*this);  // Callback configures this object
    }
    
    void set(string key, string value) {
        settings[key] = value;
    }
    
    string get(string key) {
        return settings[key];
    }
};

// Usage
Config config;
config.apply([](Config& c) {
    c.set("host", "localhost");
    c.set("port", "8080");
    c.set("timeout", "30");
});
```

---

## Best Practices

### 1. **Choose the Right Implementation Method**

```cpp
// Use function pointer: Simple, no state needed, C compatibility
void process(int arr[], int size, void (*callback)(int));

// Use std::function: Need flexibility (lambdas with capture)
void process(const vector<int>& v, function<void(int)> callback);

// Use template: Maximum performance, compile-time optimization
template<typename Callback>
void process(const vector<int>& v, Callback callback);
```

### 2. **Check for nullptr Before Calling**

```cpp
void processData(Data d, function<void(Data)> callback) {
    // Process data...
    
    if (callback) {  // ✅ Always check if callback exists
        callback(d);
    }
}
```

### 3. **When Updating Classes: Manage Lifetimes Carefully**

```cpp
// ❌ DANGER: Capturing short-lived object by reference
void dangerousPattern() {
    MyClass obj;
    scheduleCallback([&obj]() {
        obj.update();  // obj might be destroyed!
    });
}  // obj destroyed here

// ✅ SAFE: Capture by value
void safePattern1() {
    MyClass obj;
    scheduleCallback([obj]() mutable {
        obj.update();  // Works on copy
    });
}

// ✅ SAFE: Use shared_ptr for shared ownership
void safePattern2() {
    auto obj = make_shared<MyClass>();
    scheduleCallback([obj]() {
        obj->update();  // obj kept alive
    });
}

// ✅ SAFE: Ensure object lifetime exceeds callback lifetime
class Application {
    MyClass member;  // Lives as long as Application
    
    void setup() {
        scheduleCallback([this]() {
            member.update();  // Safe: member outlives callback
        });
    }
};
```

### 4. **Thread Safety When Updating Shared Objects**

```cpp
// ❌ UNSAFE: Multiple threads modifying without protection
class UnsafeCounter {
    int count = 0;
    
public:
    void increment() { count++; }  // Race condition!
};

// ✅ SAFE: Protect with mutex
class SafeCounter {
    int count = 0;
    mutex mtx;
    
public:
    void increment() {
        lock_guard<mutex> lock(mtx);
        count++;  // Thread-safe
    }
};
```

### 5. **Document Callback Requirements**

```cpp
/**
 * Process array with custom operation
 * 
 * @param callback Called for each element. 
 *                 Signature: void callback(int element)
 *                 Must not throw exceptions.
 *                 Called exactly once per element.
 *                 Execution order: sequential from index 0 to size-1
 *                 Can modify passed objects if they're non-const references
 */
void processArray(int arr[], int size, function<void(int)> callback);
```

### 6. **Be Careful with Captures in Lambdas**

```cpp
void scheduleCallback(function<void()> callback) {
    // Callback will be called later...
}

// ❌ DANGER: Capturing local variable by reference
void badExample() {
    int x = 42;
    scheduleCallback([&x]() {  // x captured by reference
        cout << x;
    });
}  // x destroyed here, but callback still has reference!

// ✅ SAFE: Capture by value
void goodExample() {
    int x = 42;
    scheduleCallback([x]() {  // x captured by value (copied)
        cout << x;
    });
}  // Safe: callback has its own copy
```

### 7. **Use const& for Complex Parameters**

```cpp
// ❌ Inefficient: copies vector
void process(vector<int> vec, function<void(int)> callback);

// ✅ Efficient: no copy
void process(const vector<int>& vec, function<void(int)> callback);
```

### 8. **Consider Exception Safety**

```cpp
void processWithCallback(function<void()> callback) {
    try {
        callback();  // Callback might throw
    } catch (const exception& e) {
        // Handle or log error
        cerr << "Callback threw: " << e.what() << endl;
    }
}
```

### 9. **Use Forward Declaration for Function Pointers**

```cpp
// Forward declare callback type for clarity
using ErrorCallback = void(*)(const string&);
using SuccessCallback = void(*)(int);

void fetchData(const string& url, 
               SuccessCallback onSuccess,
               ErrorCallback onError);
```

---

## Common Patterns

### Pattern 1: Error Callback

```cpp
void fetchData(string url, 
               function<void(Data)> onSuccess,
               function<void(string)> onError) {
    try {
        Data result = downloadData(url);
        onSuccess(result);
    } catch (const exception& e) {
        onError(e.what());
    }
}

// Usage
fetchData("http://api.example.com",
    [](Data d) { cout << "Success: " << d << endl; },
    [](string err) { cout << "Error: " << err << endl; }
);
```

### Pattern 2: Progress Callback

```cpp
void processLargeFile(string filename, 
                     function<void(int)> onProgress) {
    int totalLines = countLines(filename);
    int processed = 0;
    
    for (string line : readLines(filename)) {
        processLine(line);
        processed++;
        
        int percent = (processed * 100) / totalLines;
        onProgress(percent);  // Report progress
    }
}

// Usage
processLargeFile("huge.txt", [](int percent) {
    cout << "Progress: " << percent << "%" << endl;
});
```

### Pattern 3: Chain of Callbacks

```cpp
void step1(function<void(int)> next) {
    int result = compute();
    next(result);
}

void step2(int input, function<void(string)> next) {
    string result = format(input);
    next(result);
}

void step3(string input, function<void()> next) {
    save(input);
    next();
}

// Usage (callback chain)
step1([](int val) {
    step2(val, [](string str) {
        step3(str, []() {
            cout << "All done!" << endl;
        });
    });
});
```

---

## Performance Considerations

### Function Pointer vs std::function vs Template

```cpp
// Function pointer: Zero overhead
void process1(int arr[], int size, void (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);  // Direct call
    }
}

// std::function: Small overhead (type erasure, virtual dispatch)
void process2(int arr[], int size, function<void(int)> callback) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);  // Indirect call through wrapper
    }
}

// Template: Best performance (inlined at compile-time)
template<typename Callback>
void process3(int arr[], int size, Callback callback) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);  // Can be fully inlined
    }
}
```

**Benchmarks (approximate):**
- Function pointer: 1.0x (baseline)
- std::function: 1.1-1.3x (small overhead from type erasure)
- Template: 0.9-1.0x (often optimized to be fastest)

**When to use each:**
- **Function pointer**: C compatibility, simple cases, zero overhead critical
- **std::function**: Need runtime flexibility, capturing lambdas, storage in containers
- **Template**: Maximum performance, compile-time known callbacks

---

## Compilation and Execution

```bash
# Compile with C++11 or later for std::function
g++ -std=c++11 callback_example.cpp -o callback_example

# With optimization for better performance
g++ -std=c++11 -O2 callback_example.cpp -o callback_example

# With C++14 for better lambda support
g++ -std=c++14 -O2 callback_example.cpp -o callback_example

# With C++17 for latest features
g++ -std=c++17 -O2 callback_example.cpp -o callback_example

# Run
./callback_example
```

## Expected Output

```
Doubled: 2 4 6 8 10 
Squared: 1 4 9 16 25 
Custom operation: 2 6 12 20 30
```

---

## Understanding the Category Hierarchy

```
┌─────────────────────────────────────────────────────────┐
│                    C++ ECOSYSTEM                        │
│                                                         │
│  ┌────────────────────────────────────────────────┐   │
│  │  CORE LANGUAGE FEATURES                        │   │
│  │  • Pointers, Functions, Classes, Templates     │   │
│  └────────────────────────────────────────────────┘   │
│                          ↓                              │
│  ┌────────────────────────────────────────────────┐   │
│  │  STANDARD LIBRARY                              │   │
│  │  • std::function, std::bind, containers        │   │
│  └────────────────────────────────────────────────┘   │
│                          ↓                              │
│  ┌────────────────────────────────────────────────┐   │
│  │  PROGRAMMING TECHNIQUES & IDIOMS               │   │
│  │  • Callbacks ← HERE                            │   │
│  │  • RAII                                        │   │
│  │  • Template Metaprogramming                    │   │
│  └────────────────────────────────────────────────┘   │
│                          ↓                              │
│  ┌────────────────────────────────────────────────┐   │
│  │  DESIGN PATTERNS                               │   │
│  │  • Observer (uses callbacks)                   │   │
│  │  • Strategy (uses callbacks)                   │   │
│  │  • Command (uses callbacks)                    │   │
│  └────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

---

## Key Takeaways

### About Callbacks as a Concept

1. **Callbacks are a technique**, not a language feature
2. **Multiple categories**: Functional programming, IoC mechanism, design pattern component
3. **Language-agnostic**: Exists in C, C++, JavaScript, Python, Java, etc.
4. **Not formally a design pattern**, but used to implement design patterns
5. **Flexible implementation**: Can use function pointers, std::function, lambdas, functors

### About Implementation in C++

1. **Multiple implementation methods** available in C++
2. **Function pointers**: Traditional C-style, zero overhead
3. **std::function**: Modern, flexible, small overhead
4. **Lambdas**: Concise, can capture state
5. **Templates**: Best performance when type known at compile-time

### About Usage

1. **Callbacks enable** flexible, reusable, and extensible code
2. **Common use cases**: Event handling, async operations, customization
3. **Choose implementation** based on needs: performance vs flexibility
4. **Always consider** lifetime and exception safety
5. **Modern C++ favors** std::function and lambdas over raw function pointers
6. **Callbacks can update classes** through references, pointers, captures, or member functions
7. **When updating classes**: Be careful with object lifetimes, thread safety, and callback order

---

## Further Reading

### Related C++ Features
- Function pointers in depth
- Lambda expressions and captures
- std::function and type erasure
- std::bind and placeholders
- Member function pointers

### Programming Concepts
- Functional programming in C++
- Inversion of Control (IoC)
- Higher-order functions
- Closures and lexical scope

### Design Patterns Using Callbacks
- Observer pattern
- Strategy pattern
- Command pattern
- Template Method pattern
- Chain of Responsibility

### Advanced Topics
- Callback hell and solutions
- Promises and Futures (C++11)
- Coroutines (C++20)
- Async/await patterns
- Event loops and event-driven architecture

---

## Summary: Where Callbacks Fit

**Callbacks are:**
- ✅ A **programming technique** for passing executable code
- ✅ A **functional programming concept** (functions as first-class citizens)
- ✅ An **inversion of control mechanism**
- ✅ A **component** used in many design patterns
- ✅ **Implemented** using various C++ language features

**Callbacks are NOT:**
- ❌ A C++ language feature (they USE language features)
- ❌ A formal design pattern (they're used BY patterns)
- ❌ Limited to a single implementation method

**Best description:** *"Callbacks are a programming technique that enables passing executable code as parameters, implemented in C++ through function pointers, std::function, lambdas, or functors."*

