/*
 * Chat Application (in-process)
 *
 * A small chat that combines four architecture patterns:
 *   - Layered       : Network (transport) / Chat (logic) / Ui (rendering)
 *   - Client-Server : clients only talk to the server, never to each other
 *   - Pub-Sub       : a message sent to a channel reaches its subscribers
 *   - Event-Driven  : every packet is an event that is handled from a queue
 *
 * The "network" is an in-memory queue, so the example needs no sockets and no Qt.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o chat
 */

#include <deque>
#include <functional>
#include <map>
#include <print>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ============================================================================
// LAYER 1: Network - moves packets between numbered endpoints
// ============================================================================

namespace Network {

struct Packet {
    int from;
    int to;
    std::string text;
};

class Hub {
public:
    using Handler = std::function<void(const Packet&)>;

    int attach(Handler handler) {
        handlers_.push_back(std::move(handler));
        return static_cast<int>(handlers_.size()) - 1;
    }

    void send(int from, int to, std::string text) {
        queue_.push_back({from, to, std::move(text)});
    }

    // Event loop: handle packets until none are left.
    void run() {
        while (!queue_.empty()) {
            Packet packet = std::move(queue_.front());
            queue_.pop_front();
            handlers_[packet.to](packet);
        }
    }

private:
    std::vector<Handler> handlers_;
    std::deque<Packet> queue_;
};

}  // namespace Network

// ============================================================================
// LAYER 3: Ui - turns protocol text into something a user reads
// ============================================================================

namespace Ui {

// Splits "WORD rest of line" into {"WORD", "rest of line"}.
std::pair<std::string, std::string> split_word(std::string_view line) {
    auto space = line.find(' ');
    if (space == std::string_view::npos) {
        return {std::string(line), ""};
    }
    return {std::string(line.substr(0, space)), std::string(line.substr(space + 1))};
}

std::string render(std::string_view line) {
    auto [command, rest] = split_word(line);
    if (command == "MSG") {
        auto [channel, tail] = split_word(rest);
        auto [sender, text] = split_word(tail);
        return "#" + channel + " " + sender + ": " + text;
    }
    if (command == "INFO") {
        return "* " + rest;
    }
    return "! " + rest;
}

}  // namespace Ui

// ============================================================================
// LAYER 2: Chat - server (pub-sub broker) and client
// ============================================================================

namespace Chat {

class Server {
public:
    explicit Server(Network::Hub& hub) : hub_(hub) {
        id_ = hub_.attach([this](const Network::Packet& p) { on_packet(p); });
    }

    int id() const { return id_; }

private:
    void on_packet(const Network::Packet& packet) {
        auto [command, rest] = Ui::split_word(packet.text);

        if (command == "HELLO") {
            names_[packet.from] = rest;
            reply(packet.from, "INFO welcome " + rest);
        } else if (command == "JOIN") {
            channels_[rest].insert(packet.from);
            publish(rest, "INFO " + names_[packet.from] + " joined #" + rest, packet.from);
            reply(packet.from, "INFO you joined #" + rest);
        } else if (command == "LEAVE") {
            channels_[rest].erase(packet.from);
            publish(rest, "INFO " + names_[packet.from] + " left #" + rest, packet.from);
            reply(packet.from, "INFO you left #" + rest);
        } else if (command == "SAY") {
            auto [channel, text] = Ui::split_word(rest);
            if (!channels_[channel].contains(packet.from)) {
                reply(packet.from, "ERR you are not in #" + channel);
                return;
            }
            publish(channel, "MSG " + channel + " " + names_[packet.from] + " " + text,
                    packet.from);
        } else {
            reply(packet.from, "ERR unknown command " + command);
        }
    }

    void reply(int to, const std::string& text) { hub_.send(id_, to, text); }

    // Pub-Sub: deliver to every subscriber of the channel except the sender.
    void publish(const std::string& channel, const std::string& text, int except) {
        for (int subscriber : channels_[channel]) {
            if (subscriber != except) {
                reply(subscriber, text);
            }
        }
    }

    Network::Hub& hub_;
    int id_ = -1;
    std::map<int, std::string> names_;
    std::map<std::string, std::set<int>> channels_;
};

class Client {
public:
    Client(Network::Hub& hub, int server_id, std::string name)
        : hub_(hub), server_id_(server_id), name_(std::move(name)) {
        id_ = hub_.attach([this](const Network::Packet& p) {
            std::println("  [{}] {}", name_, Ui::render(p.text));
        });
        hub_.send(id_, server_id_, "HELLO " + name_);
    }

    void join(const std::string& channel) { hub_.send(id_, server_id_, "JOIN " + channel); }
    void leave(const std::string& channel) { hub_.send(id_, server_id_, "LEAVE " + channel); }
    void say(const std::string& channel, const std::string& text) {
        hub_.send(id_, server_id_, "SAY " + channel + " " + text);
    }

private:
    Network::Hub& hub_;
    int server_id_;
    std::string name_;
    int id_ = -1;
};

}  // namespace Chat

// ============================================================================
// MAIN - Demonstrate All Examples
// ============================================================================

int main() {
    Network::Hub hub;
    Chat::Server server(hub);
    Chat::Client alice(hub, server.id(), "alice");
    Chat::Client bob(hub, server.id(), "bob");
    Chat::Client carol(hub, server.id(), "carol");

    // === EXAMPLE 1: Connect and join a channel ===
    std::println("=== Example 1: Connect and join a channel ===");
    alice.join("general");
    bob.join("general");
    hub.run();

    // === EXAMPLE 2: Publish to a channel ===
    std::println("");
    std::println("=== Example 2: Publish to a channel ===");
    alice.say("general", "hello everyone");
    bob.say("general", "hi alice");
    hub.run();

    // === EXAMPLE 3: Only subscribers receive messages ===
    std::println("");
    std::println("=== Example 3: Only subscribers receive messages ===");
    carol.join("random");
    bob.join("random");
    alice.say("random", "can you hear me?");
    carol.say("random", "welcome to #random, bob");
    hub.run();

    // === EXAMPLE 4: Leave a channel ===
    std::println("");
    std::println("=== Example 4: Leave a channel ===");
    bob.leave("general");
    alice.say("general", "bob has gone");
    bob.say("general", "am I still here?");
    hub.run();

    return 0;
}
