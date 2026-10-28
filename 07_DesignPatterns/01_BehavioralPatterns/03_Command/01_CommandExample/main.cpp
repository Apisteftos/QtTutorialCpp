/*
 * Command: a remote button that triggers speaker actions
 *
 * Intent: turn a request into an object so it can be stored, passed around and
 * executed later by code that knows nothing about the receiver.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <memory>
#include <print>
#include <string>
#include <utility>
#include <vector>

// Receiver: the object that really does the work.
class Speaker {
public:
    void mute() { std::println("  speaker: muted"); }
    void setVolume(int level) { std::println("  speaker: volume {}", level); }
    void enqueue(const std::string& track) { std::println("  speaker: queued '{}'", track); }
};

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

// A simple command: one call on the receiver.
class MuteCommand : public Command {
public:
    explicit MuteCommand(Speaker& speaker) : speaker_(speaker) {}
    void execute() override { speaker_.mute(); }

private:
    Speaker& speaker_;
};

// A complex command: several calls, with data given at creation time.
class PartyModeCommand : public Command {
public:
    PartyModeCommand(Speaker& speaker, int volume, std::vector<std::string> tracks)
        : speaker_(speaker), volume_(volume), tracks_(std::move(tracks)) {}

    void execute() override {
        speaker_.setVolume(volume_);
        for (const auto& track : tracks_) {
            speaker_.enqueue(track);
        }
    }

private:
    Speaker& speaker_;
    int volume_;
    std::vector<std::string> tracks_;
};

// Invoker: only knows the Command interface.
class RemoteButton {
public:
    void assign(std::unique_ptr<Command> command) { command_ = std::move(command); }

    void press() {
        if (!command_) {
            std::println("button: nothing assigned");
            return;
        }
        std::println("button: pressed");
        command_->execute();
    }

private:
    std::unique_ptr<Command> command_;
};

int main() {
    Speaker speaker;
    RemoteButton button;

    button.press();

    button.assign(std::make_unique<MuteCommand>(speaker));
    button.press();

    button.assign(std::make_unique<PartyModeCommand>(
        speaker, 8, std::vector<std::string>{"Intro", "Dance floor"}));
    button.press();
    return 0;
}
