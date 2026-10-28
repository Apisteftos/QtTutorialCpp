#include <fcntl.h>
#include <unistd.h>
#include <print>

const char* FIFO_PATH = "/tmp/my_fifo";

int main() {
    std::println("=== Named Pipe - Reader ===");

    // 1. Open for reading — BLOCKS until writer opens it
    std::println("Waiting for writer to connect...");
    int fd = open(FIFO_PATH, O_RDONLY);
    std::println("Writer connected! Reading...");
    std::println("");

    // 2. Read messages
    char buffer[256];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        std::println("[Reader] Received: {}", buffer);
    }

    // 3. Cleanup
    close(fd);
    std::println("");
    std::println("[Reader] Pipe closed. Done.");
    return 0;
}