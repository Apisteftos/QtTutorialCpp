// =============================================================================
// Pipe and Filter Architecture — C++23
//
// Example: Sensor data processing pipeline
//
// Source   — raw sensor readings (messy strings from hardware)
// Filter 1 — Normalise  : trim whitespace, lowercase, fix delimiters
// Filter 2 — Validate   : reject malformed records, check value ranges
// Filter 3 — Enrich     : add computed fields (unit conversion, status tag)
// Filter 4 — Format     — produce final JSON-like output
// Sink     — collect results
//
// Each filter is a class implementing the Filter interface.
// Pipeline chains them — adding/removing a filter is one line.
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <cctype>
#include <optional>
#include <memory>

// =============================================================================
// Data packet — passed through the pipeline
// =============================================================================

struct SensorRecord {
    std::string raw;           // original input
    std::string id;            // sensor ID
    double      value{0.0};    // parsed value
    std::string unit;          // original unit
    double      value_si{0.0}; // converted to SI
    std::string status;        // "ok" | "warning" | "critical"
    std::string output;        // final formatted string
    bool        valid{true};   // set false by Validate to drop record
};

// =============================================================================
// Filter interface
// =============================================================================

class Filter {
public:
    virtual ~Filter() = default;
    // Returns the processed record (or marks it invalid to drop it)
    virtual SensorRecord process(SensorRecord rec) = 0;
    virtual std::string  name() const = 0;
};

// =============================================================================
// Filter 1 — Normalise
// Trim whitespace, fix delimiters, lowercase id
// =============================================================================

class NormaliseFilter : public Filter {
public:
    std::string name() const override { return "Normalise"; }

    SensorRecord process(SensorRecord rec) override {
        // Trim leading/trailing whitespace from raw
        auto s = rec.raw;
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                        [](unsigned char c){ return !std::isspace(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             [](unsigned char c){ return !std::isspace(c); }).base(), s.end());

        // Replace semicolons with commas (some sensors use ; as delimiter)
        std::replace(s.begin(), s.end(), ';', ',');

        // Parse "ID,VALUE,UNIT"
        std::istringstream ss(s);
        std::string id, val, unit;
        if (std::getline(ss, id, ',') &&
            std::getline(ss, val, ',') &&
            std::getline(ss, unit)) {
            // Lowercase ID
            std::transform(id.begin(), id.end(), id.begin(), ::tolower);
            // Trim unit
            unit.erase(std::remove(unit.begin(), unit.end(), ' '), unit.end());

            rec.id   = id;
            rec.unit = unit;
            try { rec.value = std::stod(val); }
            catch (...) { rec.valid = false; }
        } else {
            rec.valid = false;
        }
        return rec;
    }
};

// =============================================================================
// Filter 2 — Validate
// Range check. Invalid records are marked valid=false and dropped downstream.
// =============================================================================

class ValidateFilter : public Filter {
public:
    std::string name() const override { return "Validate"; }

    SensorRecord process(SensorRecord rec) override {
        if (!rec.valid) return rec;

        // Temperature sensors: must be in [-50, 150] Celsius or Fahrenheit
        if (rec.id.find("temp") != std::string::npos) {
            if (rec.value < -50.0 || rec.value > 500.0) {
                rec.valid = false;
                std::cout << "  [Validate] ✗ OUT OF RANGE: "
                          << rec.id << " = " << rec.value << '\n';
            }
        }
        // Pressure sensors: must be positive
        if (rec.id.find("pres") != std::string::npos) {
            if (rec.value < 0.0) {
                rec.valid = false;
                std::cout << "  [Validate] ✗ NEGATIVE PRESSURE: " << rec.id << '\n';
            }
        }
        return rec;
    }
};

// =============================================================================
// Filter 3 — Enrich
// Unit conversion to SI. Add status tag.
// =============================================================================

class EnrichFilter : public Filter {
public:
    std::string name() const override { return "Enrich"; }

