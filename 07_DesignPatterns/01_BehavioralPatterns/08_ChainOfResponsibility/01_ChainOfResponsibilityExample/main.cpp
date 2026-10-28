/*
 * Chain of Responsibility: expense approval
 *
 * Intent: pass a request along a chain of handlers. Each handler decides to
 * process it or to forward it to the next one.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <string>

struct Expense {
    std::string description;
    double amount;
};

class Approver {
public:
    virtual ~Approver() = default;

    // Returns the argument so calls can be chained: a.setNext(b).setNext(c).
    Approver& setNext(Approver& next) {
        next_ = &next;
        return next;
    }

    void handle(const Expense& expense) const {
        if (canApprove(expense)) {
            std::println("{} approved '{}' ({})", title(), expense.description, expense.amount);
        } else if (next_ != nullptr) {
            std::println("{} passes '{}' on", title(), expense.description);
            next_->handle(expense);
        } else {
            std::println("'{}' ({}) was rejected: nobody may approve it", expense.description,
                         expense.amount);
        }
    }

protected:
    virtual bool canApprove(const Expense& expense) const = 0;
    virtual std::string title() const = 0;

private:
    Approver* next_ = nullptr;
};

class TeamLead : public Approver {
protected:
    bool canApprove(const Expense& e) const override { return e.amount <= 100; }
    std::string title() const override { return "team lead"; }
};

class Director : public Approver {
protected:
    bool canApprove(const Expense& e) const override { return e.amount <= 1000; }
    std::string title() const override { return "director"; }
};

class FinanceBoard : public Approver {
protected:
    bool canApprove(const Expense& e) const override { return e.amount <= 10000; }
    std::string title() const override { return "finance board"; }
};

int main() {
    TeamLead lead;
    Director director;
    FinanceBoard board;
    lead.setNext(director).setNext(board);

    lead.handle({"team lunch", 80});
    std::println("");
    lead.handle({"new laptops", 900});
    std::println("");
    lead.handle({"conference trip", 4500});
    std::println("");
    lead.handle({"office renovation", 25000});
    return 0;
}
