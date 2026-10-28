// =============================================================================
// Reliability Patterns — C++23
//
// Three patterns that keep systems working under failure:
//
//   1. Retry with exponential backoff — transient failure recovery
//   2. Circuit Breaker — stop hammering a failing service
//   3. Fallback        — serve degraded response instead of error
// =============================================================================

#include <string>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <cassert>
#include <optional>
#include <atomic>
#include <print>

using Ms = std::chrono::milliseconds;

// =============================================================================
// Simulated flaky service
// =============================================================================

class FlakyService {
public:
    explicit FlakyService(int fail_first_n) : fail_count_(fail_first_n) {}

    std::string call(const std::string& request) {
        calls_++;
        if (remaining_fails_ > 0) {
            remaining_fails_--;
            throw std::runtime_error("Service unavailable (503)");
        }
        return "OK:" + request;
    }

    void reset()    { remaining_fails_ = fail_count_; }
    void setAlwaysDown() { remaining_fails_ = 99999; }
    void setAlwaysUp()   { remaining_fails_ = 0; }

    int totalCalls() const { return calls_; }

private:
    int fail_count_;
    int remaining_fails_{0};
    int calls_{0};
};

// =============================================================================
// 1. Retry with Exponential Backoff
// =============================================================================

struct RetryConfig {
    int max_attempts{3};
    Ms  initial_delay{10};   // ms — short for demo
    double backoff_multiplier{2.0};
    Ms  max_delay{500};
};

template<typename Fn>
auto retry(Fn&& fn, const RetryConfig& cfg = {})
    -> std::invoke_result_t<Fn>
{
    int  attempt = 0;
    Ms   delay   = cfg.initial_delay;

    while (true) {
        attempt++;
        try {
            return fn();
        } catch (const std::exception& e) {
            std::println("  [Retry] attempt {} failed: {}", attempt, e.what());
            if (attempt >= cfg.max_attempts)
                throw;  // re-throw after all attempts exhausted

            std::println("  [Retry] waiting {}ms...", delay.count());
            std::this_thread::sleep_for(delay);

            // Exponential backoff
            delay = Ms(static_cast<long long>(delay.count() * cfg.backoff_multiplier));
            if (delay > cfg.max_delay) delay = cfg.max_delay;
        }
    }
}

// =============================================================================
// 2. Circuit Breaker
// =============================================================================

enum class CircuitState { Closed, Open, HalfOpen };

std::string to_string(CircuitState s) {
    switch (s) {
    case CircuitState::Closed:   return "CLOSED";
    case CircuitState::Open:     return "OPEN";
    case CircuitState::HalfOpen: return "HALF-OPEN";
    }
    return "?";
}

struct CircuitBreakerConfig {
    int failure_threshold{3};      // open after this many consecutive failures
    Ms  open_timeout{Ms(100)};     // how long to stay open before trying again
};

class CircuitBreaker {
public:
    using Config = CircuitBreakerConfig;

    explicit CircuitBreaker(const std::string& name, Config cfg = Config{})
        : name_(name), cfg_(cfg) {}

    template<typename Fn>
    auto call(Fn&& fn) -> std::invoke_result_t<Fn> {
        switch (state_) {
        case CircuitState::Open:
            if (std::chrono::steady_clock::now() - open_since_ > cfg_.open_timeout) {
                state_ = CircuitState::HalfOpen;
                std::println("  [CB:{}] → HALF-OPEN (probing)", name_);
            } else {
                rejected_++;
                throw std::runtime_error("[CB:" + name_ + "] Circuit OPEN — fast fail");
            }
            break;
        default:
            break;
        }

        try {
            auto result = fn();
            onSuccess();
            return result;
        } catch (...) {
            onFailure();
            throw;
        }
    }

    CircuitState state()    const { return state_; }
    int failures()          const { return consecutive_failures_; }
    int rejected()          const { return rejected_; }

private:
    void onSuccess() {
        consecutive_failures_ = 0;
        if (state_ == CircuitState::HalfOpen) {
            state_ = CircuitState::Closed;
            std::println("  [CB:{}] → CLOSED (recovered)", name_);
        }
    }

    void onFailure() {
        consecutive_failures_++;
        if (state_ == CircuitState::HalfOpen || consecutive_failures_ >= cfg_.failure_threshold) {
            state_      = CircuitState::Open;
            open_since_ = std::chrono::steady_clock::now();
            std::println("  [CB:{}] → OPEN (failures={})", name_, consecutive_failures_);
        }
    }