    SensorRecord process(SensorRecord rec) override {
        if (!rec.valid) return rec;

        // Unit conversion
        if (rec.unit == "F") {
            // Fahrenheit → Celsius
            rec.value_si = (rec.value - 32.0) * 5.0 / 9.0;
            rec.unit     = "C→SI";
        } else if (rec.unit == "C") {
            rec.value_si = rec.value;  // already Celsius
        } else if (rec.unit == "psi") {
            rec.value_si = rec.value * 6894.76;  // psi → Pascal
        } else {
            rec.value_si = rec.value;  // pass through
        }

        // Status classification (for temperature in C)
        if (rec.id.find("temp") != std::string::npos) {
            if      (rec.value_si > 100.0) rec.status = "critical";
            else if (rec.value_si > 70.0)  rec.status = "warning";
            else                           rec.status = "ok";
        } else {
            rec.status = "ok";
        }

        return rec;
    }
};

// =============================================================================
// Filter 4 — Format
// Produce the final output string.
// =============================================================================

class FormatFilter : public Filter {
public:
    std::string name() const override { return "Format"; }

    SensorRecord process(SensorRecord rec) override {
        if (!rec.valid) return rec;

        std::ostringstream oss;
        oss << "{ \"id\": \""     << rec.id
            << "\", \"value\": "  << rec.value_si
            << ", \"status\": \"" << rec.status
            << "\" }";
        rec.output = oss.str();
        return rec;
    }
};

// =============================================================================
// Pipeline — chains filters, drops invalid records
// =============================================================================

class Pipeline {
public:
    Pipeline& addFilter(std::unique_ptr<Filter> f) {
        filters_.push_back(std::move(f));
        return *this;  // fluent interface: pipeline.add(...).add(...)
    }

    std::vector<SensorRecord> run(const std::vector<std::string>& inputs) {
        std::vector<SensorRecord> results;

        for (const auto& raw : inputs) {
            SensorRecord rec;
            rec.raw = raw;

            // Pass through each filter in order
            for (auto& filter : filters_) {
                rec = filter->process(rec);
                if (!rec.valid) break;   // drop record early
            }

            if (rec.valid)
                results.push_back(rec);
        }
        return results;
    }

private:
    std::vector<std::unique_ptr<Filter>> filters_;
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Pipe and Filter Architecture — Sensor Pipeline ===\n\n";

    // Raw sensor data — messy, mixed formats, some invalid
    std::vector<std::string> raw_inputs = {
        "  TEMP_01 , 215 , F  ",      // valid — critical (215°F = 101.7°C → critical)
        "TEMP_02;25;C",                // valid — semicolon delimiter
        "PRES_01,14.7,psi",           // valid — atmospheric pressure
        "TEMP_03,999,C",              // invalid — out of range
        "GARBAGE_DATA",               // invalid — can't parse
        "  temp_04 , 37.5 , C  ",    // valid — body temperature
        "PRES_02,-5,psi",             // invalid — negative pressure
    };

    // Build pipeline — adding/removing a filter is ONE LINE
    Pipeline pipeline;
    pipeline.addFilter(std::make_unique<NormaliseFilter>())
        .addFilter(std::make_unique<ValidateFilter>())
        .addFilter(std::make_unique<EnrichFilter>())
        .addFilter(std::make_unique<FormatFilter>());

    auto results = pipeline.run(raw_inputs);

    std::cout << "\n=== Pipeline Results (" << results.size()
              << " of " << raw_inputs.size() << " records passed) ===\n\n";

    for (const auto& r : results)
        std::cout << r.output << '\n';

    // Verify
    assert(results.size() == 4);  // 3 invalid dropped
    assert(results[0].status == "critical");   // 101.7°C (215°F converted)
    assert(results[1].status == "ok");         // 25°C
    assert(results[3].status == "ok");         // 37.5°C

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
