#include "ConcreteStates.h"
#include <iostream>

// DraftState implementation
void DraftState::render() const {
    std::cout << "Rendering document in draft state (watermarked)" << std::endl;
}

void DraftState::publish(Document* document) {
    std::cout << "Can't publish directly from draft. Sending to moderation..." << std::endl;
    document->setState(std::make_unique<ModerationState>());
}

void DraftState::review(Document* document) {
    std::cout << "Draft is being reviewed. Moving to moderation state." << std::endl;
    document->setState(std::make_unique<ModerationState>());
}

std::string DraftState::getName() const {
    return "Draft";
}

// ModerationState implementation
void ModerationState::render() const {
    std::cout << "Rendering document in moderation state (with review notes)" << std::endl;
}

void ModerationState::publish(Document* document) {
    std::cout << "Document approved. Publishing..." << std::endl;
    document->setState(std::make_unique<PublishedState>());
}

void ModerationState::review(Document* document) {
    std::cout << "Document needs further revision. Returning to draft state." << std::endl;
    document->setState(std::make_unique<DraftState>());
}

std::string ModerationState::getName() const {
    return "Moderation";
}

// PublishedState implementation
void PublishedState::render() const {
    std::cout << "Rendering published document (clean version)" << std::endl;
}

void PublishedState::publish(Document* document) {
    std::cout << "Document is already published." << std::endl;
}

void PublishedState::review(Document* document) {
    std::cout << "Published document is being reviewed for updates. Moving to moderation." << std::endl;
    document->setState(std::make_unique<ModerationState>());
}

std::string PublishedState::getName() const {
    return "Published";
}