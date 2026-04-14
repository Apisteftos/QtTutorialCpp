#include <iostream>
#include <fcntl.h>
#include <mqueue.h>
#include <cstring>
#include <unistd.h>

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
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║    IPC — Message Queue Receiver      ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";

    // 1. Open the existing queue for reading
    mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed — did you run sender first?");
        return 1;
    }

    // 2. Get queue attributes to confirm state
    struct mq_attr attr;
    mq_getattr(mq, &attr);
    std::cout << "Queue opened: " << QUEUE_NAME << "\n";
    std::cout << "Messages waiting: " << attr.mq_curmsgs << "\n\n";

    // 3. Receive all messages
    char buffer[MAX_MSG_SIZE];
    unsigned int priority;
    int received = 0;

    std::cout << "Reading messages (sorted by priority, highest first):\n";
    std::cout << std::string(55, '-') << "\n";

    while (true) {
        ssize_t bytes = mq_receive(mq, buffer, MAX_MSG_SIZE, &priority);
        if (bytes == -1) {
            // EAGAIN means queue is empty (non-blocking mode)
            // In blocking mode, this means a real error
            break;
        }

        Message* msg = (Message*)buffer;
        std::cout << "[Receiver] Got msg #" << msg->id
                  << " priority=" << priority
                  << " → \"" << msg->text << "\"\n";
        received++;
    }

    std::cout << std::string(55, '-') << "\n";
    std::cout << "\nTotal received: " << received << " messages\n";

    // 4. Close and delete the queue
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    std::cout << "[Receiver] Queue deleted. Done.\n";

    return 0;
}
