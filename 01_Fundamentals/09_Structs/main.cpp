#include <iostream>
#include <string>
#include <print>
#include <compare>   // std::partial_ordering / <=>
#include <array>
#include <cmath>
#include <type_traits>

// ============================================================
// STRUCTS in C++23
// ============================================================
// Topics covered:
//   1.  Basic struct declaration and aggregate initialization
//   2.  Designated initializers (C++20)
//   3.  struct vs class — the only real difference
//   4.  Default member initializers
//   5.  Constructors inside a struct
//   6.  Member functions (methods) inside a struct
//   7.  Nested structs
//   8.  Structured bindings on a struct (C++17)
//   9.  Struct inheritance
//  10.  operator<=> — the spaceship operator (C++20)
//  11.  POD / trivial vs non-trivial structs
//  12.  sizeof, alignment, and padding
//  13.  static members in a struct
//  14.  Passing structs: by value vs by reference
//  15.  Arrays of structs
// ============================================================

// ------------------------------------------------------------
// 1. Basic struct declaration and aggregate initialization
// ------------------------------------------------------------
struct Point2D {
    int x;
    int y;
};

void example1_basicStruct() {
    std::println("--- 1. Basic struct + aggregate init ---");

    Point2D p1{10, 20};        // brace-init, order matters
    Point2D p2 = {1, 2};       // also valid (copy-list-init)
    Point2D p3;                // members are UNINITIALIZED (no default member init)

    p3 = {0, 0};                // assign afterward if needed

    std::println("p1 = ({}, {})", p1.x, p1.y);
    std::println("p2 = ({}, {})", p2.x, p2.y);
    std::println("p3 = ({}, {})", p3.x, p3.y);
}

// ------------------------------------------------------------
// 2. Designated initializers (C++20)
// ------------------------------------------------------------
struct Config {
    int    width;
    int    height;
    bool   fullscreen;
    double volume;
};

void example2_designatedInitializers() {
    std::println("--- 2. Designated initializers (C++20) ---");

    // Members must be initialized IN DECLARATION ORDER,
    // but you can skip some (skipped ones get default member init,
    // or are zero/value-initialized if no default is given).
    Config cfg{.width = 1920, .height = 1080, .fullscreen = true, .volume = 0.8};

    std::println("Config: {}x{}, fullscreen={}, volume={}",
                 cfg.width, cfg.height, cfg.fullscreen, cfg.volume);
}

// ------------------------------------------------------------
// 3. struct vs class — the only real difference
// ------------------------------------------------------------
// struct members and base classes default to PUBLIC.
// class members and base classes default to PRIVATE.
// Everything else (methods, constructors, inheritance, templates,
// virtual functions...) works identically in both.

struct StructVersion {
    int value = 5;   // public by default
};

class ClassVersion {
    int value = 5;   // private by default
public:
    int getValue() const { return value; }
};

void example3_structVsClass() {
    std::println("--- 3. struct vs class (default access) ---");

    StructVersion s;
    std::println("StructVersion.value (public by default) = {}", s.value);

    ClassVersion c;
    std::println("ClassVersion.getValue() (value is private) = {}", c.getValue());
}

// ------------------------------------------------------------
// 4. Default member initializers
// ------------------------------------------------------------
struct Player {
    std::string name = "Unnamed";
    int         health = 100;
    int         level = 1;
};

void example4_defaultMemberInitializers() {
    std::println("--- 4. Default member initializers ---");

    Player p1;                       // uses all defaults
    Player p2{"Kostas", 80};         // overrides name and health, level stays 1

    std::println("p1: {} hp={} lvl={}", p1.name, p1.health, p1.level);
    std::println("p2: {} hp={} lvl={}", p2.name, p2.health, p2.level);
}

// ------------------------------------------------------------
// 5. Constructors inside a struct
// ------------------------------------------------------------
// Adding a user-defined constructor makes the type NON-aggregate,
// so brace-init now calls the constructor instead of aggregate init.
struct Rectangle {
    double width;
    double height;

    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const { return width * height; }
};

