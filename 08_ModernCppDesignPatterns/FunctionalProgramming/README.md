# Functional Programming in C++

## 📚 Introduction

Functional programming is a programming paradigm that treats computation as the evaluation of mathematical functions and avoids changing state and mutable data. While C++ is primarily an imperative and object-oriented language, modern C++ (C++11+) has embraced many functional programming concepts.

## 🎯 Core Concepts

### 1. **Pure Functions**
Functions that:
- Always return the same output for the same input
- Have no side effects
- Don't modify external state

```cpp
// Pure function
int add(int a, int b) {
    return a + b;  // No side effects, always same result
}

// Impure function
int counter = 0;
int addWithSideEffect(int a, int b) {
    counter++;  // Side effect! Modifies external state
    return a + b;
}
```

### 2. **Immutability**
Preferring const and avoiding mutation:

```cpp
// Imperative (mutation)
void processVector(std::vector<int>& vec) {
    for (auto& item : vec) {
        item *= 2;  // Mutates original
    }
}

// Functional (immutable)
std::vector<int> processVector(const std::vector<int>& vec) {
    std::vector<int> result;
    std::transform(vec.begin(), vec.end(), 
                   std::back_inserter(result),
                   [](int x) { return x * 2; });
    return result;  // Returns new vector
}
```

### 3. **Higher-Order Functions**
Functions that take other functions as parameters or return functions:

```cpp
// Higher-order function
auto applyTwice(std::function<int(int)> f) {
    return [f](int x) { return f(f(x)); };
}

auto addOne = [](int x) { return x + 1; };
auto addTwo = applyTwice(addOne);
int result = addTwo(5);  // 7
```

### 4. **Function Composition**
Combining simple functions to build complex ones:

```cpp
auto compose = [](auto f, auto g) {
    return [f, g](auto x) { return f(g(x)); };
};

auto multiplyBy2 = [](int x) { return x * 2; };
auto add10 = [](int x) { return x + 10; };
auto multiplyThenAdd = compose(add10, multiplyBy2);

int result = multiplyThenAdd(5);  // (5 * 2) + 10 = 20
```

## 🚀 Why Functional Programming in C++?

### Advantages:
✅ **More Readable** - Express *what* to do, not *how*
✅ **Less Bugs** - Immutability prevents many errors
✅ **Easier Testing** - Pure functions are easy to test
✅ **Composability** - Build complex from simple
✅ **Parallelization** - No shared state = easier concurrency
✅ **Reasoning** - Easier to understand code flow

### When to Use:
- Data transformations (map, filter, reduce)
- Pipeline processing
- Algorithms on collections
- Event handling
- Asynchronous operations

## 📊 Imperative vs Functional Comparison

### Example: Filter and Transform

**Imperative Style:**
```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> result;

// Find even numbers and square them
for (const auto& n : numbers) {
    if (n % 2 == 0) {           // Filter
        result.push_back(n * n); // Transform
    }
}
```

**Functional Style (C++20 Ranges):**
```cpp
auto result = numbers 
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; });
```

**Benefits of Functional:**
- More declarative (says *what*, not *how*)
- No manual loop management
- No intermediate variables
- Easier to read and understand
- Can be lazy-evaluated

## 🛠️ C++ Tools for Functional Programming

### 1. **Lambda Expressions** (C++11)
```cpp
auto add = [](int a, int b) { return a + b; };
auto multiply = [](int a, int b) { return a * b; };
```

### 2. **std::function** (C++11)
```cpp
std::function<int(int, int)> operation = add;
int result = operation(5, 3);  // 8
```

### 3. **STL Algorithms** (All versions)
```cpp
std::transform()   // Map
std::copy_if()     // Filter
std::accumulate()  // Reduce/Fold
std::for_each()    // Iterate with side effects
```

### 4. **Ranges** (C++20)
```cpp
std::views::filter()
std::views::transform()
std::views::take()
std::views::drop()
```

### 5. **Concepts** (C++20)
Type constraints for generic functions

## 📝 Files in This Directory

### 1. `imperative_vs_functional.cpp`
**Difficulty**: 🟢 Beginner

Side-by-side comparisons of imperative and functional approaches to common problems:
- Filtering collections
- Transforming data
- Summing values
- Finding elements
- Sorting

**Learn**: How functional style differs from imperative

---

### 2. `functional_style_examples.cpp`
**Difficulty**: 🟢 Beginner

