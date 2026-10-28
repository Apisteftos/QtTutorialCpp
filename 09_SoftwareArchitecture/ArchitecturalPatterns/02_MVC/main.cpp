// =============================================================================
// MVC — Model · View · Controller — C++23
//
// Example: Console Task Manager
//
// Model    — owns Task data, enforces rules, notifies observers on change
// View     — renders the task list to stdout (Observer of Model)
// Controller — receives commands, calls Model, triggers View refresh
//
// The Observer pattern wires Model → View without Model knowing about View.
// This is exactly how Qt's signals/slots work under the hood.
// =============================================================================

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <print>

// =============================================================================
// Observer interface — Model notifies through this
// =============================================================================

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

// =============================================================================
// MODEL — data + rules, no display logic
// =============================================================================

struct Task {
    int         id;
    std::string title;
    bool        done{false};
};

class TaskModel {
public:
    void add(const std::string& title) {
        if (title.empty())
            throw std::invalid_argument("Task title cannot be empty");
        tasks_.push_back({next_id_++, title, false});
        notify();
    }

    void complete(int id) {
        auto it = std::find_if(tasks_.begin(), tasks_.end(),
                               [id](const Task& t){ return t.id == id; });
        if (it == tasks_.end())
            throw std::runtime_error("Task not found: " + std::to_string(id));
        it->done = true;
        notify();
    }

    void remove(int id) {
        auto before = tasks_.size();
        tasks_.erase(std::remove_if(tasks_.begin(), tasks_.end(),
                                    [id](const Task& t){ return t.id == id; }), tasks_.end());
        if (tasks_.size() == before)
            throw std::runtime_error("Task not found: " + std::to_string(id));
        notify();
    }

    const std::vector<Task>& getTasks() const { return tasks_; }

    int pendingCount() const {
        return static_cast<int>(
            std::count_if(tasks_.begin(), tasks_.end(),
                          [](const Task& t){ return !t.done; }));
    }

    // Observer pattern — Model knows about the interface, not the View
    void attach(Observer* obs) { observers_.push_back(obs); }

private:
    void notify() {
        for (Observer* obs : observers_) obs->update();
    }

    std::vector<Task>     tasks_;
    std::vector<Observer*> observers_;
    int                   next_id_{1};
};

// =============================================================================
// VIEW — display only, reads Model state, no business logic
// =============================================================================

class ConsoleView : public Observer {
public:
    explicit ConsoleView(const TaskModel& model) : model_(model) {}

    // Called by Model via Observer::update() whenever state changes
    void update() override {
        render();
    }

    void render() const {
        const auto& tasks = model_.getTasks();
        if (tasks.empty()) {
            std::println("│  (no tasks)                 │");
        } else {
            for (const auto& t : tasks) {
                std::string status = t.done ? "✓" : "○";
                std::print("│ [{}] #{} {}", status, t.id, t.title);
                // pad to fixed width
                int pad = 22 - static_cast<int>(t.title.size())
                          - std::to_string(t.id).size();
                for (int i = 0; i < pad; i++) std::print(" ");
                std::println("│");
            }
        }
        std::println("├─────────────────────────────┤");
        std::println("│  Pending: {}                    │", model_.pendingCount());
        std::println("└─────────────────────────────┘");
    }

private:
    const TaskModel& model_;   // reads Model, never mutates it
};

// =============================================================================
// CONTROLLER — thin glue, no logic, just delegation
// =============================================================================

class TaskController {
public:
    TaskController(TaskModel& model, ConsoleView& view)
        : model_(model), view_(view) {}

    void handleAdd(const std::string& title) {
        std::println("");
        std::println("[Controller] ADD: \"{}\"", title);
        try {
            model_.add(title);   // Model updates → notifies View automatically
        } catch (const std::exception& e) {
            std::println("[Controller] Error: {}", e.what());
        }
    }

    void handleComplete(int id) {
        std::println("");
        std::println("[Controller] COMPLETE: #{}", id);
        try {
            model_.complete(id);
        } catch (const std::exception& e) {
            std::println("[Controller] Error: {}", e.what());
        }
    }

    void handleRemove(int id) {
        std::println("");
        std::println("[Controller] REMOVE: #{}", id);
        try {
            model_.remove(id);
        } catch (const std::exception& e) {
            std::println("[Controller] Error: {}", e.what());
        }
    }

private:
    TaskModel&   model_;
    ConsoleView& view_;   // controller knows the view exists but rarely calls it directly
        // Model → View notification handles most rendering
};

// =============================================================================
// main — wire MVC together
// =============================================================================

int main()
{
    // 1. Create Model
    TaskModel model;

    // 2. Create View (needs Model to read from)
    ConsoleView view(model);

    // 3. Register View as observer of Model
    model.attach(&view);

    // 4. Create Controller (wires Model + View)
    TaskController ctrl(model, view);

    std::print("=== MVC Task Manager Demo ===");

    // Simulate user interactions via Controller
    ctrl.handleAdd("Write layered architecture example");
    ctrl.handleAdd("Write MVC example");
    ctrl.handleAdd("Write event-driven example");
    ctrl.handleComplete(1);
    ctrl.handleComplete(2);
    ctrl.handleAdd("Push all examples to GitHub");
    ctrl.handleRemove(3);

    // Error handling
    ctrl.handleComplete(99);   // non-existent id
    ctrl.handleAdd("");        // empty title

    return 0;
}
