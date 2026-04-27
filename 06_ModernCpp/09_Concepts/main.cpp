#include <iostream>
#include <concepts>
#include <string>
#include <vector>
#include <list>
#include <ranges>
#include <print>
#include <type_traits>

// ============================================================
// C++20 CONCEPTS
// ============================================================
// Topics covered:
//   1.  What are concepts — type constraints
//   2.  Built-in concepts — std::integral, std::floating_point etc.
//   3.  requires clause — ad-hoc constraints
//   4.  Custom concepts — define your own
//   5.  Concept with multiple constraints — &&, ||
//   6.  requires expression — checking expressions
//   7.  Constrained auto
//   8.  Concepts vs SFINAE — comparison
//   9.  Subsumption — concept refinement
//  10.  Real use case — MCX codec concept
// ============================================================


// ============================================================
// Example 3 — Custom concepts
// ============================================================

// Concept: type must support +, -, *, /
template<typename T>
concept Arithmetic = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { a / b } -> std::same_as<T>;
};

// Concept: type must be printable via <<
template<typename T>
concept Printable = requires(std::ostream& os, T val) {
    { os << val } -> std::same_as<std::ostream&>;
};

// Concept: type must have .size() returning something convertible to size_t
template<typename T>
concept Sizeable = requires(T c) {
    { c.size() } -> std::convertible_to<std::size_t>;
};

// Concept: Container — has begin, end, size, value_type
template<typename T>
concept Container = requires(T c) {
    typename T::value_type;
    { c.begin() } -> std::input_or_output_iterator;
    { c.end()   } -> std::input_or_output_iterator;
    { c.size()  } -> std::convertible_to<std::size_t>;
};

// Concept: Comparable — supports <, >, ==
template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b  } -> std::convertible_to<bool>;
    { a > b  } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};


// ============================================================
// Example 4 — Functions using concepts (4 syntaxes)
// ============================================================

// Syntax 1: requires clause after template
template<typename T>
    requires std::integral<T>
T doubled_v1(T x) { return x * 2; }

// Syntax 2: concept in template parameter
template<std::integral T>
T doubled_v2(T x) { return x * 2; }

// Syntax 3: constrained auto
auto doubled_v3(std::integral auto x) { return x * 2; }

// Syntax 4: requires clause at end
template<typename T>
T doubled_v4(T x) requires std::integral<T> { return x * 2; }


// ============================================================
// Example 5 — Multiple constraints
// ============================================================

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<typename T>
concept SignedNumber = Number<T> && std::is_signed_v<T>;

template<Number T>
T square(T x) { return x * x; }

template<SignedNumber T>
T absoluteVal(T x) { return x < 0 ? -x : x; }

template<Container T>
void printContainer(const T& c) {
    std::cout << "  [ ";
    for (const auto& item : c) std::cout << item << ' ';
    std::cout << "]\n";
}

template<typename T>
    requires Comparable<T> && Printable<T>
T findMax(const std::vector<T>& v) {
    T maxVal = v[0];
    for (const auto& item : v)
        if (item > maxVal) maxVal = item;
    return maxVal;
}


// ============================================================
// Example 8 — Concepts vs SFINAE
// ============================================================

// SFINAE — old way (ugly, hard to read)
template<typename T,
         typename = std::enable_if_t<std::is_integral_v<T>>>
T sfinae_double(T x) { return x * 2; }

// Concepts — new way (clear, readable)
template<std::integral T>
T concept_double(T x) { return x * 2; }


// ============================================================
// Example 9 — Subsumption (concept refinement)
// ============================================================

template<typename T>
concept Shape = requires(T s) {
    { s.area()     } -> std::floating_point;
    { s.perimeter()} -> std::floating_point;
};

template<typename T>
concept ColoredShape = Shape<T> && requires(T s) {
    { s.color() } -> std::convertible_to<std::string>;
};

struct Circle {
    double r;
    double area()      const { return 3.14159 * r * r; }
    double perimeter() const { return 2.0 * 3.14159 * r; }
};

struct ColoredCircle {
    double r;
    std::string c;
    double area()       const { return 3.14159 * r * r; }
    double perimeter()  const { return 2.0 * 3.14159 * r; }
    std::string color() const { return c; }
};

