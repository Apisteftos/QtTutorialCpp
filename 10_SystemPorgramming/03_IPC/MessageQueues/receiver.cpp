#include <fcntl.h>
#include <mqueue.h>
#include <cstring>
#include <unistd.h>
#include <print>

// ============================================
// MESSAGE QUEUE RECEIVER
// ============================================
// Reads messages from the queue.
// Messages are delivered in PRIORITY ORDER (highest first),
// NOT in the order they were sent.
// ============================================

const char* QUEUE_NAME   = "/ipc_message_queue";
const int   MAX_MSG_SIZE = 256;

struct Message {
    int  id;
    char text[200];
};

int main() {
    std::println("╔══════════════════════════════════════╗");
    std::println("║    IPC — Message Queue Receiver      ║");
    std::println("╚══════════════════════════════════════╝");
    std::println("");

    // 1. Open the existing queue for reading
    mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed — did you run sender first?");
        return 1;
    }

    // 2. Get queue attributes to confirm state
    struct mq_attr attr;
    mq_getattr(mq, &attr);
    std::println("Queue opened: {}", QUEUE_NAME);
    std::println("Messages waiting: {}", attr.mq_curmsgs);
    std::println("");

    // 3. Receive all messages
    char buffer[MAX_MSG_SIZE];
    unsigned int priority;
    int received = 0;

    std::println("Reading messages (sorted by priority, highest first):");
    std::println("{}", std::string(55, '-'));

    while (true) {
        ssize_t bytes = mq_receive(mq, buffer, MAX_MSG_SIZE, &priority);
        if (bytes == -1) {
            // EAGAIN means queue is empty (non-blocking mode)
            // In blocking mode, this means a real error
            break;
        }

        Message* msg = (Message*)buffer;
        std::println("[Receiver] Got msg #{} priority={} → \"{}\"", msg->id, priority, msg->text);
        received++;
    }

    std::println("{}", std::string(55, '-'));
    std::println("");
    std::println("Total received: {} messages", received);

    // 4. Close and delete the queue
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    std::println("[Receiver] Queue deleted. Done.");

    return 0;
}
