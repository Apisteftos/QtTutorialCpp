#include <iostream>
#include <csignal>      // signal(), kill(), sigaction()
#include <unistd.h>     // getpid(), sleep()
#include <cstring>
#include <sys/wait.h>

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
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "EXAMPLE 1: Basic Signal Handlers\n";
    std::cout << std::string(60, '=') << "\n\n";

    // Register handlers using signal() — simple but limited
    signal(SIGINT,  handleSIGINT);
    signal(SIGTERM, handleSIGTERM);
    signal(SIGUSR1, handleSIGUSR1);
    signal(SIGUSR2, handleSIGUSR2);

    std::cout << "Process PID: " << getpid() << "\n";
    std::cout << "Registered handlers for SIGINT, SIGTERM, SIGUSR1, SIGUSR2\n\n";
    std::cout << "From another terminal, try:\n";
    std::cout << "  kill -SIGUSR1 " << getpid() << "\n";
    std::cout << "  kill -SIGUSR2 " << getpid() << "\n";
    std::cout << "  kill -SIGTERM " << getpid() << "\n";
    std::cout << "  (or press Ctrl+C)\n\n";
    std::cout << "Waiting for signals (5 seconds)...\n";

    // Wait loop — process signals as they arrive
    int seconds = 0;
    while (running && seconds < 5) {
        sleep(1);
        seconds++;
        std::cout << "  [tick " << seconds << "] USR1=" << usr1_count
                  << " USR2=" << usr2_count << "\n";
    }

    std::cout << "\nFinal count — SIGUSR1: " << usr1_count
              << "  SIGUSR2: " << usr2_count << "\n";
}

// ============================================
// EXAMPLE 2: sigaction() — more robust handler
// ============================================

void handleAlarm(int signum) {
    write(STDOUT_FILENO, "[Handler] SIGALRM — timer fired!\n", 33);
}

void example2_sigaction() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "EXAMPLE 2: sigaction() — Robust Signal Handling\n";
    std::cout << std::string(60, '=') << "\n\n";

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

    std::cout << "Setting alarm for 2 seconds...\n";
    alarm(2);  // sends SIGALRM after 2 seconds

    std::cout << "Waiting...\n";
    sleep(3);  // will be interrupted by SIGALRM at 2s
    std::cout << "Done.\n";
}

// ============================================
// EXAMPLE 3: Sending signals between processes
// ============================================

void example3_send_signal() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "EXAMPLE 3: Parent Sending Signal to Child\n";
    std::cout << std::string(60, '=') << "\n\n";

    // Reset running flag
    running = 1;
    signal(SIGUSR1, handleSIGUSR1);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed\n";
        return;
    }

    // ─── CHILD ───
    if (pid == 0) {
        std::cout << "[Child]  PID=" << getpid() << " waiting for SIGUSR1...\n";
        while (running) {
            pause();  // sleep until any signal arrives
        }
        std::cout << "[Child]  Received SIGUSR1, exiting.\n";
        exit(0);
    }

    // ─── PARENT ───
    std::cout << "[Parent] PID=" << getpid()
              << " will send SIGUSR1 to child PID=" << pid << "\n";
    sleep(1);

    std::cout << "[Parent] Sending SIGUSR1...\n";
    kill(pid, SIGUSR1);  // send signal to specific PID

    // Also set running=0 so child exits the loop
    running = 0;
    kill(pid, SIGUSR1);

    wait(nullptr);
    std::cout << "[Parent] Child finished.\n";
}

// ============================================
// EXAMPLE 4: Ignoring signals
// ============================================

void example4_ignore_signal() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "EXAMPLE 4: Ignoring Signals\n";
    std::cout << std::string(60, '=') << "\n\n";

    std::cout << "Ignoring SIGTERM for 3 seconds...\n";
    std::cout << "Try: kill -SIGTERM " << getpid() << "\n\n";

    signal(SIGTERM, SIG_IGN);  // ignore SIGTERM
    sleep(3);

    std::cout << "Restoring default SIGTERM handler.\n";
    signal(SIGTERM, SIG_DFL);  // restore default behavior
}

// ============================================
// MAIN
// ============================================

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║     IPC — Signals Example in C++     ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";

    example1_basic_handlers();
    example2_sigaction();
    example3_send_signal();
    example4_ignore_signal();

    std::cout << "\n✅ All signal examples completed.\n";
    return 0;
}
