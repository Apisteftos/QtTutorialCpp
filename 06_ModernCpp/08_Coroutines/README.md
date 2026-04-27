# C++20 Coroutines

## What are Coroutines?

A coroutine is a function that can **suspend** its execution and **resume** later,
preserving its state across suspensions.

---

## Three keywords

```mermaid
flowchart LR
    subgraph Keywords
        ret["co_return\nReturn a value\nand finish"]
        yield["co_yield\nSuspend and produce\na value (generator)"]
        await["co_await\nSuspend until\nawaitable completes"]
    end
```

---

## Coroutine vs regular function

```mermaid
sequenceDiagram
    participant Caller
    participant RegFunc as Regular Function
    participant Coro as Coroutine

    Caller->>RegFunc: call()
    RegFunc-->>Caller: return (done)

    Caller->>Coro: call() — creates coroutine
    Coro-->>Caller: suspend (co_yield value1)
    Caller->>Coro: resume()
    Coro-->>Caller: suspend (co_yield value2)
    Caller->>Coro: resume()
    Coro-->>Caller: co_return (done)
```

---

## Generator pattern — co_yield

```mermaid
flowchart TD
    subgraph Generator ["Generator&lt;int&gt; fibonacci()"]
        S1["a=0, b=1"]
        S2["co_yield a → suspend\nyield 0"]
        S3["resume\nnext = a+b, a=b, b=next"]
        S4["co_yield a → suspend\nyield 1"]
        S5["...infinite..."]
        S1 --> S2 --> S3 --> S4 --> S5
    end

    subgraph Caller
        C1["for (int n : fibonacci())"]
        C2["use n=0"]
        C3["++it → resume"]
        C4["use n=1"]
        C5["..."]
        C1 --> C2 --> C3 --> C4 --> C5
    end
```

```cpp
Generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;          // suspend here, produce a
        int next = a + b;
        a = b;
        b = next;
    }
}

// Usage — take first 10
int count = 0;
for (int n : fibonacci()) {
    std::cout << n << ' ';
    if (++count >= 10) break;
}
// 0 1 1 2 3 5 8 13 21 34
```

---

## Generator vs vector

```mermaid
flowchart LR
    subgraph VecWay ["std::vector approach"]
        V1["precompute ALL values"]
        V2["store in memory"]
        V3["O(n) memory"]
        V1 --> V2 --> V3
    end

    subgraph GenWay ["Generator approach"]
        G1["compute ON DEMAND"]
        G2["no storage needed"]
        G3["O(1) memory"]
        G1 --> G2 --> G3
    end
```

---

## co_await — async tasks

```cpp
// co_await suspends until the awaitable is ready
Task<std::string> fetchData(const std::string& url) {
    auto response = co_await httpGet(url);   // suspends here
    co_return response.body;                  // resumes here
}

// Reads like synchronous code — no callbacks!
Task<void> processAll() {
    auto data1 = co_await fetchData("url1");
    auto data2 = co_await fetchData("url2");
    std::cout << data1 << data2;
    co_return;
}
```

---

## Coroutine infrastructure (required boilerplate)

```mermaid
classDiagram
    class Generator~T~ {
        +promise_type
        +iterator begin()
        +iterator end()
        +handle
    }
    class promise_type {
        +T current_value
        +get_return_object() Generator
        +initial_suspend() suspend_always
        +final_suspend() suspend_always
        +yield_value(T) suspend_always
        +return_void()
    }
    Generator~T~ --> promise_type : contains
```

> In C++23 `std::generator<T>` is standardized — no boilerplate needed.
> On GCC 14: use the custom `Generator<T>` from this file or install `cppcoro`.

---

## Coroutine vs Thread vs Callback

| | Callback | Thread | Coroutine |
|--|:--------:|:------:|:---------:|
| Memory | Low | High (MB stack) | Low (KB stack) |
| OS involvement | No | Yes | No |
| Code style | Fragmented | Sequential | Sequential |
| Switching | Function call | OS scheduler | Explicit suspend |
| Parallelism | No | Yes | No (cooperative) |

---

## Practical use cases

```mermaid
flowchart TD
    coro["Coroutines"]
    coro --> gen["Generator — lazy sequences\nfibonacci, ID generators\ninfinite ranges"]
    coro --> async["Async I/O — co_await\nNetwork, file, timers\nwith cppcoro/Boost.Asio"]
    coro --> state["State machines\nsuspend at each state\nresume on event"]
    coro --> pipeline["Data pipelines\nproducer-consumer\nwithout threads"]
```

---

## Async frameworks for co_await

```bash
# cppcoro — C++ coroutine library
# https://github.com/lewissbaker/cppcoro

# Boost.Asio — network + coroutines
sudo apt install libboost-all-dev

# QCoro — Qt coroutines
# https://github.com/danvratil/qcoro
```

---

## When to use Coroutines

✅ Lazy infinite sequences — `co_yield` generators
✅ Async I/O without callbacks — `co_await`
✅ Complex state machines — suspend at each state
✅ Producer-consumer pipelines without threads

❌ Simple synchronous code — unnecessary complexity
❌ CPU-bound parallel work — use threads instead
❌ When framework support is needed but not available
