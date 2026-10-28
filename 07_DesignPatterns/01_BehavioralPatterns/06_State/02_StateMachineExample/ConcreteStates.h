#ifndef CONCRETE_STATES_H
#define CONCRETE_STATES_H

#include "DocumentState.h"
#include "Document.h"

// Draft State
class DraftState : public DocumentState {
public:
    void render() const override;
    void publish(Document* document) override;
    void review(Document* document) override;
    std::string getName() const override;
};

// Moderation State
class ModerationState : public DocumentState {
public:
    void render() const override;
    void publish(Document* document) override;
    void review(Document* document) override;
    std::string getName() const override;
};

// Published State
class PublishedState : public DocumentState {
public:
    void render() const override;
    void publish(Document* document) override;
    void review(Document* document) override;
    std::string getName() const override;
};

#endif // CONCRETE_STATES_H