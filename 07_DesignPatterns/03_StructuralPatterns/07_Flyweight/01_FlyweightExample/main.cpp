/*
 * Flyweight: trees in a forest
 *
 * Intent: save memory by sharing the data that many objects have in common
 * (the tree type: name, colour, texture) and keeping only the individual data
 * (position) in each object.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <map>
#include <memory>
#include <print>
#include <string>
#include <utility>
#include <vector>

// Flyweight: shared and immutable.
struct TreeType {
    std::string name;
    std::string color;
    std::string texture;
};

class TreeTypeFactory {
public:
    std::shared_ptr<const TreeType> get(const std::string& name, const std::string& color,
                                        const std::string& texture) {
        std::string key = name + "/" + color + "/" + texture;
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;
        }
        std::println("factory: creating new type {}", key);
        auto type = std::make_shared<const TreeType>(TreeType{name, color, texture});
        cache_.emplace(key, type);
        return type;
    }

    std::size_t typeCount() const { return cache_.size(); }

private:
    std::map<std::string, std::shared_ptr<const TreeType>> cache_;
};

// Context: holds the individual part and a pointer to the shared part.
class Tree {
public:
    Tree(int x, int y, std::shared_ptr<const TreeType> type) : x_(x), y_(y), type_(std::move(type)) {}

    void draw() const {
        std::println("  {} ({}, {}) at ({}, {})", type_->name, type_->color, type_->texture, x_, y_);
    }

private:
    int x_;
    int y_;
    std::shared_ptr<const TreeType> type_;
};

class Forest {
public:
    void plant(int x, int y, const std::string& name, const std::string& color,
               const std::string& texture) {
        trees_.emplace_back(x, y, factory_.get(name, color, texture));
    }

    void draw() const {
        std::println("forest has {} trees but only {} tree types:", trees_.size(),
                     factory_.typeCount());
        for (const auto& tree : trees_) {
            tree.draw();
        }
    }

private:
    TreeTypeFactory factory_;
    std::vector<Tree> trees_;
};

int main() {
    Forest forest;
    forest.plant(1, 2, "oak", "green", "rough");
    forest.plant(5, 3, "oak", "green", "rough");
    forest.plant(9, 7, "birch", "white", "smooth");
    forest.plant(12, 1, "oak", "green", "rough");
    forest.plant(4, 8, "birch", "white", "smooth");
    forest.plant(7, 7, "oak", "autumn", "rough");

    forest.draw();
    return 0;
}
