/*
 * Strategy: discount rules at a checkout
 *
 * Intent: put a family of algorithms behind one interface so the client can
 * swap them at run time.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <format>
#include <memory>
#include <numeric>
#include <print>
#include <string>
#include <utility>
#include <vector>

class DiscountStrategy {
public:
    virtual ~DiscountStrategy() = default;
    virtual double apply(double total) const = 0;
    virtual std::string label() const = 0;
};

class NoDiscount : public DiscountStrategy {
public:
    double apply(double total) const override { return total; }
    std::string label() const override { return "no discount"; }
};

class PercentDiscount : public DiscountStrategy {
public:
    explicit PercentDiscount(double percent) : percent_(percent) {}
    double apply(double total) const override { return total * (1.0 - percent_ / 100.0); }
    std::string label() const override { return std::format("{}% off", percent_); }

private:
    double percent_;
};

class FixedDiscount : public DiscountStrategy {
public:
    explicit FixedDiscount(double amount) : amount_(amount) {}
    double apply(double total) const override { return std::max(0.0, total - amount_); }
    std::string label() const override { return std::format("{} off", amount_); }

private:
    double amount_;
};

class Checkout {
public:
    explicit Checkout(std::unique_ptr<DiscountStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<DiscountStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    void pay(const std::vector<double>& prices) const {
        double sum = std::accumulate(prices.begin(), prices.end(), 0.0);
        std::println("items: {}  rule: {}  to pay: {}", sum, strategy_->label(),
                     strategy_->apply(sum));
    }

private:
    std::unique_ptr<DiscountStrategy> strategy_;
};

int main() {
    const std::vector<double> basket = {20.0, 30.0, 50.0};

    Checkout checkout(std::make_unique<NoDiscount>());
    checkout.pay(basket);

    checkout.setStrategy(std::make_unique<PercentDiscount>(10));
    checkout.pay(basket);

    checkout.setStrategy(std::make_unique<FixedDiscount>(15));
    checkout.pay(basket);

    checkout.setStrategy(std::make_unique<FixedDiscount>(500));
    checkout.pay(basket);
    return 0;
}
