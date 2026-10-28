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
//   1.  Built-in concepts — std::integral, std::floating_point etc.
//   2.  requires clause — ad-hoc constraints
//   3.  Custom concepts — define your own
//   4.  Concept with multiple constraints — &&, ||
//   5.  requires expression — checking expressions
//   6.  Constrained auto
//   7.  Concepts vs SFINAE — comparison
//   8.  Subsumption — concept refinement
//   9.  Real use case — text encoder concept
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
    std::print("  [ ");
    for (const auto& item : c) std::print("{} ", item);
    std::println("]");
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
// Example 10 — Text encoder concept
// ============================================================

template<typename T>
concept TextEncoder = requires(T encoder, const std::string& data) {
    { encoder.name()    } -> std::convertible_to<std::string>;
    { encoder.maxLength() } -> std::convertible_to<int>;
    { encoder.encode(data) } -> std::convertible_to<std::string>;
    { encoder.decode(data) } -> std::convertible_to<std::string>;
};

struct BracketEncoder {
    std::string name()    const { return "BRACKET"; }
    int         maxLength() const { return 256; }
    std::string encode(const std::string& data) const {
        return "[BRACKET encoded: " + data + "]";
    }
    std::string decode(const std::string& data) const {
        return "[decoded: " + data + "]";
    }
};

struct PrefixEncoder {
    std::string name()    const { return "PREFIX"; }
    int         maxLength() const { return 1024; }
    std::string encode(const std::string& data) const {
        return "[PREFIX encoded: " + data + "]";
    }
    std::string decode(const std::string& data) const {
        return "[decoded: " + data + "]";
    }
};

template<TextEncoder T>
void processText(const T& encoder, const std::string& text) {
    std::println("  encoder: {} (max length {})", encoder.name(), encoder.maxLength());
    auto encoded = encoder.encode(text);
    std::println("  encoded: {}", encoded);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 1: Built-in Concepts (<concepts> header)");
    std::println("=====================================================");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Custom concepts");
    std::println("=====================================================");

    std::println("int is Arithmetic:    {}", Arithmetic<int>);
    std::println("double is Arithmetic: {}", Arithmetic<double>);
    std::println("string is Arithmetic: {}", Arithmetic<std::string>);
    std::println("int is Printable:     {}", Printable<int>);
    std::println("vector is Sizeable:   {}", Sizeable<std::vector<int>>);
    std::println("vector is Container:  {}", Container<std::vector<int>>);
    std::println("list is Container:    {}", Container<std::list<int>>);
    std::println("int is Container:     {}", Container<int>);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: 4 syntaxes for constrained functions");
    std::println("=====================================================");

    std::println("doubled_v1(5) = {}", doubled_v1(5));
    std::println("doubled_v2(5) = {}", doubled_v2(5));
    std::println("doubled_v3(5) = {}", doubled_v3(5));
    std::println("doubled_v4(5) = {}", doubled_v4(5));

    // doubled_v1(3.14);  // ERROR — not integral
    std::println("All 4 give same result — choose the style you prefer");
    std::println("Recommended: syntax 2 (template<std::integral T>)");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Multiple constraints && and ||");
    std::println("=====================================================");

    std::println("square(5)      = {}", square(5));
    std::println("square(3.14)   = {}", square(3.14));
    std::println("absoluteVal(-7)= {}", absoluteVal(-7));

    std::vector<int>         vi = {3,1,4,1,5,9};
    std::list<std::string>   ls = {"alpha","beta","gamma"};

    std::print("printContainer(vector): ");
    printContainer(vi);
    std::print("printContainer(list):   ");
    printContainer(ls);

    std::println("findMax vector<int>: {}", findMax(vi));
    std::vector<std::string> vs = {"apple","zebra","mango"};
    std::println("findMax vector<string>: {}", findMax(vs));


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: requires expression — inspect type");
    std::println("=====================================================");

    std::println("int is Arithmetic:    {}", Arithmetic<int>);
    std::println("string is Arithmetic: {}", Arithmetic<std::string>);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Constrained auto");
    std::println("=====================================================");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: Concepts vs SFINAE");
    std::println("=====================================================");

    std::println("sfinae_double(5)  = {}", sfinae_double(5));
    std::println("concept_double(5) = {}", concept_double(5));


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: Subsumption — concept refinement");
    std::println("=====================================================");

    Circle        c  { 5.0 };
    ColoredCircle cc { 5.0, "red" };

    std::println("Circle (satisfies Shape):");
    describe(c);   // calls Shape overload

    std::println("ColoredCircle (satisfies ColoredShape):");
    describe(cc);  // calls ColoredShape overload — more specific wins

    std::println("");
    std::println("Circle satisfies Shape:        {}", Shape<Circle>);
    std::println("Circle satisfies ColoredShape: {}", ColoredShape<Circle>);
    std::println("ColoredCircle satisfies Shape: {}", Shape<ColoredCircle>);
    std::println("ColoredCircle satisfies ColoredShape: {}", ColoredShape<ColoredCircle>);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: TextEncoder concept");
    std::println("=====================================================");

    std::println("BracketEncoder satisfies TextEncoder: {}", TextEncoder<BracketEncoder>);
    std::println("PrefixEncoder satisfies TextEncoder:  {}", TextEncoder<PrefixEncoder>);
    std::println("int satisfies TextEncoder:        {}", TextEncoder<int>);

    BracketEncoder bracket;
    PrefixEncoder  prefix;

    std::println("");
    std::println("BRACKET:");
    processText(bracket, "Hello world");

    std::println("");
    std::println("PREFIX:");
    processText(prefix, "Hello world");

    std::println("");
    std::println("=== Concepts complete ===");
    return 0;
}
