/*
 * type_list.cpp
 *
 * Type Lists and Compile-Time Type Manipulation
 *
 * Difficulty: 🔴 Advanced (Expert Level)
 *
 * Key Concepts:
 * - Type lists (compile-time type collections)
 * - Recursive type manipulation
 * - Type list operations (append, find, transform)
 * - Parameter packs
 * - Variadic templates
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <type_traits>
#include <string>
#include <algorithm>
#include <print>

// ============================================================================
// COMMON TYPE LIST DEFINITION (used across namespaces)
// ============================================================================

template<typename... Types>
struct TypeList {};

// ============================================================================
// PART 1: Basic Type List
// ============================================================================

namespace Basic {

// Empty type list
struct TypeListEnd {};

// Type list node
template<typename Head, typename Tail = TypeListEnd>
struct TypeListNode {
    using head = Head;
    using tail = Tail;
};

// Helper to create type lists
template<typename... Types>
struct MakeTypeList;

template<>
struct MakeTypeList<> {
    using type = TypeListEnd;
};

template<typename Head, typename... Tail>
struct MakeTypeList<Head, Tail...> {
    using type = TypeListNode<Head, typename MakeTypeList<Tail...>::type>;
};

template<typename... Types>
using MakeTypeList_t = typename MakeTypeList<Types...>::type;

void demonstrate() {
    std::println("");
    std::println("=== Basic Type List ===");

    // Create a type list: int, double, char
    using MyList = MakeTypeList_t<int, double, char>;

    std::println("");
    std::println("Type list created: int, double, char");
    std::println("  Head: {}", typeid(typename MyList::head).name());
    std::println("  Tail head: {}", typeid(typename MyList::tail::head).name());

    std::println("");
    std::println("✅ Type list = compile-time data structure!");
}
}

// ============================================================================
// PART 2: Modern Type List with Variadic Templates
// ============================================================================

namespace Modern {

// Get length of type list
template<typename List>
struct Length;

template<typename... Types>
struct Length<TypeList<Types...>> {
    static constexpr size_t value = sizeof...(Types);
};

template<typename List>
inline constexpr size_t Length_v = Length<List>::value;

// Check if type list is empty
template<typename List>
struct IsEmpty : std::false_type {};

template<>
struct IsEmpty<TypeList<>> : std::true_type {};

template<typename List>
inline constexpr bool IsEmpty_v = IsEmpty<List>::value;

// Get first type (head)
template<typename List>
struct Front;

template<typename Head, typename... Tail>
struct Front<TypeList<Head, Tail...>> {
    using type = Head;
};

template<typename List>
using Front_t = typename Front<List>::type;

void demonstrate() {
    std::println("");
    std::println("=== Modern Type List ===");

    using MyList = TypeList<int, double, char, float>;

    std::println("");
    std::println("Type list: int, double, char, float");
    std::println("  Length: {}", Length_v<MyList>);
    std::println("  Is empty: {}", IsEmpty_v<MyList>);
    std::println("  Is empty (empty list): {}", IsEmpty_v<TypeList<>>);
    std::println("  Front type: {}", typeid(Front_t<MyList>).name());

    std::println("");
    std::println("✅ Variadic templates = cleaner type lists!");
}
}

// ============================================================================
// PART 3: Type List Operations - Append
// ============================================================================

namespace Operations {

// Append type to end of list
template<typename List, typename NewType>
struct PushBack;

template<typename... Types, typename NewType>
struct PushBack<TypeList<Types...>, NewType> {
    using type = TypeList<Types..., NewType>;
};

template<typename List, typename NewType>
using PushBack_t = typename PushBack<List, NewType>::type;

// Prepend type to front of list
template<typename List, typename NewType>
struct PushFront;

template<typename... Types, typename NewType>
struct PushFront<TypeList<Types...>, NewType> {
    using type = TypeList<NewType, Types...>;
};

template<typename List, typename NewType>
using PushFront_t = typename PushFront<List, NewType>::type;

// Remove first element
template<typename List>
struct PopFront;

template<typename Head, typename... Tail>
struct PopFront<TypeList<Head, Tail...>> {
    using type = TypeList<Tail...>;
};

template<typename List>
using PopFront_t = typename PopFront<List>::type;

void demonstrate() {
    std::println("");
    std::println("=== Type List Operations ===");

    using List1 = TypeList<int, double>;
    using List2 = PushBack_t<List1, char>;      // int, double, char
    using List3 = PushFront_t<List2, float>;    // float, int, double, char
    using List4 = PopFront_t<List3>;            // int, double, char

    std::println("");
    std::println("Operations:");
    std::println("  Start: int, double");
    std::println("  After PushBack<char>: length = {}", Modern::Length_v<List2>);
    std::println("  After PushFront<float>: length = {}", Modern::Length_v<List3>);
    std::println("  After PopFront: length = {}", Modern::Length_v<List4>);

    std::println("");
    std::println("✅ Manipulate types like data!");
}
}

// ============================================================================
// PART 4: Type List Search - Contains
// ============================================================================

namespace Search {

// Check if list contains type
template<typename List, typename T>
struct Contains;

template<typename T>
struct Contains<TypeList<>, T> : std::false_type {};

template<typename Head, typename... Tail, typename T>
struct Contains<TypeList<Head, Tail...>, T>
    : std::conditional_t<
          std::is_same_v<Head, T>,
          std::true_type,
          Contains<TypeList<Tail...>, T>
          > {};

template<typename List, typename T>
inline constexpr bool Contains_v = Contains<List, T>::value;

// Find index of type
template<typename List, typename T, size_t Index = 0>
struct IndexOf;

template<typename T, size_t Index>
struct IndexOf<TypeList<>, T, Index> {
    static constexpr size_t value = static_cast<size_t>(-1);
};

template<typename Head, typename... Tail, typename T, size_t Index>
struct IndexOf<TypeList<Head, Tail...>, T, Index> {
    static constexpr size_t value = std::is_same_v<Head, T>
                                        ? Index
                                        : IndexOf<TypeList<Tail...>, T, Index + 1>::value;
};

template<typename List, typename T>
inline constexpr size_t IndexOf_v = IndexOf<List, T>::value;

void demonstrate() {
    std::println("");
    std::println("=== Type List Search ===");

    using MyList = TypeList<int, double, char, float>;

    std::println("");
    std::println("Searching in: int, double, char, float");
    std::println("  Contains int: {}", Contains_v<MyList, int>);
    std::println("  Contains char: {}", Contains_v<MyList, char>);
    std::println("  Contains std::string: {}", Contains_v<MyList, std::string>);

    std::println("");
    std::println("Indices:");
    std::println("  Index of int: {}", IndexOf_v<MyList, int>);
    std::println("  Index of char: {}", IndexOf_v<MyList, char>);
    std::println("  Index of float: {}", IndexOf_v<MyList, float>);

    std::println("");
    std::println("✅ Search types at compile-time!");
}
}

// ============================================================================
// PART 5: Type List Transformations
// ============================================================================

namespace Transform {

// Transform each type using a metafunction
template<typename List, template<typename> class Func>
struct Transform;

template<template<typename> class Func>
struct Transform<TypeList<>, Func> {
    using type = TypeList<>;
};

template<typename Head, typename... Tail, template<typename> class Func>
struct Transform<TypeList<Head, Tail...>, Func> {
    using type = Operations::PushFront_t<
        typename Transform<TypeList<Tail...>, Func>::type,
        typename Func<Head>::type
        >;
};

template<typename List, template<typename> class Func>
using Transform_t = typename Transform<List, Func>::type;

// Add pointer to type
template<typename T>
struct AddPointer {
    using type = T*;
};

// Add const to type
template<typename T>
struct AddConst {
    using type = const T;
};

void demonstrate() {
    std::println("");
    std::println("=== Type List Transformations ===");

    using Original = TypeList<int, double, char>;
    using WithPointers = Transform_t<Original, AddPointer>;
    using WithConst = Transform_t<Original, AddConst>;

    std::println("");
    std::println("Original: int, double, char");
    std::println("  First of WithPointers is pointer: {}", std::is_pointer_v<Modern::Front_t<WithPointers>>);
    std::println("  First of WithConst is const: {}", std::is_const_v<Modern::Front_t<WithConst>>);

    std::println("");
    std::println("✅ Transform all types in list!");
}
}

// ============================================================================
// PART 6: Type List Filter
// ============================================================================

namespace Filter {

// Filter types based on predicate
template<typename List, template<typename> class Pred>
struct Filter;

template<template<typename> class Pred>
struct Filter<TypeList<>, Pred> {
    using type = TypeList<>;
};

template<typename Head, typename... Tail, template<typename> class Pred>
struct Filter<TypeList<Head, Tail...>, Pred> {
    using type = std::conditional_t<
        Pred<Head>::value,
        Operations::PushFront_t<typename Filter<TypeList<Tail...>, Pred>::type, Head>,
        typename Filter<TypeList<Tail...>, Pred>::type
        >;
};

template<typename List, template<typename> class Pred>
using Filter_t = typename Filter<List, Pred>::type;

void demonstrate() {
    std::println("");
    std::println("=== Type List Filter ===");

    using AllTypes = TypeList<int, int*, double, char*, float, double*>;
    using OnlyPointers = Filter_t<AllTypes, std::is_pointer>;
    using OnlyIntegral = Filter_t<AllTypes, std::is_integral>;

    std::println("");
    std::println("Original list: int, int*, double, char*, float, double*");
    std::println("  Only pointers length: {}", Modern::Length_v<OnlyPointers>);
    std::println("  Only integral length: {}", Modern::Length_v<OnlyIntegral>);

    std::println("");
    std::println("✅ Filter types by predicate!");
}
}

// ============================================================================
// PART 7: Practical Application - Variant Helper
// ============================================================================

namespace VariantHelper {

// Get type at index
template<typename List, size_t Index>
struct At;

template<typename Head, typename... Tail>
struct At<TypeList<Head, Tail...>, 0> {
    using type = Head;
};

template<typename Head, typename... Tail, size_t Index>
struct At<TypeList<Head, Tail...>, Index> {
    using type = typename At<TypeList<Tail...>, Index - 1>::type;
};

template<typename List, size_t Index>
using At_t = typename At<List, Index>::type;

// Helper to get max size
template<typename... Types>
struct MaxSize;

template<typename T>
struct MaxSize<T> {
    static constexpr size_t value = sizeof(T);
};

template<typename First, typename... Rest>
struct MaxSize<First, Rest...> {
    static constexpr size_t value = sizeof(First) > MaxSize<Rest...>::value
                                        ? sizeof(First)
                                        : MaxSize<Rest...>::value;
};

// Type-safe variant-like class
template<typename... Types>
class SimpleVariant {
private:
    using List = TypeList<Types...>;
    static constexpr size_t Size = sizeof...(Types);

    alignas(std::max({alignof(Types)...})) unsigned char storage_[
        MaxSize<Types...>::value
    ];
    size_t type_index_;

public:
    template<typename T>
    SimpleVariant(T value) {
        static_assert(Search::Contains_v<List, T>, "Type not in variant!");
        new (storage_) T(value);
        type_index_ = Search::IndexOf_v<List, T>;
    }

    size_t index() const { return type_index_; }
};

void demonstrate() {
    std::println("");
    std::println("=== Practical: Type-Safe Variant ===");

    using MyVariant = SimpleVariant<int, double, std::string>;

    MyVariant v1(42);
    MyVariant v2(3.14);
    MyVariant v3(std::string("hello"));

    std::println("");
    std::println("Variant indices:");
    std::println("  v1(int) index: {}", v1.index());
    std::println("  v2(double) index: {}", v2.index());
    std::println("  v3(string) index: {}", v3.index());

    // This would NOT compile:
    // MyVariant v4(3.14f);  // ❌ float not in variant!

    std::println("");
    std::println("✅ Type lists enable type-safe variants!");
}
}

// ============================================================================
// PART 8: Type List Algorithms - All/Any
// ============================================================================

namespace Algorithms {

// Check if all types satisfy predicate
template<typename List, template<typename> class Pred>
struct AllOf;

template<template<typename> class Pred>
struct AllOf<TypeList<>, Pred> : std::true_type {};

template<typename Head, typename... Tail, template<typename> class Pred>
struct AllOf<TypeList<Head, Tail...>, Pred>
    : std::conditional_t<
          Pred<Head>::value,
          AllOf<TypeList<Tail...>, Pred>,
          std::false_type
          > {};

template<typename List, template<typename> class Pred>
inline constexpr bool AllOf_v = AllOf<List, Pred>::value;

// Check if any type satisfies predicate
template<typename List, template<typename> class Pred>
struct AnyOf;

template<template<typename> class Pred>
struct AnyOf<TypeList<>, Pred> : std::false_type {};

template<typename Head, typename... Tail, template<typename> class Pred>
struct AnyOf<TypeList<Head, Tail...>, Pred>
    : std::conditional_t<
          Pred<Head>::value,
          std::true_type,
          AnyOf<TypeList<Tail...>, Pred>
          > {};

template<typename List, template<typename> class Pred>
inline constexpr bool AnyOf_v = AnyOf<List, Pred>::value;

void demonstrate() {
    std::println("");
    std::println("=== Type List Algorithms ===");

    using List1 = TypeList<int, long, short>;
    using List2 = TypeList<int*, double*, char*>;
    using List3 = TypeList<int, double, char*>;

    std::println("");
    std::println("List1: int, long, short");
    std::println("  All integral: {}", AllOf_v<List1, std::is_integral>);
    std::println("  Any pointer: {}", AnyOf_v<List1, std::is_pointer>);

    std::println("");
    std::println("List2: int*, double*, char*");
    std::println("  All pointer: {}", AllOf_v<List2, std::is_pointer>);

    std::println("");
    std::println("List3: int, double, char*");
    std::println("  All pointer: {}", AllOf_v<List3, std::is_pointer>);
    std::println("  Any pointer: {}", AnyOf_v<List3, std::is_pointer>);

    std::println("");
    std::println("✅ Compile-time algorithms on type lists!");
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║            TYPE LISTS AND TYPE MANIPULATION         ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    Basic::demonstrate();
    Modern::demonstrate();
    Operations::demonstrate();
    Search::demonstrate();
    Transform::demonstrate();
    Filter::demonstrate();
    VariantHelper::demonstrate();
    Algorithms::demonstrate();

    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("🎓 TYPE LIST MASTERY:");
    std::println("{}", std::string(60, '='));
    std::println("{}", std::string(60, '='));
    std::println("");

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -Wall -Wextra type_list.cpp -o type_list
 * ./type_list
 *
 * KEY FIXES APPLIED:
 * ==================
 * 1. Fixed alignment issue in SimpleVariant using std::max with alignof
 * 2. Created MaxSize helper to avoid std::max with sizeof in array size
 * 3. Ensured all namespaces use the global TypeList definition
 * 4. Fixed all circular dependencies
 *
 * TYPE LIST OPERATIONS:
 * ====================
 * - PushFront  - Add to front
 * - PushBack   - Add to back
 * - PopFront   - Remove first
 * - Contains   - Search for type
 * - IndexOf    - Get position
 * - Transform  - Apply function to all
 * - Filter     - Keep matching types
 * - AllOf      - Check all match predicate
 * - AnyOf      - Check any match predicate
 */
