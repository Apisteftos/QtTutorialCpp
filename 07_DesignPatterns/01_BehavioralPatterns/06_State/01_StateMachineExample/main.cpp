/*
 * State: a media player
 *
 * Intent: let an object change its behaviour when its internal state changes.
 * Each state is its own class; the player forwards every request to the
 * current state object, and the state decides what happens and which state
 * comes next.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <memory>
#include <print>
#include <string>
#include <utility>

class MediaPlayer;

class PlayerState {
public:
    virtual ~PlayerState() = default;
    virtual void play(MediaPlayer& player) = 0;
    virtual void pause(MediaPlayer& player) = 0;
    virtual void stop(MediaPlayer& player) = 0;
    virtual std::string name() const = 0;
};

class MediaPlayer {
public:
    MediaPlayer();

    void setState(std::unique_ptr<PlayerState> state) { state_ = std::move(state); }
    std::string stateName() const { return state_->name(); }

    void play() { state_->play(*this); }
    void pause() { state_->pause(*this); }
    void stop() { state_->stop(*this); }

private:
    std::unique_ptr<PlayerState> state_;
};

class StoppedState : public PlayerState {
public:
    void play(MediaPlayer& player) override;
    void pause(MediaPlayer&) override { std::println("  cannot pause: nothing is playing"); }
    void stop(MediaPlayer&) override { std::println("  already stopped"); }
    std::string name() const override { return "Stopped"; }
};

class PlayingState : public PlayerState {
public:
    void play(MediaPlayer&) override { std::println("  already playing"); }
    void pause(MediaPlayer& player) override;
    void stop(MediaPlayer& player) override;
    std::string name() const override { return "Playing"; }
};

class PausedState : public PlayerState {
public:
    void play(MediaPlayer& player) override;
    void pause(MediaPlayer&) override { std::println("  already paused"); }
    void stop(MediaPlayer& player) override;
    std::string name() const override { return "Paused"; }
};

MediaPlayer::MediaPlayer() : state_(std::make_unique<StoppedState>()) {}

// The state objects replace themselves; nothing touches a member afterwards.
void StoppedState::play(MediaPlayer& player) {
    std::println("  start playing");
    player.setState(std::make_unique<PlayingState>());
}
void PlayingState::pause(MediaPlayer& player) {
    std::println("  pause playback");
    player.setState(std::make_unique<PausedState>());
}
void PlayingState::stop(MediaPlayer& player) {
    std::println("  stop playback");
    player.setState(std::make_unique<StoppedState>());
}
void PausedState::play(MediaPlayer& player) {
    std::println("  resume playing");
    player.setState(std::make_unique<PlayingState>());
}
void PausedState::stop(MediaPlayer& player) {
    std::println("  stop from pause");
    player.setState(std::make_unique<StoppedState>());
}

int main() {
    MediaPlayer player;
    std::println("state: {}", player.stateName());

    player.pause();
    player.play();
    std::println("state: {}", player.stateName());

    player.play();
    player.pause();
    std::println("state: {}", player.stateName());

    player.play();
    player.stop();
    std::println("state: {}", player.stateName());
    return 0;
}
