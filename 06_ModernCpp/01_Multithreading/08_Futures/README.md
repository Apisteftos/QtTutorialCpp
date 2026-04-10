# Futures — Async Results in C++23

Futures provide a way to get a **result from an asynchronous operation**.

---

## The three tools

| Tool | Use case |
|------|----------|
| `std::async` | Simplest — fire-and-forget async task |
| `std::promise` | Manual — set value from any thread |
| `std::packaged_task` | Wrap callable for queued execution |

---

## std::async (simplest)

```cpp
auto fut = std::async(std::launch::async, func, args...);
doOtherWork();
int result = fut.get();   // blocks if not ready
```

**Always specify `std::launch::async`** — default may choose deferred.

---

## launch policies

```cpp
std::launch::async    // new thread immediately
std::launch::deferred // runs in calling thread at .get()
```

---

## std::promise + std::future

```cpp
std::promise<int> p;
std::future<int>  f = p.get_future();

// In another thread:
p.set_value(42);

// In waiting thread:
int val = f.get();   // 42
```

### With exception
```cpp
p.set_exception(std::current_exception());
// ...
try { f.get(); } catch (const std::exception& e) { ... }
```

---

## std::packaged_task

```cpp
std::packaged_task<int(int,int)> task(multiply);
auto fut = task.get_future();
std::thread(std::move(task), 6, 7).detach();
int result = fut.get();   // 42
```

---

## Exception propagation

```cpp
auto fut = std::async(std::launch::async, []() -> int {
    throw std::runtime_error("error");
    return 0;
});
try { fut.get(); } catch (const std::exception& e) { ... }
```

---

## wait_for / wait_until

```cpp
auto status = fut.wait_for(100ms);
if      (status == std::future_status::ready)   { fut.get(); }
else if (status == std::future_status::timeout) { /* retry */ }
else if (status == std::future_status::deferred){ /* will run at get() */ }
```

---

## std::shared_future (multiple consumers)

```cpp
std::shared_future<int> sf = fut.share();

// Many threads can call sf.get() safely
std::thread t1([sf]() { std::cout << sf.get(); });
std::thread t2([sf]() { std::cout << sf.get(); });
```

---

## Parallel computation pattern

```cpp
std::vector<std::future<long long>> futures;
for (int i=0; i<chunks; ++i)
    futures.push_back(std::async(std::launch::async, processChunk, i));

long long total = 0;
for (auto& f : futures) total += f.get();
```

---

## Comparison

| | `std::async` | `std::thread` | `std::promise` |
|--|-------------|--------------|---------------|
| Returns value | ✅ | ❌ | ✅ |
| Exception propagation | ✅ auto | ❌ manual | ✅ manual |
| Simplicity | ✅ highest | Medium | Lowest |

---

## Important: future destructor blocks!

```cpp
{
    auto fut = std::async(std::launch::async, longTask);
}   // destructor blocks until longTask finishes!
```