void example5_constructors() {
    std::println("--- 5. Constructors inside a struct ---");

    Rectangle r{3.0, 4.0};   // calls the constructor
    std::println("Rectangle {}x{} area = {}", r.width, r.height, r.area());
}

// ------------------------------------------------------------
// 6. Member functions (methods) inside a struct
// ------------------------------------------------------------
struct Vector2D {
    double x = 0.0;
    double y = 0.0;

    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    double length() const {
        return std::sqrt(x * x + y * y);
    }
};

void example6_memberFunctions() {
    std::println("--- 6. Member functions inside a struct ---");

    Vector2D a{3.0, 4.0};
    Vector2D b{1.0, 2.0};
    Vector2D sum = a + b;

    std::println("a + b = ({}, {})", sum.x, sum.y);
    std::println("length(a) = {}", a.length());
}

// ------------------------------------------------------------
// 7. Nested structs
// ------------------------------------------------------------
struct Address {
    struct GeoCoordinates {
        double latitude;
        double longitude;
    };

    std::string     city;
    GeoCoordinates  coords;
};

void example7_nestedStructs() {
    std::println("--- 7. Nested structs ---");

    Address addr{"Hannover", {52.3759, 9.7320}};
    std::println("{} is at ({}, {})", addr.city, addr.coords.latitude, addr.coords.longitude);

    // Accessing the nested type by its qualified name:
    Address::GeoCoordinates origin{0.0, 0.0};
    std::println("Origin: ({}, {})", origin.latitude, origin.longitude);
}

// ------------------------------------------------------------
// 8. Structured bindings on a struct (C++17)
// ------------------------------------------------------------
struct Point3D {
    double x, y, z;
};

