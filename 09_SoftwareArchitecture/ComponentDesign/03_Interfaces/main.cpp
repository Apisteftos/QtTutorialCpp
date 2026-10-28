// =============================================================================
// Interfaces — Program to Abstractions — C++23
//
// Example: Data compression — a data link must work with different compressors
// (keep every 2nd sample, keep every 4th sample, a test mock) interchangeably.
//
// Demonstrates:
//   1. Pure interface (ICompressor) — the contract
//   2. Multiple concrete implementations (Half, Quarter, Mock)
//   3. Interface Segregation — IWorkable vs IFeedable split
//   4. Runtime polymorphism via interface reference
//   5. Testing with a mock that implements the same interface
// =============================================================================

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <print>

// =============================================================================
// 1. Pure Interface — ICompressor
// Defines the contract. No data. No implementation. Only pure virtual methods.
// =============================================================================

using Bytes = std::vector<uint8_t>;
using Samples = std::vector<int16_t>;

class ICompressor {
public:
    virtual ~ICompressor() = default;

    virtual Bytes       encode(const Samples& samples)   const = 0;
    virtual Samples         decode(const Bytes& data) const = 0;
    virtual int         keptPercent()             const = 0;  // % of samples kept
    virtual std::string name()                    const = 0;
};

// =============================================================================
// 2. Concrete implementations — all honour the same contract
// =============================================================================

// Half compressor — keeps every 2nd sample
class HalfCompressor : public ICompressor {
public:
    explicit HalfCompressor(int kept_percent = 50) : kept_percent_(kept_percent) {}

    Bytes encode(const Samples& samples) const override {
        // Simplified: mark the bytes with a compressor ID and drop samples
        Bytes out;
        out.push_back(0x48);  // 'H' — Half marker
        for (size_t i = 0; i < samples.size(); i += 2) {
            out.push_back(static_cast<uint8_t>(samples[i] & 0xFF));
        }
        return out;
    }

    Samples decode(const Bytes& data) const override {
        Samples out;
        for (size_t i = 1; i < data.size(); i++) {
            out.push_back(static_cast<int16_t>(data[i]));
            out.push_back(0);  // simplified — restore dropped sample
        }
        return out;
    }

    int         keptPercent() const override { return kept_percent_; }
    std::string name()    const override { return "Half"; }

private:
    int kept_percent_;
};

// Quarter compressor — keeps every 4th sample (smaller output, lower quality)
class QuarterCompressor : public ICompressor {
public:
    Bytes encode(const Samples& samples) const override {
        Bytes out;
        out.push_back(0x51);  // 'Q' — Quarter marker
        for (size_t i = 0; i < samples.size(); i += 4) {
            out.push_back(static_cast<uint8_t>(samples[i] & 0xFF));
        }
        return out;
    }

    Samples decode(const Bytes& data) const override {
        Samples out;
        for (size_t i = 1; i < data.size(); i++) {
            out.push_back(static_cast<int16_t>(data[i]));
            out.push_back(0);
            out.push_back(0);
            out.push_back(0);
        }
        return out;
    }

    int         keptPercent() const override { return 25; }
    std::string name()    const override { return "Quarter"; }
};

// Mock compressor — for unit testing DataLink without real compression logic
class MockCompressor : public ICompressor {
public:
    Bytes encode(const Samples& samples) const override {
        encode_calls_++;
        // Return a predictable fixed output for assertions
        return {0xFF, 0xFE, static_cast<uint8_t>(samples.size())};
    }

    Samples decode(const Bytes& data) const override {
        decode_calls_++;
        return {1, 2, 3, 4};  // predictable fixed output
    }

    int         keptPercent() const override { return 0; }
    std::string name()    const override { return "Mock"; }

    // Test introspection
    int encodeCalls() const { return encode_calls_; }
    int decodeCalls() const { return decode_calls_; }

private:
    mutable int encode_calls_{0};
    mutable int decode_calls_{0};
};

// =============================================================================
// 3. DataLink — knows ONLY about ICompressor, never about Half/Quarter/Mock
// =============================================================================

class DataLink {
public:
    // Constructor injection — receives interface reference
    explicit DataLink(ICompressor& compressor) : compressor_(compressor) {}

    Bytes transmit(const Samples& samples) {
        auto encoded = compressor_.encode(samples);
        tx_bytes_ += encoded.size();
        std::println("  [{}] encoded {} samples → {} bytes ({}% kept)",
            compressor_.name(), samples.size(), encoded.size(), compressor_.keptPercent());
        return encoded;
    }

