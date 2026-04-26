#include <iostream>
#include <unistd.h>     // pipe(), fork(), read(), write()
#include <cstring>
#include <sys/wait.h>   // waitpid()

// ============================================
// ANONYMOUS PIPE
// ============================================
// - Works only between parent and child process
// - Created BEFORE fork()
// - One direction: parent writes, child reads
//   (or reverse)
// - Destroyed when both ends are closed
// ============================================

int main() {
    std::cout << "=== Anonymous Pipe Example ===\n\n";

    int pipe_fd[2];  // pipe_fd[0] = read end
                     // pipe_fd[1] = write end

    // 1. Create the pipe BEFORE forking
    if (pipe(pipe_fd) == -1) {
        std::cerr << "Failed to create pipe\n";
        return 1;
    }

    // 2. Fork — creates child process
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed\n";
        return 1;
    }

    // ─── PARENT PROCESS ───
    if (pid > 0) {
        close(pipe_fd[0]);  // parent doesn't read, close read end

        const char* messages[] = {
            "Hello from Parent!",
            "Message #2 from Parent",
            "Goodbye from Parent!"
        };

        for (const char* msg : messages) {
            std::cout << "[Parent] Writing: " << msg << "\n";
            write(pipe_fd[1], msg, strlen(msg) + 1);
            sleep(1);
        }

        close(pipe_fd[1]);  // done writing, close write end
        waitpid(pid, nullptr, 0);  // wait for child to finish
        std::cout << "[Parent] Child finished. Done.\n";
    }

    // ─── CHILD PROCESS ───
    else {
        close(pipe_fd[1]);  // child doesn't write, close write end

        char buffer[256];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            std::cout << "[Child]  Received: " << buffer << "\n";
        }

        close(pipe_fd[0]);
        std::cout << "[Child]  Pipe closed. Exiting.\n";
    }

    return 0;
}