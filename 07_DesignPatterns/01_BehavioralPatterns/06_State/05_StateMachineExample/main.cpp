#include "DocumentStateMachine.h"
#include <print>

int main() {
    // Create a document in the default Draft state
    DocumentStateMachine doc("Sample content for switch-case state machine demonstration");
    
    // Show initial state
    std::println("Initial state: {}", doc.getStateName());
    doc.render();
    std::println("");
    
    // Send to review (Draft -> Moderation)
    std::println("Sending document for review:");
    doc.processEvent(DocumentEvent::SEND_TO_REVIEW);
    std::println("Current state: {}", doc.getStateName());
    doc.render();
    std::println("");
    
    // Try an invalid transition
    std::println("Attempting invalid transition (sending to review again):");
    doc.processEvent(DocumentEvent::SEND_TO_REVIEW);
    std::println("Current state: {} (unchanged)", doc.getStateName());
    std::println("");
    
    // Approve the document (Moderation -> Published)
    std::println("Approving document:");
    doc.processEvent(DocumentEvent::APPROVE);
    std::println("Current state: {}", doc.getStateName());
    doc.render();
    std::println("");
    
    // Request changes (Published -> Moderation)
    std::println("Requesting changes:");
    doc.processEvent(DocumentEvent::REQUEST_CHANGES);
    std::println("Current state: {}", doc.getStateName());
    std::println("");
    
    // Reject the changes (Moderation -> Draft)
    std::println("Rejecting the document:");
    doc.processEvent(DocumentEvent::REJECT);
    std::println("Current state: {}", doc.getStateName());
    
    return 0;
}