/*
 * Builder: composing e-mails step by step
 *
 * Intent: construct a complex object in steps. The same construction steps can
 * be reused by a director to produce ready-made variants.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <string>
#include <utility>
#include <vector>

struct Email {
    std::string to;
    std::string subject;
    std::string body;
    std::vector<std::string> attachments;

    void print() const {
        std::println("To: {}", to);
        std::println("Subject: {}", subject);
        std::println("Body: {}", body);
        std::println("Attachments: {}", attachments.size());
        for (const auto& file : attachments) {
            std::println("  - {}", file);
        }
    }
};

class EmailBuilder {
public:
    EmailBuilder& to(std::string address) {
        email_.to = std::move(address);
        return *this;
    }
    EmailBuilder& subject(std::string text) {
        email_.subject = std::move(text);
        return *this;
    }
    EmailBuilder& body(std::string text) {
        email_.body = std::move(text);
        return *this;
    }
    EmailBuilder& attach(std::string file) {
        email_.attachments.push_back(std::move(file));
        return *this;
    }

    // Hands out the finished product and starts a fresh one.
    Email build() {
        Email result = std::move(email_);
        email_ = Email{};
        return result;
    }

private:
    Email email_;
};

// Director: knows which steps make a certain kind of e-mail.
class EmailDirector {
public:
    static Email welcome(EmailBuilder& builder, const std::string& address, const std::string& name) {
        return builder.to(address)
            .subject("Welcome, " + name)
            .body("Your account is ready.")
            .attach("getting_started.pdf")
            .build();
    }

    static Email reminder(EmailBuilder& builder, const std::string& address) {
        return builder.to(address)
            .subject("Reminder")
            .body("Your appointment is tomorrow.")
            .build();
    }
};

int main() {
    EmailBuilder builder;

    Email custom = builder.to("sam@example.com")
                       .subject("Invoice")
                       .body("Please find the invoice attached.")
                       .attach("invoice_2026_10.pdf")
                       .attach("terms.pdf")
                       .build();
    custom.print();
    std::println("");

    EmailDirector::welcome(builder, "robin@example.com", "Robin").print();
    std::println("");

    EmailDirector::reminder(builder, "robin@example.com").print();
    return 0;
}
