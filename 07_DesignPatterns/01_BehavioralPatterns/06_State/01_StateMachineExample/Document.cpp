#include "Document.h"
#include "ConcreteStates.h"
#include <iostream>

Document::Document(const std::string& content) : content(content) {
    // Start in Draft state
    state = std::make_unique<DraftState>();
}

void Document::render() const {
    std::cout << "Document content: " << content << std::endl;
    state->render();
}

void Document::publish() {
    state->publish(this);
}

void Document::review() {
    state->review(this);
}

void Document::setState(std::unique_ptr<DocumentState> newState) {
    state = std::move(newState);
}

std::string Document::getContent() const {
    return content;
}

std::string Document::getStateName() const {
    return state->getName();
}