/*
 * State machine: a turnstile described by a transition table
 *
 * Intent: instead of one class per state, keep the allowed transitions as data
 * (state, event -> next state, action). Adding a rule means adding one table
 * row.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <print>
#include <string_view>

enum class State { Locked, Unlocked };
enum class Event { Coin, Push };

constexpr std::string_view toString(State state) {
    switch (state) {
    case State::Locked:
        return "Locked";
    case State::Unlocked:
        return "Unlocked";
    }
    return "?";
}

constexpr std::string_view toString(Event event) {
    switch (event) {
    case Event::Coin:
        return "Coin";
    case Event::Push:
        return "Push";
    }
    return "?";
}

struct Transition {
    State from;
    Event event;
    State to;
    std::string_view action;
};

constexpr std::array<Transition, 4> kTable = {{
                                               {State::Locked, Event::Coin, State::Unlocked, "unlock the arm"},
                                               {State::Locked, Event::Push, State::Locked, "stay locked"},
                                               {State::Unlocked, Event::Coin, State::Unlocked, "keep the coin"},
                                               {State::Unlocked, Event::Push, State::Locked, "let one person pass, lock again"},
                                               }};

class Turnstile {
public:
    void handle(Event event) {
        for (const auto& row : kTable) {
            if (row.from == state_ && row.event == event) {
                std::println("{} + {} -> {}: {}", toString(state_), toString(event),
                             toString(row.to), row.action);
                state_ = row.to;
                return;
            }
        }
        std::println("{} + {}: no rule", toString(state_), toString(event));
    }

    State state() const { return state_; }

private:
    State state_ = State::Locked;
};

int main() {
    Turnstile turnstile;
    turnstile.handle(Event::Push);
    turnstile.handle(Event::Coin);
    turnstile.handle(Event::Coin);
    turnstile.handle(Event::Push);
    std::println("final state: {}", toString(turnstile.state()));
    return 0;
}
