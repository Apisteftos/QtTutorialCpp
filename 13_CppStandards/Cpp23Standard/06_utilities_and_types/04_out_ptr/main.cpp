/*
 * std::out_ptr, std::inout_ptr (C++23, <memory>)
 *
 * Adapters that let a smart pointer be used with C style APIs that return
 * a pointer through a `T**` parameter.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <cstdlib>
#include <memory>
#include <print>

// --- a small C style API (simulated) ---------------------------------------
struct Handle {
    int id;
};

int createHandle(Handle** out) {          // allocates, returns the handle through `out`
    *out = new Handle{7};
    return 0;
}

int replaceHandle(Handle** inout) {       // frees the old one, gives a new one
    int oldId = (*inout)->id;
    delete *inout;
    *inout = new Handle{oldId + 1};
    return 0;
}

void destroyHandle(Handle* handle) {
    std::println("destroyHandle({})", handle->id);
    delete handle;
}

struct HandleDeleter {
    void operator()(Handle* handle) const { destroyHandle(handle); }
};

int main() {
    // === EXAMPLE 1: out_ptr ===
    std::println("=== Example 1: out_ptr ===");
    std::unique_ptr<Handle, HandleDeleter> handle;
    createHandle(std::out_ptr(handle));    // the API writes into the smart pointer
    std::println("created handle {}", handle->id);

    // === EXAMPLE 2: inout_ptr ===
    std::println("");
    std::println("=== Example 2: inout_ptr ===");
    // The API takes over the old pointer and returns a new one.
    // inout_ptr releases the old one first, so no double delete happens.
    replaceHandle(std::inout_ptr(handle));
    std::println("handle is now {}", handle->id);

    // === EXAMPLE 3: out_ptr with a shared_ptr needs a deleter ===
    std::println("");
    std::println("=== Example 3: shared_ptr ===");
    std::shared_ptr<Handle> shared;
    createHandle(std::out_ptr(shared, HandleDeleter{}));
    std::println("shared handle {}", shared->id);
    std::println("leaving main");
    return 0;
}
