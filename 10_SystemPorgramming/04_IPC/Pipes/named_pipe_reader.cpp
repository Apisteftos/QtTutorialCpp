#include <iostream>
#include <fcntl.h>
#include <unistd.h>

const char* FIFO_PATH = "/tmp/my_fifo";

int main() {
    std::cout << "=== Named Pipe - Reader ===\n";

    // 1. Open for reading — BLOCKS until writer opens it
    std::cout << "Waiting for writer to connect...\n";
    int fd = open(FIFO_PATH, O_RDONLY);
    std::cout << "Writer connected! Reading...\n\n";

    // 2. Read messages
    char buffer[256];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        std::cout << "[Reader] Received: " << buffer << "\n";
    }

    // 3. Cleanup
    close(fd);
    std::cout << "\n[Reader] Pipe closed. Done.\n";
    return 0;
}