# Callbacks — C++23

A **callback** is a function passed to another function to be called later.
It decouples the *what to do* from the *when to do it*.

---

## Three ways to pass a callback

```cpp
// 1. Function pointer — C style, fast, no state
void process(int x, void (*callback)(int));

// 2. std::function — flexible, accepts anything callable, small overhead
void process(int x, std::function<void(int)> callback);

// 3. Template — fastest, inlined by compiler, no overhead
template<typename Callback>
void process(int x, Callback callback);
```

---

## Function pointer callback

```cpp
void printDouble(int x) { std::cout << x * 2; }
void printSquare(int x) { std::cout << x * x; }

void forEach(int arr[], int n, void (*cb)(int)) {
    for (int i = 0; i < n; ++i) cb(arr[i]);
}

int nums[] = {1, 2, 3};
forEach(nums, 3, printDouble);   // pass function by name
forEach(nums, 3, printSquare);

// Store in variable
void (*fn)(int) = printDouble;
forEach(nums, 3, fn);
```

**Limitation:** cannot capture state — no closure.

---

## std::function — accepts anything callable

```cpp
#include <functional>

void process(const std::vector<int>& v, std::function<void(int)> cb) {
    for (int x : v) cb(x);
}

// Pass free function
process(v, printDouble);

// Pass lambda
process(v, [](int x) { std::cout << x * 3; });

// Pass lambda with capture
int factor = 7;
process(v, [factor](int x) { std::cout << x * factor; });

// Store and reuse
std::function<void(int)> fn = [](int x) { std::cout << x; };
process(v, fn);
```

---

## Lambda captures

```cpp
int sum = 0;

// Capture by reference — modifies outer variable
forEach(v, [&sum](int x) { sum += x; });

// Capture by value — owns a copy
int threshold = 3;
forEach(v, [threshold](int x) {
    if (x > threshold) std::cout << x;
});

// Mutable lambda — modify captured copy
int count = 0;
auto fn = [count](int) mutable { ++count; };  // outer count unchanged

// Capture all by reference
forEach(v, [&](int x) { sum += x; });

// Capture all by value
forEach(v, [=](int x) { std::cout << x * factor; });
```

---

## Callback with return value

```cpp
int reduce(const std::vector<int>& v, int init,
           std::function<int(int,int)> combiner) {
    int result = init;
    for (int x : v) result = combiner(result, x);
    return result;
}

int sum  = reduce(v, 0, [](int acc, int x){ return acc + x; });
int prod = reduce(v, 1, [](int acc, int x){ return acc * x; });
int max  = reduce(v, INT_MIN, [](int a, int b){ return std::max(a,b); });
```

---

## Member function as callback

```cpp
class Logger {
public:
    void log(const std::string& msg) { std::cout << msg; }
};

Logger logger;

// Wrap in lambda (preferred)
std::function<void(const std::string&)> cb =
    [&logger](const std::string& msg) { logger.log(msg); };

// Using std::bind (older style)
auto bound = std::bind(&Logger::log, &logger, std::placeholders::_1);
```

---

## Stored callback — deferred execution

```cpp
class Button {
    std::function<void()> m_onClick;
public:
    void setOnClick(std::function<void()> cb) { m_onClick = cb; }

    void click() {
        if (m_onClick) m_onClick();   // call if registered
    }
};

Button btn;
btn.setOnClick([]() { std::cout << "clicked!"; });
btn.click();   // fires later
```

---

## Template callback — zero overhead

```cpp
template<typename Callback>
void forEach(const std::vector<int>& v, Callback fn) {
    for (int x : v) fn(x);   // inlined by compiler — no virtual call
}

forEach(v, [](int x) { std::cout << x; });
```

Use template when: performance-critical, tight loops, no runtime dispatch needed.
Use `std::function` when: storing, returning, or choosing callback at runtime.

---

## Callback chain / pipeline

```cpp
std::vector<std::function<int(int)>> stages;
stages.push_back([](int x) { return x * 2; });
stages.push_back([](int x) { return x + 10; });
stages.push_back([](int x) { return x * x; });

int val = 3;
for (auto& stage : stages) val = stage(val);
```

---

## Success/Error callback pair

```cpp
void fetchData(const std::string& url,
               std::function<void(const std::string&)> onSuccess,
               std::function<void(const std::string&)> onError)
{
    if (ok) onSuccess(data);
    else    onError(reason);
}

fetchData(url,
    [](const std::string& data)  { std::cout << "OK: " << data; },
    [](const std::string& error) { std::cout << "ERR: " << error; }
);
```

---

## MCX event system pattern

```cpp
class McxClient {
    std::function<void(int, const std::string&)> m_onCallStarted;
    std::function<void(int, int)>                m_onFloorGranted;
    std::function<void(int, const std::string&)> m_onError;
public:
    void onCallStarted (auto cb) { m_onCallStarted  = cb; }
    void onFloorGranted(auto cb) { m_onFloorGranted = cb; }
    void onError       (auto cb) { m_onError        = cb; }
};

McxClient client;
client.onCallStarted([](int id, const std::string& group) {
    std::println("call {} started in group {}", id, group);
});
client.onFloorGranted([](int callId, int userId) {
    std::println("floor granted to user {} in call {}", userId, callId);
});
```

---

## Function pointer vs std::function vs template

| | Function ptr | `std::function` | Template |
|--|-------------|----------------|---------|
| Accepts lambda | ❌ (no capture) | ✅ | ✅ |
| Accepts member fn | ❌ | ✅ (with bind/lambda) | ✅ |
| Storable | ✅ | ✅ | ❌ (compile-time only) |
| Runtime dispatch | ✅ | ✅ | ❌ |
| Performance | Fast | Small overhead | Fastest (inlined) |
| State/capture | ❌ | ✅ | ✅ |

---

## When to use each

| Scenario | Use |
|----------|-----|
| Simple C-style callback, no state | Function pointer |
| Store callback as member variable | `std::function` |
| Return callback from function | `std::function` |
| Choose callback at runtime | `std::function` |
| Performance-critical inner loop | Template |
| Qt signal/slot equivalent | `std::function` or signals |
| MCX event handlers | `std::function` (stored callbacks) |
