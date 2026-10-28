/*
 * Prototype: a sticker book
 *
 * Intent: create new objects by copying existing ones. The client asks for a
 * copy of a registered template and never needs to know its concrete class.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <map>
#include <memory>
#include <print>
#include <string>
#include <utility>

class Sticker {
public:
    virtual ~Sticker() = default;
    virtual std::unique_ptr<Sticker> clone() const = 0;
    virtual void show() const = 0;

    void moveTo(int x, int y) {
        x_ = x;
        y_ = y;
    }

protected:
    int x_ = 0;
    int y_ = 0;
};

class TextSticker : public Sticker {
public:
    explicit TextSticker(std::string text) : text_(std::move(text)) {}
    std::unique_ptr<Sticker> clone() const override { return std::make_unique<TextSticker>(*this); }
    void show() const override { std::println("text '{}' at ({}, {})", text_, x_, y_); }

private:
    std::string text_;
};

class ImageSticker : public Sticker {
public:
    ImageSticker(std::string file, double scale) : file_(std::move(file)), scale_(scale) {}
    std::unique_ptr<Sticker> clone() const override { return std::make_unique<ImageSticker>(*this); }
    void show() const override {
        std::println("image {} x{} at ({}, {})", file_, scale_, x_, y_);
    }

private:
    std::string file_;
    double scale_;
};

// Registry of ready-made templates.
class StickerBook {
public:
    void add(const std::string& name, std::unique_ptr<Sticker> prototype) {
        templates_[name] = std::move(prototype);
    }

    std::unique_ptr<Sticker> create(const std::string& name) const {
        auto it = templates_.find(name);
        if (it == templates_.end()) {
            return nullptr;
        }
        return it->second->clone();
    }

private:
    std::map<std::string, std::unique_ptr<Sticker>> templates_;
};

int main() {
    StickerBook book;
    book.add("greeting", std::make_unique<TextSticker>("Hello!"));
    book.add("star", std::make_unique<ImageSticker>("star.png", 1.5));

    auto first = book.create("greeting");
    first->moveTo(10, 20);

    auto second = book.create("greeting");
    second->moveTo(200, 40);

    auto star = book.create("star");
    star->moveTo(5, 5);

    first->show();
    second->show();
    star->show();

    if (!book.create("unknown")) {
        std::println("no template called 'unknown'");
    }
    return 0;
}
