// =============================================================================
// Dependency Inversion Principle — C++23
//
// Example: Report generation system
//
// WITHOUT DIP: ReportGenerator directly creates PdfRenderer, HtmlRenderer.
//              Adding a new renderer = editing ReportGenerator.
//
// WITH DIP:    ReportGenerator depends on IRenderer (interface it owns).
//              Adding ExcelRenderer = new file only, zero changes to generator.
//
// Also demonstrates:
//   - Who owns the interface (the high-level module)
//   - DIP vs Dependency Injection (principle vs technique)
//   - The Open/Closed result: open for extension, closed for modification
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cassert>
#include <algorithm>

// =============================================================================
// ❌ BEFORE DIP — ReportGenerator hardcodes every renderer
// =============================================================================

namespace before_dip {

class PdfRenderer {
public:
    std::string render(const std::string& title,
                       const std::vector<std::string>& rows) const {
        std::ostringstream oss;
        oss << "%PDF-1.4 [" << title << "]\n";
        for (const auto& r : rows) oss << "  /Text (" << r << ")\n";
        return oss.str();
    }
};

class HtmlRenderer {
public:
    std::string render(const std::string& title,
                       const std::vector<std::string>& rows) const {
        std::ostringstream oss;
        oss << "<html><body><h1>" << title << "</h1><ul>\n";
        for (const auto& r : rows) oss << "  <li>" << r << "</li>\n";
        oss << "</ul></body></html>";
        return oss.str();
    }
};

// ReportGenerator must know about EVERY renderer — tight coupling
class ReportGenerator {
public:
    enum class Format { PDF, HTML };  // adding CSV = edit this enum AND the switch

    std::string generate(const std::string& title,
                         const std::vector<std::string>& data,
                         Format fmt) const {
        switch (fmt) {
        case Format::PDF:  { PdfRenderer  r; return r.render(title, data); }
        case Format::HTML: { HtmlRenderer r; return r.render(title, data); }
            // Adding CSV: must edit this class — violates Open/Closed Principle
        }
        return "";
    }
};

} // namespace before_dip

// =============================================================================
// ✅ AFTER DIP — ReportGenerator owns IRenderer, depends only on abstraction
// =============================================================================

namespace after_dip {

// ── IRenderer — defined in the HIGH-LEVEL package (business rules) ────────────
// NOT in the renderer package — this is the "inversion" of DIP
class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual std::string render(const std::string& title,
                               const std::vector<std::string>& rows) const = 0;
    virtual std::string formatName() const = 0;
};

// ── Low-level details — each implements the high-level contract ───────────────

class PdfRenderer : public IRenderer {
public:
    std::string render(const std::string& title,
                       const std::vector<std::string>& rows) const override {
        std::ostringstream oss;
        oss << "%PDF-1.4\n%Title: " << title << '\n';
        for (const auto& r : rows) oss << "/Text (" << r << ")\n";
        return oss.str();
    }
    std::string formatName() const override { return "PDF"; }
};

class HtmlRenderer : public IRenderer {
public:
    std::string render(const std::string& title,
                       const std::vector<std::string>& rows) const override {
        std::ostringstream oss;
        oss << "<html><body>\n<h1>" << title << "</h1>\n<ul>\n";
        for (const auto& r : rows) oss << "  <li>" << r << "</li>\n";
        oss << "</ul>\n</body></html>";
        return oss.str();
    }
    std::string formatName() const override { return "HTML"; }
};

class CsvRenderer : public IRenderer {
public:
    std::string render(const std::string& title,
                       const std::vector<std::string>& rows) const override {
        std::ostringstream oss;
        oss << "\"" << title << "\"\n";
        for (const auto& r : rows) oss << "\"" << r << "\"\n";
        return oss.str();
    }
    std::string formatName() const override { return "CSV"; }
};

// NEW renderer added — zero changes to ReportGenerator
class MarkdownRenderer : public IRenderer {
public:
    std::string render(const std::string& title,
                       const std::vector<std::string>& rows) const override {
        std::ostringstream oss;
        oss << "# " << title << "\n\n";
        for (const auto& r : rows) oss << "- " << r << '\n';
        return oss.str();
    }
    std::string formatName() const override { return "Markdown"; }
};

// Mock renderer — for testing ReportGenerator without any real format logic
class MockRenderer : public IRenderer {
public:
    std::string render(const std::string& title,
                       const std::vector<std::string>& rows) const override {
        render_calls_++;
        last_title_ = title;
        last_rows_  = rows;
        return "MOCK:" + title;
    }
    std::string formatName() const override { return "Mock"; }

    // Test introspection
    int         renderCalls() const { return render_calls_; }
    std::string lastTitle()   const { return last_title_; }
    int         lastRowCount() const { return static_cast<int>(last_rows_.size()); }

private:
    mutable int render_calls_{0};
    mutable std::string last_title_;
    mutable std::vector<std::string> last_rows_;
};

