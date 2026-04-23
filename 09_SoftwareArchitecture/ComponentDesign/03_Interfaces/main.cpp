// =============================================================================
// Interfaces — Program to Abstractions — C++23
//
// Example: Audio codec system — MCX/MCPTT uses codec negotiation where the
// audio engine must work with Opus, AMR, EVRC interchangeably.
//
// Demonstrates:
//   1. Pure interface (IAudioCodec) — the contract
//   2. Multiple concrete implementations (Opus, AMR, Mock)
//   3. Interface Segregation — IWorkable vs IFeedable split
//   4. Runtime polymorphism via interface reference
//   5. Testing with a mock that implements the same interface
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <numeric>

// =============================================================================
// 1. Pure Interface — IAudioCodec
// Defines the contract. No data. No implementation. Only pure virtual methods.
// =============================================================================

using Bytes = std::vector<uint8_t>;
using PCM   = std::vector<int16_t>;

class IAudioCodec {
public:
    virtual ~IAudioCodec() = default;

    virtual Bytes       encode(const PCM& pcm)   const = 0;
    virtual PCM         decode(const Bytes& data) const = 0;
    virtual int         bitrate()                 const = 0;  // kbps
    virtual std::string name()                    const = 0;
};

// =============================================================================
// 2. Concrete implementations — all honour the same contract
// =============================================================================

// Opus codec — high quality, variable bitrate
class OpusCodec : public IAudioCodec {
public:
    explicit OpusCodec(int bitrate_kbps = 24) : bitrate_(bitrate_kbps) {}

    Bytes encode(const PCM& pcm) const override {
        // Simplified: just mark bytes with codec ID and compress ratio
        Bytes out;
        out.push_back(0x4F);  // 'O' — Opus marker
        for (size_t i = 0; i < pcm.size(); i += 2) {
            out.push_back(static_cast<uint8_t>(pcm[i] & 0xFF));
        }
        return out;
    }

    PCM decode(const Bytes& data) const override {
        PCM out;
        for (size_t i = 1; i < data.size(); i++) {
            out.push_back(static_cast<int16_t>(data[i]));
            out.push_back(0);  // simplified — restore dropped sample
        }
        return out;
    }

    int         bitrate() const override { return bitrate_; }
    std::string name()    const override { return "Opus"; }

private:
    int bitrate_;
};

// AMR codec — used in cellular/MCX narrowband
class AMRCodec : public IAudioCodec {
public:
    Bytes encode(const PCM& pcm) const override {
        Bytes out;
        out.push_back(0x41);  // 'A' — AMR marker
        for (size_t i = 0; i < pcm.size(); i += 4) {
            out.push_back(static_cast<uint8_t>(pcm[i] & 0xFF));
        }
        return out;
    }

    PCM decode(const Bytes& data) const override {
        PCM out;
        for (size_t i = 1; i < data.size(); i++) {
            out.push_back(static_cast<int16_t>(data[i]));
            out.push_back(0);
            out.push_back(0);
            out.push_back(0);
        }
        return out;
    }

    int         bitrate() const override { return 12; }
    std::string name()    const override { return "AMR"; }
};

// Mock codec — for unit testing AudioEngine without real codec logic
class MockCodec : public IAudioCodec {
public:
    Bytes encode(const PCM& pcm) const override {
        encode_calls_++;
        // Return a predictable fixed output for assertions
        return {0xFF, 0xFE, static_cast<uint8_t>(pcm.size())};
    }

    PCM decode(const Bytes& data) const override {
        decode_calls_++;
        return {1, 2, 3, 4};  // predictable fixed output
    }

    int         bitrate() const override { return 0; }
    std::string name()    const override { return "Mock"; }

    // Test introspection
    int encodeCalls() const { return encode_calls_; }
    int decodeCalls() const { return decode_calls_; }

private:
    mutable int encode_calls_{0};
    mutable int decode_calls_{0};
};

// =============================================================================
// 3. AudioEngine — knows ONLY about IAudioCodec, never about Opus/AMR/Mock
// =============================================================================

class AudioEngine {
public:
    // Constructor injection — receives interface reference
    explicit AudioEngine(IAudioCodec& codec) : codec_(codec) {}

    Bytes transmit(const PCM& pcm) {
        auto encoded = codec_.encode(pcm);
        tx_bytes_ += encoded.size();
        std::cout << "  [" << codec_.name() << "] encoded "
                  << pcm.size() << " samples → "
                  << encoded.size() << " bytes"
                  << " (" << codec_.bitrate() << " kbps)\n";
        return encoded;
    }

    PCM receive(const Bytes& data) {
        auto decoded = codec_.decode(data);
        rx_samples_ += decoded.size();
        return decoded;
    }

    int txBytes()   const { return tx_bytes_; }
    int rxSamples() const { return rx_samples_; }

private:
    IAudioCodec& codec_;    // interface only — never knows the concrete type
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
    void work()  override { std::cout << "  " << name_ << " is working\n"; worked_++; }
    void eat()   override { std::cout << "  " << name_ << " is eating\n";  }
    void sleep() override { std::cout << "  " << name_ << " is sleeping\n"; }
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
    void work() override { std::cout << "  Robot " << id_ << " is working\n"; worked_++; }
    std::string role() const override { return "Automation"; }
    int workedCount() const { return worked_; }
private:
    std::string id_;
    int worked_{0};
};

// Factory function — processes any IWorkable without knowing Human vs Robot
void runShift(std::vector<IWorkable*>& workers) {
    std::cout << "  Running shift (" << workers.size() << " workers):\n";
    for (auto* w : workers) w->work();
}

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Interfaces Demo ===\n\n";

    PCM pcm_frame(160, 1000);  // 160 samples, all value 1000

    // ── Scenario 1: Production with Opus ─────────────────────────────────────
    std::cout << "--- Scenario 1: Opus Codec ---\n";
    {
        OpusCodec   opus(24);
        AudioEngine engine(opus);

        auto encoded = engine.transmit(pcm_frame);
        auto decoded = engine.receive(encoded);

        assert(engine.txBytes()   > 0);
        assert(engine.rxSamples() > 0);
        std::cout << "  ✓ Opus scenario passed\n\n";
    }

    // ── Scenario 2: Same AudioEngine, different codec ─────────────────────────
    std::cout << "--- Scenario 2: AMR Codec ---\n";
    {
        AMRCodec    amr;
        AudioEngine engine(amr);   // identical code — just different codec injected

        auto encoded = engine.transmit(pcm_frame);
        engine.receive(encoded);
        std::cout << "  ✓ AMR scenario passed\n\n";
    }

    // ── Scenario 3: Unit test with Mock — no real codec logic ─────────────────
    std::cout << "--- Scenario 3: Mock Codec (Unit Test) ---\n";
    {
        MockCodec   mock;
        AudioEngine engine(mock);

        engine.transmit(pcm_frame);
        engine.transmit(pcm_frame);
        engine.receive({0xFF, 0xFE, 0x03});

        assert(mock.encodeCalls() == 2);
        assert(mock.decodeCalls() == 1);
        std::cout << "  ✓ Mock test passed (encode=" << mock.encodeCalls()
                  << " decode=" << mock.decodeCalls() << ")\n\n";
    }

    // ── Scenario 4: Interface Segregation ────────────────────────────────────
    std::cout << "--- Scenario 4: Interface Segregation ---\n";
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
        std::cout << "  ✓ Interface segregation passed\n";
        std::cout << "  ✓ Robot not forced to implement eat()/sleep()\n";
    }

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
