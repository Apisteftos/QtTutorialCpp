/*
 * std::is_scoped_enum, std::is_implicit_lifetime,
 * std::reference_constructs_from_temporary,
 * std::reference_converts_from_temporary (C++23, <type_traits>)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <string>
#include <type_traits>
#include <version>

enum Plain { A, B };
enum class Scoped { A, B };

struct Aggregate {
    int x;
    double y;
};

struct WithDestructor {
    ~WithDestructor() {}
};

int main() {
    // === EXAMPLE 1: is_scoped_enum ===
    std::println("=== Example 1: is_scoped_enum ===");
    std::println("Plain:  {}", std::is_scoped_enum_v<Plain>);
    std::println("Scoped: {}", std::is_scoped_enum_v<Scoped>);
    std::println("int:    {}", std::is_scoped_enum_v<int>);

    // === EXAMPLE 2: is_implicit_lifetime ===
    std::println("");
    std::println("=== Example 2: is_implicit_lifetime ===");
    // Types whose objects can be created implicitly by operations such as malloc / memcpy
#ifdef __cpp_lib_is_implicit_lifetime
    std::println("int:            {}", std::is_implicit_lifetime_v<int>);
    std::println("Aggregate:      {}", std::is_implicit_lifetime_v<Aggregate>);
    std::println("WithDestructor: {}", std::is_implicit_lifetime_v<WithDestructor>);
    std::println("std::string:    {}", std::is_implicit_lifetime_v<std::string>);
#else
    // needs libstdc++ 16 (GCC 15 does not provide it yet)
    std::println("is_implicit_lifetime: not available in this standard library");
#endif

    // === EXAMPLE 3: reference binding to temporaries ===
    std::println("");
    std::println("=== Example 3: reference_constructs_from_temporary ===");
    // true means: the reference would be bound to a hidden temporary object
    // (a classic source of dangling references)
    std::println("const std::string& from const char*:        {}",
                 std::reference_constructs_from_temporary_v<const std::string&, const char*>);
    std::println("const std::string& from const std::string&: {}",
                 std::reference_constructs_from_temporary_v<const std::string&, const std::string&>);
    std::println("const int& from long:                       {}",
                 std::reference_constructs_from_temporary_v<const int&, long>);
    std::println("const int& from int&:                       {}",
                 std::reference_constructs_from_temporary_v<const int&, int&>);
    // the "converts" variant tests implicit conversion instead of direct initialization
    std::println("converts: const int& from long:             {}",
                 std::reference_converts_from_temporary_v<const int&, long>);
    std::println("converts: const int& from int&:             {}",
                 std::reference_converts_from_temporary_v<const int&, int&>);
    return 0;
}
