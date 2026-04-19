#include "DocumentStateMachine.h"
#include <iostream>

int main() {
    // Create a document in the default Draft state
    DocumentStateMachine doc("Sample content for switch-case state machine demonstration");
    
    // Show initial state
    std::cout << "Initial state: " << doc.getStateName() << std::endl;
    doc.render();
    std::cout << std::endl;
    
    // Send to review (Draft -> Moderation)
    std::cout << "Sending document for review:" << std::endl;
    doc.processEvent(DocumentEvent::SEND_TO_REVIEW);
    std::cout << "Current state: " << doc.getStateName() << std::endl;
    doc.render();
    std::cout << std::endl;
    
    // Try an invalid transition
    std::cout << "Attempting invalid transition (sending to review again):" << std::endl;
    doc.processEvent(DocumentEvent::SEND_TO_REVIEW);
    std::cout << "Current state: " << doc.getStateName() << " (unchanged)" << std::endl;
    std::cout << std::endl;
    
    // Approve the document (Moderation -> Published)
    std::cout << "Approving document:" << std::endl;
    doc.processEvent(DocumentEvent::APPROVE);
    std::cout << "Current state: " << doc.getStateName() << std::endl;
    doc.render();
    std::cout << std::endl;
    
    // Request changes (Published -> Moderation)
    std::cout << "Requesting changes:" << std::endl;
    doc.processEvent(DocumentEvent::REQUEST_CHANGES);
    std::cout << "Current state: " << doc.getStateName() << std::endl;
    std::cout << std::endl;
    
    // Reject the changes (Moderation -> Draft)
    std::cout << "Rejecting the document:" << std::endl;
    doc.processEvent(DocumentEvent::REJECT);
    std::cout << "Current state: " << doc.getStateName() << std::endl;
    
    return 0;
}