    std::string  name_;
    Config       cfg_;
    CircuitState state_{CircuitState::Closed};
    int          consecutive_failures_{0};
    int          rejected_{0};
    std::chrono::steady_clock::time_point open_since_;
};

// =============================================================================
// 3. Fallback
// =============================================================================

template<typename T>
class WithFallback {
public:
    WithFallback(std::function<T()> primary,
                 std::function<T()> fallback)
        : primary_(std::move(primary))
        , fallback_(std::move(fallback)) {}

    T execute() {
        try {
            auto result = primary_();
            fallback_used_ = false;
            return result;
        } catch (const std::exception& e) {
            std::println("  [Fallback] primary failed: {} — using fallback", e.what());
            fallback_used_  = true;
            fallback_count_++;
            return fallback_();
        }
    }

    bool fallbackUsed()  const { return fallback_used_; }
    int  fallbackCount() const { return fallback_count_; }

private:
    std::function<T()> primary_;
    std::function<T()> fallback_;
    bool fallback_used_{false};
    int  fallback_count_{0};
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::println("=== Reliability Patterns Demo ===");
    std::println("");

    // ── 1. Retry — service fails first 2 calls, succeeds on 3rd ──────────────
    std::println("--- 1. Retry with Exponential Backoff ---");
    {
        FlakyService svc(2);  // fail first 2 calls
        svc.reset();

        RetryConfig cfg;
        cfg.max_attempts = 4;
        cfg.initial_delay = Ms(5);
        cfg.backoff_multiplier = 2.0;

        std::string result = retry([&]{ return svc.call("get-user-42"); }, cfg);

        std::println("  Result: {}", result);
        assert(result == "OK:get-user-42");
        assert(svc.totalCalls() == 3);  // 2 failures + 1 success
        std::println("  ✓ Succeeded after {} attempts", svc.totalCalls());
        std::println("");
    }

    // Retry exhausted — all attempts fail
    {
        FlakyService svc(10);  // always fails
        svc.reset();

        RetryConfig cfg;
        cfg.max_attempts = 3;
        cfg.initial_delay = Ms(5);

        bool threw = false;
        try {
            retry([&]{ return svc.call("get-config"); }, cfg);
        } catch (const std::exception& e) {
            threw = true;
            std::println("  Expected: all retries exhausted: {}", e.what());
        }
        assert(threw);
        assert(svc.totalCalls() == 3);
        std::println("  ✓ Threw after max attempts");
    }

    // ── 2. Circuit Breaker ────────────────────────────────────────────────────
    std::println("");
    std::println("--- 2. Circuit Breaker ---");
    {
        FlakyService svc(10);
        svc.reset();

        CircuitBreaker cb("PaymentService", {.failure_threshold = 3,
                                             .open_timeout = Ms(50)});

        // Cause 3 failures → circuit opens
        for (int i = 0; i < 3; i++) {
            try { cb.call([&]{ return svc.call("pay"); }); }
            catch (...) {}
        }
        assert(cb.state() == CircuitState::Open);
        std::println("  Circuit is OPEN after 3 failures");

        // Next call is rejected immediately (fast fail)
        try { cb.call([&]{ return svc.call("pay"); }); }
        catch (const std::exception& e) {
            std::println("  Fast fail: {}", e.what());
        }
        assert(cb.rejected() == 1);

        // Wait for timeout → half-open → service recovers → closed
        std::this_thread::sleep_for(Ms(60));
        svc.setAlwaysUp();

        std::string result = cb.call([&]{ return svc.call("pay"); });
        assert(cb.state() == CircuitState::Closed);
        assert(result == "OK:pay");
        std::println("  ✓ Circuit recovered and CLOSED");
    }

    // ── 3. Fallback ───────────────────────────────────────────────────────────
    std::println("");
    std::println("--- 3. Fallback ---");
    {
        FlakyService svc(0);

        // Scenario A: primary works
        svc.setAlwaysUp();
        WithFallback<std::string> wb(
            [&]{ return svc.call("get-recommendations"); },
            []{ return "cached:top-10-defaults"; }
            );

        auto r1 = wb.execute();
        assert(r1 == "OK:get-recommendations");
        assert(!wb.fallbackUsed());
        std::println("  Primary OK: {}", r1);

        // Scenario B: primary fails → fallback kicks in
        svc.setAlwaysDown();
        auto r2 = wb.execute();
        assert(r2 == "cached:top-10-defaults");
        assert(wb.fallbackUsed());
        assert(wb.fallbackCount() == 1);
        std::println("  Fallback:   {}", r2);
        std::println("  ✓ Fallback served degraded response");
    }

    std::println("");
    std::println("All assertions passed.");
    return 0;
}
