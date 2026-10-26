#include <memory>
#include <print>
#include <string>
#include <vector>

class Document {
private:
    std::string name;
    std::string content;

public:
    Document(const std::string& name, const std::string& content)
        : name(name), content(content) {
        std::println("Document '{}' created", this->name);
    }

    ~Document() {
        std::println("Document '{}' destroyed", name);
    }

    void display() const {
        std::println("Document: {} - {}", name, content);
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
        std::println("{} now has access to '{}'", username, doc->getName());
    }

    void viewDocuments() const {
        std::println("{}'s documents:", username);
        for (const auto& doc : documents) {
            std::print("  - ");
            doc->display();
            std::println("    (ref count: {})", doc.use_count());
        }
    }
};

int main() {
    // === EXAMPLE 1: Create a shared document ===
    auto doc1 = std::make_shared<Document>("Report.pdf", "Q4 Sales Report");
    std::println("Reference count: {}", doc1.use_count());
    std::println("");

    // === EXAMPLE 2: Multiple users share the same document ===
    {
        User alice("Alice");
        User bob("Bob");

        alice.addDocument(doc1);
        std::println("Reference count: {}", doc1.use_count());

        bob.addDocument(doc1);
        std::println("Reference count: {}", doc1.use_count());
        std::println("");

        // Another shared document
        auto doc2 = std::make_shared<Document>("Presentation.pptx", "Project Overview");
        alice.addDocument(doc2);

        std::println("");
        alice.viewDocuments();
        std::println("");
        bob.viewDocuments();

        std::println("");
        std::println("--- Leaving scope (users destroyed) ---");
    } // alice and bob destroyed, but doc1 still exists

    // === EXAMPLE 3: The document outlives its users ===
    std::println("");
    std::println("Reference count after users destroyed: {}", doc1.use_count());
    doc1->display();

    // Create another reference
    std::shared_ptr<Document> doc1_copy = doc1;
    std::println("Reference count after copy: {}", doc1.use_count());

    std::println("");
    std::println("--- Exiting main ---");
    return 0;
}
