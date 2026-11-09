/*
 * C++23 formatting of ranges, pair and tuple, std::range_formatter,
 * std::format_kind and the std::formattable concept
 * (<format>, <print>)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o 02_formatter
 * (the range and tuple formatters need GCC 15 or newer)
 */

#include <array>
#include <format>
#include <map>
#include <print>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// === EXAMPLE 1: Built-in formatters for ranges, std::pair and std::tuple ===
void builtinFormatters() {
    std::println("=== Example 1: built-in formatters ===");

    std::vector<int> v{1, 2, 3};
    std::vector<double> d{1.0, 2.5, 3.14159};
    std::vector<std::string> names{"red", "green"};
    std::set<int> s{3, 1, 2};
    std::map<int, std::string> m{{1, "one"}, {2, "two"}};
    std::pair<int, std::string> p{7, "seven"};
    std::tuple<int, std::string, double> t{1, "hi", 2.5};

    std::println("{}", v);        // sequence -> [1, 2, 3]
    std::println("{}", s);        // set      -> {1, 2, 3}
    std::println("{}", m);        // map      -> {1: "one", 2: "two"}
    std::println("{}", names);    // strings inside ranges are quoted
    std::println("{}", p);        // pair     -> (7, "seven")
    std::println("{}", t);        // tuple    -> (1, "hi", 2.5)

    // Range format spec:  {[range-spec]:[element-spec]}
    std::println("{:n}", v);      // 'n' = no brackets
    std::println("{::04}", v);    // element spec after the 2nd colon
    std::println("{::.2f}", d);   // each double with 2 decimals
    std::println("{:n:.1f}", d);  // no brackets + element spec
    std::println("{:n}", p);      // pair without parentheses
    std::println("{:n}", t);

    // 'm' treats a range of pairs like a map, 's' treats it like a set
    std::vector<std::pair<int, std::string>> vp{{1, "a"}, {2, "b"}};
    std::println("{}", vp);       // [(1, "a"), (2, "b")]
    std::println("{:m}", vp);     // {1: "a", 2: "b"}

    // vector<char> can be printed as a string
    std::vector<char> chars{'h', 'e', 'y'};
    std::println("{}", chars);    // ['h', 'e', 'y']
    std::println("{:s}", chars);  // hey
}

// === EXAMPLE 2: std::format_kind tells the library how to treat a range ===
static_assert(std::format_kind<std::vector<int>> == std::range_format::sequence);
static_assert(std::format_kind<std::set<int>> == std::range_format::set);
static_assert(std::format_kind<std::map<int, int>> == std::range_format::map);

// A user-defined range. By default it would print like a sequence: [..]
// Specializing format_kind makes it print like a set: {..}
struct Tags {
    std::vector<std::string> items;
    auto begin() const { return items.begin(); }
    auto end() const { return items.end(); }
};

template <>
constexpr std::range_format std::format_kind<Tags> = std::range_format::set;

void formatKindDemo() {
    std::println("");
    std::println("=== Example 2: std::format_kind ===");
    Tags tags{{"cpp", "qt", "cmake"}};
    std::println("{}", tags);   // {"cpp", "qt", "cmake"}
}

// === EXAMPLE 3: std::range_formatter builds a formatter on top of the range one ===
struct Vec3 {
    std::array<double, 3> v;
};

template <>
struct std::formatter<Vec3> {
    std::range_formatter<double> rf;

    constexpr formatter() {
        rf.set_brackets("<", ">");
        rf.set_separator("; ");
    }

    // Reuses the range spec: {}, {:n}, {::.2f}, {:n:.1f} ...
    constexpr auto parse(std::format_parse_context& ctx) { return rf.parse(ctx); }

    // Template on the context type, and give an explicit return type instead of
    // `auto`. The std::formattable check probes format() with a context whose
    // iterator is std::back_insert_iterator<std::string>. With `auto`, the compiler
    // must instantiate the body to deduce the return type, and libstdc++ 15's
    // range_formatter::format static_asserts on that iterator type. With an
    // explicit return type the body is instantiated only when something is
    // really formatted.
    template <class FormatContext>
    typename FormatContext::iterator format(const Vec3& x, FormatContext& ctx) const {
        return rf.format(x.v, ctx);
    }
};

void rangeFormatterDemo() {
    std::println("");
    std::println("=== Example 3: std::range_formatter ===");
    Vec3 a{{1.0, 2.5, 3.14159}};
    std::println("{}", a);         // <1; 2.5; 3.14159>   (default double format drops ".0")
    std::println("{::.2f}", a);    // <1.00; 2.50; 3.14>
    std::println("{:n:.1f}", a);   // 1.0; 2.5; 3.1
}

// === EXAMPLE 4: std::formattable checks or constrains formattability ===
struct NoFormat {};

static_assert(std::formattable<int, char>);
static_assert(std::formattable<std::vector<int>, char>);
static_assert(std::formattable<std::vector<std::vector<int>>, char>);
static_assert(std::formattable<std::pair<int, std::string>, char>);
static_assert(std::formattable<Vec3, char>);
static_assert(!std::formattable<NoFormat, char>);
static_assert(!std::formattable<std::vector<NoFormat>, char>);

// Constrain a function template: a clear error instead of a wall of text
template <std::formattable<char> T>
void show(const T& value) {
    std::println("value = {}", value);
}

void formattableDemo() {
    std::println("");
    std::println("=== Example 4: std::formattable ===");
    show(42);
    show(std::vector<int>{1, 2, 3});
    show(std::pair{1, std::string{"x"}});
    // show(NoFormat{});   // error: constraint 'std::formattable<NoFormat, char>' not satisfied
}

int main() {
    builtinFormatters();
    formatKindDemo();
    rangeFormatterDemo();
    formattableDemo();
    return 0;
}
