# Command Pattern (Basic) — C++

## Intent

Encapsulate a **request as an object**, allowing you to parameterize clients
with different requests, queue or log requests, and support undoable operations.

---

## Structure from this file

```
Command (interface)
  └── Execute() = 0

SimpleCommand   ← executes directly (prints payload)
ComplexCommand  ← delegates to Receiver

Receiver        ← contains actual business logic
  └── DoSomething()
  └── DoSomethingElse()

Invoker         ← triggers commands without knowing their details
  └── on_start_ (Command*)
  └── on_finish_ (Command*)
  └── DoSomethingImportant()
```

---

## Key code

```cpp
// Command interface
class Command {
public:
    virtual ~Command() {}
    virtual void Execute() const = 0;
};

// Simple command — no receiver needed
class SimpleCommand : public Command {
    std::string pay_load_;
public:
    explicit SimpleCommand(std::string p) : pay_load_(p) {}
    void Execute() const override {
        std::cout << "SimpleCommand: " << pay_load_ << '\n';
    }
};

// Complex command — delegates to Receiver
class ComplexCommand : public Command {
    Receiver* receiver_;
    std::string a_, b_;
public:
    ComplexCommand(Receiver* r, std::string a, std::string b)
        : receiver_(r), a_(a), b_(b) {}
    void Execute() const override {
        receiver_->DoSomething(a_);
        receiver_->DoSomethingElse(b_);
    }
};

// Invoker — fires commands at the right time
class Invoker {
    Command* on_start_;
    Command* on_finish_;
public:
    void DoSomethingImportant() {
        if (on_start_)  on_start_->Execute();
        // ... do work ...
        if (on_finish_) on_finish_->Execute();
    }
};
```

---

## Client setup

```cpp
Invoker* invoker = new Invoker;
invoker->SetOnStart(new SimpleCommand("Say Hi!"));

Receiver* receiver = new Receiver;
invoker->SetOnFinish(new ComplexCommand(receiver, "Send email", "Save report"));

invoker->DoSomethingImportant();
```

The invoker does not know what the commands do — it just calls `Execute()`.

---

## Four participants

| Role | Class | Responsibility |
|------|-------|---------------|
| Command | `Command` | Declares `Execute()` interface |
| ConcreteCommand | `SimpleCommand`, `ComplexCommand` | Binds action to receiver |
| Receiver | `Receiver` | Knows how to perform the work |
| Invoker | `Invoker` | Asks command to carry out request |

---

## When to use Command Pattern

✅ Parameterize objects with operations
✅ Queue, schedule, or log operations
✅ Support undo/redo (see TextEditor example)
✅ Implement transactional behaviour (all-or-nothing)
✅ Decouple the sender from the receiver

---

## Real-world examples

- GUI button click → command object → action
- Job queue — commands serialized and executed by worker
- Macro recording — store commands, replay later
- MCX floor request → command object → floor controller
