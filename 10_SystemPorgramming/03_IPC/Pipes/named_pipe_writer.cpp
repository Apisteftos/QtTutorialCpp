#include <iostream>
#include <fcntl.h>      // open()
#include <sys/stat.h>   // mkfifo()
#include <unistd.h>
#include <cstring>

const char* FIFO_PATH = "/tmp/my_fifo";

int main() {
    std::cout << "=== Named Pipe - Writer ===\n";

    // 1. Create the FIFO file (like shm_open but for pipes)
    mkfifo(FIFO_PATH, 0666);  // ignored if already exists

    std::cout << "FIFO created at: " << FIFO_PATH << "\n";
    std::cout << "Waiting for reader to connect...\n";

    // 2. Open for writing — BLOCKS until reader opens it
    int fd = open(FIFO_PATH, O_WRONLY);
    std::cout << "Reader connected! Starting to write...\n\n";

    // 3. Write messages
    const char* messages[] = {
        "First message from Writer",
        "Second message from Writer",
        "Third message from Writer"
    };

    for (const char* msg : messages) {
        std::cout << "[Writer] Sending: " << msg << "\n";
        write(fd, msg, strlen(msg) + 1);
        sleep(1);
    }

    // 4. Cleanup
    close(fd);
    unlink(FIFO_PATH);  // delete the FIFO file
    std::cout << "\n[Writer] Done. FIFO removed.\n";
    return 0;
}