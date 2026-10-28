#include <fcntl.h>      // open()
#include <sys/stat.h>   // mkfifo()
#include <unistd.h>
#include <cstring>
#include <print>

const char* FIFO_PATH = "/tmp/my_fifo";

int main() {
    std::println("=== Named Pipe - Writer ===");

    // 1. Create the FIFO file (like shm_open but for pipes)
    mkfifo(FIFO_PATH, 0666);  // ignored if already exists

    std::println("FIFO created at: {}", FIFO_PATH);
    std::println("Waiting for reader to connect...");

    // 2. Open for writing — BLOCKS until reader opens it
    int fd = open(FIFO_PATH, O_WRONLY);
    std::println("Reader connected! Starting to write...");
    std::println("");

    // 3. Write messages
    const char* messages[] = {
        "First message from Writer",
        "Second message from Writer",
        "Third message from Writer"
    };

    for (const char* msg : messages) {
        std::println("[Writer] Sending: {}", msg);
        write(fd, msg, strlen(msg) + 1);
        sleep(1);
    }

    // 4. Cleanup
    close(fd);
    unlink(FIFO_PATH);  // delete the FIFO file
    std::println("");
    std::println("[Writer] Done. FIFO removed.");
    return 0;
}