#include <iostream>
#include <memory>
#include <vector>

class Document {
private:
    std::string name;
    std::string content;

public:
    Document(const std::string& name, const std::string& content)
        : name(name), content(content) {
        std::cout << "Document '" << name << "' created\n";
    }

    ~Document() {
        std::cout << "Document '" << name << "' destroyed\n";
    }

    void display() const {
        std::cout << "Document: " << name << " - " << content << "\n";
    }

    std::string getName() const { return name; }
};

class User {
private:
    std::string username;
    std::vector<std::shared_ptr<Document>> documents;

public:
    User(const std::string& name) : username(name) {}

    void addDocument(std::shared_ptr<Document> doc) {
        documents.push_back(doc);
        std::cout << username << " now has access to '"
                  << doc->getName() << "'\n";
    }

    void viewDocuments() const {
        std::cout << username << "'s documents:\n";
        for (const auto& doc : documents) {
            std::cout << "  - ";
            doc->display();
            std::cout << "    (ref count: " << doc.use_count() << ")\n";
        }
    }
};

int main() {
    // Create a shared document
    auto doc1 = std::make_shared<Document>("Report.pdf", "Q4 Sales Report");
    std::cout << "Reference count: " << doc1.use_count() << "\n\n";

    // Multiple users share the same document
    {
        User alice("Alice");
        User bob("Bob");

        alice.addDocument(doc1);
        std::cout << "Reference count: " << doc1.use_count() << "\n";

        bob.addDocument(doc1);
        std::cout << "Reference count: " << doc1.use_count() << "\n\n";

        // Another shared document
        auto doc2 = std::make_shared<Document>("Presentation.pptx", "Project Overview");
        alice.addDocument(doc2);

        std::cout << "\n";
        alice.viewDocuments();
        std::cout << "\n";
        bob.viewDocuments();

        std::cout << "\n--- Leaving scope (users destroyed) ---\n";
    } // alice and bob destroyed, but doc1 still exists

    std::cout << "\nReference count after users destroyed: "
              << doc1.use_count() << "\n";
    doc1->display();

    // Create another reference
    std::shared_ptr<Document> doc1_copy = doc1;
    std::cout << "Reference count after copy: " << doc1.use_count() << "\n";

    std::cout << "\n--- Exiting main ---\n";
    return 0;
}
