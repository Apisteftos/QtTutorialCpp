/*
 * map/set node handles (extract, merge), try_emplace, insert_or_assign
 * (C++17)
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <iostream>
#include <map>
#include <set>
#include <string>

void print(const char* label, const std::map<int, std::string>& m) {
    std::cout << label << ":";
    for (const auto& [key, value] : m) {
        std::cout << " " << key << "=" << value;
    }
    std::cout << "\n";
}

int main() {
    // === EXAMPLE 1: try_emplace and insert_or_assign ===
    std::cout << "=== Example 1: try_emplace, insert_or_assign ===\n";
    std::map<int, std::string> m;
    m.try_emplace(1, "one");
    auto [it, inserted] = m.try_emplace(1, "uno");   // key exists: nothing is constructed or changed
    std::cout << "second try_emplace inserted: " << std::boolalpha << inserted << ", value " << it->second << "\n";
    m.insert_or_assign(1, "ONE");                    // replaces the value
    m.insert_or_assign(2, "two");                    // inserts
    print("map", m);

    // === EXAMPLE 2: extract: change a key without copying the element ===
    std::cout << "\n=== Example 2: extract and change a key ===\n";
    auto node = m.extract(2);
    node.key() = 20;
    m.insert(std::move(node));
    print("after key change", m);

    // === EXAMPLE 3: merge ===
    std::cout << "\n=== Example 3: merge ===\n";
    std::map<int, std::string> other = {{1, "dup"}, {5, "five"}};
    m.merge(other);   // moves nodes whose keys are new; others stay in `other`
    print("merged", m);
    print("left in other", other);

    // === EXAMPLE 4: moving a node between containers ===
    std::cout << "\n=== Example 4: set node ===\n";
    std::set<std::string> a = {"x", "y"};
    std::set<std::string> b;
    b.insert(a.extract("x"));
    std::cout << "a has " << a.size() << " element, b has " << b.size() << "\n";
    return 0;
}