template<Shape T>
void describe(const T& s) {
    std::println("  Shape: area={:.2f} perimeter={:.2f}", s.area(), s.perimeter());
}

template<ColoredShape T>
void describe(const T& s) {
    std::println("  ColoredShape: area={:.2f} color={}", s.area(), s.color());
}


// ============================================================
// Example 10 — MCX Codec concept
// ============================================================

template<typename T>
concept McxCodec = requires(T codec, const std::string& data) {
    { codec.name()    } -> std::convertible_to<std::string>;
    { codec.bitrate() } -> std::convertible_to<int>;
    { codec.encode(data) } -> std::convertible_to<std::string>;
    { codec.decode(data) } -> std::convertible_to<std::string>;
};

struct AmrWbCodec {
    std::string name()    const { return "AMR-WB"; }
    int         bitrate() const { return 23850; }
    std::string encode(const std::string& data) const {
        return "[AMR-WB encoded: " + data + "]";
    }
    std::string decode(const std::string& data) const {
        return "[decoded: " + data + "]";
    }
};

struct OpusCodec {
    std::string name()    const { return "OPUS"; }
    int         bitrate() const { return 32000; }
    std::string encode(const std::string& data) const {
        return "[OPUS encoded: " + data + "]";
    }
    std::string decode(const std::string& data) const {
        return "[decoded: " + data + "]";
    }
};

