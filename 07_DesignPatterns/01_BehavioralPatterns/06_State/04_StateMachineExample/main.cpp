#include <typeinfo>
#include <print>
/**
 * The base State class declares methods that all Concrete State should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */

class Context;

class State {
    /**
   * @var Context
   */
protected:
    Context *context_;

public:
    virtual ~State() {
    }

    void set_context(Context *context) {
        this->context_ = context;
    }

    virtual void Handle1() = 0;
    virtual void Handle2() = 0;
};

/**
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */
class Context {
    /**
   * @var State A reference to the current state of the Context.
   */
private:
    State *state_;

public:
    Context(State *state) : state_(nullptr) {
        this->TransitionTo(state);
    }
    ~Context() {
        delete state_;
    }
    /**
   * The Context allows changing the State object at runtime.
   */
    void TransitionTo(State *state) {
        std::println("Context: Transition to {}.", typeid(*state).name());
        if (this->state_ != nullptr)
            delete this->state_;
        this->state_ = state;
        this->state_->set_context(this);
    }
    /**
   * The Context delegates part of its behavior to the current State object.
   */
    void Request1() {
        this->state_->Handle1();
    }
    void Request2() {
        this->state_->Handle2();
    }
};

/**
 * Concrete States implement various behaviors, associated with a state of the
 * Context.
 */

class ConcreteStateA : public State {
public:
    void Handle1() override;

    void Handle2() override {
        std::println("ConcreteStateA handles request2.");
    }
};

class ConcreteStateB : public State {
public:
    void Handle1() override {
        std::println("ConcreteStateB handles request1.");
    }
    void Handle2() override {
        std::println("ConcreteStateB handles request2.");
        std::println("ConcreteStateB wants to change the state of the context.");
        this->context_->TransitionTo(new ConcreteStateA);
    }
};

void ConcreteStateA::Handle1() {
    {
        std::println("ConcreteStateA handles request1.");
        std::println("ConcreteStateA wants to change the state of the context.");

        this->context_->TransitionTo(new ConcreteStateB);
    }
}

/**
 * The client code.
 */
void ClientCode() {
    Context *context = new Context(new ConcreteStateA);
    context->Request1();
    context->Request2();
    delete context;
}

int main() {
    ClientCode();
    return 0;
}
