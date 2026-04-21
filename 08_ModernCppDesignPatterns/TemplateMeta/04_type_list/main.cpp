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
 * Compile: g++ -std=c++20 -Wall -Wextra type_list.cpp -o type_list
 */

#include <iostream>
#include <type_traits>
#include <string>
#include <algorithm>

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
    std::cout << "\n=== Basic Type List ===\n";

    // Create a type list: int, double, char
    using MyList = MakeTypeList_t<int, double, char>;

    std::cout << "\nType list created: int, double, char\n";
    std::cout << "  Head: " << typeid(typename MyList::head).name() << "\n";
    std::cout << "  Tail head: " << typeid(typename MyList::tail::head).name() << "\n";

    std::cout << "\n✅ Type list = compile-time data structure!\n";
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
    std::cout << "\n=== Modern Type List ===\n";

    using MyList = TypeList<int, double, char, float>;

    std::cout << "\nType list: int, double, char, float\n";
    std::cout << "  Length: " << Length_v<MyList> << "\n";
    std::cout << "  Is empty: " << IsEmpty_v<MyList> << "\n";
    std::cout << "  Is empty (empty list): " << IsEmpty_v<TypeList<>> << "\n";
    std::cout << "  Front type: " << typeid(Front_t<MyList>).name() << "\n";

    std::cout << "\n✅ Variadic templates = cleaner type lists!\n";
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
    std::cout << "\n=== Type List Operations ===\n";

    using List1 = TypeList<int, double>;
    using List2 = PushBack_t<List1, char>;      // int, double, char
    using List3 = PushFront_t<List2, float>;    // float, int, double, char
    using List4 = PopFront_t<List3>;            // int, double, char

    std::cout << "\nOperations:\n";
    std::cout << "  Start: int, double\n";
    std::cout << "  After PushBack<char>: length = " << Modern::Length_v<List2> << "\n";
    std::cout << "  After PushFront<float>: length = " << Modern::Length_v<List3> << "\n";
    std::cout << "  After PopFront: length = " << Modern::Length_v<List4> << "\n";

    std::cout << "\n✅ Manipulate types like data!\n";
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
    std::cout << "\n=== Type List Search ===\n";

    using MyList = TypeList<int, double, char, float>;

    std::cout << "\nSearching in: int, double, char, float\n";
    std::cout << "  Contains int: " << Contains_v<MyList, int> << "\n";
    std::cout << "  Contains char: " << Contains_v<MyList, char> << "\n";
    std::cout << "  Contains std::string: " << Contains_v<MyList, std::string> << "\n";

    std::cout << "\nIndices:\n";
    std::cout << "  Index of int: " << IndexOf_v<MyList, int> << "\n";
    std::cout << "  Index of char: " << IndexOf_v<MyList, char> << "\n";
    std::cout << "  Index of float: " << IndexOf_v<MyList, float> << "\n";

    std::cout << "\n✅ Search types at compile-time!\n";
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
    std::cout << "\n=== Type List Transformations ===\n";

    using Original = TypeList<int, double, char>;
    using WithPointers = Transform_t<Original, AddPointer>;
    using WithConst = Transform_t<Original, AddConst>;

    std::cout << "\nOriginal: int, double, char\n";
    std::cout << "  First of WithPointers is pointer: "
              << std::is_pointer_v<Modern::Front_t<WithPointers>> << "\n";
    std::cout << "  First of WithConst is const: "
              << std::is_const_v<Modern::Front_t<WithConst>> << "\n";

    std::cout << "\n✅ Transform all types in list!\n";
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
    std::cout << "\n=== Type List Filter ===\n";

    using AllTypes = TypeList<int, int*, double, char*, float, double*>;
    using OnlyPointers = Filter_t<AllTypes, std::is_pointer>;
    using OnlyIntegral = Filter_t<AllTypes, std::is_integral>;

    std::cout << "\nOriginal list: int, int*, double, char*, float, double*\n";
    std::cout << "  Only pointers length: " << Modern::Length_v<OnlyPointers> << "\n";
    std::cout << "  Only integral length: " << Modern::Length_v<OnlyIntegral> << "\n";

    std::cout << "\n✅ Filter types by predicate!\n";
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
    std::cout << "\n=== Practical: Type-Safe Variant ===\n";

    using MyVariant = SimpleVariant<int, double, std::string>;

    MyVariant v1(42);
    MyVariant v2(3.14);
    MyVariant v3(std::string("hello"));

    std::cout << "\nVariant indices:\n";
    std::cout << "  v1(int) index: " << v1.index() << "\n";
    std::cout << "  v2(double) index: " << v2.index() << "\n";
    std::cout << "  v3(string) index: " << v3.index() << "\n";

    // This would NOT compile:
    // MyVariant v4(3.14f);  // ❌ float not in variant!

    std::cout << "\n✅ Type lists enable type-safe variants!\n";
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
    std::cout << "\n=== Type List Algorithms ===\n";

    using List1 = TypeList<int, long, short>;
    using List2 = TypeList<int*, double*, char*>;
    using List3 = TypeList<int, double, char*>;

    std::cout << "\nList1: int, long, short\n";
    std::cout << "  All integral: " << AllOf_v<List1, std::is_integral> << "\n";
    std::cout << "  Any pointer: " << AnyOf_v<List1, std::is_pointer> << "\n";

    std::cout << "\nList2: int*, double*, char*\n";
    std::cout << "  All pointer: " << AllOf_v<List2, std::is_pointer> << "\n";

    std::cout << "\nList3: int, double, char*\n";
    std::cout << "  All pointer: " << AllOf_v<List3, std::is_pointer> << "\n";
    std::cout << "  Any pointer: " << AnyOf_v<List3, std::is_pointer> << "\n";

    std::cout << "\n✅ Compile-time algorithms on type lists!\n";
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║            TYPE LISTS AND TYPE MANIPULATION         ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

    Basic::demonstrate();
    Modern::demonstrate();
    Operations::demonstrate();
    Search::demonstrate();
    Transform::demonstrate();
    Filter::demonstrate();
    VariantHelper::demonstrate();
    Algorithms::demonstrate();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 TYPE LIST MASTERY:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "✅ Type lists = compile-time type collections\n";
    std::cout << "✅ Operations: push, pop, search, filter, transform\n";
    std::cout << "✅ Algorithms: all, any, contains, index\n";
    std::cout << "✅ All computed at compile-time!\n";
    std::cout << "✅ Zero runtime cost\n";
    std::cout << "\n💡 Practical uses:\n";
    std::cout << "   - Type-safe variants\n";
    std::cout << "   - Generic visitors\n";
    std::cout << "   - Compile-time polymorphism\n";
    std::cout << "   - Meta-data structures\n";
    std::cout << "\n💡 think-cell uses type lists for:\n";
    std::cout << "   - Generic algorithm selection\n";
    std::cout << "   - Type-safe interfaces\n";
    std::cout << "   - Compile-time dispatch\n";
    std::cout << "   - Zero-cost abstractions\n";
    std::cout << "\n🚀 This is EXPERT-level template metaprogramming!\n";
    std::cout << std::string(60, '=') << "\n\n";

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
