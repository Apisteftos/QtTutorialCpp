#ifndef DOCUMENT_STATE_H
#define DOCUMENT_STATE_H

#include <string>

class Document;

// Abstract State base class
class DocumentState {
public:
    virtual ~DocumentState() = default;
    virtual void render() const = 0;
    virtual void publish(Document* document) = 0;
    virtual void review(Document* document) = 0;
    virtual std::string getName() const = 0;
};

// Concrete states (forward declarations)
class DraftState;
class ModerationState;
class PublishedState;

#endif // DOCUMENT_STATE_H