template<McxCodec T>
void processAudio(const T& codec, const std::string& audio) {
    std::println("  codec: {} ({}bps)", codec.name(), codec.bitrate());
    auto encoded = codec.encode(audio);
    std::println("  encoded: {}", encoded);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What are Concepts?\n";
    std::cout << "=====================================================\n";

    std::cout << "Concepts are COMPILE-TIME constraints on template types.\n\n";
    std::cout << "Without concepts:\n";
    std::cout << "  template<typename T>\n";
    std::cout << "  T add(T a, T b) { return a + b; }\n";
    std::cout << "  // Works on int, double, string — even if that's wrong!\n\n";
    std::cout << "With concepts:\n";
    std::cout << "  template<std::integral T>\n";
    std::cout << "  T add(T a, T b) { return a + b; }\n";
    std::cout << "  // ONLY works on int, long, short etc.\n";
    std::cout << "  // Clear error message if you try double or string\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Built-in Concepts (<concepts> header)\n";
    std::cout << "=====================================================\n";

    std::println("is int integral:         {}", std::integral<int>);
    std::println("is double integral:      {}", std::integral<double>);
    std::println("is double floating_point:{}", std::floating_point<double>);
    std::println("is int floating_point:   {}", std::floating_point<int>);
    std::println("is int signed_integral:  {}", std::signed_integral<int>);
    std::println("is uint unsigned:        {}", std::unsigned_integral<unsigned int>);

    std::println("same_as<int,int>:        {}", std::same_as<int,int>);
    std::println("same_as<int,double>:     {}", std::same_as<int,double>);
    std::println("derived_from<bool,int>:  {}", std::derived_from<bool,int>);
    std::println("convertible int→double:  {}", std::convertible_to<int,double>);

    // Range concepts
    std::println("vector is range:         {}", std::ranges::range<std::vector<int>>);
    std::println("int is range:            {}", std::ranges::range<int>);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Custom concepts\n";
    std::cout << "=====================================================\n";

    std::println("int is Arithmetic:    {}", Arithmetic<int>);
    std::println("double is Arithmetic: {}", Arithmetic<double>);
    std::println("string is Arithmetic: {}", Arithmetic<std::string>);
    std::println("int is Printable:     {}", Printable<int>);
    std::println("vector is Sizeable:   {}", Sizeable<std::vector<int>>);
    std::println("vector is Container:  {}", Container<std::vector<int>>);
    std::println("list is Container:    {}", Container<std::list<int>>);
    std::println("int is Container:     {}", Container<int>);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: 4 syntaxes for constrained functions\n";
    std::cout << "=====================================================\n";

    std::println("doubled_v1(5) = {}", doubled_v1(5));
    std::println("doubled_v2(5) = {}", doubled_v2(5));
    std::println("doubled_v3(5) = {}", doubled_v3(5));
    std::println("doubled_v4(5) = {}", doubled_v4(5));

    // doubled_v1(3.14);  // ERROR — not integral
    std::cout << "All 4 give same result — choose the style you prefer\n";
    std::cout << "Recommended: syntax 2 (template<std::integral T>)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Multiple constraints && and ||\n";
    std::cout << "=====================================================\n";

    std::println("square(5)      = {}", square(5));
    std::println("square(3.14)   = {}", square(3.14));
    std::println("absoluteVal(-7)= {}", absoluteVal(-7));

    std::vector<int>         vi = {3,1,4,1,5,9};
    std::list<std::string>   ls = {"alpha","beta","gamma"};

    std::cout << "printContainer(vector): ";
    printContainer(vi);
    std::cout << "printContainer(list):   ";
    printContainer(ls);

    std::println("findMax vector<int>: {}", findMax(vi));
    std::vector<std::string> vs = {"apple","zebra","mango"};
    std::println("findMax vector<string>: {}", findMax(vs));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: requires expression — inspect type\n";
    std::cout << "=====================================================\n";

    std::cout << "requires expressions check:\n";
    std::cout << "  Simple:     { expr }          — expr must be valid\n";
    std::cout << "  Type:       typename T::type  — type must exist\n";
    std::cout << "  Compound:   { expr } -> T     — expr must return T\n";
    std::cout << "  Nested:     requires(...)     — sub-requires\n\n";

    std::println("int is Arithmetic:    {}", Arithmetic<int>);
    std::println("string is Arithmetic: {}", Arithmetic<std::string>);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Constrained auto\n";
    std::cout << "=====================================================\n";

    // auto with concept constraint
    std::integral auto x = 42;          // must be integral
    std::floating_point auto y = 3.14;  // must be floating point

    std::println("integral auto x = {}", x);
    std::println("floating_point auto y = {}", y);

    // In function parameters (C++20)
    auto addInts = [](std::integral auto a, std::integral auto b) {
        return a + b;
    };

    std::println("addInts(3, 4) = {}", addInts(3, 4));
    // addInts(3.0, 4.0);  // ERROR — not integral


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Concepts vs SFINAE\n";
    std::cout << "=====================================================\n";

    std::println("sfinae_double(5)  = {}", sfinae_double(5));
    std::println("concept_double(5) = {}", concept_double(5));

    std::cout << "\nSFINAE error message (old):\n";
    std::cout << "  'enable_if' not found, candidate not viable...\n";
    std::cout << "  substitution failure... template argument deduction...\n\n";
    std::cout << "Concepts error message (new):\n";
    std::cout << "  'double' does not satisfy 'std::integral'\n";
    std::cout << "  Much clearer — exactly what constraint failed\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Subsumption — concept refinement\n";
    std::cout << "=====================================================\n";

    Circle        c  { 5.0 };
    ColoredCircle cc { 5.0, "red" };

    std::cout << "Circle (satisfies Shape):\n";
    describe(c);   // calls Shape overload

    std::cout << "ColoredCircle (satisfies ColoredShape):\n";
    describe(cc);  // calls ColoredShape overload — more specific wins

    std::println("\nCircle satisfies Shape:        {}", Shape<Circle>);
    std::println("Circle satisfies ColoredShape: {}", ColoredShape<Circle>);
    std::println("ColoredCircle satisfies Shape: {}", Shape<ColoredCircle>);
    std::println("ColoredCircle satisfies ColoredShape: {}", ColoredShape<ColoredCircle>);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: McxCodec concept\n";
    std::cout << "=====================================================\n";

    std::println("AmrWbCodec satisfies McxCodec: {}", McxCodec<AmrWbCodec>);
    std::println("OpusCodec satisfies McxCodec:  {}", McxCodec<OpusCodec>);
    std::println("int satisfies McxCodec:        {}", McxCodec<int>);

    AmrWbCodec amr;
    OpusCodec  opus;

    std::cout << "\nAMR-WB:\n";
    processAudio(amr, "Hello ALPHA");

    std::cout << "\nOPUS:\n";
    processAudio(opus, "Hello ALPHA");

    std::cout << "\n=== Concepts complete ===\n";
    return 0;
}
