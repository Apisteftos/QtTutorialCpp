#include <iostream>
#include <sys/socket.h>   // socket(), bind(), listen(), accept()
#include <netinet/in.h>   // sockaddr_in
#include <arpa/inet.h>    // inet_ntoa()
#include <unistd.h>       // close(), read(), write()
#include <cstring>

// ============================================
// WHAT ARE SOCKETS?
// ============================================
// A socket is a communication endpoint.
// Two sockets connected together form a channel
// that can send and receive data in BOTH directions.
//
// Unlike pipes (one-way, same machine only),
// sockets work:
//   - Both directions (bidirectional)
//   - Same machine (localhost)
//   - OR across a NETWORK
//
// Socket types:
//   SOCK_STREAM  → TCP — reliable, ordered (we use this)
//   SOCK_DGRAM   → UDP — fast, no guarantee
//
// Server workflow:
//   socket() → bind() → listen() → accept() → read/write → close()
//
// Client workflow:
//   socket() → connect() → read/write → close()
// ============================================

const int   PORT       = 9090;
const int   BACKLOG    = 5;     // max pending connections
const char* SERVER_IP  = "127.0.0.1";

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║      IPC — Socket Server             ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";

    // 1. Create socket
    // AF_INET = IPv4, SOCK_STREAM = TCP
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket() failed");
        return 1;
    }
    std::cout << "[Server] Socket created.\n";

    // 2. Allow reuse of port immediately after restart
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. Bind to address and port
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;   // accept on all interfaces
    address.sin_port        = htons(PORT);  // host-to-network byte order

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == -1) {
        perror("bind() failed");
        close(server_fd);
        return 1;
    }
    std::cout << "[Server] Bound to port " << PORT << ".\n";

    // 4. Start listening
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen() failed");
        close(server_fd);
        return 1;
    }
    std::cout << "[Server] Listening for connections...\n\n";

    // 5. Accept ONE client connection
    sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept() failed");
        close(server_fd);
        return 1;
    }
    std::cout << "[Server] Client connected from "
              << inet_ntoa(client_addr.sin_addr) << "\n\n";

    // 6. Communication loop — receive then respond
    char buffer[1024];
    int  round = 1;

    while (true) {
        // Receive from client
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0) {
            std::cout << "[Server] Client disconnected.\n";
            break;
        }

        std::cout << "[Server] Received: \"" << buffer << "\"\n";

        // Check for exit command
        if (strcmp(buffer, "quit") == 0) {
            std::cout << "[Server] Client sent quit. Closing.\n";
            break;
        }

        // Send response back
        char response[1024];
        snprintf(response, sizeof(response),
                 "Server ACK #%d — got your message: \"%s\"", round, buffer);

        send(client_fd, response, strlen(response), 0);
        std::cout << "[Server] Sent:     \"" << response << "\"\n\n";
        round++;
    }

    // 7. Cleanup
    close(client_fd);
    close(server_fd);
    std::cout << "[Server] Shutdown complete.\n";
    return 0;
}
