#include <iostream>
#include <fcntl.h>        // O_CREAT, O_WRONLY
#include <mqueue.h>       // mq_open, mq_send, mq_close, mq_unlink
#include <cstring>
#include <unistd.h>

// ============================================
// WHAT IS A MESSAGE QUEUE?
// ============================================
// A message queue is a kernel-managed list of messages.
// Sender puts messages IN, receiver takes messages OUT.
//
//  Sender                 Kernel                Receiver
//  ┌──────┐   mq_send()  ┌─────────────────┐  mq_receive()  ┌──────────┐
//  │      │ ───────────→ │ [msg3][msg2][msg1]│ ────────────→ │          │
//  └──────┘              └─────────────────┘                 └──────────┘
//
// Key features:
//   - Messages have a PRIORITY (higher = received first)
//   - Each message has a fixed MAX SIZE
//   - Queue has a MAX number of messages
//   - Sender blocks if queue is FULL
//   - Receiver blocks if queue is EMPTY
//   - Named with "/" prefix, visible in /dev/mqueue/
// ============================================

const char* QUEUE_NAME  = "/ipc_message_queue";
const int   MAX_MSG_SIZE = 256;
const int   MAX_MSGS     = 10;

struct Message {
    int  id;
    char text[200];
};

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║    IPC — Message Queue Sender        ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";

    // 1. Configure queue attributes
    struct mq_attr attr;
    attr.mq_flags   = 0;
    attr.mq_maxmsg  = MAX_MSGS;       // max messages in queue
    attr.mq_msgsize = MAX_MSG_SIZE;   // max bytes per message
    attr.mq_curmsgs = 0;

    // 2. Create and open the queue for writing
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed");
        return 1;
    }

    std::cout << "Queue created: " << QUEUE_NAME << "\n";
    std::cout << "Max messages: " << MAX_MSGS << "\n";
    std::cout << "Max msg size: " << MAX_MSG_SIZE << " bytes\n\n";

    // 3. Send messages with different priorities
    const char* texts[] = {
        "Normal priority message",
        "Another normal message",
        "HIGH PRIORITY — process me first!",
        "Low priority message",
        "Final message"
    };
    unsigned int priorities[] = { 5, 5, 10, 1, 5 };

    for (int i = 0; i < 5; i++) {
        Message msg;
        msg.id = i + 1;
        snprintf(msg.text, sizeof(msg.text), "%s", texts[i]);

        int result = mq_send(mq, (char*)&msg, sizeof(Message), priorities[i]);
        if (result == -1) {
            perror("mq_send failed");
        } else {
            std::cout << "[Sender] Sent msg #" << msg.id
                      << " priority=" << priorities[i]
                      << " → \"" << msg.text << "\"\n";
        }
        usleep(200000);  // 200ms between sends
    }

    std::cout << "\n[Sender] All messages sent. Closing queue.\n";

    // 4. Close (does NOT delete the queue)
    mq_close(mq);

    // Note: mq_unlink is called by the RECEIVER after it's done
    // so the queue persists for the receiver to read

    return 0;
}
