#ifndef DOCUMENT_STATE_MACHINE_H
#define DOCUMENT_STATE_MACHINE_H

#include <string>

// Define the possible states
enum class DocumentState {
    DRAFT,
    MODERATION,
    PUBLISHED
};

// Define the possible events/transitions
enum class DocumentEvent {
    SEND_TO_REVIEW,
    APPROVE,
    REJECT,
    REQUEST_CHANGES
};

class DocumentStateMachine {
private:
    std::string content;
    DocumentState currentState;

public:
    DocumentStateMachine(const std::string& content);
    
    // Process an event
    void processEvent(DocumentEvent event);
    
    // Actions
    void render() const;
    
    // Getters
    DocumentState getCurrentState() const;
    std::string getStateName() const;
    std::string getContent() const;
};

#endif // DOCUMENT_STATE_MACHINE_H