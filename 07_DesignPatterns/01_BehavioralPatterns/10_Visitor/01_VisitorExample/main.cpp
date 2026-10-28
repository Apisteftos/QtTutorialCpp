/*
 * Visitor: operations on a file-system tree
 *
 * Intent: add new operations to a class hierarchy without changing the classes.
 * Each element only has an accept() method; every new operation is a new
 * visitor class (double dispatch).
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <cstddef>
#include <memory>
#include <print>
#include <string>
#include <utility>
#include <vector>

class FileEntry;
class FolderEntry;

class EntryVisitor {
public:
    virtual ~EntryVisitor() = default;
    virtual void visit(const FileEntry& file) = 0;
    virtual void visit(const FolderEntry& folder) = 0;
};

class Entry {
public:
    explicit Entry(std::string name) : name_(std::move(name)) {}
    virtual ~Entry() = default;
    virtual void accept(EntryVisitor& visitor) const = 0;
    const std::string& name() const { return name_; }

private:
    std::string name_;
};

class FileEntry : public Entry {
public:
    FileEntry(std::string name, std::size_t bytes) : Entry(std::move(name)), bytes_(bytes) {}
    void accept(EntryVisitor& visitor) const override { visitor.visit(*this); }
    std::size_t bytes() const { return bytes_; }

private:
    std::size_t bytes_;
};

class FolderEntry : public Entry {
public:
    using Entry::Entry;
    void add(std::unique_ptr<Entry> child) { children_.push_back(std::move(child)); }
    void accept(EntryVisitor& visitor) const override { visitor.visit(*this); }
    const std::vector<std::unique_ptr<Entry>>& children() const { return children_; }

private:
    std::vector<std::unique_ptr<Entry>> children_;
};

// Operation 1: add up all file sizes.
class SizeVisitor : public EntryVisitor {
public:
    void visit(const FileEntry& file) override { total_ += file.bytes(); }
    void visit(const FolderEntry& folder) override {
        for (const auto& child : folder.children()) {
            child->accept(*this);
        }
    }
    std::size_t total() const { return total_; }

private:
    std::size_t total_ = 0;
};

// Operation 2: print the tree with indentation.
class PrintVisitor : public EntryVisitor {
public:
    void visit(const FileEntry& file) override {
        std::println("{}{} ({} bytes)", indent(), file.name(), file.bytes());
    }
    void visit(const FolderEntry& folder) override {
        std::println("{}{}/", indent(), folder.name());
        ++depth_;
        for (const auto& child : folder.children()) {
            child->accept(*this);
        }
        --depth_;
    }

private:
    std::string indent() const { return std::string(depth_ * 2, ' '); }
    std::size_t depth_ = 0;
};

int main() {
    auto root = std::make_unique<FolderEntry>("project");
    root->add(std::make_unique<FileEntry>("main.cpp", 1200));

    auto docs = std::make_unique<FolderEntry>("docs");
    docs->add(std::make_unique<FileEntry>("readme.md", 800));
    docs->add(std::make_unique<FileEntry>("notes.txt", 150));
    root->add(std::move(docs));

    PrintVisitor printer;
    root->accept(printer);

    SizeVisitor sizer;
    root->accept(sizer);
    std::println("total size: {} bytes", sizer.total());
    return 0;
}
