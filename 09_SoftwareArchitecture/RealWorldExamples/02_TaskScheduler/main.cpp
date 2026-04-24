// =============================================================================
// Task Scheduler — C++23
//
// A production-quality task scheduler with:
//   - Priority queue (CRITICAL > HIGH > NORMAL > LOW)
//   - Deadline-aware ordering within same priority
//   - Automatic retry with exponential backoff
//   - Dead Letter Queue (DLQ) for exhausted tasks
//   - Worker pool simulation
//   - Task lifecycle: Pending → Running → Done / Retrying → Dead
//
// Patterns used: Priority Queue, State Machine, Pipe-Filter, Event-Driven
// =============================================================================

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <functional>
#include <chrono>
#include <cassert>
#include <optional>
#include <stdexcept>
#include <iomanip>
#include <memory>

using Clock    = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Ms       = std::chrono::milliseconds;

// =============================================================================
// Priority levels
// =============================================================================

enum class Priority { Low = 1, Normal = 5, High = 7, Critical = 10 };

std::string to_string(Priority p) {
    switch (p) {
    case Priority::Critical: return "CRITICAL";
    case Priority::High:     return "HIGH    ";
    case Priority::Normal:   return "NORMAL  ";
    case Priority::Low:      return "LOW     ";
    }
    return "?";
}

// =============================================================================
// Task
// =============================================================================

enum class TaskState { Pending, Running, Done, Retrying, Dead };

std::string to_string(TaskState s) {
    switch (s) {
    case TaskState::Pending:  return "Pending";
    case TaskState::Running:  return "Running";
    case TaskState::Done:     return "Done";
    case TaskState::Retrying: return "Retrying";
    case TaskState::Dead:     return "Dead";
    }
    return "?";
}

using TaskFn = std::function<bool()>;   // returns true = success, false = failure

struct Task {
    int         id;
    std::string name;
    Priority    priority;
    TimePoint   deadline;
    TimePoint   run_after;     // earliest time to run (for retry backoff)
    TaskFn      fn;
    int         attempt{0};
    int         max_attempts{3};
    TaskState   state{TaskState::Pending};

    // Comparator for priority queue — higher priority first,
    // earlier deadline breaks ties
    bool operator<(const Task& other) const {
        if (priority != other.priority)
            return static_cast<int>(priority) < static_cast<int>(other.priority);
        return deadline > other.deadline;  // earlier deadline = higher priority
    }
};

// =============================================================================
// Scheduler
// =============================================================================

class TaskScheduler {
public:
    // Submit a task to the queue
    int submit(const std::string& name,
               Priority priority,
               TaskFn fn,
               Ms deadline_from_now = Ms(5000),
               int max_attempts = 3)
    {
        int id = next_id_++;
        Task t;
        t.id           = id;
        t.name         = name;
        t.priority     = priority;
        t.deadline     = Clock::now() + deadline_from_now;
        t.run_after    = Clock::now();
        t.fn           = std::move(fn);
        t.max_attempts = max_attempts;

        queue_.push(std::move(t));
        submitted_++;
        std::cout << "  [Scheduler] submitted #" << id
                  << " [" << to_string(priority) << "] " << name << '\n';
        return id;
    }

    // Run all pending tasks (single-threaded simulation of worker pool)
    void runAll() {
        std::cout << "\n  [Scheduler] running " << queue_.size() << " tasks\n";

        while (!queue_.empty()) {
            // Collect all tasks ready to run now
            std::vector<Task> ready;
            auto temp = queue_;   // copy — we rebuild the queue
            queue_ = {};

            while (!temp.empty()) {
                Task t = temp.top(); temp.pop();
                if (Clock::now() >= t.run_after)
                    ready.push_back(std::move(t));
                else
                    queue_.push(std::move(t));
            }

            if (ready.empty()) break;   // nothing ready yet

            for (auto& task : ready) {
                execute(task);
            }
        }

        // Drain any remaining (timed-out backoff tasks for demo)
        while (!queue_.empty()) {
            Task t = queue_.top(); queue_.pop();
            execute(t);
        }
    }

    // Stats
    int submitted()  const { return submitted_; }
    int succeeded()  const { return succeeded_; }
    int failed()     const { return failed_; }
    int deadCount()  const { return static_cast<int>(dlq_.size()); }

