# C++23: Concurrency

Back to [main README](README.md)

| Feature | Header | Standard |
|---|---|---|
| `<stdatomic.h>` (C-compatible atomics, `_Atomic(T)` macro) | `<stdatomic.h>` | C++23 (new) |

## Notes

- Concurrency is the smallest area in C++23.
- `<stdatomic.h>` lets headers be shared between C and C++ code. In pure C++ code you normally keep using `std::atomic`.
- Already **C++20**, not C++23: `std::jthread`, `std::stop_token`, `std::latch`, `std::barrier`, `std::counting_semaphore`, `std::atomic_ref`, `std::atomic<std::shared_ptr<T>>`, `std::atomic::wait/notify`.
- **C++26**, not C++23: `std::execution` (senders/receivers), `std::hazard_pointer`, `std::rcu`, `fetch_max` / `fetch_min` on atomics.
- `std::generator` (C++23) is a coroutine feature, see [README_ranges.md](README_ranges.md).

## Example

```cpp
#include <stdatomic.h>

_Atomic(int) counter = 0;

void tick() {
    atomic_fetch_add(&counter, 1);
}
```
