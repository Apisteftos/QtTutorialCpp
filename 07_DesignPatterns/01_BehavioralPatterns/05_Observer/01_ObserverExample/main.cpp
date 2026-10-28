/*
 * Observer: a thermometer with several listeners
 *
 * Intent: when one object changes, all objects that registered with it are
 * told automatically, without the subject knowing their concrete types.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <string>
#include <utility>
#include <vector>

class TemperatureListener {
public:
    virtual ~TemperatureListener() = default;
    virtual void onTemperature(double celsius) = 0;
};

// Subject: keeps a list of listeners and notifies them.
class Thermometer {
public:
    void attach(TemperatureListener* listener) { listeners_.push_back(listener); }
    void detach(TemperatureListener* listener) { std::erase(listeners_, listener); }

    void measure(double celsius) {
        std::println("thermometer: {} C", celsius);
        for (auto* listener : listeners_) {
            listener->onTemperature(celsius);
        }
    }

private:
    std::vector<TemperatureListener*> listeners_;
};

class DisplayPanel : public TemperatureListener {
public:
    explicit DisplayPanel(std::string room) : room_(std::move(room)) {}
    void onTemperature(double celsius) override {
        std::println("  display [{}]: {} C", room_, celsius);
    }

private:
    std::string room_;
};

class FrostAlarm : public TemperatureListener {
public:
    void onTemperature(double celsius) override {
        if (celsius <= 0.0) {
            std::println("  alarm: frost warning!");
        }
    }
};

int main() {
    Thermometer thermometer;
    DisplayPanel hall("hall");
    DisplayPanel garden("garden");
    FrostAlarm alarm;

    thermometer.attach(&hall);
    thermometer.attach(&garden);
    thermometer.attach(&alarm);

    thermometer.measure(12.5);
    thermometer.measure(-1.0);

    std::println("");
    std::println("garden display detached");
    thermometer.detach(&garden);
    thermometer.measure(3.0);
    return 0;
}
