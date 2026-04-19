#include "DocumentStateMachine.h"
#include <iostream>

DocumentStateMachine::DocumentStateMachine(const std::string& content)
    : content(content), currentState(DocumentState::DRAFT) {
}

void DocumentStateMachine::processEvent(DocumentEvent event) {
    // State transition logic
    switch (currentState) {
        case DocumentState::DRAFT:
            switch (event) {
                case DocumentEvent::SEND_TO_REVIEW:
                    std::cout << "Draft is being sent for review." << std::endl;
                    currentState = DocumentState::MODERATION;
                    break;
                default:
                    std::cout << "Invalid event for Draft state." << std::endl;
                    break;
            }
            break;
            
        case DocumentState::MODERATION:
            switch (event) {
                case DocumentEvent::APPROVE:
                    std::cout << "Document approved. Publishing..." << std::endl;
                    currentState = DocumentState::PUBLISHED;
                    break;
                case DocumentEvent::REJECT:
                    std::cout << "Document rejected. Returning to draft..." << std::endl;
                    currentState = DocumentState::DRAFT;
                    break;
                default:
                    std::cout << "Invalid event for Moderation state." << std::endl;
                    break;
            }
            break;
            
        case DocumentState::PUBLISHED:
            switch (event) {
                case DocumentEvent::REQUEST_CHANGES:
                    std::cout << "Changes requested for published document. Moving to moderation..." << std::endl;
                    currentState = DocumentState::MODERATION;
                    break;
                default:
                    std::cout << "Invalid event for Published state." << std::endl;
                    break;
            }
            break;
    }
}

void DocumentStateMachine::render() const {
    std::cout << "Document content: " << content << std::endl;
    
    // Render behavior based on current state
    switch (currentState) {
        case DocumentState::DRAFT:
            std::cout << "Rendering document in draft state (watermarked)" << std::endl;
            break;
        case DocumentState::MODERATION:
            std::cout << "Rendering document in moderation state (with review notes)" << std::endl;
            break;
        case DocumentState::PUBLISHED:
            std::cout << "Rendering published document (clean version)" << std::endl;
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