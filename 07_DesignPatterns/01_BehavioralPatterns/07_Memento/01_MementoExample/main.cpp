/*
 * Memento: undo for a whiteboard
 *
 * Intent: save and restore an object's state without exposing its internals.
 * Only the Whiteboard can create or read a Snapshot; the History just stores
 * them.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <cstddef>
#include <print>
#include <string>
#include <utility>
#include <vector>

// Originator
class Whiteboard {
public:
    // Memento: its content is private, only Whiteboard (a friend) can use it.
    class Snapshot {
    public:
        std::size_t lineCount() const { return lines_.size(); }

    private:
        friend class Whiteboard;
        explicit Snapshot(std::vector<std::string> lines) : lines_(std::move(lines)) {}
        std::vector<std::string> lines_;
    };

    void write(const std::string& line) { lines_.push_back(line); }

    Snapshot save() const { return Snapshot(lines_); }
    void restore(const Snapshot& snapshot) { lines_ = snapshot.lines_; }

    void show() const {
        std::println("whiteboard ({} lines):", lines_.size());
        for (const auto& line : lines_) {
            std::println("  {}", line);
        }
    }

private:
    std::vector<std::string> lines_;
};

// Caretaker: stores snapshots, never looks inside them.
class History {
public:
    void remember(Whiteboard::Snapshot snapshot) { stack_.push_back(std::move(snapshot)); }

    bool undo(Whiteboard& board) {
        if (stack_.empty()) {
            std::println("history: nothing to undo");
            return false;
        }
        board.restore(stack_.back());
        std::println("history: restored snapshot with {} lines", stack_.back().lineCount());
        stack_.pop_back();
        return true;
    }

private:
    std::vector<Whiteboard::Snapshot> stack_;
};

int main() {
    Whiteboard board;
    History history;

    history.remember(board.save());
    board.write("Agenda");

    history.remember(board.save());
    board.write("1. Budget");
    board.write("2. Hiring");

    board.show();

    history.undo(board);
    board.show();

    history.undo(board);
    board.show();

    history.undo(board);
    return 0;
}
