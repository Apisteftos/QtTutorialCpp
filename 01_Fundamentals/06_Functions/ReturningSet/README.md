# Returning Set Values — C++

Demonstrates using `std::set<std::tuple<int,int>>` as a global data structure
to store and iterate over a set of 2D coordinate points — specifically a
**3×3 blurring kernel** used in image processing.

---

## Example from this file

```cpp
std::set<std::tuple<int,int>> CoorPoints = {
    {-1,-1}, {0,-1}, {1,-1},
    {-1, 0}, {0, 0}, {1, 0},
    {-1, 1}, {0, 1}, {1, 1}
};

for (const auto& point : CoorPoints) {
    std::cout << "(" << std::get<0>(point)
              << ", " << std::get<1>(point) << ")\n";
}
```

---

## What is a 3×3 blurring kernel?

The 9 coordinate offsets represent the **relative positions** of a pixel
and its 8 neighbours in a 2D image:

```
(-1,-1) (0,-1) (1,-1)
(-1, 0) (0, 0) (1, 0)   ← (0,0) = center pixel
(-1, 1) (0, 1) (1, 1)
```

Used in convolution operations — apply these offsets to every pixel
to sample its neighbourhood for blurring, sharpening, edge detection etc.

---

## Why std::set here?

`std::set` keeps elements **sorted** and **unique**.
For `std::tuple`, the default sort is lexicographic — tuples compared
element by element, so `(-1,-1)` comes before `(0,-1)` etc.

```cpp
std::set<std::tuple<int,int>> points = { {0,0}, {1,1}, {-1,-1} };
// Stored in sorted order: (-1,-1), (0,0), (1,1)
```

---

## Accessing tuple elements

```cpp
std::tuple<int,int> p = {3, 7};

std::get<0>(p)   // 3 — first element (x)
std::get<1>(p)   // 7 — second element (y)

// C++17 structured binding — cleaner
auto [x, y] = p;
std::cout << x << " " << y;
```

---

## Returning a set from a function

```cpp
std::set<std::tuple<int,int>> getKernel3x3() {
    return {
        {-1,-1}, {0,-1}, {1,-1},
        {-1, 0}, {0, 0}, {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };
}

auto kernel = getKernel3x3();
for (const auto& [x, y] : kernel)   // C++17 structured binding
    std::cout << "(" << x << "," << y << ")\n";
```

---

## Related patterns

```cpp
// Return vector of pairs — when order matters
std::vector<std::pair<int,int>> getPoints();

// Return set of tuples — when uniqueness and sorting matter
std::set<std::tuple<int,int>> getKernel();

// Return struct — when fields need names
struct Point { int x, y; };
std::vector<Point> getPoints();
```
