#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

// ============================================
// SOCKET CLIENT
// ============================================
// Connects to the server and exchanges messages.
// Client workflow:
//   socket() → connect() → send/recv → close()
// ============================================

const int   PORT      = 9090;
const char* SERVER_IP = "127.0.0.1";  // localhost

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║      IPC — Socket Client             ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";

    // 1. Create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket() failed");
        return 1;
    }

    // 2. Set server address
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);  // convert IP string

    // 3. Connect to server
    std::cout << "[Client] Connecting to " << SERVER_IP << ":" << PORT << "...\n";
    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect() failed — is the server running?");
        close(sock_fd);
        return 1;
    }
    std::cout << "[Client] Connected!\n\n";

    // 4. Send messages and receive responses
    const char* messages[] = {
        "Hello Server!",
        "How are you?",
        "Sending data from Client A",
        "Last message before quit",
        "quit"
    };

    char buffer[1024];

    for (const char* msg : messages) {
        // Send message
        send(sock_fd, msg, strlen(msg), 0);
        std::cout << "[Client] Sent:     \"" << msg << "\"\n";

        // Exit after sending quit
        if (strcmp(msg, "quit") == 0) break;

        // Receive response
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            std::cout << "[Client] Server closed connection.\n";
            break;
        }
        std::cout << "[Client] Received: \"" << buffer << "\"\n\n";
    }

    // 5. Cleanup
    close(sock_fd);
    std::cout << "\n[Client] Connection closed.\n";
    return 0;
}
