#include "ConcreteStates.h"
#include <print>

// DraftState implementation
void DraftState::render() const {
    std::println("Rendering document in draft state (watermarked)");
}

void DraftState::publish(Document* document) {
    std::println("Can't publish directly from draft. Sending to moderation...");
    document->setState(std::make_unique<ModerationState>());
}

void DraftState::review(Document* document) {
    std::println("Draft is being reviewed. Moving to moderation state.");
    document->setState(std::make_unique<ModerationState>());
}

std::string DraftState::getName() const {
    return "Draft";
}

// ModerationState implementation
void ModerationState::render() const {
    std::println("Rendering document in moderation state (with review notes)");
}

void ModerationState::publish(Document* document) {
    std::println("Document approved. Publishing...");
    document->setState(std::make_unique<PublishedState>());
}

void ModerationState::review(Document* document) {
    std::println("Document needs further revision. Returning to draft state.");
    document->setState(std::make_unique<DraftState>());
}

std::string ModerationState::getName() const {
    return "Moderation";
}

// PublishedState implementation
void PublishedState::render() const {
    std::println("Rendering published document (clean version)");
}

void PublishedState::publish(Document* document) {
    std::println("Document is already published.");
}

void PublishedState::review(Document* document) {
    std::println("Published document is being reviewed for updates. Moving to moderation.");
    document->setState(std::make_unique<ModerationState>());
}

std::string PublishedState::getName() const {
    return "Published";
}