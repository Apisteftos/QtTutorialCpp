// =============================================================================
// API Design Principles — C++23
//
// Demonstrates five API design principles with before/after comparison:
//
//   1. Fluent Interface (Builder) — method chaining, validated at build()
//   2. Typed Errors              — std::expected<T,E> vs return codes
//   3. Boolean Parameter Smell   — enum vs bool flag
//   4. Narrow Inputs             — take only what you need
//   5. Const Correctness         — promises to callers via const
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <cassert>
#include <expected>
#include <algorithm>
#include <sstream>
#include <unordered_map>

// =============================================================================
// 1. Fluent Interface — Query Builder
// =============================================================================

namespace fluent {

class QueryBuilder {
public:
    // Each method returns *this — enables chaining
    QueryBuilder& select(std::initializer_list<std::string> cols) {
        columns_ = cols;
        return *this;
    }

    QueryBuilder& from(const std::string& table) {
        table_ = table;
        return *this;
    }

    QueryBuilder& where(const std::string& condition) {
        conditions_.push_back(condition);
        return *this;
    }

    QueryBuilder& orderBy(const std::string& col, bool ascending = true) {
        order_col_ = col;
        ascending_ = ascending;
        return *this;
    }

    QueryBuilder& limit(int n) {
        limit_ = n;
        return *this;
    }

    // Validation happens here — not scattered across setters
    std::string build() const {
        if (table_.empty())
            throw std::logic_error("QueryBuilder: table not specified");
        if (columns_.empty())
            throw std::logic_error("QueryBuilder: no columns selected");
        if (limit_ < 0)
            throw std::logic_error("QueryBuilder: limit cannot be negative");

        std::ostringstream oss;
        oss << "SELECT ";
        for (size_t i = 0; i < columns_.size(); i++) {
            oss << columns_[i];
            if (i + 1 < columns_.size()) oss << ", ";
        }
        oss << "\nFROM " << table_;
        if (!conditions_.empty()) {
            oss << "\nWHERE ";
            for (size_t i = 0; i < conditions_.size(); i++) {
                oss << conditions_[i];
                if (i + 1 < conditions_.size()) oss << "\n  AND ";
            }
        }
        if (!order_col_.empty())
            oss << "\nORDER BY " << order_col_ << (ascending_ ? " ASC" : " DESC");
        if (limit_ > 0)
            oss << "\nLIMIT " << limit_;

        return oss.str();
    }

private:
    std::string              table_;
    std::vector<std::string> columns_;
    std::vector<std::string> conditions_;
    std::string              order_col_;
    bool                     ascending_{true};
    int                      limit_{0};
};

} // namespace fluent

// =============================================================================
// 2. Typed Errors — std::expected<T, Error>
// =============================================================================

namespace typed_errors {

// ❌ Bad: opaque return codes
int bad_parse_age(const std::string& s) {
    try { return std::stoi(s); }
    catch (...) { return -1; }   // -1 means error but caller might forget to check
}

// ✅ Good: typed error — caller MUST handle both cases
enum class ParseError { InvalidFormat, OutOfRange };

std::string to_string(ParseError e) {
    switch (e) {
    case ParseError::InvalidFormat: return "invalid format";
    case ParseError::OutOfRange:    return "out of range";
    }
    return "unknown";
}

std::expected<int, ParseError> parse_age(const std::string& s) {
    try {
        int val = std::stoi(s);
        if (val < 0 || val > 150)
            return std::unexpected(ParseError::OutOfRange);
        return val;
    } catch (...) {
        return std::unexpected(ParseError::InvalidFormat);
    }
}

} // namespace typed_errors

// =============================================================================
// 3. Boolean Parameter Smell → Use enum
// =============================================================================

namespace bool_smell {

// ❌ Bad — what does true mean here?
void bad_sort(std::vector<int>& v, bool b) {
    if (b) std::sort(v.begin(), v.end());
    else   std::sort(v.rbegin(), v.rend());
}
// Caller: bad_sort(v, true);   — true = ascending? descending? unclear!

// ✅ Good — intent is obvious at call site
enum class SortOrder { Ascending, Descending };

void good_sort(std::vector<int>& v, SortOrder order) {
    if (order == SortOrder::Ascending)
        std::sort(v.begin(), v.end());
    else
        std::sort(v.rbegin(), v.rend());
}
// Caller: good_sort(v, SortOrder::Ascending);  — crystal clear

// ❌ Bad — multiple booleans compound the problem
void bad_connect(const std::string& host, bool secure, bool async, bool retry) {}
// Caller: bad_connect("host", true, false, true);  — impossible to read

// ✅ Good — named options struct
struct ConnectOptions {
    bool secure{true};
    bool async{false};
    bool retry{true};
    int  timeout_ms{5000};
};

void good_connect(const std::string& host, ConnectOptions opts = {}) {
    std::cout << "  connect(" << host
              << " secure=" << opts.secure
              << " async="  << opts.async << ")\n";
}
// Caller: good_connect("host", {.secure=true, .retry=false});  — clear!

} // namespace bool_smell

// =============================================================================
// 4. Narrow Inputs — take only what the function needs
// =============================================================================

namespace narrow_inputs {

struct User {
    int         id;
    std::string name;
    std::string email;
    std::string password_hash;   // sensitive!
    int         age;
};

// ❌ Bad — takes entire User when it only needs the name
std::string bad_greet(const User& user) {
    return "Hello, " + user.name + "!";
    // Also has access to password_hash — bad!
}

// ✅ Good — takes only what it needs
std::string good_greet(const std::string& name) {
    return "Hello, " + name + "!";
}

// ❌ Bad — takes User just to send email
void bad_send_welcome(const User& user) {
    std::cout << "  send email to: " << user.email << '\n';
    // Has unnecessary access to age, password_hash, etc.
}

// ✅ Good — takes only the email address
void good_send_welcome(const std::string& email) {
    std::cout << "  send email to: " << email << '\n';
}

} // namespace narrow_inputs

