// =============================================================================
// Cohesion — C++23
//
// Shows a LOW-cohesion class (UserManager does everything) refactored into
// HIGH-cohesion components, each with one clear responsibility.
//
// Pattern:
//   BEFORE: UserManager — auth + email + logging + validation (4 reasons to change)
//   AFTER:  Authenticator + EmailSender + Logger + Validator (1 reason each)
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <sstream>

// =============================================================================
// ❌ BEFORE — Low Cohesion
// UserManager has 4 unrelated responsibilities.
// Change in email provider → touches auth code.
// Change in log format    → touches validation code.
// =============================================================================

namespace before {

class UserManager {
public:
    // Responsibility 1: Authentication
    bool login(const std::string& user, const std::string& pass) {
        if (user.empty() || pass.empty()) return false;
        // Simulate: only "admin"/"secret" is valid
        bool ok = (user == "admin" && pass == "secret");
        // Mixed with logging (responsibility 3!)
        log("LOGIN " + user + (ok ? " OK" : " FAIL"));
        return ok;
    }

    // Responsibility 2: Email
    bool sendEmail(const std::string& to, const std::string& subject,
                   const std::string& body) {
        if (to.find('@') == std::string::npos) return false;
        email_log_.push_back("[SMTP] To:" + to + " Subj:" + subject);
        log("EMAIL sent to " + to);   // again mixed with logging!
        return true;
    }

    // Responsibility 3: Logging
    void log(const std::string& msg) {
        logs_.push_back("[LOG] " + msg);
    }

    // Responsibility 4: Validation
    bool validateUsername(const std::string& name) {
        if (name.size() < 3 || name.size() > 32) return false;
        return std::all_of(name.begin(), name.end(),
                           [](char c){ return std::isalnum(c) || c == '_'; });
    }

    // Getters for test verification
    const std::vector<std::string>& logs()      const { return logs_; }
    const std::vector<std::string>& emailLog()  const { return email_log_; }

private:
    std::vector<std::string> logs_;
    std::vector<std::string> email_log_;
};

} // namespace before

// =============================================================================
// ✅ AFTER — High Cohesion
// Each class has exactly ONE reason to change.
// =============================================================================

namespace after {

// ── Responsibility 1: Authentication only ────────────────────────────────────
class Authenticator {
public:
    bool login(const std::string& user, const std::string& pass) const {
        if (user.empty() || pass.empty()) return false;
        return (user == "admin" && pass == "secret");
    }

    std::string hashPassword(const std::string& pass) const {
        // Simplified hash (real: bcrypt/argon2)
        size_t h = std::hash<std::string>{}(pass);
        std::ostringstream oss;
        oss << std::hex << h;
        return oss.str();
    }
};

// ── Responsibility 2: Email only ─────────────────────────────────────────────
class EmailSender {
public:
    bool send(const std::string& to, const std::string& subject,
              const std::string& body) {
        if (to.find('@') == std::string::npos) return false;
        sent_.push_back("[SMTP] To:" + to + " Subj:" + subject);
        return true;
    }

    const std::vector<std::string>& sent() const { return sent_; }

private:
    std::vector<std::string> sent_;
};

// ── Responsibility 3: Logging only ───────────────────────────────────────────
class Logger {
public:
    void log(const std::string& msg) {
        entries_.push_back("[LOG] " + msg);
        std::cout << "  " << entries_.back() << '\n';
    }

    const std::vector<std::string>& entries() const { return entries_; }

private:
    std::vector<std::string> entries_;
};

// ── Responsibility 4: Validation only ────────────────────────────────────────
class Validator {
public:
    bool validateUsername(const std::string& name) const {
        if (name.size() < 3 || name.size() > 32) return false;
        return std::all_of(name.begin(), name.end(),
                           [](char c){ return std::isalnum(c) || c == '_'; });
    }