Point3D midpoint(const Point3D& a, const Point3D& b) {
    return {(a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2};
}

void example8_structuredBindings() {
    std::println("--- 8. Structured bindings (C++17) ---");

    auto [mx, my, mz] = midpoint({0, 0, 0}, {4, 8, 12});
    std::println("midpoint = ({}, {}, {})", mx, my, mz);
}

// ------------------------------------------------------------
// 9. Struct inheritance
// ------------------------------------------------------------
struct Shape {
    std::string name;

    explicit Shape(std::string n) : name(std::move(n)) {}
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

struct Circle : Shape {
    double radius;

    Circle(double r) : Shape("Circle"), radius(r) {}
    double area() const override { return 3.14159265 * radius * radius; }
};

void example9_structInheritance() {
    std::println("--- 9. Struct inheritance ---");

    Circle c{5.0};
    std::println("{} area = {}", c.name, c.area());

    // struct inheritance is public by default (unlike class, which defaults private)
}

// ------------------------------------------------------------
// 10. operator<=> — the spaceship operator (C++20)
// ------------------------------------------------------------
struct Version {
    int major;
    int minor;
    int patch;

    // Compiler auto-generates ==, !=, <, <=, >, >= from this ONE operator
    auto operator<=>(const Version&) const = default;
};

void example10_spaceshipOperator() {
    std::println("--- 10. operator<=> (C++20) ---");

    Version v1{1, 2, 0};
    Version v2{1, 3, 0};

    std::println("v1 < v2  : {}", v1 < v2);
    std::println("v1 == v2 : {}", v1 == v2);
}

// ------------------------------------------------------------
// 11. POD / trivial vs non-trivial structs
// ------------------------------------------------------------
struct TrivialPoint {   // POD: no constructors, no virtuals, no private members
    int x;
    int y;
};

struct NonTrivialPoint {
    int x;
    int y;
    NonTrivialPoint(int a, int b) : x(a), y(b) {}   // user-defined ctor -> non-trivial
};

void example11_podVsNonTrivial() {
    std::println("--- 11. POD / trivial vs non-trivial ---");

    std::println("TrivialPoint    is_trivial   = {}", std::is_trivial_v<TrivialPoint>);
    std::println("TrivialPoint    is_standard_layout = {}", std::is_standard_layout_v<TrivialPoint>);
    std::println("NonTrivialPoint is_trivial   = {}", std::is_trivial_v<NonTrivialPoint>);

    // POD types can be memcpy'd safely and have predictable memory layout,
    // which matters for things like shared memory, network protocols, and MCX packet framing.
}

// ------------------------------------------------------------
// 12. sizeof, alignment, and padding
// ------------------------------------------------------------
struct Unpacked {
    char  a;   // 1 byte
    int   b;   // 4 bytes  (compiler inserts 3 bytes padding before this)
    char  c;   // 1 byte   (compiler inserts 3 bytes padding after this)
};

struct Reordered {
    int   b;   // 4 bytes
    char  a;   // 1 byte
    char  c;   // 1 byte  (2 bytes padding at the end to satisfy int alignment)
};

#pragma pack(push, 1)
struct Packed {
    char a;
    int  b;
    char c;
};
#pragma pack(pop)

void example12_sizeofAndPadding() {
    std::println("--- 12. sizeof, alignment, padding ---");

    std::println("sizeof(Unpacked)   = {} (padding hurts here)", sizeof(Unpacked));
    std::println("sizeof(Reordered)  = {} (reordering members helps)", sizeof(Reordered));
    std::println("sizeof(Packed)     = {} (#pragma pack forces tight layout, may hurt perf)", sizeof(Packed));

    // Rule of thumb: order struct members largest -> smallest to minimize padding.
    // Relevant for wire-format structs (protocol headers, FFmpeg-style packet structs, etc.)
}

// ------------------------------------------------------------
// 13. static members in a struct
// ------------------------------------------------------------
struct Counter {
    static inline int instanceCount = 0;   // shared across ALL instances (C++17 inline)

    Counter() { ++instanceCount; }
};

void example13_staticMembers() {
    std::println("--- 13. static members ---");

    Counter c1, c2, c3;
    std::println("Counter::instanceCount = {}", Counter::instanceCount);
}

// ------------------------------------------------------------
// 14. Passing structs: by value vs by reference
// ------------------------------------------------------------
struct BigData {
    std::array<double, 1000> values{};
};

void byValue(BigData data) {          // COPIES the entire 1000-double array
    data.values[0] = 999;             // modifies only the local copy
}

void byReference(BigData& data) {     // no copy, modifies the caller's object
    data.values[0] = 999;
}

void byConstReference(const BigData& data) {  // no copy, read-only, safest for large structs
    std::println("first value = {}", data.values[0]);
}

void example14_passingStructs() {
    std::println("--- 14. Passing structs by value vs reference ---");

    BigData bd;
    bd.values[0] = 1;

    byValue(bd);
    std::println("after byValue:          bd.values[0] = {} (unchanged)", bd.values[0]);

    byReference(bd);
    std::println("after byReference:      bd.values[0] = {} (changed)", bd.values[0]);

    byConstReference(bd);
}

// ------------------------------------------------------------
// 15. Arrays of structs
// ------------------------------------------------------------
struct Employee {
    std::string name;
    int         yearsOfExperience;
};

void example15_arraysOfStructs() {
    std::println("--- 15. Arrays of structs ---");

    std::array<Employee, 3> team{{
        {"Alan",     17},
        {"Kostas",    12},
        {"Ulrich",   30}
    }};

    for (const auto& [name, years] : team) {
        std::println("{} — {} years of experience", name, years);
    }
}

// ============================================================
int main() {
    example1_basicStruct();
    example2_designatedInitializers();
    example3_structVsClass();
    example4_defaultMemberInitializers();
    example5_constructors();
    example6_memberFunctions();
    example7_nestedStructs();
    example8_structuredBindings();
    example9_structInheritance();
    example10_spaceshipOperator();
    example11_podVsNonTrivial();
    example12_sizeofAndPadding();
    example13_staticMembers();
    example14_passingStructs();
    example15_arraysOfStructs();

    return 0;
}
