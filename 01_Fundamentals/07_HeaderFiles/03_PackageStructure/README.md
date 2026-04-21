# Package Structure — Organizing Headers into Directories

## What is this?

Organizing your header files into **subdirectories** creates a logical
package structure. The include path becomes part of the API — it tells
users where a symbol comes from and groups related headers together.

---

## Project structure

```mermaid
flowchart TD
    root["03_PackageStructure/"]

    root --> main["main.cpp\n#include 'Package/API/package.h'"]
    root --> cmake["CMakeLists.txt"]
    root --> pkg["Package/"]

    pkg --> api["API/\n← PUBLIC headers\nusers include these"]
    pkg --> internal["Internal/\n← PRIVATE headers\nusers never include these"]

    api --> pkgh["package.h\nPackageClass\nPackageUtils"]
    internal --> detail["detail.h\nformatValue()\nMAX_ITEMS"]

    style api fill:#267326,color:#fff
    style internal fill:#8B0000,color:#fff
```

---

## Include path = package location

```cpp
// The path tells you everything:
#include "Package/API/package.h"
//        ↑        ↑   ↑
//        package  API public header
```

```mermaid
flowchart LR
    inc["#include 'Package/API/package.h'"]
    inc --> pkg["Package/\n→ package name\n→ namespace Package"]
    inc --> api["API/\n→ public boundary\n→ safe to include"]
    inc --> header["package.h\n→ the header file"]
```

---

## Namespace mirrors directory

```mermaid
flowchart LR
    subgraph Directory
        D["Package/API/package.h"]
    end
    subgraph Namespace
        N["namespace Package { ... }"]
    end
    subgraph Usage
        U["Package::PackageClass\nPackage::PackageUtils"]
    end

    Directory --> Namespace --> Usage
```

Convention: **directory name = namespace name**. This makes the code
self-documenting — you always know where a symbol comes from.

---

## Public API vs Internal headers

```mermaid
flowchart TD
    user["External user\n(main.cpp)"]

    subgraph Public ["✅ Public — users include these"]
        api["Package/API/package.h\nPackageClass\nPackageUtils"]
    end

    subgraph Private ["❌ Private — users never include these"]
        internal["Package/Internal/detail.h\nformatValue()\nMAX_ITEMS\nimplementation helpers"]
    end

    user --> Public
    user -. "should NOT include" .-> Private
```

---

## Key code

```cpp
// package.h — organized in a directory
namespace Package {

    class PackageClass {
    public:
        int         first;
        std::string second;

        PackageClass(int one, const std::string& two)
            : first(one), second(two) {}

        void display() const {
            std::cout << first << " | " << second << '\n';
        }
    };

    class PackageUtils {
    public:
        static int  add(int a, int b) { return a + b; }
        static std::string version()  { return "1.0.0"; }
    };
}
```

```cpp
// main.cpp
#include "Package/API/package.h"   // include via path

Package::PackageClass pk1(23, "amazing");
Package::PackageClass pk2(13, "awaiting");
pk1.display();
pk2.display();
```

---

## CMake — include path setup

```cmake
target_include_directories(03_PackageStructure PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}   # project root = include search root
)
```

With `main.cpp` in the root and `Package/API/package.h` in a subdirectory,
setting the root as the include path lets `#include "Package/API/package.h"`
resolve correctly.

---

## How real libraries do this

```mermaid
flowchart TD
    subgraph Qt ["Qt Framework"]
        Q1["#include &lt;QThread&gt;"]
        Q2["#include &lt;QtCore/QString&gt;"]
        Q3["Usage: QThread, QObject\n(Q prefix instead of namespace)"]
    end

    subgraph Boost ["Boost Libraries"]
        B1["#include &lt;boost/filesystem.hpp&gt;"]
        B2["#include &lt;boost/algorithm/string.hpp&gt;"]
        B3["Usage: boost::filesystem::path\nboost::algorithm::to_upper"]
    end

    subgraph STL ["C++ Standard Library"]
        S1["#include &lt;vector&gt;"]
        S2["#include &lt;algorithm&gt;"]
        S3["Usage: std::vector\nstd::sort"]
    end
```

---

## Old style vs package structure

```cpp
// OLD — flat directory, no organization
#include "PackageClass.h"      // where does this come from?
#include "Utils.h"             // which Utils? name collision risk!
PackageClass obj;              // no namespace — global pollution

// MODERN — organized structure
#include "Package/API/package.h"   // clear origin
Package::PackageClass obj;         // namespaced — no collision
```

---

## When to use this pattern

✅ Project has multiple logical components
✅ Distributing a library — clear public API boundary
✅ Preventing name collisions between components
✅ Following real-world library conventions (Qt, Boost)

❌ Single-file toy programs — overkill
❌ Very small projects with 2–3 headers total