    Samples receive(const Bytes& data) {
        auto decoded = compressor_.decode(data);
        rx_samples_ += decoded.size();
        return decoded;
    }

    int txBytes()   const { return tx_bytes_; }
    int rxSamples() const { return rx_samples_; }

private:
    ICompressor& compressor_;    // interface only — never knows the concrete type
    int tx_bytes_{0};
    int rx_samples_{0};
};

// =============================================================================
// 4. Interface Segregation — IWorkable / IFeedable example
// =============================================================================

class IWorkable {
public:
    virtual ~IWorkable() = default;
    virtual void work() = 0;
    virtual std::string role() const = 0;
};

class IFeedable {
public:
    virtual ~IFeedable() = default;
    virtual void eat()   = 0;
    virtual void sleep() = 0;
};

// Human implements BOTH — has all needs
class Human : public IWorkable, public IFeedable {
public:
    explicit Human(const std::string& name) : name_(name) {}
    void work()  override { std::println("  {} is working", name_); worked_++; }
    void eat()   override { std::println("  {} is eating", name_);  }
    void sleep() override { std::println("  {} is sleeping", name_); }
    std::string role() const override { return "Engineer"; }
    int workedCount() const { return worked_; }
private:
    std::string name_;
    int worked_{0};
};

// Robot implements ONLY IWorkable — robots don't eat or sleep
class Robot : public IWorkable {
public:
    explicit Robot(const std::string& id) : id_(id) {}
    void work() override { std::println("  Robot {} is working", id_); worked_++; }
    std::string role() const override { return "Automation"; }
    int workedCount() const { return worked_; }
private:
    std::string id_;
    int worked_{0};
};

// Factory function — processes any IWorkable without knowing Human vs Robot
void runShift(std::vector<IWorkable*>& workers) {
    std::println("  Running shift ({} workers):", workers.size());
    for (auto* w : workers) w->work();
}

// =============================================================================
// main
// =============================================================================

int main()
{
    std::println("=== Interfaces Demo ===");
    std::println("");

    Samples sample_frame(160, 1000);  // 160 samples, all value 1000

    // ── Scenario 1: Production with the Half compressor ─────────────────────────────────────
    std::println("--- Scenario 1: Half Compressor ---");
    {
        HalfCompressor   half(50);
        DataLink engine(half);

        auto encoded = engine.transmit(sample_frame);
        auto decoded = engine.receive(encoded);

        assert(engine.txBytes()   > 0);
        assert(engine.rxSamples() > 0);
        std::println("  ✓ Half-compressor scenario passed");
        std::println("");
    }

    // ── Scenario 2: Same DataLink, different compressor ─────────────────────────
    std::println("--- Scenario 2: Quarter Compressor ---");
    {
        QuarterCompressor quarter;
        DataLink engine(quarter);   // identical code — just a different compressor injected

        auto encoded = engine.transmit(sample_frame);
        engine.receive(encoded);
        std::println("  ✓ Quarter-compressor scenario passed");
        std::println("");
    }

    // ── Scenario 3: Unit test with Mock — no real compression logic ─────────────────
    std::println("--- Scenario 3: Mock Compressor (Unit Test) ---");
    {
        MockCompressor mock;
        DataLink engine(mock);

        engine.transmit(sample_frame);
        engine.transmit(sample_frame);
        engine.receive({0xFF, 0xFE, 0x03});

        assert(mock.encodeCalls() == 2);
        assert(mock.decodeCalls() == 1);
        std::println("  ✓ Mock test passed (encode={} decode={})", mock.encodeCalls(), mock.decodeCalls());
        std::println("");
    }

    // ── Scenario 4: Interface Segregation ────────────────────────────────────
    std::println("--- Scenario 4: Interface Segregation ---");
    {
        Human kostas("Kostas");
        Robot r2d2("R2D2");

        // Both are IWorkable — processable in the same loop
        std::vector<IWorkable*> workers = {&kostas, &r2d2};
        runShift(workers);

        // Only Human is IFeedable — Robot excluded at compile time
        IFeedable& feedable = kostas;
        feedable.eat();

        assert(kostas.workedCount() == 1);
        assert(r2d2.workedCount()   == 1);
        std::println("  ✓ Interface segregation passed");
        std::println("  ✓ Robot not forced to implement eat()/sleep()");
    }

    std::println("");
    std::println("All assertions passed.");
    return 0;
}
