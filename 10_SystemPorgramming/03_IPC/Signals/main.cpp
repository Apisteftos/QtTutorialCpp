#include <csignal>      // signal(), kill(), sigaction()
#include <unistd.h>     // getpid(), sleep()
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/wait.h>
#include <print>

// ============================================
// WHAT ARE SIGNALS?
// ============================================
// Signals are SOFTWARE INTERRUPTS sent to a process.
// They carry NO data — just a notification number.
//
// Common signals:
//   SIGINT  (2)  → Ctrl+C pressed
//   SIGTERM (15) → polite termination request
//   SIGKILL (9)  → force kill (cannot be caught!)
//   SIGUSR1 (10) → user-defined signal 1
//   SIGUSR2 (12) → user-defined signal 2
//   SIGCHLD (17) → child process terminated
//   SIGALRM (14) → alarm timer expired
//
// Three things a process can do with a signal:
//   1. HANDLE IT  → run a custom handler function
//   2. IGNORE IT  → signal(SIGTERM, SIG_IGN)
//   3. DEFAULT    → OS default action (usually terminate)
//
// NOTE: SIGKILL and SIGSTOP can NEVER be caught or ignored
// ============================================

// ─── Global flag for clean shutdown ───
volatile sig_atomic_t running = 1;  // sig_atomic_t is async-signal-safe
volatile sig_atomic_t usr1_count = 0;
volatile sig_atomic_t usr2_count = 0;

// ============================================
// EXAMPLE 1: Basic signal handler
// ============================================

void handleSIGINT(int signum) {
    // Keep this function SHORT and SIMPLE
    // Avoid: malloc, printf, mutex locks inside handlers
    write(STDOUT_FILENO, "\n[Handler] SIGINT received! Shutting down...\n", 45);
    running = 0;
}

void handleSIGTERM(int signum) {
    write(STDOUT_FILENO, "[Handler] SIGTERM received! Cleaning up...\n", 43);
    running = 0;
}

void handleSIGUSR1(int signum) {
    usr1_count = usr1_count + 1;   // ++ deprecated on volatile in C++20
    write(STDOUT_FILENO, "[Handler] SIGUSR1 received!\n", 28);
}

void handleSIGUSR2(int signum) {
    usr2_count = usr2_count + 1;   // ++ deprecated on volatile in C++20
    write(STDOUT_FILENO, "[Handler] SIGUSR2 received!\n", 28);
}


void example1_basic_handlers() {
    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("EXAMPLE 1: Basic Signal Handlers");
    std::println("{}", std::string(60, '='));
    std::println("");

    // Register handlers using signal() — simple but limited
    signal(SIGINT,  handleSIGINT);
    signal(SIGTERM, handleSIGTERM);
    signal(SIGUSR1, handleSIGUSR1);
    signal(SIGUSR2, handleSIGUSR2);

    std::println("Process PID: {}", getpid());
    std::println("Registered handlers for SIGINT, SIGTERM, SIGUSR1, SIGUSR2");
    std::println("");
    std::println("From another terminal, try:");
    std::println("  kill -SIGUSR1 {}", getpid());
    std::println("  kill -SIGUSR2 {}", getpid());
    std::println("  kill -SIGTERM {}", getpid());
    std::println("  (or press Ctrl+C)");
    std::println("");
    std::println("Waiting for signals (5 seconds)...");

    // Wait loop — process signals as they arrive
    int seconds = 0;
    while (running && seconds < 5) {
        sleep(1);
        seconds++;
        std::println("  [tick {}] USR1={} USR2={}", seconds, static_cast<int>(usr1_count), static_cast<int>(usr2_count));
    }

    std::println("");
    std::println("Final count — SIGUSR1: {}  SIGUSR2: {}", static_cast<int>(usr1_count), static_cast<int>(usr2_count));
}

// ============================================
// EXAMPLE 2: sigaction() — more robust handler
// ============================================

void handleAlarm(int signum) {
    write(STDOUT_FILENO, "[Handler] SIGALRM — timer fired!\n", 33);
}

void example2_sigaction() {
    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("EXAMPLE 2: sigaction() — Robust Signal Handling");
    std::println("{}", std::string(60, '='));
    std::println("");

    // sigaction gives more control than signal():
    // - Block other signals while handler runs
    // - Restart interrupted system calls (SA_RESTART)
    // - Get info about signal sender (SA_SIGINFO)

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleAlarm;
    sa.sa_flags   = SA_RESTART;  // restart read()/write() if interrupted
    sigemptyset(&sa.sa_mask);    // don't block other signals during handler

    sigaction(SIGALRM, &sa, nullptr);

    std::println("Setting alarm for 2 seconds...");
    alarm(2);  // sends SIGALRM after 2 seconds

    std::println("Waiting...");
    sleep(3);  // will be interrupted by SIGALRM at 2s
    std::println("Done.");
}

// ============================================
// EXAMPLE 3: Sending signals between processes
// ============================================

void example3_send_signal() {
    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("EXAMPLE 3: Parent Sending Signal to Child");
    std::println("{}", std::string(60, '='));
    std::println("");

    // Reset running flag
    running = 1;
    signal(SIGUSR1, handleSIGUSR1);

    pid_t pid = fork();

    if (pid < 0) {
        std::println(stderr, "Fork failed");
        return;
    }

    // ─── CHILD ───
    if (pid == 0) {
        std::println("[Child]  PID={} waiting for SIGUSR1...", getpid());
        while (running) {
            pause();  // sleep until any signal arrives
        }
        std::println("[Child]  Received SIGUSR1, exiting.");
        exit(0);
    }

    // ─── PARENT ───
    std::println("[Parent] PID={} will send SIGUSR1 to child PID={}", getpid(), pid);
    sleep(1);

    std::println("[Parent] Sending SIGUSR1...");
    kill(pid, SIGUSR1);  // send signal to specific PID

    // Also set running=0 so child exits the loop
    running = 0;
    kill(pid, SIGUSR1);

    wait(nullptr);
    std::println("[Parent] Child finished.");
}

// ============================================
// EXAMPLE 4: Ignoring signals
// ============================================

void example4_ignore_signal() {
    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("EXAMPLE 4: Ignoring Signals");
    std::println("{}", std::string(60, '='));
    std::println("");

    std::println("Ignoring SIGTERM for 3 seconds...");
    std::println("Try: kill -SIGTERM {}", getpid());
    std::println("");

    signal(SIGTERM, SIG_IGN);  // ignore SIGTERM
    sleep(3);

    std::println("Restoring default SIGTERM handler.");
    signal(SIGTERM, SIG_DFL);  // restore default behavior
}

// ============================================
// MAIN
// ============================================

int main() {
    std::println("╔══════════════════════════════════════╗");
    std::println("║     IPC — Signals Example in C++     ║");
    std::println("╚══════════════════════════════════════╝");

    example1_basic_handlers();
    example2_sigaction();
    example3_send_signal();
    example4_ignore_signal();

    std::println("");
    std::println("✅ All signal examples completed.");
    return 0;
}