    void printDLQ() const {
        if (dlq_.empty()) {
            std::cout << "  [DLQ] empty\n";
            return;
        }
        std::cout << "  [DLQ] " << dlq_.size() << " dead tasks:\n";
        for (const auto& t : dlq_)
            std::cout << "    #" << t.id << " " << t.name
                      << " (" << t.attempt << " attempts)\n";
    }

private:
    void execute(Task& task) {
        task.state   = TaskState::Running;
        task.attempt++;

        std::cout << "  [Worker]    #" << task.id
                  << " attempt " << task.attempt << "/" << task.max_attempts
                  << " [" << to_string(task.priority) << "] "
                  << task.name;

        bool ok = false;
        try { ok = task.fn(); }
        catch (const std::exception& e) {
            std::cout << " → EXCEPTION: " << e.what();
        }

        if (ok) {
            task.state = TaskState::Done;
            succeeded_++;
            std::cout << " → ✓ DONE\n";
        } else if (task.attempt < task.max_attempts) {
            task.state = TaskState::Retrying;
            // Exponential backoff: 0ms, 0ms, 0ms for demo (no real sleep)
            // In production: Ms(100 * (1 << (task.attempt - 1)))
            task.run_after = Clock::now();  // immediate for demo
            queue_.push(task);
            std::cout << " → ↩ RETRY (attempt " << task.attempt + 1 << " queued)\n";
        } else {
            task.state = TaskState::Dead;
            failed_++;
            dlq_.push_back(task);
            std::cout << " → 💀 DEAD (max retries exhausted)\n";
        }
    }

    std::priority_queue<Task> queue_;
    std::vector<Task>         dlq_;
    int next_id_{1};
    int submitted_{0};
    int succeeded_{0};
    int failed_{0};
};

// =============================================================================
// Simulated services (task implementations)
// =============================================================================

class EmailService {
public:
    bool send(const std::string& to, const std::string& subject) {
        send_count_++;
        std::cout << " [email→" << to << "]";
        return true;   // always succeeds in demo
    }
    int sendCount() const { return send_count_; }
private:
    int send_count_{0};
};

class PaymentService {
public:
    bool process(int amount, int& attempt_tracker) {
        attempt_tracker++;
        std::cout << " [payment $" << amount << " attempt=" << attempt_tracker << "]";
        // Simulates transient failure: fails first 2 attempts, succeeds on 3rd
        return attempt_tracker >= 3;
    }
};

class AlertService {
public:
    bool sendAlert(const std::string& msg) {
        alert_count_++;
        std::cout << " [ALERT: " << msg << "]";
        return true;
    }
    int alertCount() const { return alert_count_; }
private:
    int alert_count_{0};
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Task Scheduler Demo ===\n\n";

    TaskScheduler scheduler;
    EmailService   email;
    PaymentService payment;
    AlertService   alert;
    int payment_attempts = 0;

    // ── Submit tasks (mixed priorities) ──────────────────────────────────────
    std::cout << "--- Submitting tasks ---\n";

    // CRITICAL — alert, deadline in 1s, 1 attempt (no retry for alerts)
    scheduler.submit("Send system alert", Priority::Critical,
                     [&]{ return alert.sendAlert("CPU > 95%"); },
                     Ms(1000), 1);

    // HIGH — payment that will fail twice before succeeding
    scheduler.submit("Process payment $500", Priority::High,
                     [&]{ return payment.process(500, payment_attempts); },
                     Ms(3000), 3);

    // NORMAL — email notifications
    scheduler.submit("Email: welcome to Alice", Priority::Normal,
                     [&]{ return email.send("alice@example.com", "Welcome"); },
                     Ms(5000), 2);

    scheduler.submit("Email: invoice to Bob", Priority::Normal,
                     [&]{ return email.send("bob@example.com", "Invoice"); },
                     Ms(5000), 2);

    // LOW — maintenance task that always fails (goes to DLQ)
    scheduler.submit("Cleanup old logs", Priority::Low,
                     []{ return false; },   // simulates always-failing task
                     Ms(60000), 2);

    // CRITICAL — another alert (tests ordering: should run before HIGH)
    scheduler.submit("Send PTT group alert", Priority::Critical,
                     [&]{ return alert.sendAlert("Group ALPHA: channel change"); },
                     Ms(500), 1);

    // ── Run all tasks ─────────────────────────────────────────────────────────
    std::cout << "\n--- Running ---\n";
    scheduler.runAll();

    // ── Results ───────────────────────────────────────────────────────────────
    std::cout << "\n--- Dead Letter Queue ---\n";
    scheduler.printDLQ();

    std::cout << "\n=== Summary ===\n";
    std::cout << "Tasks submitted  : " << scheduler.submitted()  << '\n';
    std::cout << "Tasks succeeded  : " << scheduler.succeeded()  << '\n';
    std::cout << "Tasks dead (DLQ) : " << scheduler.deadCount()  << '\n';
    std::cout << "Emails sent      : " << email.sendCount()      << '\n';
    std::cout << "Alerts sent      : " << alert.alertCount()     << '\n';
    std::cout << "Payment attempts : " << payment_attempts       << '\n';

    assert(scheduler.submitted()  == 6);
    assert(scheduler.succeeded()  == 5);   // all except log cleanup
    assert(scheduler.deadCount()  == 1);   // log cleanup → DLQ
    assert(email.sendCount()      == 2);
    assert(alert.alertCount()     == 2);
    assert(payment_attempts       == 3);   // failed twice, succeeded third

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