// =============================================================================
// 5. Const Correctness
// =============================================================================

namespace const_correct {

class UserRepository {
public:
    void add(int id, const std::string& name) {
        users_[id] = name;
    }

    // ✅ const — promises: calling this does NOT modify the object
    //    Caller can trust this is a safe read-only operation
    std::optional<std::string> find(int id) const {
        auto it = users_.find(id);
        if (it == users_.end()) return std::nullopt;
        return it->second;
    }

    // ✅ const — safe to call on a const UserRepository reference
    int count() const { return static_cast<int>(users_.size()); }

    // No const — modifies state — caller knows this changes the object
    void remove(int id) { users_.erase(id); }

private:
    std::unordered_map<int, std::string> users_;
};

// This function takes const ref — it only needs to read
// Compiler enforces: cannot accidentally call non-const methods
void printRepo(const UserRepository& repo) {
    std::cout << "  Repository has " << repo.count() << " users\n";
    // repo.remove(1);  // ← compile error: remove() is not const
    //                  // const correctness catches bugs at compile time
}

} // namespace const_correct

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== API Design Principles Demo ===\n\n";

    // ── 1. Fluent Interface ───────────────────────────────────────────────────
    std::cout << "--- 1. Fluent Interface (Query Builder) ---\n";
    {
        auto query = fluent::QueryBuilder{}
                         .select({"id", "name", "email"})
                         .from("users")
                         .where("age > 18")
                         .where("active = true")
                         .orderBy("name")
                         .limit(10)
                         .build();

        std::cout << query << "\n\n";
        assert(query.find("SELECT") != std::string::npos);
        assert(query.find("LIMIT 10") != std::string::npos);

        // Validation — missing table
        bool threw = false;
        try {
            fluent::QueryBuilder{}.select({"id"}).build();
        } catch (const std::logic_error&) { threw = true; }
        assert(threw);
        std::cout << "  ✓ Validation catches missing table at build()\n";
    }

    // ── 2. Typed Errors ───────────────────────────────────────────────────────
    std::cout << "\n--- 2. Typed Errors ---\n";
    {
        // Bad API — easy to ignore -1
        int bad = typed_errors::bad_parse_age("abc");
        assert(bad == -1);  // silent failure — easy to miss
        std::cout << "  Bad API: parse_age('abc') = " << bad << " (easy to ignore)\n";

        // Good API — caller must handle error case
        auto r1 = typed_errors::parse_age("25");
        assert(r1.has_value() && *r1 == 25);
        std::cout << "  Good API: parse_age('25') = " << *r1 << '\n';

        auto r2 = typed_errors::parse_age("abc");
        assert(!r2.has_value());
        assert(r2.error() == typed_errors::ParseError::InvalidFormat);
        std::cout << "  Good API: parse_age('abc') = error: "
                  << typed_errors::to_string(r2.error()) << '\n';

        auto r3 = typed_errors::parse_age("999");
        assert(r3.error() == typed_errors::ParseError::OutOfRange);
        std::cout << "  Good API: parse_age('999') = error: "
                  << typed_errors::to_string(r3.error()) << '\n';
        std::cout << "  ✓ Caller cannot accidentally ignore typed errors\n";
    }

    // ── 3. Boolean Parameter Smell ────────────────────────────────────────────
    std::cout << "\n--- 3. Boolean Parameter → Enum ---\n";
    {
        std::vector<int> v = {5, 2, 8, 1, 9, 3};

        bool_smell::good_sort(v, bool_smell::SortOrder::Ascending);
        assert(std::is_sorted(v.begin(), v.end()));
        std::cout << "  good_sort(v, SortOrder::Ascending) ✓\n";

        bool_smell::good_sort(v, bool_smell::SortOrder::Descending);
        assert(std::is_sorted(v.rbegin(), v.rend()));
        std::cout << "  good_sort(v, SortOrder::Descending) ✓\n";

        bool_smell::good_connect("db.server.com", {.secure=true, .async=false});
        std::cout << "  ✓ Named options — intent is obvious at call site\n";
    }

    // ── 4. Narrow Inputs ─────────────────────────────────────────────────────
    std::cout << "\n--- 4. Narrow Inputs ---\n";
    {
        narrow_inputs::User user{1, "Kostas", "k@example.com", "hash123", 35};

        auto g1 = narrow_inputs::good_greet(user.name);
        assert(g1 == "Hello, Kostas!");
        std::cout << "  good_greet(name): " << g1 << '\n';
        std::cout << "  ✓ No access to password_hash or age\n";

        narrow_inputs::good_send_welcome(user.email);
        std::cout << "  ✓ Only email passed — principle of least privilege\n";
    }

    // ── 5. Const Correctness ──────────────────────────────────────────────────
    std::cout << "\n--- 5. Const Correctness ---\n";
    {
        const_correct::UserRepository repo;
        repo.add(1, "Kostas");
        repo.add(2, "George");

        const_correct::printRepo(repo);  // takes const ref — safe

        auto found = repo.find(1);
        assert(found.has_value() && *found == "Kostas");

        auto missing = repo.find(99);
        assert(!missing.has_value());

        repo.remove(2);
        assert(repo.count() == 1);
        std::cout << "  ✓ const methods callable on const ref\n";
        std::cout << "  ✓ non-const methods only callable on mutable object\n";
    }

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
