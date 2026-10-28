/*
 * Mediator: a sign-up form
 *
 * Intent: components do not talk to each other directly. They report to a
 * mediator, which knows the rules and updates the other components.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <string>
#include <utility>

class Widget;

class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void changed(const Widget& source) = 0;
};

class Widget {
public:
    Widget(Mediator& mediator, std::string name) : mediator_(mediator), name_(std::move(name)) {}
    virtual ~Widget() = default;
    const std::string& name() const { return name_; }

protected:
    void notifyMediator() { mediator_.changed(*this); }

private:
    Mediator& mediator_;
    std::string name_;
};

class TextField : public Widget {
public:
    using Widget::Widget;
    void type(const std::string& text) {
        text_ = text;
        std::println("{}: typed '{}'", name(), text_);
        notifyMediator();
    }
    const std::string& text() const { return text_; }

private:
    std::string text_;
};

class CheckBox : public Widget {
public:
    using Widget::Widget;
    void toggle() {
        checked_ = !checked_;
        std::println("{}: {}", name(), checked_ ? "checked" : "unchecked");
        notifyMediator();
    }
    bool checked() const { return checked_; }

private:
    bool checked_ = false;
};

class SubmitButton : public Widget {
public:
    using Widget::Widget;
    void setEnabled(bool enabled) {
        enabled_ = enabled;
        std::println("{}: {}", name(), enabled_ ? "enabled" : "disabled");
    }
    void click() const {
        std::println("{}: {}", name(), enabled_ ? "form sent" : "click ignored (disabled)");
    }

private:
    bool enabled_ = false;
};

// Concrete mediator: owns the widgets and contains all interaction rules.
class SignupForm : public Mediator {
public:
    SignupForm() : userName_(*this, "user name"), terms_(*this, "terms"), submit_(*this, "submit") {}

    void changed(const Widget&) override {
        bool ready = !userName_.text().empty() && terms_.checked();
        submit_.setEnabled(ready);
    }

    TextField userName_;
    CheckBox terms_;
    SubmitButton submit_;
};

int main() {
    SignupForm form;

    form.submit_.click();
    form.userName_.type("alex");
    form.terms_.toggle();
    form.submit_.click();
    form.userName_.type("");
    form.submit_.click();
    return 0;
}
