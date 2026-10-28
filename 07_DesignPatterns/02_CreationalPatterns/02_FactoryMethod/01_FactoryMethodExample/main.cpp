/*
 * Factory Method: notifications through different channels
 *
 * Intent: the base class defines the workflow and calls a virtual "factory
 * method" to get the object it needs; subclasses decide which concrete class
 * is created.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <memory>
#include <print>
#include <string>
#include <vector>

// Product
class Channel {
public:
    virtual ~Channel() = default;
    virtual void deliver(const std::string& text) const = 0;
    virtual std::string name() const = 0;
};

class EmailChannel : public Channel {
public:
    void deliver(const std::string& text) const override {
        std::println("  [email] subject line: {}", text);
    }
    std::string name() const override { return "email"; }
};

class SmsChannel : public Channel {
public:
    void deliver(const std::string& text) const override {
        std::println("  [sms] {}", text.substr(0, 20));
    }
    std::string name() const override { return "sms"; }
};

class PushChannel : public Channel {
public:
    void deliver(const std::string& text) const override {
        std::println("  [push] {}", text);
    }
    std::string name() const override { return "push"; }
};

// Creator
class Notifier {
public:
    virtual ~Notifier() = default;

    void notify(const std::string& text) const {
        auto channel = createChannel();  // the factory method
        std::println("sending via {}", channel->name());
        channel->deliver(text);
    }

protected:
    virtual std::unique_ptr<Channel> createChannel() const = 0;
};

class EmailNotifier : public Notifier {
protected:
    std::unique_ptr<Channel> createChannel() const override {
        return std::make_unique<EmailChannel>();
    }
};

class SmsNotifier : public Notifier {
protected:
    std::unique_ptr<Channel> createChannel() const override {
        return std::make_unique<SmsChannel>();
    }
};

class PushNotifier : public Notifier {
protected:
    std::unique_ptr<Channel> createChannel() const override {
        return std::make_unique<PushChannel>();
    }
};

// Client code works with the Notifier interface only.
void alert(const Notifier& notifier) {
    notifier.notify("Your parcel arrives tomorrow morning");
}

int main() {
    std::vector<std::unique_ptr<Notifier>> notifiers;
    notifiers.push_back(std::make_unique<EmailNotifier>());
    notifiers.push_back(std::make_unique<SmsNotifier>());
    notifiers.push_back(std::make_unique<PushNotifier>());

    for (const auto& notifier : notifiers) {
        alert(*notifier);
    }
    return 0;
}