    bool validateEmail(const std::string& email) const {
        auto at = email.find('@');
        if (at == std::string::npos) return false;
        auto dot = email.find('.', at);
        return dot != std::string::npos && dot > at + 1;
    }
};

// ── Composition — wire the components together where needed ──────────────────
// The login workflow now uses Authenticator + Logger separately.
// Each can be tested independently. Each can be changed independently.

class LoginWorkflow {
public:
    LoginWorkflow(Authenticator& auth, Logger& logger, Validator& validator)
        : auth_(auth), logger_(logger), validator_(validator) {}

    bool execute(const std::string& user, const std::string& pass) {
        if (!validator_.validateUsername(user)) {
            logger_.log("INVALID username: " + user);
            return false;
        }
        bool ok = auth_.login(user, pass);
        logger_.log("LOGIN " + user + (ok ? " OK" : " FAIL"));
        return ok;
    }

private:
    Authenticator& auth_;
    Logger&        logger_;
    Validator&     validator_;
};

} // namespace after

// =============================================================================
// main — demonstrate and compare both versions
// =============================================================================

int main()
{
    std::cout << "=== Cohesion Demo ===\n\n";

    // ── BEFORE ───────────────────────────────────────────────────────────────
    std::cout << "--- BEFORE: Low Cohesion ---\n";
    before::UserManager um;
    bool ok1 = um.login("admin", "secret");
    bool ok2 = um.login("hacker", "wrong");
    um.sendEmail("user@example.com", "Welcome", "Hello!");
    um.validateUsername("ko");   // too short — silent fail, no logging

    std::cout << "Login admin: "  << ok1 << '\n';
    std::cout << "Login hacker: " << ok2 << '\n';
    std::cout << "Logs: "         << um.logs().size()     << " entries\n";
    std::cout << "Emails: "       << um.emailLog().size() << " sent\n\n";

    assert(ok1 == true);
    assert(ok2 == false);
    assert(um.logs().size() == 3);    // 2 logins + 1 email
    assert(um.emailLog().size() == 1);

    // ── AFTER ────────────────────────────────────────────────────────────────
    std::cout << "--- AFTER: High Cohesion ---\n";

    after::Authenticator auth;
    after::EmailSender   email;
    after::Logger        logger;
    after::Validator     validator;
    after::LoginWorkflow loginFlow(auth, logger, validator);

    // Each component tested independently
    assert(validator.validateUsername("admin")  == true);
    assert(validator.validateUsername("ko")     == false);  // too short
    assert(validator.validateUsername("x y z") == false);  // spaces
    assert(validator.validateEmail("a@b.com")  == true);
    assert(validator.validateEmail("notvalid") == false);
    std::cout << "✓ Validator tests pass\n";

    bool a1 = loginFlow.execute("admin", "secret");
    bool a2 = loginFlow.execute("hacker", "wrong");
    bool a3 = loginFlow.execute("ko", "any");          // invalid username

    assert(a1 == true);
    assert(a2 == false);
    assert(a3 == false);
    assert(logger.entries().size() == 3);
    std::cout << "✓ LoginWorkflow tests pass\n";

    bool e1 = email.send("dev@example.com", "Welcome", "Hello!");
    bool e2 = email.send("bad-address", "Hi", "...");

    assert(e1 == true);
    assert(e2 == false);
    assert(email.sent().size() == 1);
    std::cout << "✓ EmailSender tests pass\n";

    // Hash is deterministic
    std::string h1 = auth.hashPassword("secret");
    std::string h2 = auth.hashPassword("secret");
    assert(h1 == h2);
    assert(h1 != "secret");
    std::cout << "✓ Authenticator tests pass\n";

    std::cout << "\nKey difference:\n";
    std::cout << "  BEFORE — 1 class, 4 reasons to change\n";
    std::cout << "  AFTER  — 4 classes, 1 reason each\n";
    std::cout << "\nAll assertions passed.\n";
    return 0;
}
