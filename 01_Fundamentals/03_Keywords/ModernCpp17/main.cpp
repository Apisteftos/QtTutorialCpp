#include <iostream>
#include <variant>
#include <any>
#include <optional>
#include <string>

// ============================================================
// C++17 SPECIAL KEYWORDS / NEW LANGUAGE FEATURES
// ============================================================
//
// C++17 did NOT add many brand-new keywords like earlier standards.
// Instead it introduced NEW CORE FEATURES using existing keywords.
//
// The most important C++17-related ones are:
// ------------------------------------------------------------
// 1. if constexpr
// 2. structured bindings (auto [a,b])
// 3. inline variables
// 4. constexpr improvements
// 5. fold expressions (variadic templates)
// 6. std::optional (library feature, not keyword)
// 7. std::variant (library feature, not keyword)
// 8. std::any (library feature, not keyword)
// ============================================================

// ============================================================
// 1. if constexpr (COMPILE-TIME IF)
// ============================================================

template <typename T>
void printTypeInfo(T value)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::cout << "Integral type: " << value << "\n";
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        std::cout << "Floating type: " << value << "\n";
    }
    else
    {
        std::cout << "Other type\n";
    }
}

// ============================================================
// 2. STRUCTURED BINDINGS (auto [..])
// ============================================================

std::pair<int, std::string> getUser()
{
    return {1, "Alex"};
}

// ============================================================
// 3. INLINE VARIABLES (C++17)
// ============================================================

inline int globalCounter = 0; // can be defined in headers safely

// ============================================================
// 4. FOLD EXPRESSIONS (variadic templates)
// ============================================================

template <typename... Args>
auto sum(Args... args)
{
    return (args + ...); // C++17 fold expression
}

// ============================================================
// 5. std::optional (C++17 LIBRARY FEATURE)
// ============================================================

std::optional<int> safeDivide(int a, int b)
{
    if (b == 0)
        return std::nullopt;
    return a / b;
}

// ============================================================
// 6. std::variant (TYPE-SAFE UNION)
// ============================================================

std::variant<int, float, std::string> getValue(bool flag)
{
    if (flag)
        return 42;
    else
        return std::string("Hello");
}

// ============================================================
// 7. std::any (ANY TYPE STORAGE)
// ============================================================

std::any storeAnything()
{
    return 3.14;
}

// ============================================================
// MAIN DEMO
// ============================================================

int main()
{
    // ---------------- if constexpr ----------------
    std::cout << "if constexpr demo:\n";
    printTypeInfo(10);
    printTypeInfo(3.14);
    std::cout << "\n";

    // ---------------- structured bindings ----------------
    std::cout << "structured bindings:\n";
    auto [id, name] = getUser();
    std::cout << "id=" << id << " name=" << name << "\n\n";

    // ---------------- inline variable ----------------
    globalCounter++;
    std::cout << "inline variable counter = " << globalCounter << "\n\n";

    // ---------------- fold expression ----------------
    std::cout << "fold expression sum = " << sum(1, 2, 3, 4, 5) << "\n\n";

    // ---------------- optional ----------------
    auto result = safeDivide(10, 2);
    if (result)
        std::cout << "safeDivide result = " << *result << "\n";
    else
        std::cout << "division failed\n";
    std::cout << "\n";

    // ---------------- variant ----------------
    auto v = getValue(true);
    std::cout << "variant holds int = " << std::get<int>(v) << "\n\n";

    // ---------------- any ----------------
    std::any a = storeAnything();
    std::cout << "any holds double = " << std::any_cast<double>(a) << "\n";

    return 0;
}