Practical examples of functional programming patterns:
- Pure functions
- Immutability
- Higher-order functions
- Function composition
- Currying

**Learn**: Core functional programming concepts

---

### 3. `lambda_composition.cpp`
**Difficulty**: 🟡 Intermediate

Advanced lambda usage:
- Lambda captures
- Generic lambdas
- Recursive lambdas
- Composing lambdas
- Lambda as return values

**Learn**: Advanced lambda techniques

---

### 4. `ranges_introduction.cpp`
**Difficulty**: 🟡 Intermediate

C++20 Ranges library:
- Range views
- Range adaptors
- Lazy evaluation
- Pipe operator
- Custom ranges

**Learn**: Modern C++20 ranges

## 🎓 Learning Path

**Step 1**: Read `imperative_vs_functional.cpp`
- Understand the differences
- Run both versions
- Compare readability

**Step 2**: Study `functional_style_examples.cpp`
- Try each example
- Modify and experiment
- Write your own functions

**Step 3**: Master `lambda_composition.cpp`
- Understand captures
- Practice composition
- Build complex lambdas

**Step 4**: Explore `ranges_introduction.cpp`
- Learn C++20 ranges
- Use pipe operator
- Create pipelines

## 💡 Common Patterns

### Map (Transform)
```cpp
// Convert all to uppercase
auto uppercase = names 
    | std::views::transform([](std::string s) { 
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s; 
    });
```

### Filter (Select)
```cpp
// Keep only positive numbers
auto positive = numbers 
    | std::views::filter([](int n) { return n > 0; });
```

### Reduce (Fold)
```cpp
// Sum all numbers
int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
```

### Pipeline (Compose)
```cpp
// Complex transformation
auto result = data
    | std::views::filter(isValid)
    | std::views::transform(normalize)
    | std::views::take(10);
```

## ⚠️ Common Pitfalls

### 1. **Overusing Functional Style**
Not everything needs to be functional. Use imperative for simple loops:
```cpp
// Overkill
std::for_each(vec.begin(), vec.end(), [](int x) { 
    std::cout << x << "\n"; 
});

// Better
for (int x : vec) {
    std::cout << x << "\n";
}
```

### 2. **Performance Concerns**
Be aware of copies:
```cpp
// Creates copies
auto result = vec 
    | std::views::transform([](std::string s) { /* ... */ });

// Better: use references
auto result = vec 
    | std::views::transform([](const std::string& s) { /* ... */ });
```

### 3. **Readability**
Don't sacrifice readability for functional style:
```cpp
// Hard to read
auto x = f(g(h(j(k(data)))));

// Better: pipeline
auto x = data | k() | j() | h() | g() | f();
```

## 🔧 Compilation Requirements

- **C++11**: Lambdas, auto
- **C++14**: Generic lambdas
- **C++17**: constexpr if
- **C++20**: Ranges (required for many examples)

Compile with:
```bash
g++ -std=c++20 -Wall -Wextra filename.cpp -o output
```

## 📚 Further Reading

1. **Books**:
   - "Functional Programming in C++" by Ivan Čukić
   - "Functional Programming Patterns in Scala and Clojure" (concepts apply)

2. **Online Resources**:
   - CppReference: Ranges library
   - think-cell blog posts on functional C++
   - Eric Niebler's Range-v3 library

3. **Conference Talks**:
   - "Functional Programming in C++" - Various CppCon talks
   - "C++20 Ranges in Practice"

## 🎯 think-cell Connection

think-cell heavily uses functional programming:

> "We prefer functional-style generic algorithms and ranges over imperative programming."

They:
- Use ranges extensively
- Prefer pure functions
- Value composition over complexity
- Built their own range library (beyond Boost.Range)
- Focus on zero-cost abstractions

**Their Philosophy**: Write code that reads like what it does, not how it does it.

## 🚀 Practice Exercises

Try these challenges:

1. **Convert imperative to functional**: Take a nested loop and convert it to ranges
2. **Build a pipeline**: Process text file (read → filter → transform → output)
3. **Compose functions**: Build complex transformations from simple ones
4. **Custom range**: Create your own range adaptor

---

**Next Steps**: After mastering functional programming, move to `TemplateMeta/` to see how to move computations to compile-time!

**Remember**: Functional programming is a tool, not a religion. Use it where it makes code clearer and more maintainable! 🎉