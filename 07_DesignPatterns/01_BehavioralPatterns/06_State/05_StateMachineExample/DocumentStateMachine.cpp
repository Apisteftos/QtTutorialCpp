#include "DocumentStateMachine.h"
#include <print>

DocumentStateMachine::DocumentStateMachine(const std::string& content)
    : content(content), currentState(DocumentState::DRAFT) {
}

void DocumentStateMachine::processEvent(DocumentEvent event) {
    // State transition logic
    switch (currentState) {
        case DocumentState::DRAFT:
            switch (event) {
                case DocumentEvent::SEND_TO_REVIEW:
                    std::println("Draft is being sent for review.");
                    currentState = DocumentState::MODERATION;
                    break;
                default:
                    std::println("Invalid event for Draft state.");
                    break;
            }
            break;
            
        case DocumentState::MODERATION:
            switch (event) {
                case DocumentEvent::APPROVE:
                    std::println("Document approved. Publishing...");
                    currentState = DocumentState::PUBLISHED;
                    break;
                case DocumentEvent::REJECT:
                    std::println("Document rejected. Returning to draft...");
                    currentState = DocumentState::DRAFT;
                    break;
                default:
                    std::println("Invalid event for Moderation state.");
                    break;
            }
            break;
            
        case DocumentState::PUBLISHED:
            switch (event) {
                case DocumentEvent::REQUEST_CHANGES:
                    std::println("Changes requested for published document. Moving to moderation...");
                    currentState = DocumentState::MODERATION;
                    break;
                default:
                    std::println("Invalid event for Published state.");
                    break;
            }
            break;
    }
}

void DocumentStateMachine::render() const {
    std::println("Document content: {}", content);
    
    // Render behavior based on current state
    switch (currentState) {
        case DocumentState::DRAFT:
            std::println("Rendering document in draft state (watermarked)");
            break;
        case DocumentState::MODERATION:
            std::println("Rendering document in moderation state (with review notes)");
            break;
        case DocumentState::PUBLISHED:
            std::println("Rendering published document (clean version)");
            break;
    }
}

DocumentState DocumentStateMachine::getCurrentState() const {
    return currentState;
}

std::string DocumentStateMachine::getStateName() const {
    switch (currentState) {
        case DocumentState::DRAFT:
            return "Draft";
        case DocumentState::MODERATION:
            return "Moderation";
        case DocumentState::PUBLISHED:
            return "Published";
        default:
            return "Unknown";
    }
}

std::string DocumentStateMachine::getContent() const {
    return content;
}