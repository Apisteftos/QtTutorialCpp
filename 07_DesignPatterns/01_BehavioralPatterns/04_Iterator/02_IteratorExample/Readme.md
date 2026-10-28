# Iterator — `02_IteratorExample`

> **Intent:** Gives sequential access to the elements of a collection without exposing its internal representation.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: BOOK COLLECTION ITERATOR
- EXAMPLE 2: PLAYLIST ITERATOR
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Book` | — |
| `Iterator` | — |
| `Aggregate` | — |
| `BookCollection` | `Aggregate<Book>` |
| `BookIterator` | `Iterator<Book>` |
| `ReverseBookIterator` | `Iterator<Book>` |
| `FilteredBookIterator` | `Iterator<Book>` |
| `Song` | — |
| `Playlist` | — |
| `SequentialIterator` | `Iterator<Song>` |
| `ShuffleIterator` | `Iterator<Song>` |

## The pattern in short

- A collection that can create an iterator.
- An iterator type with the usual operations (dereference, advance, compare).

**Use it when:** You want to traverse a custom container with range-for or the standard algorithms.

**Watch out:** To work with the standard algorithms an iterator must provide the iterator_traits typedefs (or satisfy the C++20 iterator concepts).

## Sample output (first lines)

```text
========================================
ITERATOR PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: BOOK COLLECTION        ║
╚════════════════════════════════════╝

📚 Forward Iteration:
---------------------
📖 "1984" by George Orwell (1949)
📖 "To Kill a Mockingbird" by Harper Lee (1960)
📖 "The Great Gatsby" by F. Scott Fitzgerald (1925)
📖 "Pride and Prejudice" by Jane Austen (1813)
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_IteratorExample
./02_IteratorExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 164 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored).

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### BENEFITS OF ITERATOR PATTERN

- ✅ Single Responsibility: Iteration logic separated from collection
- ✅ Open/Closed: Add new iterators without changing collection
- ✅ Uniform Access: Same interface for different collections
- ✅ Multiple Iterators: Can iterate same collection multiple ways
- ✅ Encapsulation: Collection internals remain hidden
- ✅ Flexibility: Easy to add new traversal algorithms

### WHEN TO USE ITERATOR

1. Access collection elements without exposing internal structure
2. Support multiple simultaneous traversals of collection
3. Provide uniform interface for different collection types
4. Need different ways to traverse same collection
5. Simplify collection interface by moving traversal to iterator

### ITERATOR TYPES

**Forward Iterator:**

- Traverses from beginning to end
- Most common type
- Example: BookIterator

**Reverse Iterator:**

- Traverses from end to beginning
- Useful for backward navigation
- Example: ReverseBookIterator

**Filtered Iterator:**

- Only returns elements matching criteria
- Combines iteration with filtering
- Example: FilteredBookIterator (year >= 1940)

**Random/Shuffle Iterator:**

- Traverses in random order
- Useful for playlists, games
- Example: ShuffleIterator

**Bidirectional Iterator:**

- Can move both forward and backward
- Supports next() and previous()
- Used in doubly-linked lists

### INTERNAL vs EXTERNAL ITERATOR

**External Iterator (Used in examples above):**

- Client controls iteration
- More flexible and powerful
- Can have multiple active iterators
- Example: while(it.hasNext()) { it.next(); }

**Internal Iterator:**

- Collection controls iteration
- Client passes operation to execute
- Simpler but less flexible
- Example: collection.forEach(lambda)

### REAL-WORLD EXAMPLES

- C++ STL iterators (vector, list, map)
- Java Iterator and Iterable interfaces
- Python __iter__ and __next__ methods
- Database result sets (JDBC ResultSet)
- File system directory traversal
- Tree/Graph traversal algorithms (DFS, BFS)
- Menu navigation systems
- Playlist management (shuffle, repeat)
- Pagination in web applications
- Game inventory systems

### ITERATOR vs FOR-EACH LOOP

**Iterator Pattern:**

- Explicit iteration control
- Can pause/resume iteration
- Multiple iterators on same collection
- Works with any traversal algorithm

**For-Each Loop:**

- Syntactic sugar over iterators
- Simpler syntax
- Limited control (can't pause)
- Built-in language feature

### KEY COMPONENTS

```cpp
1. Iterator Interface: hasNext(), next(), current(), reset()
```

2. Concrete Iterator: Implements traversal algorithm

```cpp
3. Aggregate Interface: createIterator()
```

4. Concrete Aggregate: Returns concrete iterator

### ADVANTAGES

- Separates traversal from collection structure
- Supports multiple simultaneous iterations
- Uniform interface for different collections
- Easy to add new traversal methods
- Encapsulates internal collection structure
- Simplifies collection interface

### DISADVANTAGES (closing summary)

- Can be overkill for simple collections
- Slightly more complex than direct access
- May have performance overhead
- Need to maintain iterator state