// ── ReportGenerator — HIGH-LEVEL module, depends only on IRenderer ────────────
// This class never changes when new renderers are added.

struct Report {
    std::string              title;
    std::vector<std::string> rows;
};

class ReportGenerator {
public:
    // Dependency Injection — receives IRenderer, does not create it
    explicit ReportGenerator(IRenderer& renderer) : renderer_(renderer) {}

    std::string generate(const Report& report) const {
        // Pre-processing: business logic lives here (sorting, filtering)
        auto sorted = report.rows;
        std::sort(sorted.begin(), sorted.end());

        std::cout << "  Generating " << renderer_.formatName()
                  << " report: \"" << report.title << "\""
                  << " (" << sorted.size() << " rows)\n";

        return renderer_.render(report.title, sorted);
    }

    // Batch generation — same generator, different renderers for each report
    std::vector<std::string> generateAll(
        const Report& report,
        std::vector<IRenderer*> renderers) const
    {
        std::vector<std::string> outputs;
        auto sorted = report.rows;
        std::sort(sorted.begin(), sorted.end());
        for (auto* r : renderers)
            outputs.push_back(r->render(report.title, sorted));
        return outputs;
    }

private:
    IRenderer& renderer_;   // interface owned by this high-level module
};

} // namespace after_dip

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Dependency Inversion Principle Demo ===\n\n";

    // ── BEFORE ───────────────────────────────────────────────────────────────
    std::cout << "--- BEFORE DIP ---\n";
    {
        before_dip::ReportGenerator gen;
        std::vector<std::string> data = {"Bellman-Ford", "Dijkstra", "BFS"};
        auto pdf  = gen.generate("Algorithms", data, before_dip::ReportGenerator::Format::PDF);
        auto html = gen.generate("Algorithms", data, before_dip::ReportGenerator::Format::HTML);
        std::cout << "  PDF  output: " << pdf.substr(0, 30)  << "...\n";
        std::cout << "  HTML output: " << html.substr(0, 30) << "...\n";
        std::cout << "  ⚠️  Adding CSV requires editing ReportGenerator\n\n";
    }

    // ── AFTER ────────────────────────────────────────────────────────────────
    std::cout << "--- AFTER DIP ---\n";

    after_dip::Report report {
        "Algorithm Performance Report",
        {"Floyd-Warshall O(V³)", "Dijkstra O((V+E)logV)",
         "Bellman-Ford O(VE)",   "BFS O(V+E)"}
    };

    // Each renderer injected independently
    std::cout << "\nScenario 1: Individual renders\n";
    {
        after_dip::PdfRenderer  pdf;
        after_dip::HtmlRenderer html;
        after_dip::CsvRenderer  csv;
        after_dip::MarkdownRenderer md;   // NEW — zero changes to ReportGenerator

        after_dip::ReportGenerator gen(pdf);   // inject PDF
        auto pdf_out = gen.generate(report);
        assert(pdf_out.find("%PDF") != std::string::npos);

        after_dip::ReportGenerator gen2(html);  // inject HTML — same generator class
        auto html_out = gen2.generate(report);
        assert(html_out.find("<html>") != std::string::npos);

        after_dip::ReportGenerator gen3(md);
        auto md_out = gen3.generate(report);
        assert(md_out.find("# Algorithm") != std::string::npos);

        std::cout << "  ✓ PDF, HTML, Markdown renders passed\n";
    }

    // Batch: generate same report in all formats at once
    std::cout << "\nScenario 2: Batch generation\n";
    {
        after_dip::PdfRenderer      pdf;
        after_dip::HtmlRenderer     html;
        after_dip::CsvRenderer      csv;
        after_dip::MarkdownRenderer md;
        after_dip::ReportGenerator  gen(pdf);  // primary renderer (required by ctor)

        std::vector<after_dip::IRenderer*> all = {&pdf, &html, &csv, &md};
        auto outputs = gen.generateAll(report, all);
        assert(outputs.size() == 4);
        std::cout << "  ✓ Batch generation: " << outputs.size() << " formats\n";
    }

    // Unit test: MockRenderer verifies generator logic without real format
    std::cout << "\nScenario 3: Unit Test with Mock\n";
    {
        after_dip::MockRenderer    mock;
        after_dip::ReportGenerator gen(mock);

        gen.generate(report);
        gen.generate({"Empty", {}});

        assert(mock.renderCalls()  == 2);
        assert(mock.lastTitle()    == "Empty");
        assert(mock.lastRowCount() == 0);
        std::cout << "  ✓ Mock test: renderCalls=" << mock.renderCalls() << "\n";
    }

    std::cout << "\nKey difference:\n";
    std::cout << "  BEFORE — add CSV → edit ReportGenerator + enum + switch\n";
    std::cout << "  AFTER  — add Markdown → new file only, zero edits to generator\n";
    std::cout << "\nAll assertions passed.\n";
    return 0;
}
