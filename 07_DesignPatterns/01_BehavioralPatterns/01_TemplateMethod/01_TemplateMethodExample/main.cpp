/*
 * Template Method: report generator
 *
 * Intent: define the skeleton of an algorithm in a base class and let
 * subclasses fill in single steps without changing the order of the steps.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <cstddef>
#include <memory>
#include <print>
#include <string>
#include <vector>

class ReportGenerator {
public:
    virtual ~ReportGenerator() = default;

    // The template method: the order of the steps is fixed here.
    void generate(const std::vector<std::string>& rows) const {
        std::println("--- {} report ---", name());
        writeHeader();
        for (std::size_t i = 0; i < rows.size(); ++i) {
            writeRow(i + 1, rows[i]);
        }
        if (wantsFooter()) {
            writeFooter(rows.size());
        }
    }

protected:
    // Steps every report must provide.
    virtual std::string name() const = 0;
    virtual void writeHeader() const = 0;
    virtual void writeRow(std::size_t number, const std::string& row) const = 0;

    // Hooks with a default behaviour; subclasses may override them.
    virtual bool wantsFooter() const { return true; }
    virtual void writeFooter(std::size_t count) const {
        std::println("rows: {}", count);
    }
};

class CsvReport : public ReportGenerator {
protected:
    std::string name() const override { return "CSV"; }
    void writeHeader() const override { std::println("number;text"); }
    void writeRow(std::size_t number, const std::string& row) const override {
        std::println("{};{}", number, row);
    }
};

class HtmlReport : public ReportGenerator {
protected:
    std::string name() const override { return "HTML"; }
    void writeHeader() const override { std::println("<ol>"); }
    void writeRow(std::size_t, const std::string& row) const override {
        std::println("  <li>{}</li>", row);
    }
    void writeFooter(std::size_t) const override { std::println("</ol>"); }
};

class PlainReport : public ReportGenerator {
protected:
    std::string name() const override { return "plain text"; }
    void writeHeader() const override {}
    void writeRow(std::size_t number, const std::string& row) const override {
        std::println("{}. {}", number, row);
    }
    bool wantsFooter() const override { return false; }
};

int main() {
    const std::vector<std::string> rows = {"apples", "bread", "coffee"};

    std::vector<std::unique_ptr<ReportGenerator>> reports;
    reports.push_back(std::make_unique<CsvReport>());
    reports.push_back(std::make_unique<HtmlReport>());
    reports.push_back(std::make_unique<PlainReport>());

    for (const auto& report : reports) {
        report->generate(rows);
        std::println("");
    }
    return 0;
}
