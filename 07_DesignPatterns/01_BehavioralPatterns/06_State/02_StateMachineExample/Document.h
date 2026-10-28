#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <memory>
#include "DocumentState.h"

class Document {
private:
    std::string content;
    std::unique_ptr<DocumentState> state;

public:
    Document(const std::string& content);
    
    void render() const;
    void publish();
    void review();
    
    void setState(std::unique_ptr<DocumentState> newState);
    std::string getContent() const;
    std::string getStateName() const;
};

#endif // DOCUMENT_H